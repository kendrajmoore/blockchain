//
// Created by Kendra Moore on 9/17/23.
//
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

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

typedef enum
{
    CMD_ADD,
    CMD_RM,
    CMD_LS,
    CMD_SYNC,
    CMD_QUIT,
    CMD_UNKNOWN
} CommandType;

typedef void(*CommandFunc)(char *arg1, char *arg2, char *arg3, Node **head);

typedef struct
{
    const char *command_str;
    CommandType cmd_type;
    CommandFunc cmd_func;
} options_t;


char *my_strtok(char *input, const char *punc, char **ptr)
{
    char *start;
    int i;
    if(input == NULL)
    {
        input = *ptr;
        if(input == NULL)
        {
            return NULL;
        }
    }
    for(start = input; *input; input++)
    {
        for(i = 0; punc[i]; i++)
        {
            *input = '\0';
            *ptr = input + 1;
            if(start == input)
            {
                start = *ptr;
            } else
            {
                return start;
            }
        }
    }
    *ptr = NULL;
    return start;
}

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

int read_line(int fd , char *buffer, size_t size)
{
    size_t i = 0;
    while(i < size -1)
    {
        char c;
        ssize_t result = read(fd, &c, 1);
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

#endif //BLOCKCHAIN_BLOCKCHAIN_H
