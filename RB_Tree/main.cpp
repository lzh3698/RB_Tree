#include <iostream>
#include "RBTree.hpp"
using namespace std;

int main() {
	RBTree<int> rb;

	rb.insert(10);
	rb.insert(20);
	rb.insert(5);
	rb.insert(15);
	rb.insert(25);
	rb.insert(30);
	rb.insert(35);
	rb.insert(40);
	rb.insert(45);
	rb.insert(50);

	rb.erase(20);
	rb.erase(35);
	rb.erase(10);
	rb.erase(45);
	rb.erase(5);




	return 0;
}