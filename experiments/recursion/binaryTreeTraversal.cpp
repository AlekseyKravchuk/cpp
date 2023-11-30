/* Binary Tree Traversal - Preorder, Inorder, Postorder */
#include <iostream>
#include <vector>
#include <memory>

using std::cout;
using std::cin;
using std::endl;
using std::flush;

struct Node {
    char data;
    struct Node *left;
    struct Node *right;
};

void myPreOrderTraversal(Node* root) {
    if(root == nullptr) {
        return;
    }
    // visit current node; visiting a node refers to
    cout << root->data << " " << flush;
    myPreOrderTraversal(root->left);
    myPreOrderTraversal(root->right);
}

void myInOrderTraversal(Node* root) {
    if(root == nullptr) {
        return;
    }
    myInOrderTraversal(root->left);
    cout << root->data << " " << flush;
    myInOrderTraversal(root->right);
}

void myPostOrderTraversal(Node* root) {
    if(root == nullptr) {
        return;
    }
    myPostOrderTraversal(root->left);
    myPostOrderTraversal(root->right);
    cout << root->data << " " << flush;
}


// Function to Insert Node in a Binary Search Tree
Node* Insert(Node* root, char data) {
    if(root == NULL) {
        root = new Node();
        root->data = data;
        root->left = root->right = NULL;
    }
    else if(data <= root->data)
        root->left = Insert(root->left,data);
    else
        root->right = Insert(root->right,data);
    return root;
}


int main() {
    /*Code To Test the logic
      Creating an example tree
                 F
              /     \
             D       J
           /  \    /   \
          B    E  G     K
        /  \       \
       A    C       I
                  /
                 H
    */
    Node* root = nullptr;
    // std::unique_ptr<Node> root(new Node(nullptr));
    std::vector<char> letters {'F', 'D', 'J', 'B', 'E', 'G', 'K', 'A', 'C', 'I', 'H'};

    for(auto letter: letters) {
        root = Insert(root, letter);
    }

    //Print Nodes in Preorder.
    cout<<"Preorder:  ";
    myPreOrderTraversal(root);
    cout<<"\n";

    //Print Nodes in Inorder
    // Note Important!!!
    // !!! Inorder traversal of BST gives us a sorted list !!!
    cout<<"Inorder:   ";
    myInOrderTraversal(root);
    cout<<"\n";

    //Print Nodes in Postorder
    cout<<"Postorder: ";
    myPostOrderTraversal(root);
    cout<<"\n";
}
