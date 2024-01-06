#include "network.h"


Network::Network() {}

Network::Network(Genome &g) {
	// Add genome connection genes as weights
	for(auto &c : g._connections) {
		_weights[std::pair<int, int>(c->_inNodeID, c->_outNodeID)] = c->_weight;
	}
	// Add genome node genes as nodes
	for(auto &n : g._nodes) {
		if(n->_layer >= _network.size()) { _network.push_back({}); }
		_network[n->_layer-1].push_back(Neuron(n->_id));
	}
}

Network::~Network() {}


std::ostream &operator<<(std::ostream &out, const Network &n) {
	for(size_t i = 0; i < n._network.size(); ++i) {
		for(size_t j = 0; j < n._network[i].size(); ++j) {
			out << n._network[i][j]._id << " ";
		}
		out << "\n";
	}
	return out;
}
