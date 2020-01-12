#include "node.h"

int Node::counter = 0;

Node::Node(void) {
	id = counter++;
	num_edges = 0;
	current_chg = stored_chg = threshold = 
		chg_cons_pcnt = chg_cons_fixed = 
		decay_rate_pcnt = decay_rate_fixed = 0.0;
}

Node::Node(double T, double Cp, double Cf, double Dp, double Df) {
	id = counter++;
	num_edges = 0;
	current_chg = 0.0;
	stored_chg = 0.0;
	threshold = T;
	chg_cons_pcnt = Cp;
	chg_cons_fixed = Cf;
	decay_rate_pcnt = Dp;
	decay_rate_fixed = Df;
}

Node::~Node(void) {
	edges.clear();
}

bool Node::operator==(Node& other) {
	return this->get_id() == other.get_id();
}

int Node::add_edge(double out_pcnt, double out_fixed, Node * node) {
	// Appends a tuple to the end of the edges list and returns the number of edges
	for (std::vector<std::tuple<double, double, Node*>>::const_iterator n = edges.begin(); n != edges.end(); ++n) {
		if (std::get<2>(*n) == node)
			return -1;
	}
	edges.emplace_back(out_pcnt, out_fixed, node);
	return ++num_edges;
}

void Node::del_edge(Node* to_del) {
	for (int i = 0; i < edges.size(); i++) {
		if (std::get<2>(edges[i]) == to_del)
			edges.erase(edges.begin() + i);
	}
}

void Node::print_edges(void) {
	// Prints all edges that are outgoing from the current node
	std::cout << "Edges from Node " << this->get_id() << ":" << std::endl;
	for (std::vector<std::tuple<double, double, Node*>>::const_iterator n = edges.begin(); n != edges.end(); ++n) {
		std::cout << "\tNode id: " << std::get<2>(*n)->get_id() << ", Out Percent: " << std::get<0>(*n) << ", Out Fixed: " << std::get<1>(*n) << std::endl;
	}
}

int Node::get_num_edges() {
	return num_edges;
}

int Node::get_id(void) {
	return id;
}

void Node::set_id(int new_id) {
	id = new_id;
}

void Node::set_counter(int new_counter) {
	counter = new_counter;
}

double Node::pulse(double outgoing_signal) {
	// Send pulse to each connected Node using the weight defined in each pair
	// This loop iterates over the vector 'edges' using a vector const_iterator n
	double pulse_val = 0.0;
	//std::cout << "Node: (" << get_id() << ") sending pulse to:" << std::endl;
	for (std::vector<std::tuple<double, double, Node*>>::const_iterator n = edges.begin(); n != edges.end(); ++n) {
		// get<0>: out_pcnt, get<1>: out_fixed, get<2>: node
		// Multiply the outgoing signal by out_pcnt and add out_fixed
		pulse_val = (std::get<0>(*n) * outgoing_signal) + std::get<1>(*n);
		std::get<2>(*n)->add_charge(pulse_val);
		//std::cout << "\tNode: (" << std::get<2>(*n)->get_id() << ") got a charge of: " << pulse_val << std::endl;
	}
	return pulse_val;
}

double Node::trigger(void) {
	// Subtract consumption amount for an activation
	double outgoing_signal = 0.0;
	if (current_chg >= threshold) {
		if (current_chg <= chg_cons_fixed)
		{
			outgoing_signal = current_chg;
			current_chg = 0.0;
		}
		else
		{
			outgoing_signal = chg_cons_fixed;
			current_chg -= chg_cons_fixed;
		}
		outgoing_signal += current_chg * chg_cons_pcnt;
		current_chg *= 1.0 - chg_cons_pcnt;
		//std::cout << "Node: (" << get_id() << ") threshold signal sent is: " << outgoing_signal << std::endl;
	}
	return outgoing_signal;
}

bool Node::over_threshold() {
	return current_chg >= threshold;
}

void Node::decay(void) {
	// Decay the current node charge by the set amount
	if (current_chg <= decay_rate_fixed)
		current_chg = 0.0;
	else
		current_chg -= decay_rate_fixed;
	current_chg *= 1.0 - decay_rate_pcnt;
}

void Node::add_charge(double charge) {
	// Add the charge sent in by an external pulse
	stored_chg += charge;
}

void Node::assimilate_charge(void) {
	current_chg += stored_chg;
	stored_chg = 0.0;
}

void Node::inject_charge(double charge) {
	current_chg += charge;
}