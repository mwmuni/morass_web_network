#include "morassnetwork.h"

MorassNetwork::MorassNetwork(void) {

}

MorassNetwork::~MorassNetwork(void) {

}

std::vector<double> MorassNetwork::step(void) {
	std::vector<double> pulses = process_thresholds();
	process_decay();
	process_inputs();
	return pulses;
}

void MorassNetwork::inject_node(int node_id, double input) {
	nodes[node_id]->inject_charge(input);
}

int MorassNetwork::length() {
	return nodes.size();
}

int MorassNetwork::add_node(double T, double Cp, double Cf, double Dp, double Df) {
	nodes.push_back(new Node(T, Cp, Cf, Dp, Df));
	return length();
}

void MorassNetwork::del_node(int node_id) {
	if (node_id < nodes.size())
		nodes.erase(nodes.begin() + node_id);
	for (int i = 0; i < nodes.size(); i++)
		nodes[i]->set_id(i);
	if (nodes.size() > 0)
		nodes[0]->set_counter(nodes.size());
}

int MorassNetwork::add_edge(double out_pcnt, double out_fixed, int start_node, int end_node) {
	int return_val = nodes[start_node]->add_edge(out_pcnt, out_fixed, nodes[end_node]);
	if (return_val == -1)
		std::cout << "Edge already existed between Node " << start_node << " and Node " << end_node << std::endl;
	return return_val;
}

void MorassNetwork::del_edge(int start_node, int end_node) {
	nodes[start_node]->del_edge(nodes[end_node]);
}

void MorassNetwork::print_network(void) {
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->print_edges();
	}
}

std::vector<double> MorassNetwork::process_thresholds(void) {
	std::vector<double> pulses;
	for (int i = 0; i < nodes.size(); i++) pulses.push_back(0.0);
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i]->over_threshold())
			pulses[i] = nodes[i]->pulse(nodes[i]->trigger());
	}
	return pulses;
}

void MorassNetwork::process_decay(void) {
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->decay();
	}
}

void MorassNetwork::process_inputs(void) {
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->assimilate_charge();
	}
}