#include "node.h"


Node::Node() {}

Node::Node(int id, Type t) {
	_id = id;
	_type = t;
}

std::ostream &operator<<(std::ostream &out, const Node n) {
	out << n._id << "," << n._type << " ";
	return out;
}
