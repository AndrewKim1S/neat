#include "network.h"


Network::Network() {}

Network::Network(Genome &g) {
	// Add genome connection genes as weights
	for(auto &c : g._connections) {
		if(c->_enabled) {
			_weights[std::pair<int, int>(c->_inNodeID, c->_outNodeID)] = c->_weight;

			_fwdAdjacent[c->_outNodeID].push_back(c->_inNodeID);
			_bckAdjacent[c->_inNodeID].push_back(c->_outNodeID);
		}
	}
	// Add genome node genes as nodes
	for(auto &n : g._nodes) {
		if(n->_layer >= int(_network.size())) { _network.push_back({}); }
		Neuron *neuron = new Neuron(n->_id);
		_network[n->_layer-1].push_back(neuron);
		_lookupHelper[n->_id] = neuron;
	}
}

Network::~Network() {
	for(auto it = _lookupHelper.begin(); it != _lookupHelper.end(); ++it) {
		delete it->second;
	}
}


void Network::feedForward(const std::vector<double> &inputs) {
	// Pass inputs directly to input (sensor) layer outputs
	for(size_t i = 0; i < _network[0].size(); ++i) {
		_network[0][i]->_output = inputs[i];
	}
	// Propogate forward for the hidden & output layers
	for(size_t i = 1; i < _network.size(); ++i) {
		Layer prevLayer = _network[i-1];
		// Loop through neuron within layers
		for(size_t j = 0; j < _network[i].size(); ++j) {
			Neuron *n = _network[i][j];
			double summation = 0;
			// Calculate the summation for each neuron 
			for(auto &k : _fwdAdjacent[n->_id]) {
				Connection c{k, n->_id};
				summation += _weights[c] * _lookupHelper[k]->_output;
			}
			// Set the neuron output 
			n->_output = Network::Neuron::activationFunction(summation);
		}
	}
}

void Network::backPropogation(const std::vector<double> &targets) {

}


// TODO may need to fix as all of the outputs may not be in the last layer
std::vector<double> Network::getOutputs() {
	std::vector<double> outputs;
	for(size_t i = 0; i < _network[_network.size()-1].size(); ++i) {
		outputs.push_back(_network[_network.size()-1][i]->_output);
	}
	return outputs;
}


std::ostream &operator<<(std::ostream &out, const Network &n) {
	out << "nodes\n";
	for(size_t i = 0; i < n._network.size(); ++i) {
		for(size_t j = 0; j < n._network[i].size(); ++j) {
			out << n._network[i][j]->_id << " ";
		}
		out << "\n";
	}
	out << "weights\n";
	for(auto it = n._weights.begin(); it != n._weights.end(); ++it) {
		out << it->first.first << ", " << it->first.second << ": " << 
			it->second << "\n";
	}
	return out;
}
