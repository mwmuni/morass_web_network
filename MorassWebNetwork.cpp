// MorassWebNetwork.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;
#include "geneticalgorithm.h"
#include "morassnetwork.h"

int main()
{
    GeneticAlgorithm* ga = new GeneticAlgorithm(0, 10, .5);
    //MorassNetwork* mn = ga->evolve_for_pi(10);
    ga->evolve_for_pi(1000);
    //ga->evolve_for_mnist(1000);
    //MorassNetwork* mn = new MorassNetwork();
    /*std::vector<std::tuple<double, int>> pulses;
    mn->add_node(5., .2, .5, .05, .1);
    mn->add_node(10., .5, 2, .1, 0.);
    mn->add_edge(.8, 2, 1, 0);
    mn->add_edge(.1, 1, 0, 1);
    mn->add_node(3., .1, 1., .33, 0.);
    mn->add_node(6., 1. / 3., 0., .2, 0.5);
    mn->add_node(20., .2, 1.5, .1, 0.);
    mn->add_node(4., .8, 2., 0.1, 0.1);
    mn->add_edge(.5, 1, 0, 1);
    mn->add_edge(.5, 1, 0, 2);
    mn->add_edge(.5, 1, 0, 3);
    mn->add_edge(1. / 3., 2, 2, 1);
    mn->add_edge(1., 3, 3, 2);
    mn->add_edge(2., 1, 1, 2);
    mn->print_network();
    //return 0;
    for (int i = 0; i < 100; i++) {
    //while (true) {
        mn->inject_node(0, 2.);
        pulses = mn->step();
        for (int i = 0; i < pulses.size(); i++) {
            std::cout << "Node " << i << " pulsed " << std::get<0>(pulses[i]) << ", ";
        }
        std::cout << std::endl;
    }*/
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
