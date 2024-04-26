/*
 *  CS4280 - Project P2
 *      - by. Jeremy Zahrndt
 *      - codeGeneration.cpp
 *      Resources:
 *      - https://www.geeksforgeeks.org/tree-traversals-inorder-preorder-and-postorder/
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "codeGeneration.h"
#include "parser.h"


// Code Generation function --------------------------------------------------------------------------------------------------
void codeGeneration(node_t* tree) {
    char fileBuf[100];
    sprintf(fileBuf,"%s.asm", file);
    filePointer = fopen(fileBuf, "w");

    if (tree->label == 'S') {
        codeGenS(tree);
    } else {
        printf("codeGeneration.cpp: ERROR running codeGenS() - label was not S");
    }

    // Add STOP and symbolTable to codeGenerationFile
    fprintf(filePointer, "STOP\n");
    for (int i = 0; i < 100; i++) {
        if (symbolTable[i][0] != '\0') {
            fprintf(filePointer, "%s 0\n", symbolTable[i]);
        }
    }
}

// S -> CD      read an int and allocate memory, any number of additional operations ---------------------------------------------------------
void codeGenS(node_t* sNode) {
    printf("codeGenS()\n");

    codeGenC(sNode->childOne);
    codeGenD(sNode->childTwo);
}

// A -> FX      sum | int or identifier ------------------------------------------------------------------------------------------------------
void codeGenA(node_t* aNode) {
    printf("codeGenA()\n");

    codeGenF(aNode->childOne);
    codeGenX(aNode->childTwo);
}

// B -> .t2A!      assign value of A to identifier t2 ----------------------------------------------------------------------------------------
void codeGenB(node_t* bNode) {
    printf("codeGenB()\n");


    // ...


}

// C -> t2*     read in int, allocate memory (ex: v10 for %10), assign value = int ------------------------------------------------------------
void codeGenC(node_t* cNode) {
    printf("codeGenC()\n");

    fprintf(filePointer, "READ %s\n", cNode->childOne->tokenInstance);
}

// D -> Y       originally: D -> H?D | empty for recursion ------------------------------------------------------------------------------------
void codeGenD(node_t* dNode) {
    printf("codeGenD()\n");

    codeGenY(dNode->childOne);
}

// E -> ,AAH | ,;FH     if first A > second A, do H   |    do H, F times  ---------------------------------------------------------------------
void codeGenE(node_t* eNode) {
    printf("codeGenE()\n");

}

// F -> t1 | t2        number  |  identifier  -------------------------------------------------------------------------------------------------
void codeGenF(node_t* fNode) {
    printf("codeGenF()\n");

    // T1 Token
    if (fNode->tokenId == T1_Token) {
        if (isalpha(fNode->tokenInstance[0])) {
            fprintf(filePointer, "LOAD %s\n", fNode->tokenInstance + 1);
        } else {
            fprintf(filePointer, "LOAD -%s\n", fNode->tokenInstance + 1);
        }
    }
    // T2 Token
    else {
        fprintf(filePointer, "LOAD %s\n", fNode->tokenInstance);
    }
}

// G -> B | C | J       assignment  |  read int and allocate memory  |  print value to screen  ------------------------------------------------
void codeGenG(node_t* gNode) {
    printf("codeGenG()\n");
}

// H -> E? | G. | empty     if, for   |  assignment, read int and allocate memory  |  print value  --------------------------------------------
void codeGenH(node_t* hNode){
    printf("codeGenH()\n");
}

// J -> *"A.        print integer value to screen (sum, int, or identifier) -------------------------------------------------------------------
void codeGenJ(node_t* jNode) {
    printf("codeGenJ()\n");
    fprintf(filePointer, "WRITE ");
}

// X -> F?$ | .
void codeGenX(node_t* xNode) {
    printf("codeGenX()\n");

}

// Y -> H?Y | empty
void codeGenY(node_t* yNode) {
    printf("codeGenY()\n");

}





