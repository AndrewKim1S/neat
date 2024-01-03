#include <cstdlib>
#include <fstream>
#include "genome.h"
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

	Genome test;
	test.addNode(1,1,NodeGene::Type::SENSOR);
	test.addNode(2,1,NodeGene::Type::SENSOR);
	test.addNode(3,2,NodeGene::Type::HIDDEN);
	test.addNode(4,3,NodeGene::Type::OUTPUT);
	test.addConnection(1,3,0.5,true,1);
	test.addConnection(3,4,0.7,true,2);
	test.addConnection(2,3,0.6,true,3);

	Genome test2;

	std::cout << "Original Network test 1\n";
	visualizeGraph(test);
	test.printGenome();
	
	std::cout << "Original Network test 2\n";
	visualizeGraph(test2);
	test2.printGenome();
	
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

	//crossover(test, test2);

	return 0;
}


