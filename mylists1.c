#include "myshell.h"

/**
 * list_len - determines length of linked list
 * @first_node: pointer to first node
 *
 * Return: size of list
 */
size_t list_len(const list_t *first_node)
{
    size_t i = 0;

    while (first_node)
    {
        first_node = first_node->next;
        i++;
    }
    return (i);
}

/**
 * list_to_strings - returns an array of strings of the list->str
 * @list_head: pointer to first node
 *
 * Return: array of strings
 */
char **list_to_strings(list_t *list_head)
{
    list_t *node = list_head;
    size_t i = list_len(list_head), j;
    char **strs;
    char *str;

    if (!list_head || !i)
        return (NULL);
    strs = malloc(sizeof(char *) * (i + 1));
    if (!strs)
        return (NULL);
    for (i = 0; node; node = node->next, i++)
    {
        str = malloc(_strlen(node->str) + 1);
        if (!str)
        {
            for (j = 0; j < i; j++)
                free(strs[j]);
            free(strs);
            return (NULL);
        }

        str = _strcpy(str, node->str);
        strs[i] = str;
    }
    strs[i] = NULL;
    return (strs);
}

/**
 * print_list - prints all elements of a list_t linked list
 * @first_node: pointer to first node
 *
 * Return: size of list
 */
size_t print_list(const list_t *first_node)
{
    size_t i = 0;

    while (first_node)
    {
        _puts(convert_number(first_node->num, 10, 0));
        _putchar(':');
        _putchar(' ');
        _puts(first_node->str ? first_node->str : "(nil)");
        _puts("\n");
        first_node = first_node->next;
        i++;
    }
    return (i);
}

/**
 * node_starts_with - returns node whose string starts with prefix
 * @list_head: pointer to list head
 * @prefix: string to match
 * @next_char: the next character after prefix to match
 *
 * Return: match node or null
 */
list_t *node_starts_with(list_t *list_head, char *prefix, char next_char)
{
    char *p = NULL;

    while (list_head)
    {
        p = starts_with(list_head->str, prefix);
        if (p && ((next_char == -1) || (*p == next_char)))
            return (list_head);
        list_head = list_head->next;
    }
    return (NULL);
}

/**
 * get_node_index - gets the index of a node
 * @first_node: pointer to list head
 * @target_node: pointer to the node
 *
 * Return: index of node or -1
 */
ssize_t get_node_index(list_t *first_node, list_t *target_node)
{
    size_t i = 0;

    while (first_node)
    {
        if (first_node == target_node)
            return (i);
        first_node = first_node->next;
        i++;
    }
    return (-1);
}
