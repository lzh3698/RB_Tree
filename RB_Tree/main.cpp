//#include <iostream>
//#include "RBTree.hpp"
//using namespace std;
//
//int main() {
//	RBTree<int> rb;
//
//	rb.insert(10);
//	rb.insert(20);
//	rb.insert(5);
//	rb.insert(15);
//	rb.insert(25);
//	rb.insert(30);
//	rb.insert(35);
//	rb.insert(40);
//	rb.insert(45);
//	rb.insert(50);
//
//	rb.erase(20);
//	rb.erase(35);
//	rb.erase(10);
//	rb.erase(45);
//	rb.erase(5);
//
//	rb.pre_order();
//	rb.in_order();
//	rb.post_order();
//	rb.level_order();
//
//	cout << rb.max() << endl;
//	cout << rb.min() << endl;;
//
//
//	return 0;
//}

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "RBTree.hpp"

int main() {
    RBTree<int> tree;
    std::cout << tree.size() << std::endl;
    // ²âÊÔ²åÈë
    std::vector<int> nums = { 10, 20, 5, 15, 30, 25, 35, 1, 8, 12, 18, 28, 32, 38 };
    for (int num : nums) {
        tree.insert(num);
    }

    std::cout << tree.size() << std::endl;

    // ²âÊÔ±éÀú
    std::cout << "In-order traversal: ";
    tree.in_order();

    // ²âÊÔ²éÕÒ
    std::cout << "Contains 15: " << (tree.find(15) ? "Yes" : "No") << std::endl;
    std::cout << "Contains 100: " << (tree.find(100) ? "Yes" : "No") << std::endl;

    // ²âÊÔ×îÖµ
    std::cout << "Min: " << tree.min() << std::endl;
    std::cout << "Max: " << tree.max() << std::endl;

    // ²âÊÔÉ¾³ý
    tree.erase(15);
    tree.erase(30);
    std::cout << "After deleting 15 and 30, in-order: ";
    tree.in_order();

    // ²âÊÔ´óÐ¡
    std::cout << "Size: " << tree.size() << std::endl;

    // ²âÊÔÇå¿Õ
    tree.clear();
    std::cout << "After clear, size: " << tree.size() << std::endl;

    return 0;
}