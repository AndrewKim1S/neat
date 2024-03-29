# NEAT Implementation 


### source files:
neat.cpp   | main file

node_gene.h             | genes

node_gene.cpp

connection_gene.h       | genes

connection_gene.cpp

genome.h                | genotype network

genome.cpp

network.h               | neural network

network.cpp

species.h               | species 

species.cpp


### Compile Instructions:
make

./obj/a.out


### How It Works
Initialization: 
Start with a population of small, simple networks with no hidden nodes.
Evaluation: 
Each genome in the population is evaluated in the task at hand. Its performance is converted into a fitness score.
Speciation: 
The population is divided into species based on the topological and weight differences. This is done to protect innovation.
Reproduction: 
Genomes reproduce within their species, with the fitter ones having more offspring. Reproduction includes both cloning and crossover.
Mutation: 
New structure is introduced through mutations. There are three types of mutations: weight mutation, link mutation, and node mutation.
Selection: 
Only the best genomes from each species are carried over to the next generation.
Repeat: 
Steps 2-6 are repeated until an acceptable solution is found or a certain number of generations have passed.
