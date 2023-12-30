#include "genome.h"


Genome::Genome() {
	_innovationCounter = 1;
}

Genome::~Genome() {}

void Genome::addNode(NodeGene &n) {
	_nodes.push_back(n);
}

void Genome::addConnection(ConnectionGene &c) {
	_connections.push_back(c);
	_innovationCounter ++;
}

// TODO
// check node layer so connections go forward
bool Genome::mutateConnection() {
	int srcNodeID;
	int dstNodeID;
	bool found = false;
	while (!found) {
		bool check = true;
		srcNodeID = 1 + (rand() % _nodes.size());
		dstNodeID = 1 + (rand() % _nodes.size());
		
		for(auto &co : _connections) {
			if((co._inNodeID == srcNodeID && co._outNodeID == dstNodeID) || 
				(co._inNodeID == dstNodeID && co._outNodeID == srcNodeID)) {
				check = false;
			}
		}
		
		if((_nodes[dstNodeID - 1]._type != NodeGene::Type::SENSOR) &&
			(srcNodeID != dstNodeID) &&
			(check)){ 
			found = true;
		}
	}

	_connections.push_back(ConnectionGene(srcNodeID, dstNodeID, 1.0, true, _innovationCounter));
	_innovationCounter ++;
	return true;
}

// TODO
bool Genome::mutateNode() {
	return true;
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
