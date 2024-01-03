#pragma once

#include <iostream>

/* Connection class - Represents connection between two network nodes
 * Node IDs
 * weight
 * enabled
 * innovation number
 */

class ConnectionGene {

public:
	ConnectionGene();
	ConnectionGene(int in, int out, double weight,
		bool enabled, int inn);
	ConnectionGene(const ConnectionGene &c);

	friend std::ostream &operator<<(std::ostream &out, const ConnectionGene &c);
	
	int _inNodeID;
	int _outNodeID;
	double _weight;
	bool _enabled;
	int _innovationNumber;
	
};

