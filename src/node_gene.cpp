#include "node_gene.h"


NodeGene::NodeGene() {}

NodeGene::NodeGene(int id, int layer, Type t) {
	_id = id;
	_layer = layer;
	_type = t;
}

NodeGene::NodeGene(const NodeGene &n): _id(n._id), _layer(n._layer), 
	_type(n._type) {}

std::ostream &operator<<(std::ostream &out, const NodeGene n) {
	out << n._id << "," << n._layer << " ";
	return out;
}
