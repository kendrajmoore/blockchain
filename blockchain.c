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



int main(){
    return 0;
}