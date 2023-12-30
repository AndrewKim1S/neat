#pragma once

#include <vector>
#include <string>
#include "nodeGene.h"
#include "connectionGene.h"

/* Genome class - It is the genotype of the neural network
 * list of node genes in the network
 * list of connection genes 
 */

class Genome {

public:
	Genome();
	~Genome();

	void addNode(NodeGene &n);
	void addConnection(ConnectionGene &c);

	bool mutateConnection();
	bool mutateNode();

	void printGenome() const;


	std::vector<NodeGene> _nodes;
	std::vector<ConnectionGene> _connections;

	int _innovationCounter;
};
	
std::string generateDotCode(const Genome &g);
Genome crossover(const Genome &g1, const Genome &g2);
