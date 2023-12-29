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
	g_nodeNumber = 1;
	g_innovationNumber = 1;

	Node n1{1, Node::Type::SENSOR};
	Node n2{2, Node::Type::SENSOR};
	Node n3{3, Node::Type::HIDDEN};

	Connection c1{1,3,0.5,true,1};
	Connection c2{2,3,0.75,true,2};

	Genome g;
	g.addNode(n1);
	g.addNode(n2);
	g.addNode(n3);
	g.addConnection(c1);
	g.addConnection(c2);

	g.printGenome();
	visualizeGraph(g);

	return 0;
}


