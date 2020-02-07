#pragma once
#include "morassnetwork.h"
#include "node.h"
#include "stdlib.h"
#include "sstream"
#include <random>

class GeneticAlgorithm
{
private:
	int num_inputs;
	int num_outputs;
	double max_threshold;
	double parent_ratio;
	std::string pi = "31415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841";
	std::vector<std::vector<int>> inputs;
	std::vector<std::vector<int>> outputs;
	//double mutation_chance;
	std::vector<MorassNetwork*> webs;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()
	//std::uniform_real_distribution<> dis;
public:
	GeneticAlgorithm();
	GeneticAlgorithm(int inputs, int outputs, double merge_ratio, double max_T = 30);
	~GeneticAlgorithm();
	int size(void);
	bool add_random_edge(MorassNetwork* web, int start = -1, int end = -1, int max_attempts = 100);
	bool del_random_edge(MorassNetwork* web, int max_attempts = 100);
	void add_random_node_with_edges(MorassNetwork* web, unsigned int inc_edges = 1, unsigned int out_edges = 1);
	bool del_random_node(MorassNetwork* web);
	void randomise_edge(MorassNetwork* web);
	void randomise_node(MorassNetwork* web); //Does not affect edges
	void mutate_web(MorassNetwork* web);
	MorassNetwork* merge_webs(MorassNetwork* web_a, MorassNetwork* web_b);
	void generate_webs(int webs_to_make, int max_nodes, bool fully_connected = true);
	MorassNetwork* evolve_for_pi(unsigned int webs_to_make);
	MorassNetwork* deep_copy(MorassNetwork* web);
};

