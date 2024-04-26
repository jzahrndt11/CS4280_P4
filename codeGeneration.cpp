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
#include <string.h>
#include "codeGeneration.h"
#include "parser.h"


// Code Generation function ---------------------------------------------------------------
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

/*
 *   S -> CD
 *          - read an int and allocate memory, any number of additional operations
 *          - childOne: C
 *          - childTwo: D
 */
void codeGenS(node_t* sNode) {
    printf("-> codeGenS(start)\n");

    codeGenC(sNode->childOne);
    codeGenD(sNode->childTwo);

    printf("<- codeGenS(end)\n");
}

/*
 *   A -> FX
 *          - sum | int or identifier
 *          - childOne: F
 *          - childTwo: X
 *   X -> F?$ | .
 *          - INFO ...
 *          - childOne: F  |  .
 *          - childTwo: ?$
 */
int codeGenA(node_t* aNode) {
    printf("-> codeGenA(start)\n");

    // A -> X -> ?$  ==  sum
    if (strcmp(aNode->childTwo->childTwo->tokenInstance, "?$") == 0) {
        int sum = 0;
        sum = atoi(aNode->childOne->tokenInstance) + atoi(aNode->childTwo->childOne->tokenInstance);
        printf("codeGenA(): sum = %d", sum);

        printf("<- codeGenA(end)\n");
        return sum;
    }
    // A -> X -> . ==  int of identifier
    else {
        int num = 0;
        num = atoi(aNode->childOne->tokenInstance);

        printf("<- codeGenA(end)\n");
        return num;
    }
}


/*
 *   B -> .t2A!
 *          - assign value of A to identifier t2
 *          - childOne: .
 *          - childTwo: t2 Token
 *          - childThree: A
 *          - childFour: !
 */
void codeGenB(node_t* bNode) {
    printf("-> codeGenB(start)\n");

    // Logic Goes here ...
    char tempBuf[100];
    int aNum = codeGenA(bNode->childThree);
    sprintf(tempBuf, "%d", aNum);

    fprintf(filePointer, "LOAD %s\nSTORE %s\n", tempBuf, bNode->childTwo->tokenInstance);

    printf("<- codeGenA(end)\n");
}

/*
 *   C -> t2*
 *          - read in int, allocate memory (ex: v10 for %10), assign value = int
 *          - childOne: t2 token
 *          - childTwo: *
 */
void codeGenC(node_t* cNode) {
    printf("-> codeGenC(start)\n");

    fprintf(filePointer, "READ %s\n", cNode->childOne->tokenInstance);

    printf("<- codeGenC(end)\n");
}

/*
 *   D -> Y
 *          - originally: D -> H?D | empty for recursion
 *          - childOne: Y
 */
void codeGenD(node_t* dNode) {
    printf("-> codeGenD(start)\n");

    codeGenY(dNode->childOne);

    printf("<- codeGenD(end)\n");
}

/*
 *   E -> ,AAH | ,;FH
 *          - if first A > second A, do H   |    do H, F times
 *          - childOne:  ,  |  ,;
 *          - childTwo: A  |  F
 *          - childThree: A  |  H
 *          - childFour: H  |  empty
 */
void codeGenE(node_t* eNode) {
    printf("-> codeGenE(start)\n");

    // Logic Here ...

    printf("<- codeGenE(end)\n");
}

/*
 *   F -> t1 | t2
 *          - number  |  identifier
 *          - childOne: t1 Token  |  t2 Token
 */
char* codeGenF(node_t* fNode) {
    printf("-> codeGenF(start)\n");

    // LOGIC goes Here ...
    if (fNode->childOne->tokenId == T2_Token) {
        return fNode->childOne->tokenInstance;
    } else {
        if (isalpha(fNode->childOne->tokenInstance[0]) == 0) {
            return fNode->childOne->tokenInstance - 1;
        } else {
            char* strBuff;
            sprintf(strBuff, "-%s", fNode->childOne->tokenInstance - 1);
            return strBuff;
        }
    }
    printf("<- codeGenF(end)\n");
}

/*
 *   G -> B | C | J
 *          - assignment  |  read int and allocate memory  |  print value to screen
 *          - childOne: B  |  C  |  J
 */
void codeGenG(node_t* gNode) {
    printf("-> codeGenG(start)\n");

    // Logic Here ...
    if (gNode->childOne->label == 'B') {
        codeGenB(gNode->childOne);
    } else if (gNode->childOne->label == 'C') {
        codeGenC(gNode->childOne);
    } else {
        codeGenJ(gNode->childOne);
    }

    printf("<- codeGenG(end)\n");
}

/*
 *   H -> E? | G. | empty
 *          - if, for   |  assignment, read int and allocate memory  |  print value
 *          - childOne: E  |  G  | empty
 *          - childTwo: ?  |  .
 */
void codeGenH(node_t* hNode){
    printf("-> codeGenH(start)\n");

    // Logic Here ...
    if (strcmp(hNode->childOne->tokenInstance, "Empty") == 0) {
        printf("<- codeGenH(end)\n");
        return;
    }
    else if (hNode->childOne->label == 'E') {
        codeGenE(hNode->childOne);
    } else {
        codeGenG(hNode->childOne);
    }

    printf("<- codeGenH(end)\n");
}

/*
 *   J -> *"A.
 *          - print integer value to screen (sum, int, or identifier)
 *          - childOne: *"
 *          - childTwo: A
 *          - childThree: .
 */
void codeGenJ(node_t* jNode) {
    printf("-> codeGenJ(start)\n");

    // Logic Here ...

    printf("<- codeGenJ(end)\n");
}

/*
 *   X -> F?$ | .
 *          - INFO ...
 *          - childOne: F  |  .
 *          - childTwo: ?$
 */
void codeGenX(node_t* xNode) {
    printf("-> codeGenX(start)\n");

    // Logic Here ...


    printf("<- codeGenX(end)\n");
}

/*
 *   Y -> H?Y | empty
 *          - INFO ...
 *          - childOne = H  |  empty
 *          - childTwo = ?
 *          - childThree = Y
 */
void codeGenY(node_t* yNode) {
    printf("-> codeGenY(start)\n");

    if (strcmp(yNode->childOne->tokenInstance, "Empty") == 0) {
        return;
    } else {
        codeGenH(yNode->childOne);
        codeGenY(yNode->childThree);
    }

    printf("<- codeGenY(end)\n");
}





