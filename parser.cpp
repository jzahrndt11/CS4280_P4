/*
 *  CS4280 - Project P4
 *      - by. Jeremy Zahrndt
 *      - parser.cpp
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "string.h"
#include "parser.h"
#include "scanner.h"
#include "buildTree.h"

const char* tokenNames[] = {
        "EOF Token",
        "T1 Token",
        "T2 Token",
        "T3 Token",
        "Unknown"
};

// Initialization of Global Variables
char nextChar = 0;
Token tokenInfo;
const char* filteredFile = "filter.txt";

// Parser Function -------------------------------------------------------------------------------------------------------------------------------
node_t* parser() {
    node_t* root = NULL;
    memset(tokenInfo.tokenInstance, '\0', MAX_TOKEN_SIZE);

    // filter out comments
    filter();

    // open filter.txt after comments have been removed
    filePointer = fopen(filteredFile, "r");
    if (filePointer == nullptr) {
        perror("Fatal: Error Opening File!\n");
        exit(EXIT_FAILURE);
    }

    // get first char
    nextChar = fgetc(filePointer);

    // run scanner
    tokenInfo = scanner();

    // start production S
    root = S();

    // Check for EOFtk
    if (tokenInfo.tokenId != EOF_Token) {
        printf("parser.cpp: Error in parser()\n");
        exit(EXIT_FAILURE);
    }

    return root;
}

// function to remove comments -----------------------------------------------------------------------------------------------------------------
void filter() {
    bool comment = false;
    FILE* filteredFilePointer;

    filteredFilePointer = fopen(filteredFile, "w");

    if (filteredFilePointer == nullptr) {
        perror("Error opening file { filteredFile }");
        exit(EXIT_FAILURE);
    }

    int c;
    while ((c = fgetc(filePointer)) != EOF) {
        // Skip Comments
        while (comment) {
            c = fgetc(filePointer);

            // keep new line
            if (c == 10) {
                fputc(c, filteredFilePointer);
            }

            // end of comment
            if (c == 35) {
                comment = false;
                c = fgetc(filePointer);
            }
        }

        // Check for start of comment
        if (c == 35) {
            comment = true;
            continue;
        }

        while (isspace(c)) {
            // keep new line
            if (c == 10) {
                fputc(c,filteredFilePointer);
                c = fgetc(filePointer);
            } else {
                c = fgetc(filePointer);
            }
        }

        // if not a comment or a whitespace add to new file
        fputc(c, filteredFilePointer);
    }
    fclose(filePointer);
    fclose(filteredFilePointer);
}

// S -> CD          ( First set: t2 ) ---------------------------------------------------------------------------------------------------------------------------
node_t* S() {
    //printf("S()\n");
    node_t* p = create('S');

    p->childOne = C();    // run non-terminal C
    p->childTwo = D();    // run non-terminal D

    //printf("End of S()\n");
    return p;
}

// A -> FX          ( First set: t1 t2 )
node_t* A() {
    //printf("A()\n");
    node_t* p = create('A');

    p->childOne = F();    // run non-terminal F
    p-> childTwo = X();    // run non-terminal X

    //printf("End of A()\n");
    return p;
}

// B -> .t2A!       ( First set: . ) ---------------------------------------------------------------------------------------------------------------------------
node_t* B() {
    //printf("B()\n");
    if (tokenInfo.tokenId == T3_Token && tokenInfo.tokenInstance[0] == '.') {
        node_t* p = create('B');

        //printf("Process { . } in B()\n");   // process .
            node_t* bTokenPtr = create('Z');
            bTokenPtr->tokenId = tokenInfo.tokenId;
            bTokenPtr->lineNum = tokenInfo.lineNum;
            sprintf(bTokenPtr->tokenInstance, "%s", tokenInfo.tokenInstance);
            p->childTwo = bTokenPtr;
        tokenInfo = scanner();  //consume .

        if (tokenInfo.tokenId == T2_Token) {
            //printf("Process t2 token { %s } in B()\n", tokenInfo.tokenInstance);  // process t2
                node_t* bTokenPtr2 = create('Z');
                bTokenPtr2->tokenId = tokenInfo.tokenId;
                bTokenPtr2->lineNum = tokenInfo.lineNum;
                sprintf(bTokenPtr2->tokenInstance, "%s", tokenInfo.tokenInstance);
                p->childTwo = bTokenPtr2;
            tokenInfo = scanner();  //consume t2

            p->childThree = A();    // run non-terminal A

            if (tokenInfo.tokenId == T3_Token && tokenInfo.tokenInstance[0] == '!') {
                //printf("Process { ! } in B()\n");   // process !
                    node_t* bTokenPtr3 = create('Z');
                    bTokenPtr3->tokenId = tokenInfo.tokenId;
                    bTokenPtr3->lineNum = tokenInfo.lineNum;
                    sprintf(bTokenPtr3->tokenInstance, "%s", tokenInfo.tokenInstance);
                    p->childFour = bTokenPtr3;
                tokenInfo = scanner();  // consume !

                //printf("End of B()\n");
                return p;
            }
            else {
                printf("ERROR parser.cpp-B(): { ! } required. You gave { %s } - Line %d\n", tokenInfo.tokenInstance, tokenInfo.lineNum);
                exit(EXIT_FAILURE);
            }
        }
        else {
            printf("ERROR parser.cpp-B(): { t2 } required. You gave { %s } - Line %d\n", tokenNames[tokenInfo.tokenId], tokenInfo.lineNum);
            exit(EXIT_FAILURE);
        }
    }
    else {
        printf("ERROR parser.cpp-B(): { . } required. You gave { %s } - Line %d\n", tokenInfo.tokenInstance, tokenInfo.lineNum);
        exit(EXIT_FAILURE);
    }
}

// C -> t2*         ( First set: t2 ) ------------------------------------------------------------------------------------------------------------------
node_t* C() {
    //printf("C()\n");
    if (tokenInfo.tokenId == T2_Token) {
        node_t* p = create('C');

        //printf("Process t2 token { %s } in C()\n", tokenInfo.tokenInstance);  // process t2
            node_t* cTokenPtr = create('Z');
            cTokenPtr->tokenId = tokenInfo.tokenId;
            cTokenPtr->lineNum = tokenInfo.lineNum;
            sprintf(cTokenPtr->tokenInstance, "%s", tokenInfo.tokenInstance);
            p->childOne = cTokenPtr;
        tokenInfo = scanner();  // consume t2

        if (tokenInfo.tokenId == T3_Token && tokenInfo.tokenInstance[0] == '*') {
            //printf("Process { * } in C()\n");  // process *
                node_t* cTokenPtr2 = create('Z');
                cTokenPtr2->tokenId = tokenInfo.tokenId;
                cTokenPtr2->lineNum = tokenInfo.lineNum;
                sprintf(cTokenPtr2->tokenInstance, "%s", tokenInfo.tokenInstance);
                p->childTwo = cTokenPtr2;
            tokenInfo = scanner();  // consume *

            //printf("End of C()\n");
            return p;
        }
        else {
            printf("ERROR parser.cpp-C(): { * } required. You gave { %s } - Line %d\n", tokenInfo.tokenInstance, tokenInfo.lineNum);
            exit(EXIT_FAILURE);
        }
    } else {
        printf("ERROR parser.cpp-C(): { t2 } required. You gave { %s } - Line %d\n", tokenNames[tokenInfo.tokenId], tokenInfo.lineNum);
        exit(EXIT_FAILURE);
    }
}

// D -> Y           ( First set: , ,; . t2 *" ? empty )
node_t* D() {
    //printf("D()\n");
    node_t* p = create('D');
    p->childOne = Y();    // run non-terminal Y

    //printf("End of D()\n");
    return p;
}

// E -> ,AAH | ,;FH             ( First set: , | ,; ) -----------------------------------------------------------------------------------------------
node_t* E() {
    //printf("E()\n");
    if (tokenInfo.tokenId == T3_Token && tokenInfo.tokenInstance[0] == ',' && tokenInfo.tokenInstance[1] != ';'){
        node_t* p = create('E');

        //printf("Process { , } in E()\n");  // process ,
            node_t* eTokenPtr = create('Z');
            eTokenPtr->tokenId = tokenInfo.tokenId;
            eTokenPtr->lineNum = tokenInfo.lineNum;
            sprintf(eTokenPtr->tokenInstance, "%s", tokenInfo.tokenInstance);
            p->childOne = eTokenPtr;
        tokenInfo = scanner();  // consume ,

        p->childTwo = A();    // run non-terminal A
        p->childThree = A();    // run non-terminal A
        p->childFour = H();    // run non-terminal H

        //printf("End of E()\n");
        return p;
    }
    else if (tokenInfo.tokenId == T3_Token && tokenInfo.tokenInstance[0] == ',' && tokenInfo.tokenInstance[1] == ';') {
        node_t* p = create('E');

        //printf("Process { ,; } in E()\n");  // process ,;
            node_t* eTokenPtr = create('Z');
            eTokenPtr->tokenId = tokenInfo.tokenId;
            eTokenPtr->lineNum = tokenInfo.lineNum;
            sprintf(eTokenPtr->tokenInstance, "%s", tokenInfo.tokenInstance);
            p->childOne = eTokenPtr;
        tokenInfo = scanner();  // consume ,;

        p->childTwo = F();    // run non-terminal F
        p->childThree = H();    // run non-terminal H

        //printf("End of E()\n");
        return p;
    }
    else {
        printf("ERROR parser.cpp-E(): { , | ,; } required. You gave { %s } - Line %d\n", tokenInfo.tokenInstance, tokenInfo.lineNum);
        exit(EXIT_FAILURE);
    }
}

// F -> t1 | t2         ( First set: t1 | t2 ) --------------------------------------------------------------------------------------------------------
node_t* F() {
    //printf("F()\n");
    if (tokenInfo.tokenId == T1_Token) {
        node_t* p = create('F');

        //printf("Process t1 token { %s } in F()\n", tokenInfo.tokenInstance);  // process t1
            node_t* fTokenPtr = create('Z');
            fTokenPtr->tokenId = tokenInfo.tokenId;
            fTokenPtr->lineNum = tokenInfo.lineNum;
            sprintf(fTokenPtr->tokenInstance, "%s", tokenInfo.tokenInstance);
            p->childOne = fTokenPtr;
        tokenInfo = scanner();  // consume t1

        //printf("End of F()\n");
        return p;
    }
    else if (tokenInfo.tokenId == T2_Token) {
        node_t* p = create('F');

        //printf("Process t2 token { %s } in F()\n", tokenInfo.tokenInstance);  // process t2
            node_t* fTokenPtr = create('Z');
            fTokenPtr->tokenId = tokenInfo.tokenId;
            fTokenPtr->lineNum = tokenInfo.lineNum;
            sprintf(fTokenPtr->tokenInstance, "%s", tokenInfo.tokenInstance);
            p->childOne = fTokenPtr;
        tokenInfo = scanner();  // consume t2

        //printf("End of F()\n");
        return p;
    }
    else {
        printf("ERROR parser.cpp-F(): { t1 | t2 } required. You gave { %s } - Line %d\n", tokenNames[tokenInfo.tokenId], tokenInfo.lineNum);
        exit(EXIT_FAILURE);
    }
}

// G -> B | C | J           ( First set: . | t2 | *" ) --------------------------------------------------------------------------------------------
node_t* G() {
    //printf("G()\n");
    if (tokenInfo.tokenId == T2_Token) {
        node_t* p = create('G');

        p->childOne = C();    // run non-terminal C

        //printf("End of G()\n");
        return p;
    }
    else if (tokenInfo.tokenId == T3_Token && tokenInfo.tokenInstance[0] == '.') {
        node_t* p = create('G');

        p->childOne = B();    // run non-terminal B

        //printf("End of G()\n");
        return p;
    }
    else if (tokenInfo.tokenId == T3_Token && tokenInfo.tokenInstance[0] == '*' && tokenInfo.tokenInstance[1] == '"') {
        node_t* p = create('G');

        p->childOne = J();    // run non-terminal J

        //printf("End of G()\n");
        return p;
    }
    else {
        printf("ERROR parser.cpp-F(): { t2 | . | *\" } required. You gave { %s } - Line %d\n", tokenInfo.tokenInstance, tokenInfo.lineNum);
        exit(EXIT_FAILURE);
    }
}

// H -> E? | G. | empty         ( First set: , ,; | . t2 *" | empty ) -------------------------------------------------------------------------------
node_t* H(){
    //printf("H()\n");
    if (tokenInfo.tokenId == T3_Token && tokenInfo.tokenInstance[0] == ',' ) {
        node_t* p = create('H');

        p->childOne = E();    // run non-terminal E

        if (tokenInfo.tokenId == T3_Token && tokenInfo.tokenInstance[0] == '?') {
            //printf("Process { ? } in H()\n");   // process ?
                node_t* hTokenPtr = create('Z');
                hTokenPtr->tokenId = tokenInfo.tokenId;
                hTokenPtr->lineNum = tokenInfo.lineNum;
                sprintf(hTokenPtr->tokenInstance, "%s", tokenInfo.tokenInstance);
                p->childTwo = hTokenPtr;
            tokenInfo = scanner();  //consume ?

            //printf("End of H()\n");
            return p;
        }
        else {
            printf("ERROR parser.cpp-H(): { ? } required. You gave { %s } - Line %d\n", tokenInfo.tokenInstance, tokenInfo.lineNum);
            exit(EXIT_FAILURE);
        }
    }
    else if (tokenInfo.tokenId == T2_Token || (tokenInfo.tokenId == T3_Token && (tokenInfo.tokenInstance[0] == '.'
        || (tokenInfo.tokenInstance[0] == '*' && tokenInfo.tokenInstance[1] == '"')))) {

        node_t* p = create('H');

        p->childOne = G();    // run non-terminal G

            if (tokenInfo.tokenId == T3_Token && tokenInfo.tokenInstance[0] == '.') {
                //printf("Process { . } in H()\n");   // process .
                    node_t* hTokenPtr = create('Z');
                    hTokenPtr->tokenId = tokenInfo.tokenId;
                    hTokenPtr->lineNum = tokenInfo.lineNum;
                    sprintf(hTokenPtr->tokenInstance, "%s", tokenInfo.tokenInstance);
                    p->childTwo = hTokenPtr;
                tokenInfo = scanner();  //consume .

                //printf("End of H()\n");
                return p;
            } else {
                printf("ERROR parser.cpp-H(): { . } required. You gave { %s } - Line %d\n", tokenInfo.tokenInstance, tokenInfo.lineNum);
                exit(EXIT_FAILURE);
            }
    }
    else {
        node_t* p = create('H');
        //printf("{empty} End of H()\n");
            node_t* hTokenPtr = create('Z');
            sprintf(hTokenPtr->tokenInstance, "%s", "Empty");
            p->childOne = hTokenPtr;
        return p;
    }
}

// J -> *"A.        ( First set: *" ) ----------------------------------------------------------------------------------------------------------
node_t* J() {
    //printf("J()\n");
    if (tokenInfo.tokenId == T3_Token && tokenInfo.tokenInstance[0] == '*' && tokenInfo.tokenInstance[1] == '"') {
        node_t* p = create('J');

        //printf("Process { *\" } in J()\n");   // process *"
            node_t* jTokenPtr = create('Z');
            jTokenPtr->tokenId = tokenInfo.tokenId;
            jTokenPtr->lineNum = tokenInfo.lineNum;
            sprintf(jTokenPtr->tokenInstance, "%s", tokenInfo.tokenInstance);
            p->childOne = jTokenPtr;
        tokenInfo = scanner();  //consume *"

        p->childTwo = A();    // run non-terminal A

        if (tokenInfo.tokenId == T3_Token && tokenInfo.tokenInstance[0] == '.') {
            //printf("Process { . } in J()\n");   // process .
                node_t* jTokenPtr2 = create('Z');
                jTokenPtr2->tokenId = tokenInfo.tokenId;
                jTokenPtr2->lineNum = tokenInfo.lineNum;
                sprintf(jTokenPtr2->tokenInstance, "%s", tokenInfo.tokenInstance);
                p->childThree = jTokenPtr2;
            tokenInfo = scanner();  //consume .

            //printf("End of J()\n");
            return p;
        }
        else {
            printf("ERROR parser.cpp-J(): { . } required. You gave { %s } - Line %d\n", tokenInfo.tokenInstance, tokenInfo.lineNum);
            exit(EXIT_FAILURE);
        }
    }
    else {
        printf("ERROR parser.cpp-J(): { *\" } required. You gave { %s } - Line %d\n", tokenInfo.tokenInstance, tokenInfo.lineNum);
        exit(EXIT_FAILURE);
    }
}

// X -> F?$ | .         ( First set: t1 t2 | . ) ----------------------------------------------------------------------------------------------
node_t* X() {
    //printf("X()\n");
    if (tokenInfo.tokenId == T1_Token || tokenInfo.tokenId == T2_Token) {
        node_t* p = create('X');

        p->childOne = F();    // run non-terminal F

        if (tokenInfo.tokenId == T3_Token && tokenInfo.tokenInstance[0] == '?' && tokenInfo.tokenInstance[1] == '$') {
            //printf("Process { ?$ } in X()\n");   // process ?$
                node_t* xTokenPtr = create('Z');
                xTokenPtr->tokenId = tokenInfo.tokenId;
                xTokenPtr->lineNum = tokenInfo.lineNum;
                sprintf(xTokenPtr->tokenInstance, "%s", tokenInfo.tokenInstance);
                p->childTwo = xTokenPtr;
            tokenInfo = scanner();  //consume ?$

            //printf("End of X()\n");
            return p;
        }
        else {
            printf("ERROR parser.cpp-X(): { ?$ } required. You gave { %s } - Line %d\n", tokenInfo.tokenInstance, tokenInfo.lineNum);
            exit(EXIT_FAILURE);
        }
    }
    else if (tokenInfo.tokenId == T3_Token && tokenInfo.tokenInstance[0] == '.') {
        node_t* p = create('X');
        //printf("Process { . } in X()\n");   // process .
            node_t* xTokenPtr2 = create('Z');
            xTokenPtr2->tokenId = tokenInfo.tokenId;
            xTokenPtr2->lineNum = tokenInfo.lineNum;
            sprintf(xTokenPtr2->tokenInstance, "%s", tokenInfo.tokenInstance);
            p->childOne = xTokenPtr2;
        tokenInfo = scanner();  //consume .

        //printf("End of X()\n");
        return p;
    }
    else {
        printf("ERROR parser.cpp-X(): { t1 | t2 | . } required. You gave { %s } - Line %d\n", tokenInfo.tokenInstance, tokenInfo.lineNum);
        exit(EXIT_FAILURE);
    }
}

// Y -> H?Y | empty         ( First set: , ,; . t2 *" ? empty | empty ) ------------------------------------------------------------------------
node_t* Y() {
    //printf("Y()\n");
    if (tokenInfo.tokenId == T2_Token || (tokenInfo.tokenId == T3_Token && (tokenInfo.tokenInstance[0] == ',' || tokenInfo.tokenInstance[0] == '.'
        || tokenInfo.tokenInstance[0] == '?' || (tokenInfo.tokenInstance[0] == '*' && tokenInfo.tokenInstance[1] == '"')))){

            node_t* p = create('Y');

            p->childOne = H();    // run non-terminal H

            if (tokenInfo.tokenId == T3_Token && tokenInfo.tokenInstance[0] == '?') {
                //printf("Process { ? } in H()\n");   // process ?
                    node_t* yTokenPtr = create('Z');
                    yTokenPtr->tokenId = tokenInfo.tokenId;
                    yTokenPtr->lineNum = tokenInfo.lineNum;
                    sprintf(yTokenPtr->tokenInstance, "%s", tokenInfo.tokenInstance);
                    p->childTwo = yTokenPtr;
                tokenInfo = scanner();  //consume ?

                p->childThree = Y();    // run non-terminal Y (right recursion)

                //printf("End of Y()\n");
                return p;
            }
            else {
                printf("ERROR parser.cpp-Y(): { ? } required. You gave { %s } - Line %d\n", tokenInfo.tokenInstance, tokenInfo.lineNum);
                exit(EXIT_FAILURE);
            }
    }
    else {
        node_t* p = create('Y');
        //printf("{empty} End of Y()\n");
            node_t* yTokenPtr = create('Z');
            sprintf(yTokenPtr->tokenInstance, "%s", "Empty");
            p->childOne = yTokenPtr;
        return p;
    }
}