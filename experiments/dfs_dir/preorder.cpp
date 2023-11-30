// This program has not strictly related to coursera course
// This is supplementary program I used to grasp heap sort to implement
// my solution for challenge 3.7. Maximum Salary
// ============================================================================
//  Name        : binarytree_test.cpp
//  Author      : Aleksey Kravchuk
// ============================================================================

#include <iostream>

/* struct Node { */
/* 	char data; */
/* 	Node* left; */
/* 	Node* right; */
/* }; */

struct Node {
	int data;
	Node* left;
	Node* right;
};


void recPreorder(Node* root) {
	if(root == NULL) {
		return;
	}

	std::cout << root->data << std::endl;
	recPreorder(root->left);
	recPreorder(root->right);
}


void recInorder(Node* root) {
	if(root == NULL) {
		return;
	}
	recInorder(root->left);
	std::cout << root->data << std::endl;
	recInorder(root->right);
}


void recPostorder(Node* root) {
	if(root == NULL) {
		return;
	}
	recPostorder(root->left);
	recPostorder(root->right);
	std::cout << root->data << std::endl;
}

Node* insert(Node* root, int data) {
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

Node* makeSampleTree(Node* root) {
/*Code To Test the logic
  Creating an example tree
            M
		   / \
		  B   Q
		 / \   \
		A   C   Z
*/
	// key nodes are characters
	/* root = insert(root,'M'); */
	/* /\* std::cout << "root = " << root << std::endl; *\/ */

	/* root = insert(root,'B'); */
	/* /\* std::cout << "root = " << root << std::endl; *\/ */

	/* root = insert(root,'Q'); */
	/* /\* std::cout << "root = " << root << std::endl; *\/ */

	/* root = insert(root,'Z'); */
	/* /\* std::cout << "root = " << root << std::endl; *\/ */

	/* root = insert(root,'A'); */
	/* /\* std::cout << "root = " << root << std::endl; *\/ */

	/* root = insert(root,'C'); */

	root = insert(root, 5);
	/* std::cout << "root = " << root << std::endl; */

	root = insert(root, 13);
	/* std::cout << "root = " << root << std::endl; */

	root = insert(root, 4);
	/* std::cout << "root = " << root << std::endl; */

	root = insert(root, 11);
	/* std::cout << "root = " << root << std::endl; */

	root = insert(root, 9);
	/* std::cout << "root = " << root << std::endl; */

	root = insert(root, 24);


	return root;
}


int main() {
	Node* root = NULL;
	root = makeSampleTree(root);

	recPreorder(root);
}
