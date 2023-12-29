#include "genome.h"


Genome::Genome() {}

Genome::~Genome() {}

void Genome::addNode(Node n) {
	_nodes.push_back(n);
}

void Genome::addConnection(Connection c) {
	_connections.push_back(c);
}

// TODO
void Genome::mutateConnection() {
}

// TODO
void Genome::mutateNode() {
}

void Genome::printGenome() const {
	std::cout << "Nodes: ";
	for(auto& n : _nodes) {
		std::cout << n << " ";
	}
	std::cout << "\nConnections: ";
	for(auto& c : _connections) {
		std::cout << c << " ";
	}
	std::cout << "\n";
}

std::string generateDotCode(const Genome &g) {
	std::string dotCode = "digraph Network {\nrankdir=\"LR\"\nnode [shape=\"square\"]\n";
	for(auto &c : g._connections) {
		dotCode += std::to_string(c._inNodeID) + "->" +
			std::to_string(c._outNodeID) + ";\n";
	}
	dotCode += "}\n";
	return dotCode;
}

// TODO
Genome crossover(const Genome &g1, const Genome &g2) {
}
