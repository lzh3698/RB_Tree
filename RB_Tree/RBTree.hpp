#pragma once

#include <iostream>

template<class T>
class RBTree {
public:

	RBTree() : root(nullptr) {

	}

	
	void insert(const T &val) {
		if (!root) {
			root = new Node(val, Node::BLACK);
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
			parent->right = new Node(val, Node::RED, node);
			parent->right->parent = parent;
			node = parent->right;
		}
		else {
			parent->left = new Node(val, Node::RED, node);
			parent->left->parent = parent;
			node = parent->left;
		}

		adjust(node);
	}

	void erase(const T& val) {
		if (!root) {
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
				break;
			}
		}
		if (!node) {
			return;
		}

		if (node->left && node->right) {
			Node* del = node->left;
			parent = node;
			while (del) {
				parent = del;
				del = del->right;
			}
			node->val = parent->val;
			node = parent;
			parent = node->parent;
		}
		if(color(node) == Node::RED)
		{
			if (node->left) {
				if (parent->left == node) {
					parent->left = node->left;
				}
				else {
					parent->right = node->left;
				}
				node->left->parent = parent;
			}
			else if (node->right) {
				if (parent->left == node) {
					parent->left = node->right;
				}
				else {
					parent->right = node->right;
				}
				node->right->parent = parent;
			}
			else {
				if (parent->left == node) {
					parent->left = nullptr;
				}
				else {
					parent->right = nullptr;
				}
			}
			delete node;
		}
		else {
			// 删除黑色节点
			if (node == root) {
				if (node->left) {
					root = node->left;
					node->left->parent = nullptr;
					setColor(node->left, Node::BLACK);
				}
				else if(node->right){
					root = node->right;
					node->right->parent = nullptr;
					setColor(node->right, Node::BLACK);
				}
				else {
					root = nullptr;
				}
				delete node;
				return;
			}
			
			

			//判断node孩子节点有没有红色
			if (node->parent->left == node)
			{
				if (color(node->left) == Node::RED) {
					setColor(node->left, Node::BLACK);
					node->parent->left = node->left;
					node->left->parent = node->parent;
					delete node;
					return;
				}
				else if (color(node->right) == Node::RED) {
					setColor(node->right, Node::BLACK);
					node->parent->left = node->right;
					node->right->parent = node->parent;
					delete node;
					return;
				}
				
			}
			else {
				if (color(node->right) == Node::RED) {
					setColor(node->right, Node::BLACK);
					node->parent->right = node->right;
					node->right->parent = node->parent;
					delete node;
					return;
				}
				else if (color(node->left) == Node::RED) {
					setColor(node->left, Node::BLACK);
					node->parent->left = node->left;
					node->left->parent = node->parent;
					delete node;
					return;
				}
				
			}

			//循环起始点
			
			Node* del = node;
			bool rec = 0;
			while(!rec)
			{
				if (node == root) {
					if (del == root) {
						if (del->left) {
							root = del->left;
							del->left->parent = nullptr;
							setColor(del->left, Node::BLACK);
						}
						else if (del->right) {
							root = del->right;
							del->right->parent = nullptr;
							setColor(del->right, Node::BLACK);
						}
						else {
							root = nullptr;
						}
						delete del;
					}
					else {
						if (del->left) {
							if (del->parent->left == del) {
								del->parent->left = del->left;
							}
							else {
								del->parent->right == del->left;
							}
							del->left->parent = del->parent;
						}
						else if (del->right) {
							if (del->parent->left == del) {
								del->parent->left = del->right;
							}
							else {
								del->parent->right = del->right;
							}
							del->right->parent = del->parent;
						}
						else {
							if (del->parent->left == del) {
								del->parent->left = nullptr;
							}
							else {
								del->parent->right = nullptr;
							}
						}
						delete del;
					}
					return;
				}
				rec = 1;
				Node* brother = nullptr;
				if (node->parent->left == node) {
					brother = node->parent->right;
				}
				else {
					brother = node->parent->left;
				}

				if (color(brother) == Node::RED) {
					setColor(brother, Node::BLACK);
					if (node->parent->left == node) {
						leftRotate(parent);
						brother = parent->right;
					}
					else {
						rightRotate(parent);
						brother = parent->left;
					}
				}

				if (node->parent->left == node) {
					if (color(brother->right) == Node::RED) {
						setColor(brother->right, Node::BLACK);
						setColor(brother, color(parent));
						setColor(parent, Node::BLACK);
						leftRotate(parent);
					}
					else if (color(brother->left) == Node::RED) {
						setColor(brother->left, color(parent));
						setColor(parent, Node::BLACK);
						rightBalance(parent);
					}
					else {
						setColor(brother, Node::RED);
						if (color(parent) == Node::RED) {
							setColor(parent, Node::BLACK);
							if (del->left) {
								if (del->parent->left == del) {
									del->parent->left = del->left;
								}
								else {
									del->parent->right == del->left;
								}
								del->left->parent = del->parent;
							}
							else if (del->right) {
								if (del->parent->left == del) {
									del->parent->left = del->right;
								}
								else {
									del->parent->right = del->right;
								}
								del->right->parent = del->parent;
							}
							else {
								if (del->parent->left == del) {
									del->parent->left = nullptr;
								}
								else {
									del->parent->right = nullptr;
								}
							}
							delete del;
							return;
						}
						else {
							node = parent;
							parent = parent->parent;
							rec = 0;
						}
					}
				}
				else {
					if (color(brother->left) == Node::RED) {
						setColor(brother->left, Node::BLACK);
						setColor(brother, color(parent));
						setColor(parent, Node::BLACK);
						rightRotate(parent);
					}
					else if (color(brother->right) == Node::RED) {
						setColor(brother->right, color(parent));
						setColor(parent, Node::BLACK);
						leftBalance(parent);
					}
					else {
						setColor(brother, Node::RED);
						if (color(parent) == Node::RED) {
							setColor(parent, Node::BLACK);
							if (del->left) {
								if (del->parent->left == del) {
									del->parent->left = del->left;
								}
								else {
									del->parent->right == del->left;
								}
								del->left->parent = del->parent;
							}
							else if (del->right) {
								if (del->parent->left == del) {
									del->parent->left = del->right;
								}
								else {
									del->parent->right = del->right;
								}
								del->right->parent = del->parent;
							}
							else {
								if (del->parent->left == del) {
									del->parent->left = nullptr;
								}
								else {
									del->parent->right = nullptr;
								}
							}
							delete del;
							return;
						}
						else {
							node = parent;
							parent = parent->parent;
							rec = 0;
						}
					}
				}

			}
			
			if (del->left) {
				if (del->parent->left == del) {
					del->parent->left = del->left;
				}
				else {
					del->parent->right == del->left;
				}
				del->left->parent = del->parent;
			}
			else if (del->right) {
				if (del->parent->left == del) {
					del->parent->left = del->right;
				}
				else {
					del->parent->right = del->right;
				}
				del->right->parent = del->parent;
			}
			else {
				if (del->parent->left == del) {
					del->parent->left = nullptr;
				}
				else {
					del->parent->right = nullptr;
				}
			}
			delete del;
		}

	}

private:

	struct Node {
		enum Color {
			RED,
			BLACK
		};
		Node(const T& v, Color c = RED, Node *p = nullptr, Node * r = nullptr, Node *l = nullptr)
			: val(v), left(l), right(r), parent(p), color(c)
		{

		}
		T val;
		Node* left;
		Node* right;
		Node* parent;
		Color color;
	};

	void adjust(Node* node) {
		Node* parent = node->parent;
		if (parent == root || parent->color == Node::BLACK) {
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

		while (color(node) == Node::RED && color(parent) == Node::RED && color(uncle) == Node::RED) {
			//叔叔节点是红色
			setColor(uncle, Node::BLACK);
			setColor(parent, Node::BLACK);
			setColor(grand, Node::RED);

			node = grand;
			parent = node->parent;
			if (parent == root || node == nullptr || parent == nullptr) {
				setColor(root, Node::BLACK);
				return;
			}

			grand = parent->parent;

			if (grand->left == node) {
				uncle = grand->right;
			}
			else {
				uncle = grand->left;
			}
		}

		if (color(parent) == Node::RED && color(uncle) == Node::BLACK) {
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

	void leftRotate(Node *node) {
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
		}else {
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
	Node* root;
};