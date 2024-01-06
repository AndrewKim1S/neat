#pragma once

#include "genome.h"

/*
 */

class Network {

	// Neuron structure
	struct Neuron {
		Neuron(size_t id) : _id(id), _bias(0) , _output(0) {};

		size_t _id;
		double _bias;
		double _output;
		double _gradient;

		private:
			static double activationFunction(double x) {
				return 0;
			}

			static double activationFunctionDerivative(double x) {
				return 0;
			}
	};

	typedef std::vector<Neuron> Layer;
	typedef std::pair<size_t, size_t> Connection;

public:
	Network();
	Network(Genome &g);
	~Network();

	void feedForward(const std::vector<double> &inputs);
	void backPropogation(const std::vector<double> &targets);

	std::vector<double> getOutputs();

	friend std::ostream &operator<<(std::ostream &out, const Network &n);

private:
	size_t _numLayers;
	size_t _numNuerons;

	std::vector<Layer> _network;
	std::map<Connection, double> _weights;

	double _learningRate = 0.1;
};


