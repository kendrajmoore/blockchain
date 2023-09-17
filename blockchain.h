//
// Created by Kendra Moore on 9/17/23.
//
#include <stdio.h>

#ifndef BLOCKCHAIN_BLOCKCHAIN_H
#define BLOCKCHAIN_BLOCKCHAIN_H

#define BLOCKCKAIN_SAVE_FILE "block.txt"

typedef struct Block
{
    char *bid;
    struct Block *next;
} Block;

typedef struct Node
{
    int nid;
    Block *chain;
    struct Node *next;
} Node;



#endif //BLOCKCHAIN_BLOCKCHAIN_H
