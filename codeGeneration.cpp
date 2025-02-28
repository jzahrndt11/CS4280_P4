/*
 *  CS4280 - Project P4
 *      - by. Jeremy Zahrndt
 *      - codeGeneration.cpp
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codeGeneration.h"
#include "parser.h"

int tempVarCount = 0;
char tempVarTable[100][MAX_TOKEN_SIZE2];

int outLabelCount = 1;
int repeatLabelCount = 1;

// Code Generation function  -------------------------------------------------------------------------------------------
void codeGeneration(node_t* tree) {
    char fileBuf[100];
    sprintf(fileBuf,"%s.asm", file);
    filePointer = fopen(fileBuf, "w");

    if (tree->label == 'S') {
        codeGenS(tree);
    } else {
        printf("codeGeneration.cpp: ERROR running codeGenS() - label was not S");
    }

    // Add STOP
    fprintf(filePointer, "STOP\n");

    // Add Symbol Table Variables initialized to 0
    for (int i = 0; i < 100; i++) {
        if (symbolTable[i][0] != '\0') {
            fprintf(filePointer, "%s 0\n", symbolTable[i]);
        }
    }

    // Add Temp Variables initialized to 0
    for (int i = 0; i < 100; i++) {
        if (tempVarTable[i][0] != '\0') {
            fprintf(filePointer, "%s 0\n", tempVarTable[i]);
        }
    }

    printf("\n-------> COMPILATION COMPLETE <-------\n\n  Assembly Language File: %s\n\n-------------> CREATED <--------------\n\n", fileBuf);
}

/*  --------------------------------------------------------------------------------------------------------------------
 *   S -> CD
 *          - read an int and allocate memory, any number of additional operations
 *          - childOne: C
 *          - childTwo: D
 *  ------------------------------------------------------------------------------------------------------------------*/
void codeGenS(node_t* sNode) {
    codeGenC(sNode->childOne);
    codeGenD(sNode->childTwo);
}

/*  --------------------------------------------------------------------------------------------------------------------
 *   A -> FX
 *          - sum | int or identifier
 *          - childOne: F
 *          - childTwo: X
 *  ------------------------------------------------------------------------------------------------------------------*/
char* codeGenA(node_t* aNode) {
    char* num = codeGenF(aNode->childOne);
    char* returnNum = codeGenX(aNode->childTwo, num);

    return returnNum;
}


/*  --------------------------------------------------------------------------------------------------------------------
 *   B -> .t2A!
 *          - assign value of A to identifier t2
 *          - childOne: .
 *          - childTwo: t2 Token
 *          - childThree: A
 *          - childFour: !
 *  ------------------------------------------------------------------------------------------------------------------*/
void codeGenB(node_t* bNode) {
    char* aVar = codeGenA(bNode->childThree);
    fprintf(filePointer, "LOAD %s\nSTORE %s\n", aVar , bNode->childTwo->tokenInstance);
}

/*  --------------------------------------------------------------------------------------------------------------------
 *   C -> t2*
 *          - read in int, allocate memory (ex: v10 for %10), assign value = int
 *          - childOne: t2 token
 *          - childTwo: *
 *  ------------------------------------------------------------------------------------------------------------------*/
void codeGenC(node_t* cNode) {
    fprintf(filePointer, "READ %s\n", cNode->childOne->tokenInstance);
}

/*  --------------------------------------------------------------------------------------------------------------------
 *   D -> Y
 *          - originally: D -> H?D | empty for recursion
 *          - childOne: Y
 *  ------------------------------------------------------------------------------------------------------------------*/
void codeGenD(node_t* dNode) {
    codeGenY(dNode->childOne);
}

/*  --------------------------------------------------------------------------------------------------------------------
 *   E -> ,AAH | ,;FH
 *          - if first A > second A, do H   |    do H, F times
 *          - childOne:  ,  |  ,;
 *          - childTwo: A  |  F
 *          - childThree: A  |  H
 *          - childFour: H
 *  ------------------------------------------------------------------------------------------------------------------*/
void codeGenE(node_t* eNode) {
    if (eNode->childTwo->label == 'A') {
        char* aNum1 = codeGenA(eNode->childTwo);
        char* aNum2 = codeGenA(eNode->childThree);

        // if first A > second A, do H
        fprintf(filePointer, "LOAD %s\nSUB %s\nBRZNEG OUT%d\n", aNum1, aNum2, outLabelCount);
        codeGenH(eNode->childFour);
        fprintf(filePointer, "OUT%d: NOOP\n", outLabelCount);

        outLabelCount++;
    } else if (eNode->childTwo->label == 'F') {
        char* fNum = codeGenF(eNode->childTwo);

        char* tempVarBuf = newTemp();
        fprintf(filePointer, "LOAD %s\nSTORE %s\n", fNum, tempVarBuf);

        // do H, F times
        fprintf(filePointer, "LOAD %s\nREPEAT%d: STORE %s\nBRZNEG OUT%d\n", tempVarBuf, repeatLabelCount, tempVarBuf, outLabelCount);
        codeGenH(eNode->childThree);
        fprintf(filePointer, "LOAD %s\nSUB 1\nBR REPEAT%d\nOUT%d: NOOP\n", tempVarBuf, repeatLabelCount, outLabelCount);

        repeatLabelCount++;
        outLabelCount++;
    } else
        printf("codeGeneration.cpp: ERROR in codeGenE()");
}

/*  --------------------------------------------------------------------------------------------------------------------
 *   F -> t1 | t2
 *          - number  |  identifier
 *          - childOne: t1 Token  |  t2 Token
 *  ------------------------------------------------------------------------------------------------------------------*/
char* codeGenF(node_t* fNode) {
    char* strBuf;
    if (fNode->childOne->tokenId == T2_Token) {
        return strdup(fNode->childOne->tokenInstance);
    } else if (fNode->childOne->tokenId == T1_Token) {
        if (fNode->childOne->tokenInstance[0] >= 'A' && fNode->childOne->tokenInstance[0] <= 'Z') {
            return strdup(fNode->childOne->tokenInstance + 1);
        } else {
            strBuf = (char*) malloc(strlen(fNode->childOne->tokenInstance));
            sprintf(strBuf, "-%s", fNode->childOne->tokenInstance + 1);
            return strBuf;
        }
    } else {
        printf("codeGeneration.cpp: ERROR in codeGenE()");
        exit(EXIT_FAILURE);
    }
}

/*  --------------------------------------------------------------------------------------------------------------------
 *   G -> B | C | J
 *          - assignment  |  read int and allocate memory  |  print value to screen
 *          - childOne: B  |  C  |  J
 *  ------------------------------------------------------------------------------------------------------------------*/
void codeGenG(node_t* gNode) {
    if (gNode->childOne->label == 'B') {
        codeGenB(gNode->childOne);
    } else if (gNode->childOne->label == 'C') {
        codeGenC(gNode->childOne);
    } else if (gNode->childOne->label == 'J') {
        codeGenJ(gNode->childOne);
    } else
        printf("codeGeneration.cpp: ERROR in codeGenG(gNode)");
}

/*  --------------------------------------------------------------------------------------------------------------------
 *   H -> E? | G. | empty
 *          - if, for   |  assignment, read int and allocate memory  |  print value
 *          - childOne: E  |  G  | empty
 *          - childTwo: ?  |  .
 *  ------------------------------------------------------------------------------------------------------------------*/
void codeGenH(node_t* hNode){
    if (strcmp(hNode->childOne->tokenInstance, "Empty") == 0) {
        return;
    } else if (hNode->childOne->label == 'E') {
        codeGenE(hNode->childOne);
    } else if (hNode->childOne->label == 'G') {
        codeGenG(hNode->childOne);
    } else
        printf("codeGeneration.cpp: ERROR in codeGenH(hNode)");
}

/*  --------------------------------------------------------------------------------------------------------------------
 *   J -> *"A.
 *          - print integer value to screen (sum, int, or identifier)
 *          - childOne: *"
 *          - childTwo: A
 *          - childThree: .
 *  ------------------------------------------------------------------------------------------------------------------*/
void codeGenJ(node_t* jNode) {
    char* aNum = codeGenA(jNode->childTwo);
    fprintf(filePointer, "WRITE %s\n", aNum);
}

/*  --------------------------------------------------------------------------------------------------------------------
 *   X -> F?$ | .
 *          - childOne: F  |  .
 *          - childTwo: ?$
 *  ------------------------------------------------------------------------------------------------------------------*/
char* codeGenX(node_t* xNode, char* fNum1) {
    if (xNode->childOne->label == 'F') {
        char* fNum2 = codeGenF(xNode->childOne);

        char* tempVarBuf = newTemp();
        fprintf(filePointer, "LOAD %s\nADD %s\nSTORE %s\n", fNum1, fNum2, tempVarBuf);

        return tempVarBuf;
    } else {
        char* tempVarBuf = newTemp();
        fprintf(filePointer, "LOAD %s\nSTORE %s\n", fNum1, tempVarBuf);

        return tempVarBuf;
    }
}

/*  --------------------------------------------------------------------------------------------------------------------
 *   Y -> H?Y | empty
 *          - childOne = H  |  empty
 *          - childTwo = ?
 *          - childThree = Y
 *  ------------------------------------------------------------------------------------------------------------------*/
void codeGenY(node_t* yNode) {
    if (strcmp(yNode->childOne->tokenInstance, "Empty") == 0) {
        return;
    } else {
        codeGenH(yNode->childOne);
        codeGenY(yNode->childThree);
    }
}

// Function to add temp variable (allocate memory and add to TempTable for initialization) -----------------------------
char* newTemp() {
    char* temp = (char*) malloc(sizeof(char) * 10);
    sprintf(temp, "T%d", tempVarCount++);
    for (int i = 0; i < 100; i++) {
        if (tempVarTable[i][0] == '\0') {
            strcpy(tempVarTable[i], temp);
            break;
        }
    }
    return temp;
}





