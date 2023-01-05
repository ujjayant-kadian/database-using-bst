int main(){
    char data[20] = "Hello";
    Tree_Node *root = create_bst(data);
    // tree_print_sorted(root);
    tree_delete(root);
    // tree_search(root, 'e');
    // tree_search(root, 'z');
    return 0;
}