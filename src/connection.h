#pragma once

#include <iostream>

/* Connection class - Represents connection between two network nodes
 * Node IDs
 * weight
 * enabled
 * innovation number
 */

class Connection {

public:
	Connection();
	Connection(int in, int out, double weight,
		bool enabled, int inn);

	friend std::ostream &operator<<(std::ostream &out, const Connection &c);
	
	int _inNodeID;
	int _outNodeID;
	double _weight;
	bool _enabled;
	int _innovationNumber;
	
};

