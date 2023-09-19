//
// Created by Kendra Moore on 9/5/23.
//

#include <stdlib.h>
#include "blockchain.h"


int my_strncmp(const char *param1, const char *param2, int size){
    int i = 0;
    while(*param1 && (*param1 == *param2) && i < size){
        param1++;
        param2++;
        i++;
    }
    if(i == size){
        return 0;
    }
    return *(unsigned char*)param1 - *(unsigned char*)param2;
}

Node *create_node(int nid)
{

    Node *new_node = (Node*) malloc(sizeof(Node));
    if(!new_node)
    {
        printf("Cannot make a new node");
    }
    new_node->nid = nid;
    new_node->chain = NULL;
    new_node->next = NULL;
    return new_node;
}

void insert_node(int nid, Node **head)
{
   Node *added_node = create_node(nid);
   if(*head == NULL)
   {
       *head = added_node;
       return;
   }
   Node *curr = *head;
   while(curr->next)
   {
       curr = curr->next;
   }
   curr->next = added_node;
}

int print_node(Node *head)
{
    Node *curr = head;
    while(curr)
    {
        printf("%d", curr->nid);
        curr = curr->next;
    }
    return 0;
}

void free_node(Node *head)
{
    Node *temp;
    while(head)
    {
        temp = head;
        head = head ->next;
        free(temp);
    }
}

Block *create_block(char *bid)
{

    Block *new_block = (Block*) malloc(sizeof(Block));
    if(!new_block)
    {
        printf("Cannot make a new node");
    }
    new_block->bid = bid;
    new_block->next = NULL;
    return new_block;
}

void insert_block(char *bid, int nid, Node *head)
{
    Block *added_block = create_block(bid);
    Node *curr = head;


}

Node *search(Node *head, int nid)
{
    Node *curr = head;
    while(curr)
    {
        if(curr->nid == nid)
        {
            return curr;
        }
        curr->next = curr;
    }
    return NULL;
}


int main(){
    return 0;
}