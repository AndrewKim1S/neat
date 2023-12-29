#include "connection.h"


Connection::Connection() {}

Connection::Connection(int in, int out, double weight, 
	bool enabled, int inn) {
	_inNodeID = in;
	_outNodeID = out;
	_weight = weight;
	_enabled = enabled;
	_innovationNumber = inn;
}

std::ostream& operator<<(std::ostream &out, const Connection &c) {
	out << c._inNodeID << "," << c._outNodeID << "," << c._weight 
		<< "," << c._enabled << "," << c._innovationNumber << " ";
	return out;
}
