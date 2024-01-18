#pragma once

#include <vector>
#include <string>
#include <stack>
#include <map>
#include <algorithm>
#include "node_gene.h"
#include "connection_gene.h"


// Globals for speciation
extern const double g_c1;
extern const double g_c2;
extern const double g_c3;

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

	// genome
	void addNode(int id, int layer, NodeGene::Type type);
	void addConnection(int in, int out, double weight, bool enabled, int inno);

	// Mutations
	ConnectionGene* mutateConnection();
	std::pair<ConnectionGene*, ConnectionGene*> mutateNode();

	void printGenome() const;

	friend double compatabilityDistance(Genome &g1, Genome &g2);

	// Variables
	std::vector<NodeGene*> _nodes;
	std::vector<ConnectionGene*> _connections;

	double _fitness;
	double _explicitFitnessSharing;

	int _innovationCounter;

private:
	void adjustNodeLayer(int id);

	std::map<int,std::vector<int>> _adjacentNodes;
};
	

std::string generateDotCode(const Genome &g);
int findConnection(std::vector<ConnectionGene*> &list, int inn);
Genome crossover(Genome &g1, Genome &g2);
