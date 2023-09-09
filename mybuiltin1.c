#include "myshell.h"

/**
 * _myhistory - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @shell_info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _myhistory(info_t *shell_info)
{
    print_list(shell_info->history);
    return (0);
}

/**
 * unset_alias - sets alias to string
 * @shell_info: parameter struct
 * @alias_str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(info_t *shell_info, char *alias_str)
{
    char *p, c;
    int ret;

    p = _strchr(alias_str, '=');
    if (!p)
        return (1);
    c = *p;
    *p = 0;
    ret = delete_node_at_index(&(shell_info->alias),
        get_node_index(shell_info->alias, node_starts_with(shell_info->alias, alias_str, -1)));
    *p = c;
    return (ret);
}

/**
 * set_alias - sets alias to string
 * @shell_info: parameter struct
 * @alias_str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(info_t *shell_info, char *alias_str)
{
    char *p;

    p = _strchr(alias_str, '=');
    if (!p)
        return (1);
    if (!*++p)
        return (unset_alias(shell_info, alias_str));

    unset_alias(shell_info, alias_str);
    return (add_node_end(&(shell_info->alias), alias_str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @alias_node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(list_t *alias_node)
{
    char *p = NULL, *a = NULL;

    if (alias_node)
    {
        p = _strchr(alias_node->str, '=');
        for (a = alias_node->str; a <= p; a++)
            _putchar(*a);
        _putchar('\'');
        _puts(p + 1);
        _puts("'\n");
        return (0);
    }
    return (1);
}

/**
 * _myalias - mimics the alias builtin (man alias)
 * @shell_info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _myalias(info_t *shell_info)
{
    int i = 0;
    char *p = NULL;
    list_t *alias_node = NULL;

    if (shell_info->argc == 1)
    {
        alias_node = shell_info->alias;
        while (alias_node)
        {
            print_alias(alias_node);
            alias_node = alias_node->next;
        }
        return (0);
    }
    for (i = 1; shell_info->argv[i]; i++)
    {
        p = _strchr(shell_info->argv[i], '=');
        if (p)
            set_alias(shell_info, shell_info->argv[i]);
        else
            print_alias(node_starts_with(shell_info->alias, shell_info->argv[i], '='));
    }

    return (0);
}
