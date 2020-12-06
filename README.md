# morass_web_network
Prototype neural network that focuses on facilitating an all-node-input-all-node-output network. The idea is that all neurons in the brain are interconnected and are used as both input and output simultaneously.

Main steps for the network are:
  1. Process all node threshold outputs to connected nodes and store the output in a temporary variable in the destination node
  2. Decay all the nodes by some percentage and fixed amount
  3. Assimilate the temporary charge from step 1 with the current charge

These three steps are repeated continuously to simulate signals passing through a neural network. To run these three steps, run the 'step()' method in the MorassNetwork class.

Since all neurons are inputs, any node can have its charge injected (or subtracted) at any time. To do this, simply call the 'inject_node(int node_id, double input)' method.

A node is defined with the following parameters:
  - T: Threshold; when the current charge meets or exceeds this value, the node fires
  - Cp: Charge consumption percentage; when the threshold is triggered, this percentage of the current charge is removed
  - Cf: Charge consumption fixed; when the threshold is triggered, this fixed value is subtracted from the current charge (before Cp)
  - Dp: Decay percentage; this is how much the current charge decays every step current_chg*=(1-Dp)
  - Df: Decay fixed; this value is subtracted from the current charge every step

An edge is defined with the following parameters:
  - Op: Out percent; when a threshold is triggered, Op is multiplied by the pulse value of the start node and sent to the end node
  - Of: Out fixed; when a threshold is triggered, Of is added to Op and sent to the end node
  - start_node: the start node of the directed edge
  - end_node: the end node of the directed edge

![image](https://imgur.com/xgYXSEX)
