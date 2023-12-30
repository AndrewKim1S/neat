#pragma once

#include <iostream>

/* Node class - Represents a node in a neural network
 * Node ID
 * Node Type
 */

class NodeGene {

public:
	enum Type {
		SENSOR,
		HIDDEN,
		OUTPUT
	};

	NodeGene();
	NodeGene(int id, int layer, Type t);

	friend std::ostream &operator<<(std::ostream &out, const NodeGene n);

	int _id;
	int _layer;
	Type _type;
};
