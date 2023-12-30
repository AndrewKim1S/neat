#include "nodeGene.h"


NodeGene::NodeGene() {}

NodeGene::NodeGene(int id, int layer, Type t) {
	_id = id;
	_layer = layer;
	_type = t;
}

std::ostream &operator<<(std::ostream &out, const NodeGene n) {
	out << n._id << "," << n._type << " ";
	return out;
}
