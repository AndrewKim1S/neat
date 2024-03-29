#include "genome.h"


// Define globals
const double g_c1 = 1;
const double g_c2 = 1;
const double g_c3 = 1;


// Constructor & Destructor
Genome::Genome() {
	_innovationCounter = 1;
}

Genome::Genome(std::vector<NodeGene*>& nodes, 
	std::vector<ConnectionGene*>& connections) : 
	_nodes(nodes), _connections(connections) {}

Genome::~Genome() {
	for(auto &n : _nodes) {
		delete n;
	}
	for(auto &c : _connections) {
		delete c;
	}
}


// Utility Functions
void Genome::addNode(int id, int layer, NodeGene::Type type) {
	NodeGene *n = new NodeGene(id, layer, type);
	_nodes.push_back(n);
}


void Genome::addConnection(int in, int out, double weight, bool enabled,
	int inno) {
	ConnectionGene *c = new ConnectionGene(in, out, weight, enabled, inno);
	_connections.push_back(c);
	_adjacentNodes[c->_inNodeID].push_back(c->_outNodeID);
	_innovationCounter ++;
}


// Mutate the genome by adding random connection
ConnectionGene* Genome::mutateConnection() {
	int srcNodeID;
	int dstNodeID;
	bool found = false;
	while (!found) {
		bool check = true;
		srcNodeID = 1 + (rand() % _nodes.size());
		dstNodeID = 1 + (rand() % _nodes.size());
		
		for(auto &co : _connections) {
			if((co->_inNodeID == srcNodeID && co->_outNodeID == dstNodeID) || 
				(co->_inNodeID == dstNodeID && co->_outNodeID == srcNodeID)) {
				check = false;
			}
		}
		
		if((_nodes[dstNodeID - 1]->_type != NodeGene::Type::SENSOR) &&
			(srcNodeID != dstNodeID) &&
			(_nodes[srcNodeID-1]->_layer < _nodes[dstNodeID-1]->_layer) &&
			(check)){ 
			found = true;
		}
}

	_adjacentNodes[srcNodeID].push_back(dstNodeID);
	ConnectionGene *newc = new ConnectionGene(srcNodeID, dstNodeID, 1.0, true,
		_innovationCounter);
	_connections.push_back(newc);
	_innovationCounter ++;
	return newc;
}


// Mutate genome by adding a node
std::pair<ConnectionGene*, ConnectionGene*> Genome::mutateNode() {
	// Generate random connection to add node
	int randIndex = 1 + (rand() % _connections.size());
	int srcNodeID = _connections[randIndex-1]->_inNodeID;
	int dstNodeID = _connections[randIndex-1]->_outNodeID;
	int newNodeID = _nodes.size() + 1;

	// disable original connection
	_connections[randIndex-1]->_enabled = false;

	if(!(_nodes[srcNodeID-1]->_layer+1 < _nodes[dstNodeID-1]->_layer)) {
		adjustNodeLayer(dstNodeID);
	}
	
	// Add node to list
	NodeGene *newn = new NodeGene(newNodeID, _nodes[srcNodeID-1]->_layer + 1, 
		NodeGene::Type::HIDDEN);
	_nodes.push_back(newn);

	// create connection from src to new node
	ConnectionGene *newc1 = new ConnectionGene(srcNodeID, newNodeID, 1.0, true, 
		_innovationCounter);
	_connections.push_back(newc1);
	_innovationCounter ++;

	// create connection from new node to dst
	ConnectionGene *newc2 = new ConnectionGene(newNodeID, dstNodeID, 
		_connections[randIndex-1]->_weight, true, _innovationCounter);
	_connections.push_back(newc2);

	_adjacentNodes[newNodeID].push_back(dstNodeID);
	_adjacentNodes[srcNodeID].push_back(newNodeID);

	_innovationCounter ++;
	return std::pair<ConnectionGene*, ConnectionGene*>(newc1, newc2);
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
			_nodes[currentID-1]->_layer++;
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
		std::cout << *n << " ";
	}
	std::cout << "\nConnections: ";
	for(auto& c : _connections) {
		std::cout << *c << " ";
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


// Generate dot code to display graph in graphviz
std::string generateDotCode(const Genome &g) {
	std::string dotCode = "digraph Network {\nrankdir=\"LR\"\nnode [shape=\"square\"]\n";
	for(auto &n : g._nodes) {
		dotCode += std::to_string(n->_id) + ";\n";
	}
	for(auto &c : g._connections) {
		if(!c->_enabled) { continue; }
		dotCode += std::to_string(c->_inNodeID) + "->" +
			std::to_string(c->_outNodeID) + ";\n";
	}
	dotCode += "}\n";
	return dotCode;
}


// Lambda function for binary search to find gene
int findConnection(std::vector<ConnectionGene*> &list, int inn){
	int low = 0;
	int high = list.size()-1;
	while(low <= high) {
		int mid = low + (high - low)/2;
		if(list[mid]->_innovationNumber == inn) {
			return mid;
		} else if (list[mid]->_innovationNumber < inn) {
			low = mid+1;
		} else {
			high = mid-1;
		}
	}
	return -1;
};


// TODO NEED UPDATE!
// Make sure that the same nodes in diff layers are handled appropriately
// Preset chance that disabled genes become enabled again
// Remove false connections
Genome crossover(Genome &g1, Genome &g2) {
	int totalNumberInn = std::max(g1._innovationCounter, g2._innovationCounter);

	// visited array must decrement node id by 1
	bool visited[std::max(g1._innovationCounter, g2._innovationCounter)] = {false};

	std::vector<ConnectionGene*> newConnections;
	std::vector<NodeGene*> newNodes; 

	// List to keep track of disjoint and excess genes
	std::vector<int> g1DisjointExcessIndexes;
	std::vector<int> g2DisjointExcessIndexes;

	// Binary search on both genomes
	for(int i = 1; i <= totalNumberInn; ++i){
		int g1Index = findConnection(g1._connections, i);
		int g2Index = findConnection(g2._connections, i);

		// Matching genes
		if((g1Index != -1) && (g2Index != -1)) {
			ConnectionGene* newc;
			Genome* chosen;
			if(rand() % 2 == 0) {
				chosen = &g1;
				newc = new ConnectionGene(*g1._connections[g1Index]);
			} else {
				chosen = &g2;
				newc = new ConnectionGene(*g2._connections[g2Index]);
			}
			newConnections.push_back(newc);

			// Keep track of nodes
			int src = newc->_inNodeID;
			if(!visited[src-1]) {
				NodeGene* newn = new NodeGene(*chosen->_nodes[src-1]);
				newNodes.push_back(newn);
				visited[src-1] = true;
			}
			int dst = newc->_outNodeID;
			if(!visited[dst-1]) {
				NodeGene* newn = new NodeGene(*chosen->_nodes[dst-1]);
				newNodes.push_back(newn);
				visited[dst-1] = true;
			}
		} 
		
		// Disjoint or excess genes
		else if((g1Index != 1) && (g2Index == -1)){
			g1DisjointExcessIndexes.push_back(g1Index);
		}
		else if((g2Index != 1) && (g1Index == -1)){
			g2DisjointExcessIndexes.push_back(g2Index);
		}
	}

	auto addAllConnections = [&](Genome &g, std::vector<int> &indexes){
		for(auto &c : indexes) {
			ConnectionGene* newc = new ConnectionGene(
				*g._connections[c]);
			newConnections.push_back(newc);
			// Keep track of nodes
			int src = newc->_inNodeID;
			if(!visited[src-1]) {
				NodeGene* newn = new NodeGene(*g._nodes[src-1]);
				newNodes.push_back(newn);
			}
			int dst = newc->_outNodeID;
			if(!visited[dst-1]) {
				NodeGene* newn = new NodeGene(*g._nodes[dst-1]);
				newNodes.push_back(newn);
			}
		}
	};

	// Check fitness of genomes & add rest of connections
	if(g1._fitness > g2._fitness) {
		addAllConnections(g1, g1DisjointExcessIndexes);
	} else if (g2._fitness > g1._fitness) {
		addAllConnections(g2, g2DisjointExcessIndexes);
	} else {
		// g1 and g2 are equally fit
		size_t g1Index = 0;
		size_t g2Index = 0;
		while(g1Index < g1DisjointExcessIndexes.size() || 
			g2Index < g2DisjointExcessIndexes.size()) {
			ConnectionGene* newc;
			Genome *chosen;
			// If there are only g2 excess genes remaining
			if(g1Index >= g1DisjointExcessIndexes.size()) {
				newc = new ConnectionGene(
					*g2._connections[g2DisjointExcessIndexes[g2Index]]);
				g2Index++;
				chosen = &g2;
			}
			// If there are only g1 excess genes remaining
			else if(g2Index >= g2DisjointExcessIndexes.size()) {
				newc = new ConnectionGene(
					*g1._connections[g1DisjointExcessIndexes[g1Index]]);
				g1Index++;
				chosen = &g1;
			}
			// If there are both g1 & g2 disjoint nodes
			else {
				if(g1DisjointExcessIndexes[g1Index] > g2DisjointExcessIndexes[g2Index]) {
					newc = new ConnectionGene(
						*g2._connections[g2DisjointExcessIndexes[g2Index]]);
					g2Index++;
					chosen = &g2;
				} else {
					newc = new ConnectionGene(
						*g1._connections[g1DisjointExcessIndexes[g1Index]]);
					g1Index++;
					chosen = &g1;
				}
			}

			// Keep track of nodes
			int src = newc->_inNodeID;
			if(!visited[src-1]) {
				NodeGene* newn = new NodeGene(*chosen->_nodes[src-1]);
				newNodes.push_back(newn);
				visited[src-1] = true;
			}
			int dst = newc->_outNodeID;
			if(!visited[dst-1]) {
				NodeGene* newn = new NodeGene(*chosen->_nodes[dst-1]);
				newNodes.push_back(newn);
				visited[dst-1] = true;
			}
			newConnections.push_back(newc);
		}
	}

	// Sort the connections by innovation number & nodes lists by id
	std::sort(newNodes.begin(), newNodes.end(), 
		[](const NodeGene* a, const NodeGene* b) {
		return a->_id < b->_id;
	});
	
	std::sort(newConnections.begin(), newConnections.end(), 
		[](const ConnectionGene* a, const ConnectionGene* b) {
		return a->_innovationNumber < b->_innovationNumber;
	});

	return Genome(newNodes, newConnections);
}


/*
 * Function to calculate the compatabilityDistance between genomes
 */
double compatabilityDistance(Genome &g1, Genome &g2) {
	int totalNumberInn = std::max(g1._innovationCounter, g2._innovationCounter);
	
	double E = 0;									// Number of excess
	double D = 0;									// Number of disjoint
	double W = 0;									// weight differences
	double N = std::max(g1._nodes.size(), g2._nodes.size());
	N = (N < 20) ? 1 : N;
	
	int numMatch = 0;

	// Find the matching genes, & number of disjoint and excess genes
	for(int i = 1; i <= totalNumberInn; ++i) {
		int g1Index = findConnection(g1._connections, i);
		int g2Index = findConnection(g2._connections, i);

		// Matching genes
		if((g1Index != -1) && (g2Index != -1)) {
			++numMatch;
			W += std::abs(g1._connections[g1Index]->_weight
				- g2._connections[g2Index]->_weight);
		}	
		else if(i > g1._innovationCounter || i > g2._innovationCounter) {
			E++;
		}
		else { D++; }
	}

	W /= numMatch;

	std::cout << "N: " << N << " E: " << E << " D: " << D << " W: " << W << std::endl;

	double delta = (g_c1 * E)/N + (g_c2 * D)/N + (g_c3 * W);
	return delta;
}


