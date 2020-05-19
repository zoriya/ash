/*
** EPITECH PROJECT, 2020
** PSU_minishell1_2019
** File description:
** main
*/

#include "shell.h"
#include <stdlib.h>
#include <malloc.h>
#include <stddef.h>
#include <string.h>

int main(int argc, char **argv, char **env)
{
    env_t *envt = malloc(sizeof(*envt));
    char **envcp = malloc(sizeof(char *) * (env_get_length(env) + 1));
    int ret;

    if (!env || !envcp)
        return (84);
    for (int i = 0; env[i]; i++)
        envcp[i] = strdup(env[i]);
    envcp[env_get_length(env)] = NULL;
    envt->env = envcp;
    envt->vars = NULL;
    envt->history = NULL;
    envt->alias = NULL;
    envt->vars = my_setenv(envt->vars, "?", "0");
    start_shell(envt);
    ret = get_return(my_getenv(envt->vars, "?"));
    (void)argc;
    (void)argv;
    free_env(envt);
    return (ret);
}