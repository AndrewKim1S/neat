#pragma once

#include "genome.h"
#include <set>


extern const double compatabilityThreshold;

class Species {

struct GenomeComparator {
	bool operator() (Genome *g1, Genome *g2) const {
		return g1->_fitness > g2->_fitness;
	}
};

public:

	Species();
	Species(Genome* g);	// constructor with representative
	~Species();

	void add(Genome* g) { _population.insert(g); }

	std::set<Genome*, GenomeComparator> _population;
	Genome* _representative;

};

