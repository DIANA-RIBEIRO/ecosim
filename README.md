# EcoSim: Um Projeto de Simulação de Ecossistema Dinâmico

## Motivação
No mundo natural, os ecossistemas são redes complexas de interações entre vários organismos e seu ambiente. Simular um ecossistema é não apenas um problema fascinante, mas também uma oportunidade para explorar a modelagem computacional e conceitos de processamento concorrente. 

Este projeto visa fornecer um exercício envolvente e visualmente atraente que incorpora conceitos de programação essenciais, promovendo a criatividade e o pensamento crítico.

## Processo de Simulação

O processo de simulação é baseado em interações em que cada entidade (plantas, herbívoros e carnívoros) deve realizar os seguintes passos:

1. Avaliar seu entorno para tomar uma decisão.
2. Executar uma ação (crescer, mover-se, comer, reproduzir, etc.) baseada em suas próprias regras e estado.
3. Atualizar seu estado interno (como energia ou expectativa de vida).

O ecossistema avança em etapas de tempo, durante as quais todas as entidades realizam esses passos de forma concorrente. Ao fim de cada etapa, a simulação é atualizada e exibida.

## Entidades
### 1. Plantas
   - **Representação do Caractere**: 'P'
   - **Colocação Inicial**: Posicionado aleatoriamente dentro da grade com idade igual a 0.
   - **Probabilidade de Crescimento**: 20% de chance por etapa de tempo.
   - **Direção de Crescimento**: Célula vazia adjacente selecionada aleatoriamente.
   - **Regras**:
     - Se a probabilidade de crescimento aleatório for atendida, uma nova planta é criada em uma célula vazia adjacente.
     - Se nenhuma célula vazia adjacente estiver disponível, a planta não cresce.
   - **Expectativa de Vida**: Cada planta vive por 10 etapas de tempo.
   - **Decomposição**: Após 10 etapas de tempo, a planta morre e a célula fica vazia.

### 2. Herbívoros
   - **Representação do Caractere**: 'H'
   - **Colocação Inicial**: Posicionado aleatoriamente dentro da grade com energia inicial igual a 100 e idade igual 0.
   - **Probabilidade de Movimento**: 70% de chance de se mover por etapa de tempo.
   - **Direção de Movimento**: Célula adjacente selecionada aleatoriamente (excluindo células com carnívoros).
   - **Custo de Energia**: Cada movimento custa 5 unidades de energia.
   - **Probabilidade de Alimentação**: Se adjacente a uma planta, 90% de chance de comê-la.
   - **Ganho de Energia**: Ganha 30 unidades de energia ao comer uma planta.
   - **Ação de Comer**: A planta é removida e a célula fica vazia.
   - **Probabilidade de Reprodução**: 7.5% de chance de se reproduzir se a energia estiver acima de 20 unidades.
   - **Custo de Energia**: A reprodução custa 10 unidades de energia.
   - **Colocação da Prole**: Célula vazia adjacente selecionada aleatoriamente.
   - **Morte**: Se a energia chegar a 0, o herbívoro morre.
   - **Expectativa de Vida**: Cada herbívoro vive por 50 etapas de tempo (se não morrer de fome).

### 3. Carnívoros
   - **Representação do Caractere**: 'C'
   - **Colocação Inicial**: Posicionado aleatoriamente dentro da grade com energia inicial igual a 100 e idade igual 0.
   - **Probabilidade de Movimento**: 50% de chance de se mover por etapa de tempo.
   - **Direção de Movimento**: Célula adjacente selecionada aleatoriamente (incluindo células com herbívoros).
   - **Custo de Energia**: Cada movimento custa 5 unidades de energia.
   - **Probabilidade de Alimentação**: Se adjacente a um herbívoro, 100% de chance de comê-lo.
   - **Ganho de Energia**: Ganha 20 unidades de energia ao comer um herbívoro.
   - **Ação de Comer**: O herbívoro é removido e a célula fica vazia.
   - **Probabilidade de Reprodução**: 2.5% de chance de se reproduzir se a energia estiver acima de 20 unidades.
   - **Custo de Energia**: A reprodução custa 10 unidades de energia.
   - **Colocação da Prole**: Célula vazia adjacente selecionada aleatoriamente.
   - **Morte**: Se a energia chegar a 0, o carnívoro morre.
   - **Expectativa de Vida**: Cada carnívoro vive por 80 etapas de tempo (se não morrer de fome).

## Exibição

### Tecnologias Utilizadas
O projeto foi atualizado para usar uma interface web em vez de uma interface textual. O back-end é implementado em C++ usando o framework Crow para criar um serviço REST. A interface web é feita em HTML e JavaScript.

### Endpoints REST a serem Implementados

Os alunos devem implementar os seguintes endpoints REST em C++ usando o framework Crow:

1. POST /start-simulation: (Re)inicializa a simulação com números iniciais de plantas, herbívoros e carnívoros.
2. GET /next-iteration: Avança a simulação por uma etapa de tempo.


Todo o codigo referente ao processamento do body da requisição `POST /start-simulation` assim como a conversão do grid representando
o estado da simulação já está pronto, vocês só precisam implmentar a lógica de inicialização da simulação (criação das entidades e colocação inicial no grid).

Para isso vocês devem substituir os comentários `// <YOUR CODE HERE>` no arquivo `src/main.cpp`.

## Conclusão
Este projeto oferece uma jornada envolvente no mundo da modelagem e simulação computacional, combinada com habilidades práticas de programação. Através da resolução criativa de problemas e análise crítica, os alunos construirão uma representação visual dinâmica de um ecossistema, abrindo portas para uma exploração mais aprofundada em ciência da computação e no mundo natural.
