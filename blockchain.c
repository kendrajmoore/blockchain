//
// Created by Kendra Moore on 9/5/23.
//
#include "blockchain.h"

int add_node(int nid, Node **head)
{
    Node *new_node = (Node*) malloc(sizeof(Node));
    if(!new_node)
    {
        printf("Cannot make a new node");
        return 1;
    }
    new_node->nid = nid;
    new_node->chain = NULL;
    new_node->next = NULL;
   if(*head == NULL)
   {
       *head = new_node;
   }
   Node *curr = *head;
   while(curr->next)
   {
       curr = curr->next;
   }
   curr->next = new_node;
   return 0;
}

int print_node(Node *head, bool print_blocks)
{
    Node *curr = head;
    while(curr)
    {
        printf("%d", curr->nid);
       if(print_blocks)
       {
           Block *curr_block = curr->chain;
           printf(": ");
           while(curr_block)
           {
               printf("%s ", curr_block->bid);
               curr_block = curr_block->next;
           }
       }
       printf("\n");
       curr = curr->next;
    }
    return 0;
}

int free_node(Node *head)
{
    Node *temp;
    if(!head)
    {
        return 4;
    }
    while(head)
    {
        temp = head;
        head = head ->next;
        free(temp);
    }
    return 0;
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

int remove_node(int nid, Node **head)
{
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
            Block *curr_block = curr->chain;
            while(curr_block)
            {
                Block *temp = curr_block;
                curr_block = curr_block->next;
                free(temp->bid);
                free(temp);
            }
            free(curr);
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("nok: node %d doesn't exist\n", nid);
    return 4;
}


int add_block(char *bid, int nid, Node *head)
{
    Node *curr = head;
    while(curr)
    {
        if(nid == curr->nid || nid == -1)
        {
            Block *new_block = (Block*) malloc(sizeof(Block));
            if(!new_block)
            {
                printf("nok: no more resources available on the computer");
                return 1;
            }
            new_block->bid = my_strdup(bid);
            new_block->next = NULL;
            Block *curr_block = curr->chain;
            if(curr_block == NULL)
            {
                curr->chain = new_block;
            } else
            {
                while(curr_block->next != NULL)
                {
                    curr_block = curr_block->next;
                }
                curr_block->next = new_block;
            }
            return 0;
        }
        curr = curr->next;
    }
    printf("nok: node %d doesn't exist\n", nid);
    return 2;
}

int remove_block(char *bid, Node *node)
{
    if(!node)
    {
        printf("This node doesn't exist");
        return 4;
    }
    if(!bid)
    {
        printf("This block %s doesn't exist", bid);
        return 5;
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


Block *find_block(Node *start, char *bid)
{
    Block *curr_block = start->chain;
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


int save_file(Node *head)
{
    int fd = open(BLOCKCKAIN_SAVE_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd == -1)
    {
        printf("No more resources available on the computer");
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

bool compare_chains(Block *chain1, Block *chain2)
{
    Block *one = chain1;
    Block *two = chain2;
    while(one && two)
    {
        if(my_strcmp(one->bid, two->bid) != 0)
        {
            return false;
        }
        one = one->next;
        two = two->next;
    }
    if(one || two)
    {
        return false;
    }
    return true;
}

bool sync_state(Node *head)
{
    if(head == NULL || head->next == NULL)
    {
        return true;
    }
    Node *temp = head;
    Block *chain = temp->chain;
    while(temp->next)
    {
        temp = temp->next;
        if(!compare_chains(chain, temp->chain))
        {
            return false;
        }
    }
    return true;
}

Node *load_file()
{
    int fd = open(BLOCKCKAIN_SAVE_FILE, O_RDONLY);
    if(fd == -1)
    {
        printf("No Backup Found: Starting New Blockchain");
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
void process_rm(char *arg1, char *arg2, char *arg3, Node **head)
{
    if(arg1 == NULL || arg2 == NULL)
    {
        printf("nok: missing argument\n");
    } else if (my_strcmp(arg1, "node") == 0)
    {
        int nid;
        if(my_strcmp(arg2, "*") == 0)
        {
            nid = -1;
        } else
        {
            nid = my_atoi(arg2);
        }
        if(nid == -1)
        {
            while(*head != NULL)
            {
                remove_node((*head)->nid, head);
            }
            printf("Ok\n");
        } else if(remove_node(nid, head) == 0)
        {
            printf("OK\n");
        }
    } else if(my_strcmp(arg1, "block") == 0)
    {
        if(remove_block(arg2, *head) == 0)
        {
            printf("OK\n");
        } else
        {
            printf("nok: block %s doesn't exist\n", arg2);
        }
    } else
    {
        printf("nok: invalid command \n");
    }
}
void process_ls(char *arg1, char *arg2, char *arg3, Node **head)
{
    bool blocks = false;
    if(arg1 != NULL && my_strcmp(arg1, "-l") == 0)
    {
        blocks = true;
    }
    print_node(*head, blocks);
}

void process_sync(char *arg1, char *arg2, char *arg3, Node **head)
{
    sync_state(*head);
    printf("Ok\n");
}

void process_add(char *arg1, char *arg2, char *arg3, Node **head)
{
    int i = 0;
    if(arg1 == NULL || arg2 == NULL)
    {
        printf("nok: missing argument\n");
    } else if(my_strcmp(arg1, "node") == 0)
    {
        i++;
        int nid = my_atoi(arg2);
        if(add_node(nid, head) == 0)
        {
            printf("OK\n");
        }
    } else if(my_strcmp(arg1, "block") == 0)
    {
        if(arg3 == NULL)
        {
            printf("nok: missing argument");
        } else
        {
            int nid;
            if(my_strcmp(arg3, "*") == 0)
            {
                nid = -1;
            } else
            {
                nid = my_atoi(arg3);
            }
            if(add_node(nid, head) == 0)
            {
                printf("OK\n");
            }
        }
    } else
    {
        printf("nok: invalid command\n");
    }

}

void my_blockchain()
{
    printf("here");
    Node *head = load_file();
    char input[100];
    ssize_t bytes_read;
    char *ptr;
    char sync_state_c;
    options_t  command[] = {
            {"add", CMD_ADD, process_add},
            {"rm", CMD_RM, process_rm},
            {"ls", CMD_LS, process_ls},
            {"sync", CMD_SYNC, process_sync},
            {"quit", CMD_QUIT, NULL},
            {"exit", CMD_QUIT, NULL},
            {NULL, CMD_UNKNOWN, NULL}
    };
    printf("I made it here");
    while(1)
    {
        int nodes = count_nodes(head);
        if(sync_state(head))
        {
            sync_state_c = 's';
        } else
        {
            sync_state_c = '_';
        }
        printf("I made it here 2");
        printf("[%c%d]> ", sync_state_c, nodes);
        bytes_read = read(STDIN_FILENO, input, sizeof(input) -1);
        if(bytes_read <= 0)
        {
            printf("nok: failed to read input");
            continue;
        }
        input[bytes_read] = '\0';
        printf("I made it here 3");
        if(input[bytes_read -1] == '\n')
        {
            input[bytes_read -1] = '\0';
            bytes_read--;
        }
        char *cmd = my_strtok(input, " ", &ptr);
        char *arg1 = my_strtok(NULL, " ", &ptr);
        char *arg2 = my_strtok(NULL, " ", &ptr);
        char *arg3 = my_strtok(NULL, " ", &ptr);

        CommandType type = CMD_UNKNOWN;
        for(int i = 0; command[i].command_str; i++)
        {
            if(my_strcmp(cmd, command[i].command_str) == 0)
            {
                type = command[i].cmd_type;
                break;
            }
        }
        switch(type)
        {
            case CMD_ADD:
            case CMD_RM:
            case CMD_LS:
            case CMD_SYNC:
                command[type].cmd_func(arg1, arg2, arg3, &head);
                break;
            case CMD_QUIT:
                save_file(head);
                free_chain(head);
                return;

            default:
                printf("nok: invalid command\n");
        }
    }
}

int main(){
    my_blockchain();
    return 0;
}