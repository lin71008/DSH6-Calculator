#include <stdlib.h>
#include <string.h>

// typedef enum nodetype {
// 	UNKNOWN_NODE, OPT_NODE, VAL_NODE
// } nodetype;

// typedef enum opttype {
// 	UNKNOWN_OPT, ADD_OPT, SUB_OPT
// } opttype;

typedef struct node {
	struct node *left, *right;
	char val;
} node;

node* newNode(char val) {
	node* ret = (node*) malloc(sizeof(node));
	ret->left = NULL;
	ret->right = NULL;
	ret->val = val;
	return ret;
}

node* biggerestNode(node* head) {
	if (head == NULL) return NULL;
	else if (head->right == NULL) return head;
	else return biggerestNode(head->right);
}

node* smallestNode(node* head) {
	if (head == NULL) return NULL;
	else if (head->left == NULL) return head;
	else return smallestNode(head->left);
}

// // new.key > head.key
// void mergeNode(node** head, node* newnode){
// 	if ((*head) == NULL) {
// 		(*head) = newnode;
// 	} else if (newnode != NULL) {
// 		biggerestNode((*head))->right = newnode->left;
// 		newnode->left = (*head);
// 		(*head) = newnode;
// 	}
// }

// ---

