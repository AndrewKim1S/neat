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


/*
 * Function to visualize the genome or network in graphviz
 */
void visualizeGraph(const Genome &g, const std::string dir = "util/") {
	std::string dotCode = generateDotCode(g);
	std::string dotFilename = dir + "network" + 
		std::to_string(g_visualizeNumber) + ".dot";
	std::string pngFilename = dir + "output" + 
		std::to_string(g_visualizeNumber) + ".png";
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


/*
 * Function to run the XOR problem on neural network
 */
void XORproblem() {
	// Testing basic neural network on the xor problem
	// Create genome: has 2 input, 2 hidden, 1 output
	Genome xorTest;
	xorTest.addNode(1,1,NodeGene::SENSOR);
	xorTest.addNode(2,1,NodeGene::SENSOR);
	xorTest.addNode(3,2,NodeGene::HIDDEN);
	xorTest.addNode(4,2,NodeGene::HIDDEN);
	xorTest.addNode(5,3,NodeGene::OUTPUT);
	xorTest.addConnection(1,3,2,true,1);
	xorTest.addConnection(1,4,2,true,2);
	xorTest.addConnection(2,3,2,true,3);
	xorTest.addConnection(2,4,2,true,4);
	xorTest.addConnection(3,5,2,true,5);
	xorTest.addConnection(4,5,2,true,6);
	
	visualizeGraph(xorTest);
	std::cout << "XOR network genome:\n";
	xorTest.printGenome();

	Network xorNet = Network(xorTest);

	// Train the network
	// The indexes match up the input to the output
	std::vector<std::vector<double>> inputSet;
	std::vector<std::vector<double>> solutionSet;
	inputSet.push_back({0,0});
	solutionSet.push_back({0});
	inputSet.push_back({0,1});
	solutionSet.push_back({1});
	inputSet.push_back({1,0});
	solutionSet.push_back({1});
	inputSet.push_back({1,1});
	solutionSet.push_back({0});

	std::cout << "\nNetwork Before Training:\n" << xorNet;

	for(int i = 0; i < 10000; i++) {
		int randIndex = rand() % 4;
		xorNet.feedForward(inputSet[randIndex]);
		xorNet.backPropogation(solutionSet[randIndex]);
	}
	
	std::cout << "\nNetwork After Training:\n" << xorNet;

	// Test accuracy
	std::cout << "\nTesting Network Accuracy:\n";
	for(int i = 0; i < 4; i++) {
		xorNet.feedForward(inputSet[i]);
		std::cout << "Inputs: ";
		for(auto input : inputSet[i]) { std::cout << input << " "; }
		std::cout << "Outputs: ";
		for(auto output : xorNet.getOutputs()) {
			std::cout << output << " ";
		}
		std::cout << "\n";
	}
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
	/*Genome test;
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
	visualizeGraph(test);*/
	
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


	// This is tentative testing of neural net
	// Neural Network test
	
	/*Network networkTest = Network(test);
	std::cout << "Original Network\n";
	std::cout << networkTest;

	std::cout << "Test fwd & back propogation -------\n";
	for(int i = 0; i < 100; ++i) {
		networkTest.feedForward({2,2});
		networkTest.backPropogation({5});
	}
	std::cout << networkTest;
	networkTest.feedForward({2,2});

	std::cout << "Output\n";
	auto output = networkTest.getOutputs();
	for(double x : output) {
		std::cout << x << " ";
	}
	std::cout << "\n";*/

	XORproblem();

	return 0;
}


