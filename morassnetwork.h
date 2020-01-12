#pragma once
#include <vector>
#include "node.h"
class MorassNetwork
{
private:
	std::vector<Node*> nodes;	// Nodes for the graph
	std::vector<double> process_thresholds(void);
	void process_decay(void);
	void process_inputs(void);
public:
	MorassNetwork();
	~MorassNetwork();
	int length(void);
	std::vector<double> step(void);
	void inject_node(int node_id, double input);
	int add_node(double T, double Cp, double Cf, double Dp, double Df);
	void del_node(int node_id);
	int add_edge(double out_pcnt, double out_fixed, int node_start, int node_end);
	void del_edge(int start_node, int end_node);
	void print_network(void);
};

