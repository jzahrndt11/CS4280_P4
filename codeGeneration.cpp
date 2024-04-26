/*
 *  CS4280 - Project P2
 *      - by. Jeremy Zahrndt
 *      - testTree.cpp
 *      Resources:
 *      - https://www.geeksforgeeks.org/tree-traversals-inorder-preorder-and-postorder/
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codeGeneration.h"

// Test Tree function --------------------------------------------------------------------------------------------------
void codeGeneration(node_t* tree) {

    traversePreOrderCodeGen(tree, 0);

    printf("----------------Code Generation Passed--------------------------\n");

}

// Traverse Pre-order function -----------------------------------------------------------------------------------------
void traversePreOrderCodeGen(node_t* root, int level) {
    // Base case
    if (root == NULL) {
        return;
    }

    // Start Logic Here

    // traverse left-most subtree (if needed)
    if (root->childOne != NULL) {
        traversePreOrderCodeGen(root->childOne, level + 1);
    }

    // traverse next subtree (if needed)
    if (root->childTwo != NULL) {
        traversePreOrderCodeGen(root->childTwo, level + 1);
    }

    // traverse next subtree (if needed)
    if (root->childThree != NULL) {
        traversePreOrderCodeGen(root->childThree, level + 1);
    }

    // traverse right-most subtree (if needed)
    if (root->childFour != NULL) {
        traversePreOrderCodeGen(root->childFour, level + 1);
    }
}







