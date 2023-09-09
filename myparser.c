#include "myshell.h"

/**
 * is_cmd - determines if a file is an executable command
 * @info_struct: the info struct
 * @file_path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int is_cmd(info_t *info_struct, char *file_path)
{
    struct stat st;

    (void)info_struct;
    if (!file_path || stat(file_path, &st))
        return (0);

    if (st.st_mode & S_IFREG)
    {
        return (1);
    }
    return (0);
}

/**
 * dup_chars - duplicates characters
 * @path_string: the PATH string
 * @start_index: starting index
 * @stop_index: stopping index
 *
 * Return: pointer to new buffer
 */
char *dup_chars(char *path_string, int start_index, int stop_index)
{
    static char buf[1024];
    int i = 0, k = 0;

    for (k = 0, i = start_index; i < stop_index; i++)
        if (path_string[i] != ':')
            buf[k++] = path_string[i];
    buf[k] = 0;
    return (buf);
}

/**
 * find_path - finds this cmd in the PATH string
 * @info_struct: the info struct
 * @path_string: the PATH string
 * @cmd_to_find: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *find_path(info_t *info_struct, char *path_string, char *cmd_to_find)
{
    int i = 0, curr_pos = 0;
    char *path;

    if (!path_string)
        return (NULL);
    if ((_strlen(cmd_to_find) > 2) && starts_with(cmd_to_find, "./"))
    {
        if (is_cmd(info_struct, cmd_to_find))
            return (cmd_to_find);
    }
    while (1)
    {
        if (!path_string[i] || path_string[i] == ':')
        {
            path = dup_chars(path_string, curr_pos, i);
            if (!*path)
                _strcat(path, cmd_to_find);
            else
            {
                _strcat(path, "/");
                _strcat(path, cmd_to_find);
            }
            if (is_cmd(info_struct, path))
                return (path);
            if (!path_string[i])
                break;
            curr_pos = i;
        }
        i++;
    }
    return (NULL);
}
