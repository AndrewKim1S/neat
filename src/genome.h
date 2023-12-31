#pragma once

#include <vector>
#include <string>
#include <stack>
#include <map>
#include "nodeGene.h"
#include "connectionGene.h"

/* Genome class - It is the genotype of the neural network
 * list of node genes in the network
 * list of connection genes 
 */

/* _nodes holds node by id, index is id decrement by 1
 * _adjacentNodes holds node id as key, does not need to be decremented
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

	//std::set<NodeGene, NodeGeneCompare> _nodes;
	std::vector<NodeGene> _nodes;
	std::vector<ConnectionGene> _connections;

private:
	void adjustNodeLayer(int id);

	std::map<int,std::vector<int>> _adjacentNodes;
	int _innovationCounter;
};
	
std::string generateDotCode(const Genome &g);
Genome crossover(const Genome &g1, const Genome &g2);
