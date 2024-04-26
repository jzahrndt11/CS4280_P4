/*
 *  CS4280 - Project P2
 *      - by. Jeremy Zahrndt
 *      - codeGeneration.cpp
 *      Resources:
 *      - https://www.geeksforgeeks.org/tree-traversals-inorder-preorder-and-postorder/
 */
#include <stdio.h>
#include <stdlib.h>
#include "codeGeneration.h"
#include "parser.h"

// Code Generation function --------------------------------------------------------------------------------------------------
void codeGeneration(node_t* tree) {

    traversePreOrderCodeGen(tree, 0);

    printf("----------------Code Generation Passed--------------------------\n");

    // Add symbolTable to codeGenerationFile
    printf("STOP\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (symbolTable[i][0] != '\0') {
            printf("%s 0\n", symbolTable[i]);
        }
    }
}

// Traverse Pre-order function -----------------------------------------------------------------------------------------
void traversePreOrderCodeGen(node_t* root, int level) {
    // Base case
    if (root == NULL) {
        return;
    }

    // Start Logic Here
    switch (root->label) {
        case: 'S':
            //run function codeGenS(); - (Dont think I need)
            break;
        case: 'A':
            //run function codeGenA();
            break;
        case: 'B':
            //run function codeGenB();
            break;
        case: 'C':
            codeGenC(root);
            break;
        case: 'D':
            //run function codeGenD();
            break;
        case: 'E':
            //run function codeGenE();
            break;
        case: 'F':
            //run function codeGenF();
            break;
        case: 'G':
            //run function codeGenG();
            break;
        case: 'H':
            //run function codeGenH();
            break;
        case: 'J':
            //run function codeGenJ();
            break;
        case: 'X':
            //run function codeGenX();
            break;
        case: 'Y':
            //run function codeGenY();
            break;
    }

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

// S -> CD      read an int and allocate memory, any number of additional operations ----------------------------------------------------------
void codeGenS(node_t* sNode) {
    printf("codeGenS()\n");


}

// A -> FX      sum | int or identifier --------------------------------------------------------------------------------------------------------
void codeGenA(node_t* aNode) {
    printf("codeGenA()\n");

}

// B -> .t2A!       -------------------------------------------------
void codeGenB(node_t* bNode) {
    printf("codeGenB()\n");

}

// C -> t2*     read in int, allocate memory (ex: v10 for %10), assign value = int ----------------------------------
void codeGenC(node_t* cNode) {
    printf("codeGenC()\n");

    printf("READ %s\n", cNode->tokenInstance);
}

// D -> Y           ( First set: , ,; . t2 *" ? empty )
void codeGenD(node_t* dNode) {
    printf("codeGenD()\n");

}

// E -> ,AAH | ,;FH             ( First set: , | ,; ) -----------------------------------------------------------------------------------------------
void codeGenE(node_t* eNode) {
    printf("codeGenE()\n");

}

// F -> t1 | t2         ( First set: t1 | t2 ) --------------------------------------------------------------------------------------------------------
void codeGenF(node_t* fNode) {
    printf("codeGenF()\n");

}

// G -> B | C | J           ( First set: . | t2 | *" ) --------------------------------------------------------------------------------------------
void codeGenG(node_t* gNode) {
    printf("codeGenG()\n");

}

// H -> E? | G. | empty         ( First set: , ,; | . t2 *" | empty ) -------------------------------------------------------------------------------
void codeGenH(node_t* hNode){
    printf("codeGenH()\n");

}

// J -> *"A.        ( First set: *" ) ----------------------------------------------------------------------------------------------------------
void codeGenJ(node_t* jNode) {
    printf("codeGenJ()\n");

}

// X -> F?$ | .         ( First set: t1 t2 | . ) ----------------------------------------------------------------------------------------------
void codeGenX(node_t* xNode) {
    printf("codeGenX()\n");

}

// Y -> H?Y | empty         ( First set: , ,; . t2 *" ? empty | empty ) ------------------------------------------------------------------------
void codeGenY(node_t* yNode) {
    printf("codeGenY()\n");

}





