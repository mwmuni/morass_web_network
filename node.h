#pragma once
#include <iostream>
#include <vector>
#include <utility> // std::pair
#include <tuple>
//#include <bits/stdc++.h>

class Node
{
private:
	// counter: used to uniquely identify Nodes
	// id: unique identifier for Node
	// num_edges: number of outgoing edges connected to this Node
	// current_chg: holds the current Node charge
	// stored_chg: buffer for the incoming charge, allows for all nodes to be processed without triggering the threshold
	// threshold: node will pulse when current_chg exceeds threshold
	// chg_cons_pcnt: percentage of the current charge that is consumed when the Node pulses (applied after fixed consumption)
	// chg_cons_fixed: a fixed value sent on pulse that is consumed from the current_chg (will send the lesser of this value or current_chg on pulse)
	// decay_rate_pcnt: this value is multiplied (1-decay_rate_pcnt) by the current_chg and that value is set to current_chg
	// decay_rate_fixed: this fixed value is subtracted from current_chg (before decay_rate_pcnt)
	// edges: stores all of the outgoing edges connected to the current Node
	//static int counter;
	unsigned int id, num_edges;
	double current_chg, stored_chg, threshold;
	double chg_cons_pcnt, chg_cons_fixed;
	double decay_rate_pcnt, decay_rate_fixed;
	//List of edges connected to current vertex
	std::vector<std::tuple<double, double, Node*>> edges; // tuple(double out_pcnt, double out_fixed, Node* node)
public:
	Node();
	Node(unsigned int id, double T, double Cp, double Cf, double Dp, double Df);
	~Node();
	bool operator==(Node& other);
	double pulse(double outgoing_signal);
	double trigger(void);
	void decay(void);
	bool over_threshold(void);
	void add_charge(double charge);
	void assimilate_charge(void);
	void inject_charge(double charge);
	void reset_charge();
	bool add_edge(double out_pcnt, double out_fixed, Node* node);
	bool del_edge(Node* to_del);
	bool has_edge(Node* to_find);
	void set_edge(double out_pcnt, double out_fixed, int edge);
	void set_node(double T, double Cp, double Cf, double Dp, double Df);
	std::vector<std::tuple<double, double, int>> get_edges();
	unsigned int get_num_edges(void);
	unsigned int get_id(void);
	double get_current_chg(void);
	double get_stored_chg(void);
	double get_threshold(void);
	double get_chg_cons_pcnt(void);
	double get_chg_cons_fixed(void);
	double get_decay_rate_pcnt(void);
	double get_decay_rate_fixed(void);
	void set_id(int new_id);
	//void set_counter(int new_counter);
	void print_edges(void);
};