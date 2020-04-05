//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Richard Allen
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>
#include <algorithm>
#include "CSVparser.hpp"
#include <stack>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
	string bidId; // unique identifier
	string title;
	string fund;
	double amount;
	Bid() {
		amount = 0.0;
	}
};

// Internal Node Structure
struct Node {
	Bid nodeBid;
	Node* left;
	Node* right;
};
typedef struct Node* nodeptr;

//============================================================================
// Binary Search Tree class definition
//============================================================================
class BinarySearchTree {
private:
	Node* root;
	void addNode(Node* node, Bid bid);
	void inOrder(Node* node);
	void removeNode(string bidId);

public:
	BinarySearchTree();
	~BinarySearchTree();
	void InOrder();
	void Insert(Bid bid);
	void Remove(string bidId);
	Node* Search(string bidId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
	root = NULL;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
	delete root;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
	inOrder(root);
}

/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
	nodeptr node = new Node;
	node->nodeBid = bid;
	addNode(node, node->nodeBid);
}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {
	// call removeNode, start at root
	removeNode(bidId);
}

void BinarySearchTree::removeNode(string bidId) {
	// tracking nodeptrs
	nodeptr parent = NULL;
	nodeptr curr = root;

	while (curr != NULL) {
		// found match
		if (bidId == curr->nodeBid.bidId) {
			// Leaf
			if (curr->left == NULL && curr->right == NULL) {
				// root with no branches
				if (parent == NULL) {
					root = NULL;
				}
				// reassign left pointer for a left leaf
				else if (parent->left == curr) {
					parent->left = NULL;
				}
				// reassigns right pointer for a right leaf
				else {
					parent->right = NULL;
				}
			}
			// Only left child
			else if (curr->left != NULL && curr->right == NULL) {
				// if node is root, reassigns root to its left node
				if (parent == NULL) {
					root = curr->left;
				}
				// reassigns parents left pointer to the next left node
				else if (parent->left == curr) {
					parent->left = curr->left;
				}
				// same as above, but right
				else {
					parent->right = curr->left;
				}
			}
			// Only right child, same as above, but with right nodes
			else if (curr->left == NULL && curr->right != NULL) {
				if (parent == NULL) {
					root = curr->right;
				}
				else if (parent->left == curr) {
					parent->left = curr->right;
				}
				else {
					parent->right = curr->right;
				}
			}
			// Two children
			else {
				// new tracking node pointers
				nodeptr succ = curr->right;
				nodeptr succ_parent = NULL;
				// find left most node of right branch
				// this is the successor
				while (succ->left != NULL) {
					succ_parent = succ;
					succ = succ->left;
				}
				// create copy of successor
				nodeptr succ_copy = succ;
				// assign the successors copy pointers to correct nodes
				succ_copy->left = curr->left;
				succ_copy->right = curr->right;
				// replace current node with successor copy
				curr = succ_copy;
				// assigns parent node's pointer to the correct node
				if (parent->left->nodeBid.bidId == bidId) {
					parent->left = curr;
				}
				else {
					parent->right = curr;
				}
				// assigns successors parent left pointer to NULL
				succ_parent->left = NULL;
			}
			return;
		}
		// advance right
		else if (curr->nodeBid.bidId < bidId) {
			parent = curr;
			curr = curr->right;
		}
		// advance left
		else {
			parent = curr;
			curr = curr->left;
		}
	}
	return;
}

/**
 * Search for a bid
 */
Node* BinarySearchTree::Search(string bidId) {
	nodeptr curr = root;
	nodeptr match = NULL;
	while (curr != NULL) {
		if (bidId == curr->nodeBid.bidId) {
			match = curr;
			break;
		}
		else if (bidId < curr->nodeBid.bidId) {
			curr = curr->left;
		}
		else {
			curr = curr->right;
		}
	}
	return match;
}

/**
 *
 * Add Nodes to BST
 *
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
	// assigns nodes pointers to NULL
	node->left = NULL;
	node->right = NULL;
	// empty tree, create root
	if (root == NULL) {
		root = node;
	}
	else {
		// start at root
		nodeptr curr = root;
		while (curr != NULL) {

			// check left branch
			if (bid.bidId < curr->nodeBid.bidId) {
				// found empty spot, places node
				if (curr->left == NULL) {
					curr->left = node;
					curr = NULL;
				}
				// advance left
				else {
					curr = curr->left;
				}
			}
			// check right branch
			else {
				// found empty spot, places node
				if (curr->right == NULL) {
					curr->right = node;
					curr = NULL;
				}
				// advance right
				else {
					curr = curr->right;
				}
			}
		}

	}

}


/**
 *
 * Display bid information
 * 
 */
void displayBid(Bid bid) {
	cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
		<< bid.fund << endl;
	return;
}

void BinarySearchTree::inOrder(Node* node) {
	/*
	Updated to a non-recursive in order traversal of bst\
	Avoided stack overflow error
	*/
	stack<nodeptr> s;
	nodeptr curr;
	curr = node;
	while (curr != NULL || s.empty() == false)
	{
		while (curr != NULL)
		{
			s.push(curr);
			curr = curr->left;
		}

		curr = s.top();
		s.pop();
		displayBid(curr->nodeBid);
		curr = curr->right;
	}

	 /*
	 * Using the following caused a stack overflow error.
	 * Displaying to potential problems with using recursion

	if (node == NULL) {
		return;
	}
	inOrder(node->left);
	displayBid(node->nodeBid);
	inOrder(node->right);
	*/
	
}

//============================================================================
// Static methods used for testing
//============================================================================

void loadBids(string csvPath, BinarySearchTree* bst) {
	cout << "Loading CSV file " << csvPath << endl;

	// initialize the CSV Parser using the given path
	csv::Parser file = csv::Parser(csvPath);

	// read and display header row - optional
	vector<string> header = file.getHeader();
	for (auto const& c : header) {
		cout << c << " | ";
	}
	cout << "" << endl;

	try {
		// loop to read rows of a CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			// Create a data structure and add to the collection of bids
			Bid bid;
			bid.bidId = file[i][1];
			bid.title = file[i][0];
			bid.fund = file[i][8];
			bid.amount = strToDouble(file[i][4], '$');

			// push this bid to the end
			bst->Insert(bid);
		}
	}
	catch (csv::Error &e) {
		std::cerr << e.what() << std::endl;
	}
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 */
double strToDouble(string str, char ch) {
	str.erase(remove(str.begin(), str.end(), ch), str.end());
	return atof(str.c_str());
}

int main(int argc, char* argv[]) {

	// process command line arguments
	string csvPath, bidKey;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		bidKey = "98109";
		break;
	case 3:
		csvPath = argv[1];
		bidKey = argv[2];
		break;
	default:
		csvPath = "C:\\Users\\dick_\\source\\repos\\MyProject\\eBid_Monthly_Sales.csv";
		bidKey = "98109";
	}

	// Define a timer variable
	clock_t ticks;

	// Define a binary search tree to hold all bids
	BinarySearchTree* bst;

	Bid bid;
	nodeptr match;

	int choice = 0;
	bst = new BinarySearchTree();
	loadBids(csvPath, bst);

	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << "  1. Display All Bids" << endl;
		cout << "  2. Find Bid" << endl;
		cout << "  3. Remove Bid" << endl;
		cout << "  9. Exit" << endl;
		cout << "Enter choice: ";
		cin >> choice;

		switch (choice) {

		case 1:
			bst->InOrder();
			break;

		case 2:
			ticks = clock();
			match = bst->Search(bidKey);
			if (match != NULL) {
				bid = match->nodeBid;
				displayBid(bid);
			}
			else {
				cout << "Bid Id " << bidKey << " not found." << endl;
			}
			// Timer
			ticks = clock() - ticks;
			cout << "time: " << ticks << " clock ticks" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
			break;
			

		case 3:
			bst->Remove(bidKey);
			break;

		default:
			cout << "Not a valid selection. Please make another selection." << endl;
		}
	}
	bst->~BinarySearchTree();
	cout << "Good bye." << endl;

	return 0;
}
