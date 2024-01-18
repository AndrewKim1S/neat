#include "species.h"


Species::Species() {}

Species::Species(Genome* g) {
	_representative = g;
}

Species::~Species() {}
