/*
 *  CS4280 - Project P4
 *      - by. Jeremy Zahrndt
 *      - buildTree.cpp
 *
 *  Resources:
 *      - https://www.w3resource.com/c-programming-exercises/tree/c-tree-exercises-1.php
 */

#include <stdio.h>
#include <stdlib.h>
#include "buildTree.h"

// Create a New Node of the Binary Tree --------------------------------------------------------------------------------
node_t* create(char label) {
    node_t* newNode = (node_t*)malloc(sizeof(node_t));

    // check if memory allocation was successful
    if (newNode != NULL) {
        // Initialize node data
        newNode->label = label;
        newNode->childOne = NULL;
        newNode->childTwo = NULL;
        newNode->childThree = NULL;
        newNode->childFour = NULL;
        newNode->tokenInstance[0] = '\0';
        newNode->tokenId = 4; // Unknown
        newNode->lineNum = 0;
    }
    return newNode;
}
