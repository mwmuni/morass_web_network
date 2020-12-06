#include "node.h"

//int Node::counter = 0;

Node::Node(void) {
	id = NULL;
	num_edges = 0;
	current_chg = stored_chg = threshold = 
		chg_cons_pcnt = chg_cons_fixed = 
		decay_rate_pcnt = decay_rate_fixed = 0.0;
}

Node::Node(unsigned int id, double T, double Cp, double Cf, double Dp, double Df) {
	this->id = id;
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

bool Node::operator==(Node other) {
	return this->get_id() == other.get_id();
}

void Node::shift_id_down(int start_idx) {
	for (int i = 0; i < edges.size(); i++) {
		auto prev = std::get<2>(edges[i]);
		if (std::get<2>(edges[i]) > start_idx) {
			std::get<2>(edges[i]) -= 1;
			auto post = std::get<2>(edges[i]);
			assert(prev != post);
		}
	}
}

bool Node::add_edge(double out_pcnt, double out_fixed, int node) {
	// Appends a tuple to the end of the edges list and returns the number of edges
	//for (std::vector<std::tuple<double, double, Node>>::const_iterator n = edges.begin(); n != edges.end(); ++n) {
	for (unsigned int n = 0; n < edges.size(); n++) {
		if (std::get<2>(edges[n]) == node)
			return false;
	}
	edges.emplace_back(out_pcnt, out_fixed, node);
	++num_edges;
	return true;
}

bool Node::del_edge(Node to_del) {
	for (unsigned int i = 0; i < edges.size(); i++) {
		if (std::get<2>(edges[i]) == to_del.get_id()) {
			edges.erase(edges.begin() + i);
			num_edges--;
			return true;
		}
	}
	return false;
}

bool Node::del_edge(int to_del) {
	for (unsigned int i = 0; i < edges.size(); i++) {
		if (std::get<2>(edges[i]) == to_del) {
			edges.erase(edges.begin() + i);
			num_edges--;
			return true;
		}
	}
	return false;
}

bool Node::has_edge(Node to_find) {
	for (unsigned int i = 0; i < edges.size(); i++) {
		if (std::get<2>(edges[i]) == to_find.get_id()) {
			return true;
		}
	}
	return false;
}

bool Node::has_edge(int to_find) {
	for (unsigned int i = 0; i < edges.size(); i++) {
		if (std::get<2>(edges[i]) == to_find) {
			return true;
		}
	}
	return false;
}

void Node::set_edge(double out_pcnt, double out_fixed, int edge) {
	std::get<0>(edges[edge]) = out_pcnt;
	std::get<1>(edges[edge]) = out_fixed;
}

void Node::set_node(double T, double Cp, double Cf, double Dp, double Df) {
	this->threshold = T;
	this->chg_cons_pcnt = Cp;
	this->chg_cons_fixed = Cf;
	this->decay_rate_pcnt = Dp;
	this->decay_rate_fixed = Df;
}

/*std::vector<std::tuple<double, double, int>> Node::get_edges() {
	std::vector<std::tuple<double, double, int>> int_edges;
	for (unsigned int i = 0; i < edges.size(); i++) {
		auto temp = std::get<2>(edges[i]);
		std::cout << "Access check: " << std::get<2>(edges[i]) << " == " << temp << std::endl;
		int_edges.emplace_back(std::get<0>(edges[i]), std::get<1>(edges[i]), std::get<2>(edges[i]));
	}
	return int_edges;
}*/

void Node::print_edges(void) {
	// Prints all edges that are outgoing from the current node
	std::cout << "Edges from Node " << this->get_id() << ":" << std::endl;
	//for (std::vector<std::tuple<double, double, Node*>>::const_iterator n = edges.begin(); n != edges.end(); ++n) {
	for (unsigned int n = 0; n < edges.size(); n++) {
		std::cout << "\tNode id: " << std::get<2>(edges[n]) << ", Out Percent: " << std::get<0>(edges[n]) << ", Out Fixed: " << std::get<1>(edges[n]) << std::endl;
	}
}

unsigned int Node::get_num_edges() {
	return num_edges;
}

unsigned int Node::get_id(void) {
	return id;
}

void Node::set_id(int new_id) {
	id = new_id;
}

/*void Node::set_counter(int new_counter) {
	counter = new_counter;
}*/

double Node::get_current_chg(void) {
	return current_chg;
}

double Node::get_stored_chg(void) {
	return stored_chg;
}

double Node::get_threshold(void) {
	return threshold;
}

double Node::get_chg_cons_pcnt(void) {
	return chg_cons_pcnt;
}

double Node::get_chg_cons_fixed(void) {
	return chg_cons_fixed;
}

double Node::get_decay_rate_pcnt(void) {
	return decay_rate_pcnt;
}

double Node::get_decay_rate_fixed(void) {
	return decay_rate_fixed;
}

/*
double Node::pulse(double outgoing_signal) {
	// Send pulse to each connected Node using the weight defined in each pair
	// This loop iterates over the vector 'edges' using a vector const_iterator n
	double pulse_val = 0.0;
	//std::cout << "Node: (" << get_id() << ") sending pulse to:" << std::endl;
	//for (std::vector<std::tuple<double, double, Node*>>::const_iterator n = edges.begin(); n != edges.end(); ++n) {
	for (unsigned int n = 0; n < edges.size(); n++) {
		// get<0>: out_pcnt, get<1>: out_fixed, get<2>: node
		// Multiply the outgoing signal by out_pcnt and add out_fixed
		pulse_val = (std::get<0>(edges[n]) * outgoing_signal) + std::get<1>(edges[n]);
		auto iter_tuple = std::get<2>(edges[n]);
		iter_tuple->add_charge(pulse_val);
		//std::cout << "\tNode: (" << std::get<2>(*n)->get_id() << ") got a charge of: " << pulse_val << std::endl;
	}
	return pulse_val;
}
*/

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

void Node::reset_charge() {
	current_chg = 0.;
	stored_chg = 0.;
}