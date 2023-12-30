#include "connectionGene.h"


ConnectionGene::ConnectionGene() {}

ConnectionGene::ConnectionGene(int in, int out, double weight, 
	bool enabled, int inn) {
	_inNodeID = in;
	_outNodeID = out;
	_weight = weight;
	_enabled = enabled;
	_innovationNumber = inn;
}

std::ostream& operator<<(std::ostream &out, const ConnectionGene &c) {
	out << c._inNodeID << "," << c._outNodeID << "," << c._weight 
		<< "," << c._enabled << "," << c._innovationNumber << " ";
	return out;
}
