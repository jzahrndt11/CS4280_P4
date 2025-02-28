/*
 *  CS4280 - Project P4
 *      - by. Jeremy Zahrndt
 *      - codeGeneration.h
 */
#ifndef CODEGENERATION_H
#define CODEGENERATION_H

#include "node.h"

void codeGeneration(node_t*);

void codeGenS(node_t*);
char* codeGenA(node_t*);
void codeGenB(node_t*);
void codeGenC(node_t*);
void codeGenD(node_t*);
void codeGenE(node_t*);
char* codeGenF(node_t*);
void codeGenG(node_t*);
void codeGenH(node_t*);
void codeGenJ(node_t*);
char* codeGenX(node_t*, char*);
void codeGenY(node_t*);
char* newTemp();

#endif
