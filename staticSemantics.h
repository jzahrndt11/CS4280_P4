/*
 *  CS4280 - Project P2
 *      - by. Jeremy Zahrndt
 *      - staticSemantics.h
 */
#ifndef STATICSEMANTICS_H
#define STATICSEMANTICS_H

#include "node.h"

void staticSem(node_t*);
void traversePreOrder(node_t*, int);
void insert(node_t*);
bool verify(node_t*);


#endif
