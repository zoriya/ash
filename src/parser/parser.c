/*
** EPITECH PROJECT, 2020
** PSU_minishell2_2019
** File description:
** redirections
*/


#include "shell.h"
#include "redirections.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "utility.h"
#include "parser.h"
#include "builtin.h"

const parser_map parsers[] = {
    {'\'', &parse_quotes},
    {'\0', NULL}
};

char *strcat_realloc(char *dest, char *src)
{
    if (dest) {
        dest = realloc(dest, sizeof(char) * (strlen(dest) + strlen(src) + 1));
        if (!dest)
            return (NULL);
    } else {
        dest = malloc(sizeof(char) * (strlen(src) + 1));
        if (!dest)
            return (NULL);
        dest[0] = '\0';
    }
    strcat(dest, src);
    return (dest);
}

bool is_character_valid(char c)
{
    static bool is_valid = false;

    if (is_valid) {
        is_valid = false;
        return (true);
    }
    if (c == '\\') {
        is_valid = true;
    }
    if (c >= 33 && c <= 126)
        return (true);
    return (false);
}

int call_parsers(char *cmd, int index, char **data)
{
    int new_index = 0;

    for (int i = 0; parsers[i].key; i++) {
        if (cmd[index] != parsers[i].key)
            continue;
        new_index = parsers[i].parser(&cmd[index], &data);
        return (new_index);
    }
    return (0);
}

int manage_specials_parsers(char *cmd, int index, char **buffer, int *inc, char **ptr)
{
    int new_index = 0;
    char *data = NULL;

    new_index  = call_parsers(cmd, index, &data);
    if (new_index == -1)
        return (-1);
    if (new_index > 0) {
        *buffer = add_to_buffer(*buffer, *ptr, (*inc) + 1);
        *buffer = add_to_buffer(*buffer, *data, strlen(*data));
        free(data);
        *inc = 1;
        *ptr = cmd + index + 1;
        if (!(*buffer))
            return (-1);
    }
    return (0);
}

char *add_to_buffer(char *buffer, char *ptr, int nb)
{
    char *new = strndup(ptr, nb + 1);
    
    if (!new)
        return (NULL);
    new[nb - 2] = '\0';
    buffer = strcat_realloc(buffer, new);
    free(new);
    return (buffer);
}

char **parse_input(char *cmd)
{
    char **ret = malloc(sizeof(char *) * (strlen(cmd) + 1));
    char *ptr = cmd;
    int ret_inc = 0;
    char *buffer = NULL;
    int new_index = 0;

    if (!ret)
        return (NULL);
    for (int i = 0, inc = 1; i <= (int)strlen(cmd); i++, inc++) {
        if (is_character_valid(cmd[i])) {
            new_index = manage_specials_parsers(cmd, i, &buffer, &inc, &ptr);
            if (new_index == -1)
                return (NULL);
            i += new_index;
            continue;
        }
        if (inc == 1) {
            ptr = cmd + i + 1;
            inc = 0;
            continue;
        }
        buffer = add_to_buffer(buffer, ptr, inc + 1);
        if (!buffer)
            return (NULL);
        ret[ret_inc++] = buffer;
        buffer = NULL;
        ptr = cmd + i + 1;
        inc = 0;
    }
    ret[ret_inc] = NULL;
    return (ret);
}