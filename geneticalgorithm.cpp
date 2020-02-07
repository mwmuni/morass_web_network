#include "geneticalgorithm.h"
#include "omp.h"



GeneticAlgorithm::GeneticAlgorithm() {
	parent_ratio = .5;
	num_inputs = 0;
	num_outputs = 1;
	max_threshold = 1000.0;
	//std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	//std::uniform_real_distribution<> dis(-1.0, 1.0);
	//dis.param( = -1;
	//gen.seed(rd());
}

GeneticAlgorithm::GeneticAlgorithm(int inputs, int outputs, double merge_ratio, double max_T) {
	parent_ratio = merge_ratio;
	num_inputs = inputs;
	num_outputs = outputs;
	max_threshold = max_T;
	//std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	//std::uniform_real_distribution<> dis(-1.0, 1.0);
	//gen.seed(rd());
}

GeneticAlgorithm::~GeneticAlgorithm() {
	webs.clear();
}

int GeneticAlgorithm::size(void) {
	return webs.size();
}

void GeneticAlgorithm::generate_webs(int webs_to_make, int max_nodes, bool fully_connected) {
	//inputs.clear();
	for (int i = webs.size()-1; i >= 0; i--) {
		delete webs[i];
	}
	webs.clear();
	double T, Cp, Cf, Dp, Df;
	double out_pcnt, out_fixed;
	int rand_val;
	//std::random_device rd;  //Will be used to obtain a seed for the random number engine
	//std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(-1.0, 1.0);
	for (int i = 0; i < webs_to_make; i++) {
		MorassNetwork* mn = new MorassNetwork();
		int range = max_nodes;
		for (int j = 0; j < range; j++) {
			T = dis(gen) * max_threshold;
			Cp = dis(gen);
			Cf = dis(gen) * T;
			Dp = dis(gen);
			Df = dis(gen) * T;
			mn->add_node(T, Cp, Cf, Dp, Df);
		}
		if (!fully_connected) {
			for (int j = 0; j < range; j++) {
				do {
					rand_val = rand() % (range);
				} while (rand_val == j);
				out_pcnt = dis(gen);
				out_fixed = dis(gen) * max_threshold;
				mn->add_edge(out_pcnt, out_fixed, j, rand_val);
			}
			for (int j = 0; j < range; j++) {
				if (!add_random_edge(mn)) {
					break;
				}
			}
		}
		else {
			for (unsigned int j = 0; j < mn->length() - 1; j++) {
				for (unsigned int k = j + 1; k < mn->length(); k++) {
					out_pcnt = dis(gen);
					out_fixed = dis(gen) * max_threshold;
					mn->add_edge(out_pcnt, out_fixed, j, k);
					out_pcnt = dis(gen);
					out_fixed = dis(gen) * max_threshold;
					mn->add_edge(out_pcnt, out_fixed, k, j);
				}
			}
		}
		/*std::vector<int> number_tracker;
		std::vector<int> chosen_nodes;
		for (int j = 0; j < range; j++) number_tracker.push_back(0);
		for (int j = 0; j < range; j++) {
			do {
				rand_val = rand() % range;
			} while (number_tracker[rand_val] > 0);
			number_tracker[rand_val]++;
		}
		for (int j = 0; j < number_tracker.size(); j++) {
			if (number_tracker[j] > 0)
				chosen_nodes.push_back(j);
		}
		inputs.push_back(chosen_nodes);*/
		webs.push_back(mn);
	}
}

bool GeneticAlgorithm::add_random_edge(MorassNetwork* web, int start, int end, int max_attempts) {
	if (start == end && (start > -1 || end > -1)) {
		std::cout << "WARNING! Random edge start and end are identical. This is not supported." << std::endl;
		return false;
	}
	int num_neurons = web->length();
	int start_n = start, end_n = end;
	double out_pcnt, out_fixed;
	bool success = false;
	//std::random_device rd;  //Will be used to obtain a seed for the random number engine
	//std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(-1.0, 1.0);
	for (int i = 0; i < max_attempts && !success; i++) {
		if (start_n == -1)
			start_n = rand() % (num_neurons);
		if (end_n == -1)
			do {
				end_n = rand() % (num_neurons);
			} while (end_n == start_n);
		out_pcnt = dis(gen);
		out_fixed = dis(gen) * max_threshold;
		success = web->add_edge(out_pcnt, out_fixed, start_n, end_n);
	}
	return success;
}

bool GeneticAlgorithm::del_random_edge(MorassNetwork* web, int max_attempts) {
	int num_neurons = web->length();
	int start_n, end_n;
	bool success = false;
	for (int i = 0; i < max_attempts && !success; i++) {
		start_n = rand() % (num_neurons);
		do {
			end_n = rand() % (num_neurons);
		} while (end_n == start_n);
		success = web->del_edge(start_n, end_n);
	}
	return success;
}

void GeneticAlgorithm::add_random_node_with_edges(MorassNetwork* web, unsigned int inc_edges, unsigned int out_edges) {
	if (web->length() == 0) {
		std::cout << "WARNING! Can't add a node with edges to an empty web." << std::endl;
		return;
	}
	if (inc_edges > web->length() || out_edges > web->length()) {
		std::cout << "WARNING! Can't have more incoming/outgoing edges than nodes in the network." << std::endl;
		return;
	}
	double T, Cp, Cf, Dp, Df;
	double out_pcnt, out_fixed;
	int node_id, rand_val;
	//std::random_device rd;  //Will be used to obtain a seed for the random number engine
	//std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(-1.0, 1.0);
	T = dis(gen) * max_threshold;
	Cp = dis(gen);
	Cf = dis(gen) * T;
	Dp = dis(gen);
	Df = dis(gen) * T;
	web->add_node(T, Cp, Cf, Dp, Df);
	node_id = web->length() - 1; //Node was inserted at the end of the web
	//The following two for loops will keep attempting to insert edges until success
	//Guaranteed to eventually work; will not take too long since rand() evenly distributes values
	if (inc_edges == web->length()-1) {
		for (unsigned int i = 0; i < inc_edges; i++) {
			out_pcnt = dis(gen);
			out_fixed = dis(gen) * max_threshold;
			web->add_edge(out_pcnt, out_fixed, node_id, i);
		}
	}
	else {
		for (unsigned int i = 0; i < inc_edges; i++) {
			do {
				out_pcnt = dis(gen);
				out_fixed = dis(gen) * max_threshold;
				rand_val = rand() % (web->length());
			} while (!web->add_edge(out_pcnt, out_fixed, rand_val, node_id));
		}
	}
	if (out_edges == web->length()-1) {
		for (unsigned int i = 0; i < out_edges; i++) {
			out_pcnt = dis(gen);
			out_fixed = dis(gen) * max_threshold;
			web->add_edge(out_pcnt, out_fixed, node_id, i);
		}
	}
	else {
		for (unsigned int i = 0; i < out_edges; i++) {
			do {
				out_pcnt = dis(gen);
				out_fixed = dis(gen) * max_threshold;
				rand_val = rand() % (web->length());
			} while (!web->add_edge(out_pcnt, out_fixed, node_id, rand_val));
		}
	}
}

bool GeneticAlgorithm::del_random_node(MorassNetwork* web) {
	return web->del_node(rand() % (web->length()));
}

void GeneticAlgorithm::randomise_edge(MorassNetwork* web) {
	//std::random_device rd;  //Will be used to obtain a seed for the random number engine
	//std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(-1.0, 1.0);
	Node * temp_node_ptr = web->get_node(rand() % (web->length()));
	if (temp_node_ptr->get_num_edges() == 1)
		temp_node_ptr->set_edge(dis(gen), dis(gen) * max_threshold, 0);
	else if (temp_node_ptr->get_num_edges() > 0)
		temp_node_ptr->set_edge(dis(gen), dis(gen) * max_threshold, rand() % (temp_node_ptr->get_num_edges()));
}

void GeneticAlgorithm::randomise_node(MorassNetwork* web) {
	double T, Cp, Cf, Dp, Df;
	//std::random_device rd;  //Will be used to obtain a seed for the random number engine
	//std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(-1.0, 1.0);
	T = dis(gen) * max_threshold;
	Cp = dis(gen);
	Cf = dis(gen) * T;
	Dp = dis(gen);
	Df = dis(gen) * T;
	Node* temp_node_ptr = web->get_node(rand() % (web->length()));
	temp_node_ptr->set_node(T, Cp, Cf, Dp, Df);
}

void GeneticAlgorithm::mutate_web(MorassNetwork* web) {
	while (true) {
		//int range = 4 + (1 ? web->length() > 10 : 0);
		//int rand_val = rand() % (range - 1);
		//int rand_val = rand() % 5;
		std::uniform_real_distribution<> dis(0.0, 0.9999);
		int rand_val = 2 + (int)(dis(gen) * 3);
		//int rand_val = rand() % 2 + 3;
		//std::cout << rand_val;

		switch (rand_val) {
		case 0: // Add random edge
			if (add_random_edge(web))
				return;
		case 1: // Delete random edge
			if (del_random_edge(web))
				return;
			//(void)0;
		case 2: // Add random node with edges
			//std::cout << "Before insertion: " << web->length();
			add_random_node_with_edges(web, web->length(), web->length());
			//std::cout << ",\tAfter insertion: " << web->length() << std::endl;
			return;
		case 3: // Randomises an edge
			randomise_edge(web);
			return;
		case 4: // Randomises a node
			randomise_node(web);
			return;
		case 5:
			//std::cout << web->length() << ", ";
			if (del_random_node(web))
				return;
		}
	}
}

MorassNetwork* GeneticAlgorithm::merge_webs(MorassNetwork* web_a, MorassNetwork* web_b) {
	//std::random_device rd;  //Will be used to obtain a seed for the random number engine
	//std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(-1.0, 1.0);
	MorassNetwork* child = new MorassNetwork();
	std::vector<MorassNetwork*> temp_webs;
	temp_webs.push_back(web_a);

	unsigned int max_nodes = std::max(web_a->length(), web_b->length());
	unsigned int min_nodes = std::min(web_a->length(), web_b->length());
	std::vector<std::tuple<double, double, int>> edges;
	MorassNetwork* greater_web;
	if (max_nodes == web_a->length())
		greater_web = web_a;
	else
		greater_web = web_b;
	// This for loop randomly chooses a web to draw a node from; the node's parameters are copied
	for (unsigned int i = 0; i < max_nodes; i++) {
		Node* temp_node_ptr;
		if (i < min_nodes) {
			if (dis(gen) < parent_ratio) {
				temp_node_ptr = web_a->get_node(i);
			}
			else {
				temp_node_ptr = web_b->get_node(i);
			}
		}
		else {
			temp_node_ptr = greater_web->get_node(i);
		}
		child->add_node(temp_node_ptr->get_threshold(),
			temp_node_ptr->get_chg_cons_pcnt(),
			temp_node_ptr->get_chg_cons_fixed(),
			temp_node_ptr->get_decay_rate_pcnt(),
			temp_node_ptr->get_decay_rate_fixed());
	}
	#pragma omp parallel for
	for (unsigned int i = 0; i < max_nodes; i++) {
		if (i < min_nodes) {
			if (dis(gen) < parent_ratio) {
				edges = web_a->get_node(i)->get_edges();
			}
			else {
				edges = web_b->get_node(i)->get_edges();
			}
		}
		else {
			edges = greater_web->get_node(i)->get_edges();
		}
		for (unsigned int j = 0; j < edges.size(); j++)
			child->add_edge(std::get<0>(edges[j]), std::get<1>(edges[j]), i, std::get<2>(edges[j]));
	}
	return child;
}

MorassNetwork* GeneticAlgorithm::deep_copy(MorassNetwork* web) {
	MorassNetwork* new_web = new MorassNetwork();
	Node* curr_node;
	std::vector<std::tuple<double, double, int>> node_edges;
	for (unsigned int i = 0; i < web->length(); i++) {
		curr_node = web->get_node(i);
		new_web->add_node(curr_node->get_threshold(),
			curr_node->get_chg_cons_pcnt(),
			curr_node->get_chg_cons_fixed(),
			curr_node->get_decay_rate_pcnt(),
			curr_node->get_decay_rate_fixed());
	}
	for (unsigned int i = 0; i < web->length(); i++) {
		node_edges = web->get_node(i)->get_edges();
		for (unsigned int j = 0; j < node_edges.size(); j++) {
			new_web->add_edge(std::get<0>(node_edges[j]), std::get<1>(node_edges[j]), i, std::get<2>(node_edges[j]));
		}
	}
	return new_web;
}

MorassNetwork* GeneticAlgorithm::evolve_for_pi(unsigned int webs_to_make) {
	//std::random_device rd;  //Will be used to obtain a seed for the random number engine
	//std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(-1.0, 1.0);
	generate_webs(webs_to_make, 16, true);	// (num_webs, num_nodes, is_fully_connected)
	int top_to_keep = std::max(20, (int)std::sqrt(webs_to_make)); // WARNING: do not make this number too large
	unsigned int num_epochs = 100000;
	unsigned int num_steps = 100;
	unsigned int max_mutations = 30;
	unsigned int num_outputs = 10;
	unsigned int output_decimator = 0;
	int best_web = -1;
	double best_score = -1;
	unsigned int best_nodes;
	unsigned int best_edges;
	//std::vector<std::tuple<double, int>> pulses; // Order will be determined by strength of pulse, second element is pulse index
	std::vector<std::tuple<int, int>> scores; // Score is first element since it makes sorting easier, second element is web index
	std::vector<std::string> outputs;
	std::vector<MorassNetwork*> best_webs;
	//std::vector<std::vector<int>> outputs; // Could just always use nodes 0 to 9
	for (unsigned int i = 0; i < num_epochs; i++) {
		scores.clear();
		outputs.clear();
		//for (int m = 0; m < best_webs.size(); m++) {
		//	delete best_webs[m];
		//}
		best_webs.clear();
		for (unsigned int k = 0; k < webs.size(); k++) {
			webs[k]->reset_charge();
		}
		// This loop initialises the output vector and scores vector
		for (unsigned int j = 0; j < webs_to_make; j++) {
			outputs.push_back("");
			scores.emplace_back(0, j);
		}
		// This is the main part of the loop that steps through all the webs and processes the pulses
		
		bool multi_output_allowed = false;
		//TODO convert the k outer loop into a multithreaded function call

		#pragma omp parallel for critical outputs
		for (unsigned int k = 0; k < webs.size(); k++) {
			std::vector<std::tuple<double, int>> pulses;
			for (unsigned int j = 0; j < num_steps; j++) {
				bool multi_output;
				pulses = webs[k]->step();
				pulses = std::vector<std::tuple<double, int>>(pulses.begin(), pulses.begin() + num_outputs);
				std::sort(pulses.begin(), pulses.end());
				if (!multi_output_allowed) {
					multi_output = false;
					for (int p = num_outputs - 2; p >= 0; p--) {
						if (std::get<0>(pulses[p]) > 0.0) {
							multi_output = true;
						}
						else
							break;
					}
					if (!multi_output)
						outputs[k] += std::to_string(std::get<1>(pulses[num_outputs - 1]));
				}
				else {
					for (int p = num_outputs - 1; p >= 0; p--) {
						if (std::get<0>(pulses[p]) > 0.0) {
							//if (k >= 1000)
								//std::cout << "k: " << k << std::endl;
							//std::cout << "outputs[" << k << "]: " << outputs[k] << std::endl;
							outputs[k] += std::to_string(std::get<1>(pulses[p]));
						}
					}
				}
			}
		}
		// This loop compares the output of the network against pi to calculate accuracy
		for (unsigned int j = 0; j < pi.size(); j++) {
			for (unsigned int k = 0; k < outputs.size(); k++) {
				if (j < outputs[k].size())
					if (outputs[k][j] == pi[j]) {
						//std::cout << "Adding: " << std::max((200 - (signed)webs[std::get<1>(scores[k])]->length()) / ((int)j + 1), 50) << std::endl;
						std::get<0>(scores[k])+= std::max((200 - (signed)webs[std::get<1>(scores[k])]->length()*5) / ((int)j+1), 50 - std::min(40, (signed)webs[std::get<1>(scores[k])]->length()));
					}
					else {
						//std::cout << "Removing: " << (int)(5. * (double)std::abs((outputs[k][j] - '0') - (pi[j] - '0'))) << std::endl;
						std::get<0>(scores[k]) -= (int)(5. * (double)std::abs((outputs[k][j] - '0') - (pi[j] - '0')));
					}
			}
		}
		std::sort(scores.begin(), scores.end());
		best_web = -1;
		best_score = -1.0;
		best_nodes = 0;
		best_edges = 0;
		for (unsigned int j = scores.size() - 1; j >= scores.size() - (double(top_to_keep) + 1); j--) {
			best_webs.push_back(deep_copy(webs[std::get<1>(scores[j])]));
			if (best_web == -1) {
				best_web = std::get<1>(scores[j]);
				best_score = std::get<0>(scores[j]);
				best_nodes = best_webs[0]->length();
				for (unsigned int n = 0; n < best_nodes; n++) {
					Node* node_ptr = best_webs[0]->get_node(n);
					best_edges += node_ptr->get_num_edges();
				}
			}
		}

		if (output_decimator++ % 20 == 0)
			std::cout << "Best output so far was: " << outputs[best_web] << " score: " << best_score << ", nodes: " << best_nodes << ", edges: " << best_edges << std::endl;

		for (int m = webs.size()-1; m >= 0; m--) {
			delete webs[m];
		}
		webs.clear();
		if (i == num_epochs - 1)
			break;
		for (unsigned int j = 0; j < best_webs.size(); j++) {
			webs.push_back(best_webs[j]);
			for (unsigned int k = j; k < best_webs.size(); k++) {
				if (j != k)
					webs.push_back(merge_webs(best_webs[j], best_webs[k]));
			}
		}
		MorassNetwork* temp_web;
		while (webs.size() < webs_to_make) {
			temp_web = deep_copy(best_webs[webs.size() % best_webs.size()]);
			for (unsigned int j = 0; j < rand() % (max_mutations - 1) + 1; j++)
				mutate_web(temp_web);
			webs.push_back(temp_web);
		}
	}
	std::cout << "Best output was: " << outputs[best_web] << " with a score of " << best_score << std::endl;
	return deep_copy(best_webs[0]);
}