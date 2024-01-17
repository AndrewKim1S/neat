#include <cstdlib>
#include <fstream>
#include <regex>
#include <iomanip>

#include "genome.h"
#include "network.h"


// Globals
int g_nodeNumber;
int g_innovationNumber;
int g_visualizeNumber;


// Function forward declaration
void SeedsNetTest();
void XORNetTest();
void visualizeGraph(const Genome &g, const std::string dir = "util/");
std::vector<std::vector<double>> loadData(const std::string filename, 
	const std::string dir = "src/");


/*
 * Main function
 */
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
	Genome test1;
	test1.addNode(1,1,NodeGene::SENSOR);
	test1.addNode(2,1,NodeGene::SENSOR);
	test1.addNode(3,1,NodeGene::SENSOR);
	test1.addNode(4,3,NodeGene::OUTPUT);
	test1.addNode(5,2,NodeGene::HIDDEN);
	test1.addConnection(1,4,1,true,1);
	test1.addConnection(2,4,1,false,2);
	test1.addConnection(3,4,1,true,3);
	test1.addConnection(2,5,1,true,4);
	test1.addConnection(5,4,1,true,5);
	test1.addConnection(1,5,1,true,8);
	visualizeGraph(test1);

	Genome test2;
	test2.addNode(1,1,NodeGene::SENSOR);
	test2.addNode(2,1,NodeGene::SENSOR);
	test2.addNode(3,1,NodeGene::SENSOR);
	test2.addNode(4,4,NodeGene::OUTPUT);
	test2.addNode(5,2,NodeGene::HIDDEN);
	test2.addNode(6,3,NodeGene::HIDDEN);
	test2.addConnection(1,4,2,true,1);
	test2.addConnection(2,4,2,false,2);
	test2.addConnection(3,4,2,true,3);
	test2.addConnection(2,5,2,true,4);
	test2.addConnection(5,4,2,false,5);
	test2.addConnection(5,6,2,true,6);
	test2.addConnection(6,4,2,true,7);
	test2.addConnection(3,5,2,true,9);
	test2.addConnection(1,6,2,true,10);
	visualizeGraph(test2);

	compatabilityDistance(test1, test2);

	// For Neural Network Testing
	// XORproblem();
	// SeedsNetTest();
	
	return 0;
}


/*
 * Function to run the XOR problem on neural network
 */
void XORNetTest() {
	// Testing basic neural network on the xor problem
	// Create genome: has 2 input, 2 hidden, 1 output
	Genome xorTest;
	xorTest.addNode(1,1,NodeGene::SENSOR);
	xorTest.addNode(2,1,NodeGene::SENSOR);
	xorTest.addNode(3,2,NodeGene::HIDDEN);
	xorTest.addNode(4,2,NodeGene::HIDDEN);
	xorTest.addNode(5,3,NodeGene::OUTPUT);
	xorTest.addConnection(1,3,0.5,true,1);
	xorTest.addConnection(1,4,0.9 ,true,2);
	xorTest.addConnection(2,3,0.4 ,true,3);
	xorTest.addConnection(2,4,0.3 ,true,4);
	xorTest.addConnection(3,5,0.2 ,true,5);
	xorTest.addConnection(4,5,0.1 ,true,6);
	
	visualizeGraph(xorTest);
	std::cout << "----- XOR network genome -----\n";
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

	std::cout << "\n----- Network Before Training -----\n" << xorNet;

	xorNet.train(inputSet, solutionSet, 0.3, 10000, 1);
	
	std::cout << "\n----- Network After Training -----\n" << xorNet;

	// Test accuracy
	std::cout << "\n----- Testing Network Accuracy -----\n";
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


/*
 * Function to test network on the seeds dataset
 */
void SeedsNetTest() {
	std::vector<std::vector<double>> inputSet = loadData("seeds_dataset.csv");

	// I'm well aware that this is a fucking atrocity, but given that neat algo
	// usually starts with very simple topologies, this was the solution I came
	// to. This is only for purely testing the neural network
	Genome seedsTest;
	seedsTest.addNode(1,1,NodeGene::SENSOR);
	seedsTest.addNode(2,1,NodeGene::SENSOR);
	seedsTest.addNode(3,1,NodeGene::SENSOR);
	seedsTest.addNode(4,1,NodeGene::SENSOR);
	seedsTest.addNode(5,1,NodeGene::SENSOR);
	seedsTest.addNode(6,1,NodeGene::SENSOR);
	seedsTest.addNode(7,1,NodeGene::SENSOR);
	seedsTest.addNode(8,2,NodeGene::HIDDEN);
	seedsTest.addNode(9,2,NodeGene::HIDDEN);
	seedsTest.addNode(10,2,NodeGene::HIDDEN);
	seedsTest.addNode(11,2,NodeGene::HIDDEN);
	seedsTest.addNode(12,2,NodeGene::HIDDEN);
	seedsTest.addNode(13,3,NodeGene::OUTPUT);
	seedsTest.addNode(14,3,NodeGene::OUTPUT);
	seedsTest.addNode(15,3,NodeGene::OUTPUT);
	seedsTest.addConnection(1,8, 0.41, true, 1);
	seedsTest.addConnection(2,8, 0.63, true, 2);
	seedsTest.addConnection(3,8, 0.74, true, 3);
	seedsTest.addConnection(4,8, 0.92, true, 4);
	seedsTest.addConnection(5,8, 0.02, true, 5);
	seedsTest.addConnection(6,8, 0.19, true, 6);
	seedsTest.addConnection(7,8, 0.56, true, 7);
	seedsTest.addConnection(1,9, 0.77, true, 8);
	seedsTest.addConnection(2,9, 0.19, true, 9);
	seedsTest.addConnection(3,9, 0.54, true, 10);
	seedsTest.addConnection(4,9, 0.73, true, 11);
	seedsTest.addConnection(5,9, 0.89, true, 12);
	seedsTest.addConnection(6,9, 0.23, true, 13);
	seedsTest.addConnection(7,9, 0.35, true, 14);
	seedsTest.addConnection(1,10, 0.01, true, 15);
	seedsTest.addConnection(2,10, 0.63, true, 16);
	seedsTest.addConnection(3,10, 0.14, true, 17);
	seedsTest.addConnection(4,10, 0.22, true, 18);
	seedsTest.addConnection(5,10, 0.32, true, 19);
	seedsTest.addConnection(6,10, 0.49, true, 20);
	seedsTest.addConnection(7,10, 0.36, true, 21);
	seedsTest.addConnection(1,11, 0.47, true, 22);
	seedsTest.addConnection(2,11, 0.79, true, 23);
	seedsTest.addConnection(3,11, 0.04, true, 24);
	seedsTest.addConnection(4,11, 0.13, true, 25);
	seedsTest.addConnection(5,11, 0.59, true, 26);
	seedsTest.addConnection(6,11, 0.23, true, 27);
	seedsTest.addConnection(7,11, 0.56, true, 28);
	seedsTest.addConnection(1,12, 0.41, true, 29);
	seedsTest.addConnection(2,12, 0.39, true, 30);
	seedsTest.addConnection(3,12, 0.4, true, 31);
	seedsTest.addConnection(4,12, 0.3, true, 32);
	seedsTest.addConnection(5,12, 0.29, true, 33);
	seedsTest.addConnection(6,12, 0.3, true, 34);
	seedsTest.addConnection(7,12, 0.6, true, 35);

	seedsTest.addConnection(8, 13, 0.91, true, 36);
	seedsTest.addConnection(9, 13, 0.93, true, 37);
	seedsTest.addConnection(10, 13, 0.79, true, 38);
	seedsTest.addConnection(11, 13, 0.57, true, 39);
	seedsTest.addConnection(12, 13, 0.23, true, 40);
	seedsTest.addConnection(8, 14, 0.85, true, 41);
	seedsTest.addConnection(9, 14, 0.55, true, 42);
	seedsTest.addConnection(10, 14, 0.35, true, 43);
	seedsTest.addConnection(11, 14, 0.52, true, 44);
	seedsTest.addConnection(12, 14, 0.25, true, 45);
	seedsTest.addConnection(8, 15, 0.15, true, 46);
	seedsTest.addConnection(9, 15, 0.9, true, 47);
	seedsTest.addConnection(10, 15, 0.12, true, 48);
	seedsTest.addConnection(11, 15, 0.73, true, 49);
	seedsTest.addConnection(12, 15, 0.34, true, 50);

	// Create the solution set
	std::vector<std::vector<double>> solutionSet;
	for(auto &row : inputSet) {
		int result = row.back();
		switch(result) {
			case 1:
				solutionSet.push_back({1,0,0});
				break;
			case 2:
				solutionSet.push_back({0,1,0});
				break;
			case 3:
				solutionSet.push_back({0,0,1});
				break;
			default:
				break;
		}
	}

	// Visualize the network and train it
	visualizeGraph(seedsTest);
	std::cout << "----- Seeds network genome -----\n";
	seedsTest.printGenome();
	
	Network seedsNet = Network(seedsTest);

	std::cout << "\n----- Network Before Training -----\n" << seedsNet;

	seedsNet.train(inputSet, solutionSet, 0.3, 1000, 1);
	
	std::cout << "\n----- Network After Training -----\n" << seedsNet;

	// Test accuracy
	std::cout << "\n----- Testing Network Accuracy -----\n";
	for(size_t i = 0; i < inputSet.size(); i++) {
		seedsNet.feedForward(inputSet[i]);
		
		std::cout << "Inputs: ";
		for(auto input : inputSet[i]) {
			std::cout << std::fixed << std::setprecision(2) << input << " "; 
		}
		
		std::cout << "\nOutputs: ";
		for(auto output : seedsNet.getOutputs()) {
			std::cout << std::fixed << std::setprecision(2) << output << " ";
		}
		std::cout << "\n";
		std::cout << "Expected Output: ";
		std::cout << inputSet[i].back() << " ";
		std::cout << "\n";
	}
}


/*
 * Function to visualize the genome or network in graphviz
 */
void visualizeGraph(const Genome &g, const std::string dir) {
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
 * Function to load csv data 
 */
std::vector<std::vector<double>> loadData(const std::string filename, 
	const std::string dir) {
	std::regex comma(",");
	std::ifstream csvFile(dir + filename);
	std::string line;
	std::vector<std::vector<double>> data;

	// Check if the file is open
	if(!csvFile.is_open()) {
		std::cout << "Unable to open csv file\n";
		return data;
	}

	// Keep track of maximum and minimum values for normalization
	std::vector<double> max;
	std::vector<double> min;
	bool first = true;

	while(csvFile && std::getline(csvFile, line)) {
		std::vector<std::string> stringRow {
			std::sregex_token_iterator(line.begin(), line.end(), comma, -1), 
			std::sregex_token_iterator() };
		
		std::vector<double> row(stringRow.size());
		
		std::transform(stringRow.begin(), stringRow.end(), row.begin(), 
			[](std::string const &val) { return std::stod(val); });

		if(first) {
			max = row;
			min = row;
			first = false;
		} else {
			for(size_t i = 0; i < row.size(); ++i) {
				if(row[i] > max[i]) { max[i] = row[i]; }
				else if(row[i] < min[i]) { min[i] = row[i]; }
			}
		}

		data.push_back(row);
	}
	
	csvFile.close();

	for(auto &row : data) {
		for(size_t i = 0; i < row.size()-1; ++i) {
			row[i] = (row[i] - min[i]) / (max[i] - min[i]);
		}
	}
	return data;
}


