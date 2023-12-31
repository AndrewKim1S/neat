#include "genome.h"


// Constructor & Destructor
Genome::Genome() {
	_innovationCounter = 1;
}

Genome::~Genome() {}


// Utility Functions
void Genome::addNode(NodeGene &n) {
	//_nodes.insert(n);
	_nodes.push_back(n);
}

void Genome::addConnection(ConnectionGene &c) {
	_connections.push_back(c);
	_adjacentNodes[c._inNodeID].push_back(c._outNodeID);
	_innovationCounter ++;
}


// Mutate the genome by adding random connection
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
			(_nodes[srcNodeID-1]._layer < _nodes[dstNodeID-1]._layer) &&
			(check)){ 
			found = true;
		}
	}

	_adjacentNodes[srcNodeID].push_back(dstNodeID);

	_connections.push_back(ConnectionGene(srcNodeID, dstNodeID, 1.0, true, _innovationCounter));
	_innovationCounter ++;
	return true;
}

// Mutate genome by adding a node
bool Genome::mutateNode() {
	// Generate random connection to add node
	int randIndex = 1 + (rand() % _connections.size());
	int srcNodeID = _connections[randIndex-1]._inNodeID;
	int dstNodeID = _connections[randIndex-1]._outNodeID;
	int newNodeID = _nodes.size() + 1;

	// disable original connection
	_connections[randIndex-1]._enabled = false;

	if(!(_nodes[srcNodeID-1]._layer+1 < _nodes[dstNodeID-1]._layer)) {
		adjustNodeLayer(dstNodeID);
	}
	
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

	_adjacentNodes[newNodeID].push_back(dstNodeID);
	_adjacentNodes[srcNodeID].push_back(newNodeID);

	_innovationCounter ++;
	return true;
}

// DFS on the graph to update all layers 
void Genome::adjustNodeLayer(int id) {
	// visited array must decrement node id by 1
	bool visited[_nodes.size()] = {false};

	std::stack<int> stack;
	stack.push(id);

	while(!stack.empty()) {
		int currentID = stack.top();
		stack.pop();

		if(!visited[currentID-1]) {
			_nodes[currentID-1]._layer++;
			visited[currentID-1] = true;
		}
		for(int neighbor : _adjacentNodes[currentID]) {
			if(!visited[neighbor-1]) {
				stack.push(neighbor);
			}
		}
	}
}


// Auxiliary Functions
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
	/*std::cout << "Adjacent Nodes\n";
	for (auto &pair : _adjacentNodes) {
		int key = pair.first;
		std::cout << "Key: " << key << "\n";
		for(int v : pair.second) {
			std::cout << v << " ";
		}
		std::cout << "\n";
	}*/
}

std::string generateDotCode(const Genome &g) {
	std::string dotCode = "digraph Network {\nrankdir=\"LR\"\nnode [shape=\"square\"]\n";
	for(auto &n : g._nodes) {
		dotCode += std::to_string(n._id) + ";\n";
	}
	for(auto &c : g._connections) {
		if(!c._enabled) { continue; }
		dotCode += std::to_string(c._inNodeID) + "->" +
			std::to_string(c._outNodeID) + ";\n";
	}
	dotCode += "}\n";
	return dotCode;
}


// TODO
Genome crossover(const Genome &g1, const Genome &g2) {

}



