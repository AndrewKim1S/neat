#pragma once

#include <vector>
#include <string>
#include "node.h"
#include "connection.h"

/* Genome class - It is the genotype of the neural network
 * list of node genes in the network
 * list of connection genes 
 */

class Genome {

public:
	Genome();
	~Genome();

	void addNode(Node n);
	void addConnection(Connection c);

	void mutateConnection();
	void mutateNode();

	void printGenome() const;


	std::vector<Node> _nodes;
	std::vector<Connection> _connections;

};
	
std::string generateDotCode(const Genome &g);
Genome crossover(const Genome &g1, const Genome &g2);
