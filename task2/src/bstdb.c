#include "bstdb.h"
#include <stdio.h>
#include <limits.h>//For INT_MAX
#include <stdlib.h>//For malloc
// Write your submission in this file
//
// A main function and some profiling tools have already been set up to test
// your code in the task2.c file. All you need to do is fill out this file
// with an appropriate Binary Search Tree implementation.
//
// The input data will be books. A book is comprised of a title and a word
// count. You should store both these values in the tree along with a unique
// integer ID which you will generate.
//
// We are aiming for speed here. A BST based database should be orders of
// magnitude faster than a linked list implementation if the BST is written
// correctly.
//
// We have provided an example implementation of what a linked list based
// solution to this problem might look like in the db/listdb.c file. If you
// are struggling to understand the problem or what one of the functions
// below ought to do, consider looking at that file to see if it helps your
// understanding.
//
// There are 6 functions you need to look at. Each is provided with a comment
// which explains how it should behave. The functions are:
//
//  + bstdb_init
//  + bstdb_add
//  + bstdb_get_word_count
//  + bstdb_get_name
//  + bstdb_stat
//  + bstdb_quit
//
// Do not rename these functions or change their arguments/return types.
// Otherwise the profiler will not be able to find them. If you think you
// need more functionality than what is provided by these 6 functions, you
// may write additional functions in this file.

//Data Structure for the database - Binary Search Tree
typedef struct Tree_Node {
	int doc_id;
  char *name;
	int word_count;
  struct Tree_Node *left, *right;
} Tree_Node;

//Root of the Tree
Tree_Node *root;
//Variables needed for bstdb_stat
int id_collisions, number_of_nodes, number_of_inserts, number_of_searches, number_of_traversals_for_search, flag;

//Only need to initialise root and variables need for stat - nodes are created dynamically
int
bstdb_init ( void ) {
	// This function will run once (and only once) when the database first
	// starts. Use it to allocate any memory you want to use or initialize 
	// some globals if you need to. Function should return 1 if initialization
	// was successful and 0 if something went wrong.
	root = NULL;
	id_collisions = 0;
	number_of_inserts = 0;
	number_of_nodes = 0;
	number_of_searches = 0;
	number_of_traversals_for_search = 0;
	flag = 0;
	return 1;
}

//To create a node to add in the BST
Tree_Node* 
create_node ( int doc_id, char *name, int word_count ) {
	Tree_Node *new_node = (Tree_Node *)malloc(sizeof(Tree_Node));
	if(new_node != NULL) {
		new_node->doc_id = doc_id;
		new_node->name = name;
		new_node->word_count = word_count;
		new_node->left = NULL;
		new_node->right = NULL;
		number_of_nodes++;
	}

	return new_node;
}


//Using hash function to generate unique ids - Used by me in assignment 1 (hash tables)
//Distributes the string to generate numbers as far from each other
int
hash2 ( char *str ) {
	unsigned long hash = 5381;
	int i = 0;
	while (str[i] != '\0') {
			// hash = ((hash << 5) + hash) + c; 
			hash = hash * 33 + str[i];
			i++;
	}

	return (hash % INT_MAX);
}

//Recursive approach for adding nodes according to doc_id/unique_id - similar to task1
Tree_Node* 
tree_insert( Tree_Node *root, char *name, int word_count, int unique_id ){
	if(root == NULL){
		return create_node(unique_id, name, word_count);;
	} 
	if(root->doc_id > unique_id){
		root->left = tree_insert(root->left, name, word_count, unique_id);
	} else if(root->doc_id < unique_id){
		root->right = tree_insert(root->right, name, word_count, unique_id);
	} else if(root->doc_id == unique_id){
		//if the ids generated are equal, then
		id_collisions++;//increment the count of id collisions
		unique_id++;//increment the unique id by 1 - similar to linear probing
		flag++;//for bstdb_add
		root->right = tree_insert(root->right, name, word_count, unique_id);
	}
	return root;
}
int
bstdb_add ( char *name, int word_count ) {
	// This function should create a new node in the binary search tree, 
	// populate it with the name and word_count of the arguments and store
	// the result in the tree.
	//
	// This function should also generate and return an identifier that is
	// unique to this document. A user can find the stored data by passing
	// this ID to one of the two search functions below.
	//
	// How you generate this ID is up to you, but it must be an integer. Note
	// that this ID should also form the keys of the nodes in your BST, so
	// try to generate them in a way that will result in a balanced tree.
	//
	// If something goes wrong and the data cannot be stored, this function
	// should return -1. Otherwise it should return the ID of the new node
	number_of_inserts++;//Increasing this count every time the function is called to represent Total Inserts
	int unique_id = hash2(name);
	
	root = tree_insert(root, name, word_count, unique_id);//adding the node in the tree

	if (root){
		if (flag){//if there was any change in the unique id for creating the node
			unique_id += flag;//adding flag to the unique_id - how many times was unique_id incremented
			flag = 0; //resetting the flag
			return unique_id;
		} else{
			return unique_id;
		}
	}
	//If root of the tree is not created or gets deleted by any reason
	return -1;
}

//Recursive approach for search nodes according to doc_id - similar to task1
Tree_Node* 
tree_search ( Tree_Node *root, int doc_id ) {
	if(root != NULL){
        if (root->doc_id == doc_id) {
            return root;
        } else if(root->doc_id > doc_id ) {
						number_of_traversals_for_search++;//incrimenting count every time you move to the next node.
            return tree_search(root->left, doc_id);
        } else if (root->doc_id < doc_id) {
						number_of_traversals_for_search++;//incrimenting count every time you move to the next node.
            return tree_search(root->right, doc_id);
        }
    }
    //If not found returns null
    return NULL;
}

//using the tree_search function to retrieve node with same doc_id
int
bstdb_get_word_count ( int doc_id ) {
	// This is a search function. It should traverse the binary search tree
	// and return the word_count of the node with the corresponding doc_id.
	//
	// If the required node is not found, this function should return -1
	//Incrementing number of searches every time the function is called
	number_of_searches++;

	Tree_Node *search_result = tree_search(root, doc_id);
	if (search_result->doc_id == doc_id) {
		return search_result->word_count;
	}
	return -1;
}

//using the tree_search function to retrieve node with same doc_id
char*
bstdb_get_name ( int doc_id ) {
	// This is a search function. It should traverse the binary search tree
	// and return the name of the node with the corresponding doc_id.
	//
	// If the required node is not found, this function should return NULL or 0
	//Incrementing number of searches every time the function is called
	number_of_searches++;
	Tree_Node *search_result = tree_search(root, doc_id);
	if(search_result->doc_id == doc_id) {
		return search_result->name;
	}
	return 0;
}


//To check whether the tree is balanced or not
//Finding larger of two integers
int 
max ( int a, int b) {
	return (a >= b) ? a : b;
}

//For finding height of each subtree - recursive approach
int
height ( Tree_Node *root ) {
	//if the tree is empty
	if (root == NULL)
		return 0;
	
	return 1 + max(height(root->left), height(root->right));//Finding the largest amongst the both left and right subtrees
}

//Function to check if the BST is balanced - recursively checks if each of the sub tree is balanced or not
int
is_tree_balanced (Tree_Node *root) {
	//If there is not tree or you have reached the leaves of the tree
	if (root == NULL)
		return 1;
	
	int left_subtree_height = height(root->left);
	int right_subtree_height = height(root->right);

	if (abs(right_subtree_height - left_subtree_height) <= 1 && is_tree_balanced(root->left) && is_tree_balanced(root->right))
		return 1;

	//If tree is not balanced
	return 0;
}

void
bstdb_stat ( void ) {
	// Use this function to show off! It will be called once after the 
	// profiler ends. The profiler checks for execution time and simple errors,
	// but you should use this function to demonstrate your own innovation.
	//
	// Suggestions for things you might want to demonstrate are given below,
	// but in general what you choose to show here is up to you. This function
	// counts for marks so make sure it does something interesting or useful.
	//  + Check if your tree is balanced and print the result
	printf("STAT\n");
	printf("-----\n");
	printf("Is the BST balanced?\n%s\n\n", is_tree_balanced(root) ? "Yes!" : "No!" );
	//  + Does the number of nodes in the tree match the number you expect
	//    based on the number of insertions you performed?
	printf("Do 'Total Inserts' match with the number of nodes in the tree?\n");
	printf("Total Inserts -> %d\nNumber of Nodes -> %d\n%s\n\n", number_of_inserts, number_of_nodes,number_of_inserts == number_of_nodes ? "Yes!" : "No!");
	//
	//  + How many nodes on average did you need to traverse in order to find
	//    a search result? 
	float average_traversals = (float)number_of_traversals_for_search/number_of_searches;
	printf("Total Number of Traversals -> %d\nNumber of Searches -> %d\nAverage number of traversals per search -> %f\n\n", number_of_traversals_for_search, number_of_searches, average_traversals);
	//  + Can you prove that there are no accidental duplicate document IDs
	//    in the tree?
	printf("Are there any collisions between the unique ids generated?\n");
	if (id_collisions){
		printf("Yes, there are %d accidental collisions between the ids generated.\n\n", id_collisions);
	} else{
		printf("All the ids generated are unique.\n\n");
	}
}

//Recursive approach to delete every node of the tree - similar to task1
void 
tree_delete ( Tree_Node* root ) {
	//If there is no tree or reached the leaves of BST - return
	if(root == NULL){
			return;
	}
	tree_delete(root->left);
	tree_delete(root->right);
	free(root);
}



//Uses tree_delete
void
bstdb_quit ( void ) {
	// This function will run once (and only once) when the program ends. Use
	// it to free any memory you allocated in the course of operating the
	// database.
	tree_delete(root);
}
