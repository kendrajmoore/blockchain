//
// Created by Kendra Moore on 9/5/23.
//
#include "blockchain.h"


char *my_strcpy(char *dest, const char *src)
{
    char *old_dest = dest;
    while(*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return old_dest;
}

char *my_itoa(int value, char *input, int base)
{
    if(base != 10)
    {
        return NULL;
    }
    char *start = input;
    bool neg = false;
    if(value == 0)
    {
        *input = '0';
        *input++;
        *input = '\0';
        return input;
    }
    if(value < 0)
    {
        neg = true;
        value = -value;
    }
    while(value != 0)
    {
        int digit = value % 10;
        *input = '0' + digit;
        *input++;
        value /= 10;
    }
    if(neg)
    {
        *input = '-';
        *input++;
    }
    *input = '\0';
    char *end = input -1;
    while(start < end)
    {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    return input;
}

int my_atoi(const char *input)
{
    int result = 0;
    int sign = 1;
    while(*input == ' ')
    {
        input++;
    }
    if(*input == '-')
    {
        sign = -1;
        input++;
    } else if(*input == '+')
    {
        input++;
    }
    while(*input >= '0' && *input <= '9')
    {
        result = result * 10 + (*input - '0');
        input++;
    }
    return sign * result;
}

int my_strlen(const char* input)
{
    int length = 0;
    while(*input != '\0')
    {
        length++;
        input++;
    }
    return length;
}

int my_strcmp (char * param_1, char * param_2)
{
    int sum_1 = 0, sum_2 = 0;
    int i_1 = 0, i_2 = 0;

    while(i_1 < my_strlen(param_1))
    {
        sum_1 += param_1[i_1];
        i_1++;
    }

    while(i_2 < my_strlen(param_2))
    {
        sum_2 += param_2[i_2];
        i_2++;
    }

    if (sum_1 > sum_2)
        return -1;
    else if (sum_1 < sum_2)
        return 1;
    else
        return 0;
}

char *my_strdup(const char *param_1)
{
    int len = my_strlen(param_1) + 1;
    char *result = malloc(len);
    if(!result)
    {
        return NULL;
    }
    my_strcpy(result, param_1);
    return result;
}

char *my_strncat(char *dest, const char *src, size_t count)
{
    char *original_dest = dest;
    while(*dest)
    {
        dest++;
    }
    while(count-- && *src)
    {
        *dest++ = *src++;
    }
    *dest = '\0';
    return original_dest;
}

char *my_strcat(char *dest, const char *src)
{
    char *original_dest = dest;
    while(*dest)
    {
        dest++;
    }
    while(*src)
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return original_dest;
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
        return NULL;
    }
    new_block->bid = bid;
    new_block->next = NULL;
    return new_block;
}

void insert_block_to_node(char *bid, int nid, Node *head)
{
    Block *added_block = create_block(bid);
    if(!added_block)
    {
        printf("Memory allocation for new block failed.\n");
        return;
    }
    Node *curr = head;
    while(curr && curr->nid != nid)
    {
        curr = curr->next;
    }
    if(curr == NULL)
    {
        printf("No node with that nid %d\n", nid);
        return;
    }
    added_block->bid = my_strdup(bid);
    added_block->next = NULL;
    if(curr->chain == NULL)
    {
        curr->chain = added_block;
        return;
    }
    Block *curr_block = curr->chain;
    while(curr_block->next != NULL)
    {
        curr_block = curr_block->next;
    }

    curr_block->next = added_block;
}

Node *search_node(Node *head, int nid)
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

void free_chain(Block *start)
{
    Block *curr = start;
    while(curr)
    {
        Block *next_block = curr->next;
        free(curr->bid);
        free(curr);
        curr = next_block;
    }
}

int remove_node(int nid, Node **head)
{
    if(!head)
    {
        return 1;
    }
    Node *curr = *head;
    Node *prev = NULL;
    while(curr)
    {
        if(curr->nid == nid)
        {
            if(!prev)
            {
                *head = curr->next;
            } else
            {
                prev->next = curr->next;
            }
            free_chain(curr->chain);
            free(curr);
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }
    return 1;
}

int remove_block(char *bid, Node *node)
{
    if(!node || !bid)
    {
        return 1;
    }
    Block *curr_block = node->chain;
    Block *prev = NULL;
    while(curr_block != NULL)
    {
        if(my_strcmp(curr_block->bid, bid) == 0)
        {
            if(!prev)
            {
                node->chain = curr_block->next;
            } else
            {
                prev->next = curr_block->next;
            }
            free(curr_block->bid);
            free(curr_block);
            return 0;
        }
        prev = curr_block;
        curr_block = curr_block;

    }
    return 1;

}

int count_nodes(Node *head)
{
    int count = 0;
    Node *curr = head;
    while(curr)
    {
        count++;
        curr = curr->next;
    }
    return count;
}

int count_blocks(Block *start)
{
    int count = 0;
    Block *curr_block = start;
    while(curr_block)
    {
        count++;
        curr_block = curr_block->next;
    }
    return count;
}

Block *find_block(Block *start, char *bid)
{
    Block *curr_block = start;
    while(curr_block)
    {
        if(my_strcmp(curr_block->bid, bid) == 0)
        {
            return curr_block;
        }
        curr_block = curr_block->next;
    }
    return NULL;
}

int save_file(Node *head)
{
    int fd = open(BLOCKCKAIN_SAVE_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd == -1)
    {
        return 1;
    }
    char buffer[512];
    char num_buffer[50];
    Node *curr = head;
    while(curr)
    {
        my_strcpy(buffer, "Node: ");
        my_itoa(curr->nid, num_buffer, 10);
        my_strcat(buffer, num_buffer);
        my_strcat(buffer, "\n");
        write(fd, buffer, my_strlen(buffer));
        Block *curr_block = curr->chain;
        while(curr_block)
        {
            my_strcpy(buffer, "Block: ");
            my_strncat(buffer, curr_block->bid, sizeof(buffer) - my_strlen(buffer) -1);
            my_strcat(buffer, "\n");
            write(fd, buffer, my_strlen(buffer));
            curr_block = curr_block->next;
        }
        curr = curr->next;
    }
    close(fd);
    return 0;
}

int compare_chains(Node *chain1, Node *chain2)
{
    while(chain1 != NULL && chain2 != NULL)
    {
        if(chain1->nid != chain2->nid)
        {
            return 0;
        }
        Block *block1 = chain1->chain;
        Block *block2 = chain2->chain;
        while(block1 != NULL && block2 != NULL)
        {
            if(my_strcmp(block1->bid, block2->bid) != 0)
            {
                return 0;
            }
            block1 = block1->next;
            block2 = block2->next;
        }
        if(block1 != NULL || block2 != NULL)
        {
            return 0;
        }
        chain1 = chain1->next;
        chain2 = chain2->next;
    }
    if(chain1 != NULL || chain2 != NULL)
    {
        return 0;
    }
    return 1;
}

int read_line(int fd , char *buffer, size_t size)
{
    size_t i = 0;
    while(i < size -1)
    {
        char c;
        int result = read(fd, &c, 1);
        if(result == -1)
        {
            return -1;
        } else if(result == 0|| c == '\n')
        {
            buffer[i] = '\0';
            return i;
        } else
        {
            buffer[i] = c;
            i++;
        }
    }
    buffer[size -1] = '\0';
    return i;
}


Node *load_file()
{
    int fd = open(BLOCKCKAIN_SAVE_FILE, O_RDONLY);
    if(fd == -1)
    {
        return NULL;
    }
    Node *head = NULL;
    Node **curr_ptr = &head;
    char line[1024];
    while(read_line(fd, line, sizeof(line)))
    {
        if(my_strcmp(line, "---") == 0)
        {
            curr_ptr = &((*curr_ptr)->next);
            continue;
        }
        if(!head || !(*curr_ptr))
        {
            Node *added_node = (Node*)malloc(sizeof(Node));
            added_node->nid = my_atoi(line);
            added_node->chain = NULL;
            added_node->next = NULL;
            *curr_ptr = added_node;
        } else
        {
            Block *added_block = (Block*)malloc(sizeof(Block));
            added_block->bid = my_strdup(line);
            added_block->next = NULL;
            Block **curr_block_ptr = &((*curr_ptr)->chain);
            while(*curr_block_ptr)
            {
                curr_block_ptr = &((*curr_block_ptr)->next);
            }
            *curr_block_ptr = added_block;
        }
    }
    close(fd);
    return head;
}





int main(){
    return 0;
}