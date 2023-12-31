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

	visualizeGraph(g);
	//g.mutateConnection();
	g.mutateNode();
	visualizeGraph(g);
	g.printGenome();

	return 0;
}


