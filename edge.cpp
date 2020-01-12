#include "edge.h"

Edge::Edge(void) {

}

Edge::~Edge(void) {

}

int Edge::store_signal() {
	std::cout << "message from edge!" << std::endl;
	return 0;
}

int Edge::push_signal() {
	return 0;
}