#include "morassnetwork.h"

MorassNetwork::MorassNetwork(void) {

}

MorassNetwork::~MorassNetwork(void) {
	for (unsigned int m = 0; m < nodes.size(); m++) {
		nodes.erase(nodes.begin() + m);
	}
	nodes.clear();
}

void MorassNetwork::reset_charge() {
	for (unsigned int i = 0; i < nodes.size(); i++) {
		nodes[i].reset_charge();
	}
}

std::vector<std::tuple<double, int>> MorassNetwork::step(void) {
	std::vector<std::tuple<double, int>> pulses = process_thresholds();
	process_decay();
	process_inputs();
	return pulses;
}

void MorassNetwork::inject_node(unsigned int node_id, double input) {
	nodes[node_id].inject_charge(input);
}

unsigned int MorassNetwork::length() {
	return nodes.size();
}

int MorassNetwork::add_node(double T, double Cp, double Cf, double Dp, double Df) {
	//Node::Node node(nodes.size(), T, Cp, Cf, Dp, Df);
	nodes.push_back(Node(nodes.size(), T, Cp, Cf, Dp, Df));
	return length();
}

bool MorassNetwork::del_node(int node) {
	if (node < (signed)nodes.size()) {
		//Node node_to_del;
		#pragma omp parallel for schedule(dynamic)
		for (int i = 0; i < nodes.size(); i++) {
			nodes[i].del_edge(node);
			nodes[i].shift_id_down(node);
			if (nodes[i].get_id() > node)
				nodes[i].set_id(nodes[i].get_id() - 1);
		}
		nodes.erase(nodes.begin() + node);
	}
	else return false;
	for (unsigned int i = 0; i < nodes.size(); i++)
		nodes[i].set_id(i);
	return true;
	//if (nodes.size() > 0)
	//	nodes[0].set_counter(nodes.size());
}

bool MorassNetwork::add_edge(double out_pcnt, double out_fixed, unsigned int start_node, unsigned int end_node) {
	if (std::max(start_node, end_node) >= nodes.size()) {
		std::cout << "Error!: Out of range" << std::endl;
		std::cout << "start_node: " << start_node << std::endl;
		std::cout << "end_node: " << end_node << std::endl;
		std::exit(0);
	}
	return nodes[start_node].add_edge(out_pcnt, out_fixed, nodes[end_node].get_id());
}

bool MorassNetwork::del_edge(unsigned int start_node, unsigned int end_node) {
	return nodes[start_node].del_edge(nodes[end_node]);
}

std::vector<int> MorassNetwork::remove_stranded_nodes() {
	std::vector<Node> no_outputs;
	std::vector<int> to_remove;
	unsigned int curr_node_id;
	bool stranded;
	for (unsigned int i = 0; i < nodes.size(); i++) {
		if (nodes[i].get_num_edges() == 0) {
			no_outputs.push_back(nodes[i]);
		}
	}
	for (unsigned int i = 0; i < no_outputs.size(); i++) {
		stranded = true;
		curr_node_id = no_outputs[i].get_id();
		for (unsigned int j = 0; j < nodes.size() && stranded; j++)
			if (curr_node_id != j) stranded = !nodes[j].has_edge(no_outputs[i]);
		if (stranded) to_remove.push_back(curr_node_id);
	}
	for (int i = (int)to_remove.size()-1; i >= 0; i--) { //Delete in reverse order as the ids get updated after deletion
		del_node(to_remove[(unsigned)i]);
	}
	return to_remove;
}

bool MorassNetwork::is_fully_connected(void) {
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].edges.size() == nodes.size() - 1)
			return false;
	}
	return true;
}

bool MorassNetwork::has_any_edges(void) {
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i].edges.size() > 0)
			return true;
	}
	return false;
}

void MorassNetwork::print_network(void) {
	for (unsigned int i = 0; i < nodes.size(); i++) {
		nodes[i].print_edges();
	}
}

std::vector<std::tuple<double, int>> MorassNetwork::process_thresholds(void) {
	std::vector<std::tuple<double, int>> pulses;
	for (unsigned int i = 0; i < nodes.size(); i++) pulses.emplace_back(0.0, i);
	for (unsigned int i = 0; i < nodes.size(); i++) {
		if (nodes[i].over_threshold())
			std::get<0>(pulses[i]) = nodes[i].trigger();
			//std::get<0>(pulses[i]) = nodes[i].pulse(nodes[i].trigger());
		process_pulses(pulses);
	}
	return pulses;
}

void MorassNetwork::process_pulses(std::vector<std::tuple<double, int>> pulses) {
	for (unsigned int i = 0; i < nodes.size(); i++) {
		for (unsigned int n = 0; n < nodes[i].edges.size(); n++) {
			// get<0>: out_pcnt, get<1>: out_fixed, get<2>: node
			// Multiply the outgoing signal by out_pcnt and add out_fixed
			double pulse_val = (std::get<0>(nodes[i].edges[n]) * std::get<0>(pulses[i])) + std::get<1>(nodes[i].edges[n]);
			int node_idx = std::get<2>(nodes[i].edges[n]);
			nodes[node_idx].add_charge(pulse_val);
		}
	}
}

Node MorassNetwork::get_node(int node_id) {
	return nodes[node_id];
}

void MorassNetwork::process_decay(void) {
	for (unsigned int i = 0; i < nodes.size(); i++) {
		nodes[i].decay();
	}
}

void MorassNetwork::process_inputs(void) {
	for (unsigned int i = 0; i < nodes.size(); i++) {
		nodes[i].assimilate_charge();
	}
}