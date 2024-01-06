#include <cstdlib>
#include <fstream>

#include "genome.h"
#include "network.h"
//#include <graphviz/gvc.h>
//#include <graphviz/cgraph.h>


// Globals
int g_nodeNumber;
int g_innovationNumber;
int g_visualizeNumber;


void visualizeGraph(const Genome &g, const std::string dir = "util/") {
	std::string dotCode = generateDotCode(g);
	std::string dotFilename = dir + "network" + std::to_string(g_visualizeNumber) + ".dot";
	std::string pngFilename = dir + "output" + std::to_string(g_visualizeNumber) + ".png";
	std::ofstream outputFile(dotFilename);

	if(!outputFile.is_open()) {
		std::cout << "Error opening file\n";
		return;
	}

	outputFile << dotCode;
	outputFile.close();

	std::string cmd1 = "dot " + dotFilename + " -Tpng -o " + pngFilename;
	std::string cmd2 = "xdg-open " + pngFilename;
	system(cmd1.c_str());
	system(cmd2.c_str());
	g_visualizeNumber ++;
}


int main(int argc, char** argv){
	srand((unsigned) time(NULL));

	g_nodeNumber = 1;
	g_innovationNumber = 1;
	g_visualizeNumber = 1;

	/* When Constructing the base network manually like so, it is ok to assume
	 * that the layer numbers (nodes) and innovation numbers for (connections) are
	 * all correct
	*/

	// MAKE SURE THAT INNOVATIONS FOR DIFFERENT GENOMES ARE SORTED ACCORDINGLY
	Genome test;
	test.addNode(1,1,NodeGene::Type::SENSOR);
	test.addNode(2,1,NodeGene::Type::SENSOR);
	test.addNode(3,2,NodeGene::Type::HIDDEN);
	test.addNode(4,3,NodeGene::Type::OUTPUT);
	test.addConnection(1,3,1,true,1);
	test.addConnection(3,4,1,true,2);
	test.addConnection(2,3,1,true,3);

	Genome test2;
	test2.addNode(1,1,NodeGene::Type::SENSOR);
	test2.addNode(2,1,NodeGene::Type::SENSOR);
	test2.addNode(3,2,NodeGene::Type::HIDDEN);
	test2.addNode(4,3,NodeGene::Type::OUTPUT);
	test2.addConnection(1,3,2,true,1);
	test2.addConnection(3,4,2,true,2);
	test2.addConnection(2,3,2,true,3);

	test._fitness = 1;
	test2._fitness = 2;

	// Genome test

	std::cout << "Original Genome test 1\n";
	test.mutateNode();
	test.printGenome();
	visualizeGraph(test);
	
	/*test2._innovationCounter = 6;
	std::cout << "Original Network test 2\n";
	test2.mutateConnection();
	test2.printGenome();
	visualizeGraph(test2);*/
	
	/*std::cout << "Mutated Node\n";
	test.mutateNode();
	test.printGenome();
	visualizeGraph(test);
	std::cout << "Mutated Connection\n";
	test.mutateConnection();
	visualizeGraph(test);
	test.printGenome();
	std::cout << "Mutated Node\n";
	test.mutateNode();
	visualizeGraph(test);
	test.printGenome();*/

	/*
	Genome result = crossover(test, test2);
	std::cout << "Crossover result\n";
	visualizeGraph(result);
	result.printGenome();*/

	// Neural Network test
	Network networkTest = Network(test);
	std::cout << "Original Network test\n";
	std::cout << networkTest;

	return 0;
}


