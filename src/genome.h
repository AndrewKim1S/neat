#pragma once

#include <vector>
#include <string>
#include <stack>
#include <map>
#include <algorithm>
#include "nodeGene.h"
#include "connectionGene.h"

/* Genome class - It is the genotype of the neural network
 * list of node genes in the network (sorted by id)
 *list of connection genes (sorted by innovation number)
 */

/* _nodes holds node by id, index is id decrement by 1
 * _adjacentNodes holds node id as key, does not need to be decremented
 */

class Genome {

public:
	Genome();
	Genome(std::vector<NodeGene*>& nodes, 
		std::vector<ConnectionGene*>& connections);
	~Genome();

	void addNode(int id, int layer, NodeGene::Type type);
	void addConnection(int in, int out, double weight, bool enabled, int inno);

	ConnectionGene* mutateConnection();
	std::pair<ConnectionGene*, ConnectionGene*> mutateNode();

	void printGenome() const;

	std::vector<NodeGene*> _nodes;
	std::vector<ConnectionGene*> _connections;

	double _fitness;

	int _innovationCounter;

private:
	void adjustNodeLayer(int id);

	std::map<int,std::vector<int>> _adjacentNodes;
};
	

std::string generateDotCode(const Genome &g);
Genome crossover(Genome &g1, Genome &g2);
