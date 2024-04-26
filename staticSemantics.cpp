/*
 *  CS4280 - Project P2
 *      - by. Jeremy Zahrndt
 *      - staticSemantics.cpp
 *      Resources:
 *      - https://www.geeksforgeeks.org/tree-traversals-inorder-preorder-and-postorder/
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "staticSemantics.h"

const char* tokenNames1[] = {
        "EOF Token",
        "T1 Token",
        "T2 Token",
        "T3 Token",
        "Unknown"
};

const int TABLE_SIZE = 100;
char symbolTable[TABLE_SIZE][MAX_TOKEN_SIZE2];
bool isNewIdentifier = false;

// Static Semantics function --------------------------------------------------------------------------------------------------
void staticSem(node_t* tree) {
    // initialize symbolTable
    for (int i=0; i<TABLE_SIZE; i++) {
        memset(symbolTable[i], '\0', MAX_TOKEN_SIZE2);
    }

    printf("Traversing for Semantics:\n");
    traversePreOrder(tree, 0);

    printf("----------------Semantics Passed--------------------------\n");
    printf("Symbol Table:\n");
    for (int i=0; i<TABLE_SIZE; i++) {
        if (symbolTable[i][0] != '\0') {
            printf("\t%d: %s\n", i, symbolTable[i]);
        }
    }
}

// Traverse Pre-order function -----------------------------------------------------------------------------------------
void traversePreOrder(node_t* root, int level) {
    // Base case
    if (root == NULL) {
        return;
    }

    if (root->label == 'C') {
        //printf("Setting isNewIdentifier = true\n");
        isNewIdentifier = true;
    }
    else {
        if (root->tokenId == 2) {
            if (isNewIdentifier) {
                //printf("Inserting identifier to symbol table, also Setting isNewIdentifier = false\n");
                insert(root);
                isNewIdentifier = false;
            }
            else {
                //printf("Checking if identifier is in symbol table\n");
                bool inSymbolTable = false;
                inSymbolTable = verify(root);
                if (!inSymbolTable) {
                    printf("staticSemantics.cpp: ERROR - %s is not a declared identifier - Line: %d\n", root->tokenInstance, root->lineNum);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    // traverse left-most subtree (if needed)
    if (root->childOne != NULL) {
        traversePreOrder(root->childOne, level + 1);
    }

    // traverse next subtree (if needed)
    if (root->childTwo != NULL) {
        traversePreOrder(root->childTwo, level + 1);
    }

    // traverse next subtree (if needed)
    if (root->childThree != NULL) {
        traversePreOrder(root->childThree, level + 1);
    }

    // traverse right-most subtree (if needed)
    if (root->childFour != NULL) {
        traversePreOrder(root->childFour, level + 1);
    }
}

// insert the string if not already there or error if already there
void insert(node_t* identifier) {
    //printf("Made it to insert() function\n");
    for (int i=0; i<TABLE_SIZE; i++) {
        if (symbolTable[i][0] != '\0') {
            if (strcmp(symbolTable[i], identifier->tokenInstance) == 0) {
                printf("staticSemantics.cpp: ERROR - %s declared multiple times - Line: %d\n", identifier->tokenInstance, identifier->lineNum);
                exit(EXIT_FAILURE);
            }
        }
        else {
            //printf("Inserting %s into symbol table\n", identifier->tokenInstance);
            //identifier->tokenInstance[0] = 'v';
            strcpy(symbolTable[i], identifier->tokenInstance);
            break;
        }
    }
}

// return true if the string is already there and false otherwise
bool verify(node_t* identifier) {
    //printf("Made it to verify() function\n");

    for (int i=0; i<TABLE_SIZE; i++) {
        if (strcmp(symbolTable[i], identifier->tokenInstance) == 0) {
            //printf("%s was FOUND declared in the symbol table\n", identifier->tokenInstance);
            return true;
        }
    }
    //("%s was NOT-FOUND declared in the symbol table\n", identifier->tokenInstance);
    return false;
}







