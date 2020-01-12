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
	static int counter;
	int id, num_edges;
	double current_chg, stored_chg, threshold;
	double chg_cons_pcnt, chg_cons_fixed;
	double decay_rate_pcnt, decay_rate_fixed;
	//List of edges connected to current vertex
	std::vector<std::tuple<double, double, Node*>> edges; // tuple(double out_pcnt, double out_fixed, Node* node)
public:
	Node();
	Node(double T, double Cp, double Cf, double Dp, double Df);
	~Node();
	bool operator==(Node& other);
	double pulse(double outgoing_signal);
	double trigger(void);
	void decay(void);
	bool over_threshold(void);
	void add_charge(double charge);
	void assimilate_charge(void);
	void inject_charge(double charge);
	int add_edge(double out_pcnt, double out_fixed, Node* node);
	void del_edge(Node* to_del);
	int get_num_edges(void);
	int get_id(void);
	void set_id(int new_id);
	void set_counter(int new_counter);
	void print_edges(void);
};