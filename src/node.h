#pragma once

#include <iostream>

/* Node class - Represents a node in a neural network
 * Node ID
 * Node Type
 */

class Node {

public:
	enum Type {
		SENSOR,
		HIDDEN,
		OUTPUT
	};

	Node();
	Node(int id, Type t);

	friend std::ostream &operator<<(std::ostream &out, const Node n);

	int _id;
	Type _type;
};
