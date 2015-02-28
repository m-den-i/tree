#include <iostream>
using namespace std;
#include <fstream>
#include <vector>
#include <set>

struct treeNode{
	bool isLeft;
	int key;
	treeNode* left;
	treeNode* right;

	int height;
	int leftSon;
	int rightSon;
	treeNode* parent;
};

int height = 0;

treeNode* createNode(int key, treeNode* tree){
	treeNode* node = new treeNode();
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->leftSon = 0;
	node->rightSon = 0;
	node->parent = tree;
	node->height = tree->height + 1;
	if (height < node->height){
		height = node->height;
	}
	return node;
}

bool find(treeNode* tree, int value){
	if (value < tree->key){
		if (tree->left != NULL){
			find(tree->left, value);
		}
		else{
			return false;
		}
	}
	else if (value > tree->key){
		if (tree->right != NULL){
			find(tree->right, value);
		}
		else{
			return false;
		}
	}
	else {
		return true;
	}
}
void addElement(int key, treeNode* tree){
	if (find(tree, key)){
		return;
	}
	if (key < tree->key){
		if (tree->left == NULL){
			tree->left = createNode(key, tree);
			tree->left->isLeft = true;
			tree->leftSon++;
		}
		else{
			addElement(key, tree->left);
			tree->leftSon++;
		}
	}
	else if (key > tree->key){
		if (tree->right == NULL){
			tree->right = createNode(key, tree);
			tree->right->isLeft = false;
			tree->rightSon++;
		}
		else{
			addElement(key, tree->right);
			tree->rightSon++;
		}
	}
}

void redefine(treeNode* node, treeNode* defineNode){
	node->key = defineNode->key;
	if (defineNode->right != NULL){
		if (defineNode->isLeft){
			defineNode->parent->left = defineNode->right;
			defineNode->right->parent = defineNode->parent;
		}
		else{
			defineNode->parent->right = defineNode->right;
			defineNode->right->parent = defineNode->parent;
		}
	}
	else{
		if (defineNode->isLeft){
			defineNode->parent->left = NULL;
		}
		else{
			defineNode->parent->right = NULL;
		}
	}
}

treeNode* findMinRight(treeNode * node){
	if (node->left != NULL) return findMinRight(node->left);
	else {
		return node;
	}
}


void rightDeleteNode(treeNode * node){
	if (node->left == NULL && node->right == NULL){
		if (node->isLeft){
			node->parent->left = NULL;
		}
		else{
			node->parent->right = NULL;
		}
	}
	if (node->right != NULL){
		redefine(node, findMinRight(node->right));
	}
	else {
		if (node->left != NULL){
			if (node->isLeft){
				node->parent->left = node->left;
				node->left->parent = node->parent;
			}
			else{
				node->parent->right = node->left;
				node->left->parent = node->parent;
			}
		}
	}
}


void printTree(treeNode* tree, ofstream& output){
	if (tree == NULL) return;
	output << tree->key << '\n';
	printTree(tree->left,output);	
	printTree(tree->right,output);
}

vector<treeNode*> rightNodes;
void bypass(treeNode* tree, int height){
	if (tree == NULL) return;
	if (tree->height == height && tree->leftSon > tree->rightSon){
		rightNodes.push_back(tree);
	}
	bypass(tree->left, height);
	
	bypass(tree->right, height);
}

int main(){
	treeNode *tree;
	tree = new treeNode();

	ifstream input("in.txt");
	ofstream output("out.txt");
	//формируем бинарное дерево
	input >> tree->key;	
	tree->left = NULL;
	tree->right = NULL;
	tree->parent = NULL;
	tree->height = 0;
	tree->leftSon = 0;
	tree->rightSon = 0;
	
	while (!input.eof()){
		int value;
		input >> value;
		addElement(value, tree);
	}


	int halfHeight = height / 2;
	bypass(tree, halfHeight);
	if (rightNodes.size() != 0 && (rightNodes.size() % 2) != 0){
		treeNode* deleteNode = rightNodes[rightNodes.size() / 2];
		rightDeleteNode(deleteNode);
	}
	printTree(tree,output);
	input.close();
	output.close();
	return 0;
}