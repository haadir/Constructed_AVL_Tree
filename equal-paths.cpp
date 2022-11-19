#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int height(Node* root);
//bool isBalanced(Node *root);

bool equalPaths(Node * root)
{
		return height(root) != -1;
}

int height(Node* root) {
	if (root == nullptr) return 0;

	int heightL = height(root->left);
	int heightR = height(root->right);

	if (heightL == -1 || heightR == -1) {
		return -1;
	}

	if (heightL == 0) {
		return heightR + 1;
	}
	if (heightR == 0) {
		return heightL + 1;
	}
	if (heightL != heightR) {
		return -1;
	}
	return heightL + 1;
}

// bool isBalanced(Node *root) {
// 	if (root == nullptr) return 1;
// 	int leftLeaf = calculateHeightIfBalanced(root->left);
// 	int rightLeaf = calculateHeightIfBalanced(root->right);

// 	if (abs(leftLeaf - rightLeaf) <= 1 && isBalanced(root->left) && isBalanced(root->right)) {
// 		return 1;
// 	}
// 	return 0;
// }

//isbalanced isbalancedhelper int subtreeheight

