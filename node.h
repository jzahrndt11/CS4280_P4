/*
 *  CS4280 - Project P4
 *      - by. Jeremy Zahrndt
 *      - node.h
 */
#ifndef NODE_H
#define NODE_H

#include "token.h"
const int MAX_TOKEN_SIZE2 = 100;

typedef struct node_t {
    struct node_t *childOne;
    struct node_t *childTwo;
    struct node_t *childThree;
    struct node_t *childFour;
    char label;
    char tokenInstance[MAX_TOKEN_SIZE2];
    int tokenId;
    int lineNum;
} node_t;

#endif
