#pragma once

#include <iostream>
#include <queue>
#include <stdexcept>

template<class T>
class RBTree {

private:

	struct Node {
		enum Color {
			RED,
			BLACK
		};
		Node(const T& v, Color c = RED, Node* p = nullptr, Node* r = nullptr, Node* l = nullptr)
			: val(v), left(l), right(r), parent(p), color(c)
		{

		}
		T val;
		Node* left;
		Node* right;
		Node* parent;
		Color color;
	};

	Node* root;
	size_t m_size;

	void pre_order(Node* node) const {
		if (!node) {
			return;
		}

		std::cout << node->val << " ";
		pre_order(node->left);
		pre_order(node->right);
	}

	void in_order(Node* node) const {
		if (!node) {
			return;
		}

		in_order(node->left);
		std::cout << node->val << " ";
		in_order(node->right);
	}

	void post_order(Node* node) const {
		if (!node) {
			return;
		}

		post_order(node->left);
		post_order(node->right);
		std::cout << node->val << " ";
	}

	template<typename Func>
	void level_order(const Func& deal) const {
		if (!root) {
			return;
		}

		std::queue<Node*> que;
		que.push(root);

		while (!que.empty()) {
			Node* node = que.front();
			que.pop();
			if (node->left) {
				que.push(node->left);
			}
			if (node->right) {
				que.push(node->right);
			}
			deal(node);
		}
	}

	void adjust(Node* node) {
		Node* parent = node->parent;
		if (parent == root || color(parent) == Node::BLACK) {
			return;
		}

		Node* uncle = nullptr;
		Node* grand = parent->parent;
		if (grand->left == parent) {
			uncle = grand->right;
		}
		else {
			uncle = grand->left;
		}

		if (color(uncle) == Node::RED) {
			setColor(uncle, Node::BLACK);
			setColor(parent, Node::BLACK);
			setColor(grand, Node::RED);
			adjust(grand);
		}
		else if (color(parent) == Node::RED && color(uncle) == Node::BLACK) {
			setColor(parent, Node::BLACK);
			setColor(grand, Node::RED);
			if (grand->left == parent) {
				if (parent->left == node) {
					rightRotate(grand);
				}
				else {
					leftBalance(grand);
				}
			}
			else {
				if (parent->left == node) {
					rightBalance(grand);
				}
				else {
					leftRotate(grand);
				}
			}
		}
		setColor(root, Node::BLACK);
	}

	void erase_deal(Node* node) {

		if (node == root) {
			setColor(root, Node::BLACK);
			return;
		}

		Node* parent = node->parent;

		// 无红色子树，查看兄弟节点
		Node* brother = parent->left == node ? parent->right : parent->left;

		// 若兄弟为红色，将兄弟置为黑色，并将parent置为红色，旋转parent，
		// 使待删除节点to_del必然有一个黑色兄弟

		if (color(brother) == Node::RED) {
			setColor(brother, Node::BLACK);
			setColor(parent, Node::RED);
			if (node == parent->left) {
				leftRotate(parent);
				parent = node->parent;
				brother = parent->right;
			}
			else {
				rightRotate(parent);
				parent = node->parent;
				brother = parent->left;
			}
		}

		// 此时，brother为黑

		// 如果兄弟节点有红色孩子
		// 且满足LL或者RR，将红色孩子置为兄弟节点的颜色，将兄弟节点置为parent颜色，将parent置为黑色，
		// 旋转parent
		// 
		// 或者，若满足LR或者RL，将兄弟的红色孩子直接染成parent的颜色，将parent置为黑色，平衡parent
		// 处理完成，直接返回，返回后，主调函数执行 delete to_del
		if (brother) {
			if (color(brother->left) == Node::RED) {
				if (parent->left == node) {
					// RL
					setColor(brother->left, color(parent));
					setColor(parent, Node::BLACK);
					rightBalance(parent);
				}
				else {
					// LL
					setColor(brother->left, color(brother));
					setColor(brother, color(parent));
					setColor(parent, Node::BLACK);
					rightRotate(parent);
				}
				return;
			}
			else if (color(brother->right) == Node::RED) {
				if (parent->left == node) {
					// RR
					setColor(brother->right, color(brother));
					setColor(brother, color(parent));
					setColor(parent, Node::BLACK);
					leftRotate(parent);
				}
				else {
					// LR
					setColor(brother->right, color(parent));
					setColor(parent, Node::BLACK);
					leftBalance(parent);
				}
				return;
			}
		}

		// 若兄弟节点没有红色孩子，将其也置为红色，向parent回溯，若parent为红色，染黑，直接结束
		if (brother) {
			setColor(brother, Node::RED);
		}
		if (color(parent) == Node::RED) {
			setColor(parent, Node::BLACK);
			if (node->left) {
				if (parent->left == node) {
					parent->left = node->left;
					node->left->parent = parent;
				}
				else {
					parent->right = node->left;
					node->left->parent = parent;
				}
			}
			else if (node->right) {
				if (parent->left == node) {
					parent->left = node->right;
					node->right->parent = parent;
				}
				else {
					parent->right = node->right;
					node->right->parent = parent;
				}
			}
			else {
				parent->left == node ? parent->left = nullptr : parent->right = nullptr;
			}
			return;
		}

		// 若parent也为黑色，把parent当做删除节点同样处理（此路径少了一个黑色节点），
		// 将parent置为待处理节点，查看其兄弟节点，循环开始（放在erase_deal函数中递归处理）

		erase_deal(parent);

	}

	void leftRotate(Node* node) {
		Node* child = node->right;
		node->right = child->left;
		if (child->left) {
			child->left->parent = node;
		}
		child->left = node;
		child->parent = node->parent;
		if (node->parent) {
			if (node->parent->left == node) {
				node->parent->left = child;
			}
			else {
				node->parent->right = child;
			}
		}
		else {
			root = child;
		}
		node->parent = child;

	}

	void rightRotate(Node* node) {
		Node* child = node->left;
		node->left = child->right;
		if (child->right) {
			child->right->parent = node;
		}
		child->right = node;
		child->parent = node->parent;
		if (node->parent) {
			if (node->parent->right == node) {
				node->parent->right = child;
			}
			else {
				node->parent->left = child;
			}
		}
		else {
			root = child;
		}
		node->parent = child;

	}

	void leftBalance(Node* node) {

		leftRotate(node->left);
		rightRotate(node);

	}

	void rightBalance(Node* node) {
		rightRotate(node->right);
		leftRotate(node);
	}

	typename Node::Color color(Node* node) {
		if (!node) {
			return Node::BLACK;
		}
		return node->color;
	}

	void setColor(Node* node, typename Node::Color color) {
		node->color = color;
	}

public:

	RBTree() : root(nullptr), m_size(0) {

	}

	~RBTree() {
		clear();
	}

	void clear() {
		level_order([](Node* node) {
			delete node;
			});
		m_size = 0;
		root = nullptr;
	}

	void insert(const T &val) {
		if (!root) {
			root = new Node(val, Node::BLACK);
			++m_size;
			return;
		}

		Node* node = root;
		Node* parent = nullptr;
		while (node) {
			if (val > node->val) {
				parent = node;
				node = node->right;
			}
			else if (val < node->val) {
				parent = node;
				node = node->left;
			}
			else {
				return;
			}
		}
		if (val > parent->val) {
			parent->right = new Node(val, Node::RED, parent);
			node = parent->right;
		}
		else {
			parent->left = new Node(val, Node::RED, parent);
			node = parent->left;
		}

		adjust(node);
		++m_size;
	}

	void erase(const T& val) {
		// 空树
		if (!root) {
			return;
		}

		//
		// 查找要删除的元素node
		
		Node* parent = nullptr;
		Node* node = root;
		while (node) {
			parent = node;
			if (val < node->val) {
				node = node->left;
			}
			else if (val > node->val) {
				node = node->right;
			}
			else {
				break;
			}
		}

		// 元素找不到，返回

		if (!node) {
			return;
		}

		// 若待删除节点有左右子树，查找前驱节点to_del，
		// 保证真正的待删除节点最多只有一颗子树
		
		Node* to_del = node;
		if (to_del->left && to_del->right) {
			to_del = to_del->left;
			while (to_del->right) {
				to_del = to_del->right;
			}
			node->val = to_del->val;
		}

		// 此时，to_del指向真正要删除的节点
		// 若to_del为root，单独处理，改变root指针，删除后直接返回
		if (to_del == root) {
			if (to_del->left) {
				root = to_del->left;
			}
			else {
				root = to_del->right;
			}

			// root不为空
			if (root) {
				root->parent = nullptr;
				setColor(root, Node::BLACK);
			}

			// 删除
			--m_size;
			delete to_del;
			return;
		}

		// 若to_del为红色，将子树变为parent的子树，然后直接删除to_del，结束
		// 此时，to_del不为root，因此parent非空
		parent = to_del->parent;
		if (color(to_del) == Node::RED) {

			if (to_del->left) {
				// 只有左子树
				if (parent->left == to_del) {
					parent->left = to_del->left;
				}
				else {
					parent->right = to_del->left;
				}
				to_del->left->parent = parent;
			}
			else{
				// 只有右子树或者没有子树
				if (parent->left == to_del) {
					parent->left = to_del->right;
				}
				else {
					parent->right = to_del->right;
				}
				// 如果有右子树，改变其parent
				if (to_del->right) {
					to_del->right->parent = parent;
				}
			}
			--m_size;
			delete to_del;
			return;
		}

		// 此时to_del必然是黑色，判断子树有无红色
		// 若有，将其置为黑色，替换to_del，直接结束
		if (color(to_del->left) == Node::RED) {
			setColor(to_del->left, Node::BLACK);
			if (parent->left == to_del) {
				parent->left = to_del->left;
			}
			else {
				parent->right = to_del->left;
			}
			to_del->left->parent = parent;
			--m_size;
			delete to_del;
			return;
		}
		else if (color(to_del->right) == Node::RED) {
			setColor(to_del->right, Node::BLACK);
			if (parent->left == to_del) {
				parent->left = to_del->right;
				to_del->right->parent = parent;
			}
			else {
				parent->right = to_del->right;
				to_del->right->parent = parent;
			}
			--m_size;
			delete to_del;
			return;
		}

		// 无红色子树，查看兄弟节点
		Node* brother = parent->left == to_del ? parent->right : parent->left;

		// 若兄弟为红色，将兄弟置为黑色，并将parent置为红色，旋转parent，
		// 使待删除节点to_del必然有一个黑色兄弟
		
		if (color(brother) == Node::RED) {
			setColor(brother, Node::BLACK);
			setColor(parent, Node::RED);
			if (to_del == parent->left) {
				leftRotate(parent);
				parent = to_del->parent;
				brother = parent->right;
			}
			else {
				rightRotate(parent);
				parent = to_del->parent;
				brother = parent->left;
			}
		}

		// 此时，brother为黑色

		// 如果兄弟节点有红色孩子
		// 且满足LL或者RR，将红色孩子置为兄弟节点的颜色，将兄弟节点置为parent颜色，将parent置为黑色，
		// 旋转parent
		// 
		// 或者，若满足LR或者RL，将兄弟的红色孩子直接染成parent的颜色，将parent置为黑色，平衡parent
		// 处理完成，删除to_del，返回
		if (brother) {
			if (color(brother->left) == Node::RED) {
				if (parent->left == to_del) {
					// RL
					setColor(brother->left, color(parent));
					setColor(parent, Node::BLACK);
					rightBalance(parent);
					if (to_del->left) {
						parent->left = to_del->left;
						to_del->left->parent = parent;
					}
					else if (to_del->right) {
						parent->left = to_del->right;
						to_del->right->parent = parent;
					}
					else {
						parent->left = nullptr;
					}
				}
				else {
					// LL
					setColor(brother->left, color(brother));
					setColor(brother, color(parent));
					setColor(parent, Node::BLACK);
					rightRotate(parent);
					if (to_del->left) {
						parent->right = to_del->left;
						to_del->left->parent = parent;
					}
					else if (to_del->right) {
						parent->right = to_del->right;
						to_del->right->parent = parent;
					}
					else {
						parent->right = nullptr;
					}
				}
				--m_size;
				delete to_del;
				return;
			}
			else if (color(brother->right) == Node::RED) {
				if (parent->left == to_del) {
					// RR
					setColor(brother->right, color(brother));
					setColor(brother, color(parent));
					setColor(parent, Node::BLACK);
					leftRotate(parent);
					if (to_del->left) {
						parent->left = to_del->left;
						to_del->left->parent = parent;
					}
					else if (to_del->right) {
						parent->left = to_del->right;
						to_del->right->parent = parent;
					}
					else {
						parent->left = nullptr;
					}
				}
				else {
					// LR
					setColor(brother->right, color(parent));
					setColor(parent, Node::BLACK);
					leftBalance(parent);
					if (to_del->left) {
						parent->right = to_del->left;
						to_del->left->parent = parent;
					}
					else if (to_del->right) {
						parent->right = to_del->right;
						to_del->right->parent = parent;
					}
					else {
						parent->right = nullptr;
					}
				}
				--m_size;
				delete to_del;
				return;
			}
		}

		// 此时，兄弟节点没有红色孩子，将其也置为红色，向parent回溯，若parent为红色，染黑，
		// 然后删除to_del，直接结束
		if (brother) {
			setColor(brother, Node::RED);
		}

		if (color(parent) == Node::RED) {
			setColor(parent, Node::BLACK);
			if (to_del->left) {
				if (parent->left == to_del) {
					parent->left = to_del->left;
					to_del->left->parent = parent;
				}
				else {
					parent->right = to_del->left;
					to_del->left->parent = parent;
				}
			}
			else if (to_del->right) {
				if (parent->left == to_del) {
					parent->left = to_del->right;
					to_del->right->parent = parent;
				}
				else {
					parent->right = to_del->right;
					to_del->right->parent = parent;
				}
			}
			else {
				parent->left == to_del ? (parent->left = nullptr) : (parent->right = nullptr);
			}
			--m_size;
			delete to_del;
			return;
		}

		// 若parent也为黑色，把parent当做删除节点同样处理（此路径少了一个黑色节点），
		// 将parent置为待处理节点，查看其兄弟节点，循环开始（放在erase_deal函数中递归处理）
		erase_deal(parent);

		// 处理to_del的子树，然后真正删除to_del

		if (to_del->left) {
			if (parent->left == to_del) {
				parent->left = to_del->left;
			}
			else {
				parent->right = to_del->left;
			}
			to_del->left->parent = parent;
		}
		else if (to_del->right) {
			if (parent->left == to_del) {
				parent->left = to_del->right;
			}
			else {
				parent->right = to_del->right;
			}
			to_del->right->parent = parent;
		}
		else {
			parent->left == to_del ? (parent->left = nullptr) : (parent->right = nullptr);
		}

	}

	bool find(const T& val) const {
		if (!root) {
			return false;
		}

		Node* cur = root;
		while (cur) {
			if (cur->val > val) {
				cur = cur->left;
			}
			else if (cur->val < val) {
				cur = cur->right;
			}
			else {
				return true;
			}
		}

		return false;
	}

	bool empty() const {
		return !m_size;
	}

	size_t size() const {
		return m_size;
	}

	const T& max() const {
		if (!root) {
			throw std::runtime_error("Root is nullptr!");
		}

		Node* cur = root;
		while (cur->right) {
			cur = cur->right;
		}
		return cur->val;
	}

	const T& min() const {
		if (!root) {
			throw std::runtime_error("Root is nullptr!");
		}

		Node* cur = root;
		while (cur->left) {
			cur = cur->left;
		}
		return cur->val;
	}

	void pre_order() const {
		pre_order(root);
		std::cout << std::endl;
	}

	void in_order() const {
		in_order(root);
		std::cout << std::endl;
	}

	void post_order() const {
		post_order(root);
		std::cout << std::endl;
	}

	void level_order() const {
		level_order([](Node* node) {
			std::cout << node->val << " ";
			});
		std::cout << std::endl;
	}

};