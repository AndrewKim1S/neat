#include "genome.h"

// TODO change _nodes to a set

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
		
		// FIXME add layer checking & _nodes access works with sets
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
	int randIndex = 1 + (rand() % _connections.size());
	int srcNodeID = _connections[randIndex-1]._inNodeID;
	int dstNodeID = _connections[randIndex-1]._outNodeID;
	int newNodeID = _nodes.size() + 1;
	
	_connections[randIndex-1]._enabled = false;

	// FIXME change to set
	// Add node to list
	_nodes.push_back(NodeGene
		(newNodeID, _nodes[srcNodeID-1]._layer + 1, NodeGene::Type::HIDDEN));
	// create connection from src to new node
	_connections.push_back(ConnectionGene
		(srcNodeID, newNodeID, 1.0, true, _innovationCounter));
	_innovationCounter ++;
	// create connection from new node to dst
	_connections.push_back(ConnectionGene
		(newNodeID, dstNodeID, _connections[randIndex-1]._weight, true, _innovationCounter));
	_innovationCounter ++;
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
	for(auto &n : g._nodes) {
		dotCode += std::to_string(n._id) + ";\n";
	}
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
