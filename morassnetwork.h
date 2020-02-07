#pragma once
#include <vector>
#include "node.h"
class MorassNetwork
{
private:
	std::vector<Node*> nodes;	// Nodes for the graph
	std::vector<std::tuple<double, int>> process_thresholds(void);
	void process_decay(void);
	void process_inputs(void);
public:
	MorassNetwork();
	~MorassNetwork();
	void reset_charge();
	int length(void);
	std::vector<std::tuple<double, int>> step(void);
	Node* get_node(int node_id);
	void inject_node(int node_id, double input);
	int add_node(double T, double Cp, double Cf, double Dp, double Df);
	bool del_node(int node_id);
	bool add_edge(double out_pcnt, double out_fixed, int node_start, int node_end);
	bool del_edge(int start_node, int end_node);
	std::vector<int> remove_stranded_nodes();
	void print_network(void);
	MorassNetwork* deep_copy();
};

