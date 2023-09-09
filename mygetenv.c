#include "myshell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: String array copy of environ
 */
char **get_environ(info_t *info)
{
    if (!info->environ || info->env_changed)
    {
        info->environ = list_to_strings(info->env);
        info->env_changed = 0;
    }

    return (info->environ);
}

/**
 * _unsetenv - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *            constant function prototype.
 * @var_name: the string env var property to be removed
 * Return: 1 on delete, 0 otherwise
 */
int _unsetenv(info_t *info, char *var_name)
{
    list_t *node = info->env;
    size_t index = 0;
    char *p;

    if (!node || !var_name)
        return (0);

    while (node)
    {
        p = starts_with(node->str, var_name);
        if (p && *p == '=')
        {
            info->env_changed = delete_node_at_index(&(info->env), index);
            index = 0;
            node = info->env;
            continue;
        }
        node = node->next;
        index++;
    }
    return (info->env_changed);
}

/**
 * _setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var_name: the string env var property
 * @var_value: the string env var value
 * Return: Always 0
 */
int _setenv(info_t *info, char *var_name, char *var_value)
{
    char *buf = NULL;
    list_t *node;
    char *p;

    if (!var_name || !var_value)
        return (0);

    buf = malloc(_strlen(var_name) + _strlen(var_value) + 2);
    if (!buf)
        return (1);
    _strcpy(buf, var_name);
    _strcat(buf, "=");
    _strcat(buf, var_value);
    node = info->env;
    while (node)
    {
        p = starts_with(node->str, var_name);
        if (p && *p == '=')
        {
            free(node->str);
            node->str = buf;
            info->env_changed = 1;
            return (0);
        }
        node = node->next;
    }
    add_node_end(&(info->env), buf, 0);
    free(buf);
    info->env_changed = 1;
    return (0);
}
