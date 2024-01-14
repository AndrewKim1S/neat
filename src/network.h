#pragma once

#include "genome.h"
#include <math.h>

/* Network class - contains neural network that is based off of genotype of 
 * the network.
 * Important note: The inputs and the outputs of the network are constant
 */

class Network {

	// Neuron structure
	struct Neuron {
		Neuron(size_t id) : _id(id), _output(0) {
			_bias = ((double)rand() / RAND_MAX);
		};

		size_t _id;
		double _bias;
		double _output;
		double _gradient;
		double _error;

		static double activationFunction(double x) {
			//return std::max(0.0, x); // ReLu
			return 1.0 / (1.0 + exp(-x)); // sigmoid
		}

		static double activationFunctionDerivative(double x) {
			/*if(x < 0) {
				return 0.0;
			} else {
				return 1.0;
			}*/
			return x * (1.0-x);
		}
	};

	typedef std::vector<Neuron*> Layer;
	typedef std::pair<int, int> Connection;

public:
	Network();
	Network(Genome &g);
	~Network();

	// Both input & target vectors must be sorted by id
	void feedForward(const std::vector<double> &inputs);
	void backPropogation(const std::vector<double> &targets);

	std::vector<double> getOutputs();

	friend std::ostream &operator<<(std::ostream &out, const Network &n);

private:
	int _numLayers;
	size_t _numNuerons;

	std::vector<Layer> _network;
	std::map<Connection, double> _weights;

	// index is node vector holds input nodes
	std::map<int, std::vector<int>> _fwdAdjacent; 
	// index is node vector holds output nodes
	std::map<int, std::vector<int>> _bckAdjacent;

	// helper table of neurons
	std::map<int, Neuron*> _lookupHelper;

	// helper list of output neurons
	std::vector<Neuron*> _outputHelper;

	double _learningRate = 0.5;

};


