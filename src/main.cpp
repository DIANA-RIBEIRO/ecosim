#define CROW_MAIN
#define CROW_STATIC_DIR "../public"

#include "crow_all.h"
#include "json.hpp"
#include <random>
//#include "../samples/simulate_random_actions.cpp"

static const uint32_t NUM_ROWS = 15;

// Constants
const uint32_t PLANT_MAXIMUM_AGE = 10;
const uint32_t HERBIVORE_MAXIMUM_AGE = 50;
const uint32_t CARNIVORE_MAXIMUM_AGE = 80;
const uint32_t MAXIMUM_ENERGY = 200;
const uint32_t THRESHOLD_ENERGY_FOR_REPRODUCTION = 20;

// Probabilities
const double PLANT_REPRODUCTION_PROBABILITY = 0.2;
const double HERBIVORE_REPRODUCTION_PROBABILITY = 0.075;
const double CARNIVORE_REPRODUCTION_PROBABILITY = 0.025;
const double HERBIVORE_MOVE_PROBABILITY = 0.7;
const double HERBIVORE_EAT_PROBABILITY = 0.9;
const double CARNIVORE_MOVE_PROBABILITY = 0.5;
const double CARNIVORE_EAT_PROBABILITY = 1.0;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distribution(0, NUM_ROWS-1);
std::uniform_real_distribution<> dis(0.0, 1.0);

// Type definitions
enum entity_type_t
{
    empty,
    plant,
    herbivore,
    carnivore
};

struct pos_t
{
    uint32_t i;
    uint32_t j;
};

struct entity_t
{
    entity_type_t type;
    int32_t energy;
    int32_t age;
};

// Auxiliary code to convert the entity_type_t enum to a string
NLOHMANN_JSON_SERIALIZE_ENUM(entity_type_t, {
                                                {empty, " "},
                                                {plant, "P"},
                                                {herbivore, "H"},
                                                {carnivore, "C"},
                                            })

// Auxiliary code to convert the entity_t struct to a JSON object
namespace nlohmann
{
    void to_json(nlohmann::json &j, const entity_t &e)
    {
        j = nlohmann::json{{"type", e.type}, {"energy", e.energy}, {"age", e.age}};
    }
}

// Grid that contains the entities
static std::vector<std::vector<entity_t>> entity_grid;

entity_t newEmpty = {entity_type_t::empty, 0, 0};
entity_t newPlant = {entity_type_t::plant, 0, PLANT_MAXIMUM_AGE};
entity_t newHerbivore = {entity_type_t::herbivore, MAXIMUM_ENERGY, HERBIVORE_MAXIMUM_AGE};
entity_t newCarnivore = {entity_type_t::carnivore, MAXIMUM_ENERGY, CARNIVORE_MAXIMUM_AGE};

//Inicia o sistema com os dados colocados no inicio da simulacao
void startEcoSim(uint32_t NUM_PLANTS, uint32_t NUM_HERBV, uint32_t NUM_CARNV)
{
    for(int stop = 0; stop < NUM_PLANTS; stop++)
    {
        entity_grid[distribution(gen)][distribution(gen)] = newPlant;
    }

    for(uint32_t stop = 0,i ,j; stop < NUM_HERBV; stop++)
    {
        i = distribution(gen);
        j = distribution(gen);
        
        if(entity_grid[i][j].type == newEmpty.type)
        {
            entity_grid[i][j] = newHerbivore;
        }
        else
        {
            while(entity_grid[i][j].type == newEmpty.type)
            {
                if(j == NUM_ROWS) i++;
                j++;
            }
            entity_grid[i][j] = newHerbivore;
        }
    }

    for(uint32_t stop = 0,i ,j; stop < NUM_CARNV; stop++)
    {
        i = distribution(gen);
        j = distribution(gen);

        if(entity_grid[i][j].type == newEmpty.type)
        {
            entity_grid[i][j] = newCarnivore;
        }
        else
        {
            while(entity_grid[i][j].type == newEmpty.type)
            {
                if(j == NUM_ROWS) i++;
                j++;
            }
            entity_grid[i][j] = newCarnivore;
        }
    }
}

//Simula o envelhecimento dos seres do sistema
void ageSimulation()
{
    for (auto& street : entity_grid)
    {
        for (auto& space : street)
        {
            if(space.type != newEmpty.type) space.age--;
            if(space.age == 0) space = newEmpty;
        }
    }
}

//***PLANTA
//*
//Faz uma planta crescer em um espaço adjacente
void walk(int i, int j)
{
    if ((i + 1) < NUM_ROWS && (entity_grid[i + 1][j].type == newEmpty.type)) entity_grid[i + 1][j] = newPlant;
    else if((i - 1) >= 0 && (entity_grid[i - 1][j].type == newEmpty.type)) entity_grid[i - 1][j] = newPlant;
    else if((j - 1) >= 0 && (entity_grid[j - 1][j].type == newEmpty.type)) entity_grid[i][j - 1] = newPlant;
    else if((j + 1) < NUM_ROWS && (entity_grid[j + 1][j].type == newEmpty.type)) entity_grid[i][j + 1] = newPlant;
}

//Confere probabilidade de uma planta crescer
void plantGrowth()
{
    for (int i = 0; i < NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_ROWS; j++)
        {
            if(entity_grid[i][j].type == newPlant.type & dis(gen) < PLANT_REPRODUCTION_PROBABILITY) walk(i, j);
        }
    }
}

//***HERBIVORO
//*
//Movimentacao do herbívoro
void walkHerbv(int i, int j)
{
    if ((i + 1) < NUM_ROWS && (entity_grid[i + 1][j].type == newEmpty.type))
    {
        entity_grid[i][j].energy -= 5;
        entity_grid[i + 1][j] = entity_grid[i][j];
        entity_grid[i][j] = newEmpty;
    }
    else if((i - 1) >= 0 && (entity_grid[i - 1][j].type == newEmpty.type)) 
    {
        entity_grid[i][j].energy -= 5;
        entity_grid[i - 1][j] = entity_grid[i][j];
        entity_grid[i][j] = newEmpty;
    }
    else if((j - 1) >= 0 && (entity_grid[j - 1][j].type == newEmpty.type)) 
    {
        entity_grid[i][j].energy -= 5;
        entity_grid[i][j - 1] = entity_grid[i][j];
        entity_grid[i][j] = newEmpty;
    }
    else if((j + 1) < NUM_ROWS && (entity_grid[j + 1][j].type == newEmpty.type)) 
    {
        entity_grid[i][j].energy -= 5;
        entity_grid[i][j + 1] = entity_grid[i][j];
        entity_grid[i][j] = newEmpty;
    }
}

//Confere probabilidade de um herbivoro andar
void moveHerbv()
{
    for (int i = 0; i < NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_ROWS; j++)
        {
            if(entity_grid[i][j].type == newHerbivore.type & dis(gen) < HERBIVORE_MOVE_PROBABILITY) walkHerbv(i, j);
        }
    }
}

//Confere probabilidade de um herbivoro ou carnivoro comer e realiza a acao
void eat(int i, int j, entity_t animal, int32_t gainEnergy)
{
    if ((i + 1) < NUM_ROWS && (entity_grid[i + 1][j].type == animal.type))
    {
        entity_grid[i + 1][j] = newEmpty;
        entity_grid[i][j].energy += gainEnergy;
    }
    else if((i - 1) >= 0 && (entity_grid[i - 1][j].type == animal.type)) 
    {
        entity_grid[i - 1][j] = newEmpty;
        entity_grid[i][j].energy += gainEnergy;
    }
    else if((j - 1) >= 0 && (entity_grid[j - 1][j].type == animal.type)) 
    {
        entity_grid[i][j - 1] = newEmpty;
        entity_grid[i][j].energy += gainEnergy;
    }
    else if((j + 1) < NUM_ROWS && (entity_grid[j + 1][j].type == animal.type)) 
    {
        entity_grid[i][j + 1] = newEmpty;
        entity_grid[i][j].energy += gainEnergy;
    }
}

//Confere probabilidade de um herbivoro reproduzir e realiza a acao
void reproduceHerb(int i, int j)
{
    if ((i + 1) < NUM_ROWS && (entity_grid[i + 1][j].type == newEmpty.type) && (entity_grid[i + 1][j].type != newCarnivore.type))
    {
        entity_grid[i + 1][j] = newHerbivore;
        entity_grid[i][j].energy -= 10;
    }
    else if((i - 1) >= 0 && (entity_grid[i - 1][j].type == newEmpty.type) && (entity_grid[i - 1][j].type != newCarnivore.type)) 
    {
        entity_grid[i - 1][j] = newHerbivore;
        entity_grid[i][j].energy -= 10;
    }
    else if((j - 1) >= 0 && (entity_grid[j - 1][j].type == newEmpty.type) && (entity_grid[j - 1][j].type != newCarnivore.type)) 
    {
        entity_grid[i][j - 1] = newHerbivore;
        entity_grid[i][j].energy -= 10;
    }
    else if((j + 1) < NUM_ROWS && (entity_grid[j + 1][j].type == newEmpty.type) && (entity_grid[j + 1][j].type != newCarnivore.type)) 
    {
        entity_grid[i][j + 1] = newHerbivore;
        entity_grid[i][j].energy -= 10;
    }

    if(entity_grid[i][j].energy == 0) entity_grid[i][j] = newEmpty;
}

//***CARNIVORO
//*
void walkCarnv(int i, int j)
{
    if ((i + 1) < NUM_ROWS && (dis(gen) < CARNIVORE_MOVE_PROBABILITY))
    {
        entity_grid[i][j].energy -= 5;
        entity_grid[i + 1][j] = entity_grid[i][j];
        entity_grid[i][j] = newEmpty;
    }
    else if((i - 1) >= 0 && (dis(gen) < CARNIVORE_MOVE_PROBABILITY))
    {
        entity_grid[i][j].energy -= 5;
        entity_grid[i - 1][j] = entity_grid[i][j];
        entity_grid[i][j] = newEmpty;
    }
    else if((j - 1) >= 0 && (dis(gen) < CARNIVORE_MOVE_PROBABILITY))
    {
        entity_grid[i][j].energy -= 5;
        entity_grid[i][j - 1] = entity_grid[i][j];
        entity_grid[i][j] = newEmpty;
    }
    else if((j + 1) < NUM_ROWS && (dis(gen) < CARNIVORE_MOVE_PROBABILITY))
    {
        entity_grid[i][j].energy -= 5;
        entity_grid[i][j + 1] = entity_grid[i][j];
        entity_grid[i][j] = newEmpty;
    }
}

void moveCarnv(int i, int j)
{
    for (int i = 0; i < NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_ROWS; j++)
        {
            if(entity_grid[i][j].type == newHerbivore.type & dis(gen) < CARNIVORE_MOVE_PROBABILITY) walk(i, j);
        }
    }
}

void reproduceCarn(int i, int j)
{
    if ((i + 1) < NUM_ROWS && (entity_grid[i + 1][j].type == newEmpty.type))
    {
        entity_grid[i + 1][j] = newHerbivore;
        entity_grid[i][j].energy -= 10;
    }
    else if((i - 1) >= 0 && (entity_grid[i - 1][j].type == newEmpty.type)) 
    {
        entity_grid[i - 1][j] = newHerbivore;
        entity_grid[i][j].energy -= 10;
    }
    else if((j - 1) >= 0 && (entity_grid[j - 1][j].type == newEmpty.type)) 
    {
        entity_grid[i][j - 1] = newHerbivore;
        entity_grid[i][j].energy -= 10;
    }
    else if((j + 1) < NUM_ROWS && (entity_grid[j + 1][j].type == newEmpty.type)) 
    {
        entity_grid[i][j + 1] = newHerbivore;
        entity_grid[i][j].energy -= 10;
    }

    if(entity_grid[i][j].energy == 0) entity_grid[i][j] = newEmpty;
}



int main()
{
    crow::SimpleApp app;

    // Endpoint to serve the HTML page
    CROW_ROUTE(app, "/")
    ([](crow::request &, crow::response &res)
     {
        // Return the HTML content here
        res.set_static_file_info_unsafe("../public/index.html");
        res.end(); });

    CROW_ROUTE(app, "/start-simulation")
        .methods("POST"_method)([](crow::request &req, crow::response &res)
                                { 
        // Parse the JSON request body
        nlohmann::json request_body = nlohmann::json::parse(req.body);

       // Validate the request body 
        uint32_t total_entinties = (uint32_t)request_body["plants"] + (uint32_t)request_body["herbivores"] + (uint32_t)request_body["carnivores"];
        if (total_entinties > NUM_ROWS * NUM_ROWS) {
            res.code = 400;
            res.body = "Too many entities";
            res.end();
            return;
        }

        // Clear the entity grid
        entity_grid.clear();
        entity_grid.assign(NUM_ROWS, std::vector<entity_t>(NUM_ROWS, { empty, 0, 0}));
        
        // Create the entities
        // <YOUR CODE HERE> vector<int>::iterator it = v1.begin();
        startEcoSim((uint32_t)request_body["plants"], (uint32_t)request_body["herbivores"], (uint32_t)request_body["carnivores"]);
              
        // Return the JSON representation of the entity grid
        nlohmann::json json_grid = entity_grid; 
        res.body = json_grid.dump();
        res.end(); });

    // Endpoint to process HTTP GET requests for the next simulation iteration
    CROW_ROUTE(app, "/next-iteration")
        .methods("GET"_method)([]()
                               {
        // Simulate the next iteration
        // Iterate over the entity grid and simulate the behaviour of each entity
                /*
                PLANT_REPRODUCTION_PROBABILITY = 0.2;
                const double HERBIVORE_REPRODUCTION_PROBABILITY = 0.075;
                const double CARNIVORE_REPRODUCTION_PROBABILITY = 0.025;
                const double HERBIVORE_MOVE_PROBABILITY = 0.7;
                const double HERBIVORE_EAT_PROBABILITY = 0.9;
                const double CARNIVORE_MOVE_PROBABILITY = 0.5;
                const double CARNIVORE_EAT_PROBABILITY = 1.0;
                */
        ageSimulation();
        moveHerbv();
        //plantGrowth();

        // <YOUR CODE HERE>
        
        // Return the JSON representation of the entity grid
        nlohmann::json json_grid = entity_grid; 
        return json_grid.dump(); });
    app.port(8080).run();


    return 0;
}