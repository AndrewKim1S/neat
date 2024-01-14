#include "network.h"


Network::Network() {}

Network::Network(Genome &g) {
	_numLayers = 0;
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
		if(n->_layer > int(_network.size())) { 
			_network.push_back({});
			_numLayers++;
		}
		Neuron *neuron = new Neuron(n->_id);
		_network[n->_layer-1].push_back(neuron);
		_lookupHelper[n->_id] = neuron;
		if(n->_type == NodeGene::Type::OUTPUT) {
			_outputHelper.push_back(neuron);
		}
	}
	// TODO sort the neuron outputs by their id
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
			n->_output = Network::Neuron::activationFunction(summation + n->_bias);
		}
	}
}


// Backpropogation TODO
// Assumes that targets parameter vector is ordered in the same way as 
// layer output
void Network::backPropogation(const std::vector<double> &targets) {
	// Backpropogate error to all neurons
	// reverse traverse the layers from output to input
	for(int i = _numLayers-1; i >= 0; i--) {
		Layer &current = _network[i];
	
		// iterate through every neuron in the layer
		for(size_t n = 0; n < current.size(); ++n) {
			double error = 0.0;
		
			// calculate error for output 
			if(i == _numLayers-1) {
				// TODO may need fixing
				error = targets[n] - current[n]->_output;
			} 

			// calculate error for hidden
			else {
				// get all the connections that lead out of node
				for(auto &j : _bckAdjacent[current[n]->_id]) {
					// Define weight connection 
					Connection c{current[n]->_id, j};
					error += (_weights[c] * _lookupHelper[j]->_error);
				}
			}
			// set the error for each neuron
			current[n]->_error = error * 
				Neuron::activationFunctionDerivative(current[n]->_output);
		}
	}

	// Update weights of network
	for(int i = 0; i < _numLayers; i++) {
		Layer &current = _network[i];
		// input layer only is different
		for(auto &n : current) {
			// update weights x -> n
			for(auto &j : _fwdAdjacent[n->_id]) {
				Connection c{j, n->_id};
				_weights[c] += _learningRate * 
					_lookupHelper[n->_id]->_error * 
					_lookupHelper[j]->_output;
			}
			// update bias
			_lookupHelper[n->_id]->_bias += _learningRate * 
				_lookupHelper[n->_id]->_error;
		}
	}
}


// Function to train the neural network
// Each row of the training data has the inputs & last element is expected
void Network::train(const std::vector<std::vector<double>> &trainingData, 
	const std::vector<std::vector<double>> &expectedOutput, float learningRate, 
	size_t epochs, size_t numOutputs) {

	_learningRate = learningRate;
	// Iterate over epochs
	for(size_t i = 0; i < epochs; ++i) {
		double errorSum = 0;

		// Iterate over all training data
		for(size_t j = 0; j < trainingData.size(); ++j) {
			feedForward(trainingData[j]);
			std::vector<double> outputs = std::move(getOutputs());
			for(size_t k = 0; k < numOutputs; ++k) {
				// Squared error
				errorSum += static_cast<double>(std::pow((expectedOutput[j][k] - 
					outputs[k]), 2));
			}
			backPropogation(expectedOutput[j]);
		}
		
		std::cout << "[>] epoch: " << i << ", l_rate: " << learningRate 
			<< ", error: " << errorSum << "\n";
	}
}


std::vector<double> Network::getOutputs() {
	std::vector<double> outputs;
	for(size_t i = 0; i < _outputHelper.size(); ++i) {
		outputs.push_back(_outputHelper[i]->_output);
	}
	return outputs;
}


std::ostream &operator<<(std::ostream &out, const Network &n) {
	out << "nodes:\n";
	for(size_t i = 0; i < n._network.size(); ++i) {
		for(size_t j = 0; j < n._network[i].size(); ++j) {
			out << n._network[i][j]->_id << " ";
		}
	}
	out << "\nweights:\n";
	for(auto it = n._weights.begin(); it != n._weights.end(); ++it) {
		out << it->first.first << ", " << it->first.second << ": " << 
			it->second << "\n";
	}
	out << "biases:\n";
	for(size_t i = 0; i < n._network.size(); ++i) {
		for(size_t j = 0; j < n._network[i].size(); ++j) {
			out << n._network[i][j]->_id << ": " << n._network[i][j]->_bias << "\n";
		}
	}
	return out;
}


