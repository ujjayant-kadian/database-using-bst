#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "bst.h"

//Adds a node to the Binary Search Tree according to its position
void tree_insert(Tree_Node** root, char data){
    
    if(*root == NULL){
        *root = (Tree_Node *)malloc(sizeof(Tree_Node));
        (*root)->data = data;
        (*root)->left = NULL;
        (*root)->right = NULL;
    } else if(data <= (*root)->data){//Considers duplicate value
        //Recursively travel to all the nodes that have value of data lesser than passed data's value - leftmost as per the data
        tree_insert(&((*root)->left), data);
    } else if (data > (*root)->data){
        //Recursively travel to all the nodes that have value of data higher than passed data's value - rightmost as per the data
        tree_insert(&((*root)->right), data);
    }
}
//Creates a Binary Search Tree based on the string passed.
Tree_Node* create_bst (char data[]){
    Tree_Node *root = NULL;
    int i = 0;
    char value;
    while(data[i] != '\0'){
        value = toupper(data[i]);//Capitalising every char of string
        tree_insert(&root, value);
        i++;
    }
    return root;
}
//Searching for a char in the BST - Recursively traverses by following the conditions of BST
Tree_Node* tree_search(Tree_Node* root, char data){
    char value = toupper(data);
    if(root != NULL){
        if (root->data == value){
            return root;
        } else if(root->data > value){
            return tree_search(root->left, data);
        } else if (root->data < value){
            return tree_search(root->right, data);
        }
    }
    //If not found returns null
    return NULL;
}
//Prints sorted string based on alphabetical order - Recursive approach
void tree_print_sorted(Tree_Node* root){
    if (root != NULL){
        tree_print_sorted(root->left);
        printf("%c", root->data);
        tree_print_sorted(root->right);
    }
}
//Deleting every node of the tree
void tree_delete(Tree_Node* root){
    //If there is no tree or reached the leaves of BST - return
    if(root == NULL){
        return;
    }
    tree_delete(root->left);
    tree_delete(root->right);
    free(root);
}