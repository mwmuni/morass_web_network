#include "morassnetwork.h"

#include <cassert>
#include <cmath>
#include <iostream>

namespace {

bool nearly_equal(double a, double b) {
	return std::fabs(a - b) < 1e-9;
}

void pulse_reaches_destination_after_one_step() {
	MorassNetwork network;
	network.add_node(1.0, 1.0, 0.0, 0.0, 0.0);
	network.add_node(10.0, 0.0, 0.0, 0.0, 0.0);
	network.add_edge(1.0, 0.0, 0, 1);

	network.inject_node(0, 1.0);
	auto pulses = network.step();

	assert(nearly_equal(std::get<0>(pulses[0]), 1.0));
	assert(nearly_equal(network.get_node(0).get_current_chg(), 0.0));
	assert(nearly_equal(network.get_node(1).get_current_chg(), 1.0));
}

void fixed_edge_output_requires_a_pulse() {
	MorassNetwork network;
	network.add_node(10.0, 1.0, 0.0, 0.0, 0.0);
	network.add_node(10.0, 0.0, 0.0, 0.0, 0.0);
	network.add_edge(1.0, 5.0, 0, 1);

	network.step();

	assert(nearly_equal(network.get_node(1).get_current_chg(), 0.0));
}

void deleting_node_removes_and_shifts_edges() {
	MorassNetwork network;
	network.add_node(1.0, 1.0, 0.0, 0.0, 0.0);
	network.add_node(1.0, 1.0, 0.0, 0.0, 0.0);
	network.add_node(1.0, 1.0, 0.0, 0.0, 0.0);
	network.add_edge(1.0, 0.0, 0, 1);
	network.add_edge(1.0, 0.0, 0, 2);

	assert(network.del_node(1));
	assert(network.length() == 2);
	assert(network.get_node(0).get_num_edges() == 1);
	assert(network.get_node(0).has_edge(1));
}

}

int main() {
	pulse_reaches_destination_after_one_step();
	fixed_edge_output_requires_a_pulse();
	deleting_node_removes_and_shifts_edges();

	std::cout << "All tests passed." << std::endl;
	return 0;
}
