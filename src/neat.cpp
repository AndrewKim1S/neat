#include <cstdlib>
#include <fstream>
#include "genome.h"
//#include <graphviz/gvc.h>
//#include <graphviz/cgraph.h>


// Globals
int g_nodeNumber;
int g_innovationNumber;


void visualizeGraph(const Genome &g) {
	std::string dotCode = generateDotCode(g);
	std::string filename = "network.dot";
	std::ofstream outputFile(filename);

	if(!outputFile.is_open()) {
		std::cout << "Error opening file\n";
		return;
	}

	outputFile << dotCode;
	outputFile.close();

	const char* cmd1 = "dot network.dot -Tpng -o output.png";
	const char* cmd2 = "xdg-open output.png";
	system(cmd1);
	system(cmd2);
}


int main(int argc, char** argv){
	srand((unsigned) time(NULL));

	g_nodeNumber = 1;
	g_innovationNumber = 1;

	NodeGene n1{1, 1, NodeGene::Type::SENSOR};
	NodeGene n2{2, 1, NodeGene::Type::SENSOR};
	NodeGene n3{3, 2, NodeGene::Type::HIDDEN};
	NodeGene n4{4, 3, NodeGene::Type::OUTPUT};

	ConnectionGene c1{1,3,0.5,true,1};
	ConnectionGene c2{3,4,0.75,true,2};

	Genome g;
	g.addNode(n1);
	g.addNode(n2);
	g.addNode(n3);
	g.addNode(n4);
	g.addConnection(c1);
	g.addConnection(c2);

	g.mutateConnection();
	//g.printGenome();
	visualizeGraph(g);

	return 0;
}


