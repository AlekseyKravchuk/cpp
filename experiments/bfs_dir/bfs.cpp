//============================================================================
// Name        : bfs.cpp
// Description : Breadth First Search (Traversal) or Level-Order Traversal
// Author      : Kravchuk Aleksey
//============================================================================


#include <iostream>
#include <queue>
#include <memory>


struct Node {
	char data;
	Node* left;
	Node* right;
};


// Function to print Nodes in a binary tree in Level order
void printBTreeInLevelOrder(Node* root) {
	if(root == NULL) { return; }

	// the queue containing the addresses of children Nodes
	// of the current Node (which we have visited recently)
	// by VISITING a node we mean the reading or processing
	// the data in that node
	std::queue<Node*> q;

	// Initially we start with one discovered node in the queue, i.e. with root
	// A node in a queue is called DISCOVERED node, i.e. the node whose
	// address is known to us but we haven't visited it yet.
	q.push(root);

	while(!q.empty()) {
		// inside the loop we should take out a node from the front of the queue
		Node* current = q.front();

		// VISIT current node:
		std::cout << current->data << " ";

		// keep addresses of all childrens of the current node in the queue
		if(current->left  != NULL) {q.push(current->left);}
		if(current->right != NULL) {q.push(current->right);}

		q.pop(); // removing the element at front
	}
}

// Function to insert Node in a Binary Search Tree
Node* insert(Node* root, char data) {
	if(root == NULL) {
		root = new Node();
		root->data = data;
		root->left = root->right = NULL;
	} else if(data <= root->data) {
		root->left = insert(root->left, data);
	} else {
		root->right = insert(root->right, data);
	}
	return root;
}

int main() {
	/*Code To Test the logic
	  Creating an example tree
	            M
			   / \
			  B   Q
			 / \   \
			A   C   Z
    */
	Node* root = NULL;
	root = insert(root,'M');
	/* std::cout << "root = " << root << std::endl; */

	root = insert(root,'B');
	/* std::cout << "root = " << root << std::endl; */

	root = insert(root,'Q');
	/* std::cout << "root = " << root << std::endl; */

	root = insert(root,'Z');
	/* std::cout << "root = " << root << std::endl; */

	root = insert(root,'A');
	/* std::cout << "root = " << root << std::endl; */

	root = insert(root,'C');
	/* std::cout << "root = " << root << std::endl; */
	
	/* root = insert(root, 90); */
	/* root = insert(root, 30); */
	/* root = insert(root, 120); */
	/* root = insert(root, 15); */
	/* root = insert(root, 7); */
	/* root = insert(root, 15); */
	/* root = insert(root, 94); */
	/* root = insert(root, 130); */

	//Print Nodes in Level Order.
	printBTreeInLevelOrder(root);
	std::cout << std::endl;
}
