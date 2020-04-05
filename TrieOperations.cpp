//============================================================================
// Name        : TrieOperations.cpp
// Author      : Richard Allen
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Simple Trie data structure operations
//============================================================================

#include <iostream>
#include <algorithm>
#include <string>
#define CHAR_SIZE 128

using namespace std;

// TrieNode structure
struct TrieNode {
	// Array of nodes large enough for all printable ASCII characters
	struct TrieNode *children[CHAR_SIZE];

	// True for last letter of word
	bool isLastChar;
};

struct TrieNode *newNode(void) {
	struct TrieNode *node = new TrieNode;
	node->isLastChar = false;

	// Initialize with nullptrs
	for (unsigned int i = 0; i < CHAR_SIZE; i++) {
		node->children[i] = nullptr;
	}

	return node;
}

// Inserting a new string to the trie
void insert(struct TrieNode *root, string key) {
	// Start at root
	struct TrieNode *curr = root;
	
	// Assign each letter as a new node in it's perspective place
	for (unsigned int i = 0; i < key.length(); i++) {
		
		// Convert ASCII value to usable index
		int index = key[i] - ' ';
		if (curr->children[index] == nullptr) {

			curr->children[index] = newNode();
		}
		curr = curr->children[index];
	}
	// Mark as last character of that word
	curr->isLastChar = true;
	cout << key << " Inserted." << endl;
}

// Search Trie function
bool search(TrieNode *node, string key)
{
	struct TrieNode *curr = node;

	for (unsigned int i = 0; i < key.length(); i++)
	{
		// Convert ASCII value to usable index
		int index = key[i] - ' ';
		if (curr->children[index] == nullptr) {
			return false;
		}
		curr = curr->children[index];
	}
	// Only true when it finds all characters and last is an end of word.
	return (curr != nullptr && curr->isLastChar);
}

int main() {
	//Initialize root node
	struct TrieNode *root = newNode();
	string choice;
	
	// Continuing getting words from user
	while (choice != "0") {
		cout << "Enter words to input to trie." << endl;
		cout << "Enter 0 to quit." << endl;
		cin >> choice;
		if (choice == "0") {
			break;
		}
		insert(root, choice);
	}
	choice = "";
	
	// Continue getting words to search for
	while (choice != "0") {
		cout << "Enter a word to search for." << endl;
		cout << "Enter 0 to quit." << endl;
		cin >> choice;
		if (choice == "0") {
			break;
		}
		if (search(root, choice)) {
			cout << "Found it!" << endl;
		}
		else
			cout << "Word not found." << endl;
	}

}