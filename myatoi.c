#include "myshell.h"

/**
 * interactive - returns true if shell is interactive mode
 * @new_info: struct address
 *
 * Return: 1 if interactive mode, 0 otherwise
 */
int interactive(info_t *new_info)
{
	return (isatty(STDIN_FILENO) && new_info->readfd <= 2);
}

/**
 * is_delim - checks if character is a delimeter
 * @new_c: the char to check
 * @new_delim: the delimeter string
 * Return: 1 if true, 0 if false
 */
int is_delim(char new_c, char *new_delim)
{
	while (*new_delim)
		if (*new_delim++ == new_c)
			return (1);
	return (0);
}

/**
 *_isalpha - checks for alphabetic character
 *@new_c: The character to input
 *Return: 1 if c is alphabetic, 0 otherwise
 */

int _isalpha(int new_c)
{
	if ((new_c >= 'a' && new_c <= 'z') || (new_c >= 'A' && new_c <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 *_atoi - converts a string to an integer
 *@new_s: the string to be converted
 *Return: 0 if no numbers in string, converted number otherwise
 */

int _atoi(char *new_s)
{
	int i, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (i = 0;  new_s[i] != '\0' && flag != 2; i++)
	{
		if (new_s[i] == '-')
			sign *= -1;

		if (new_s[i] >= '0' && new_s[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (new_s[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}


/**
 **_strncpy - copies a string
 *@destination: the destination string to be copied to
 *@source: the source string
 *@n: the number of characters to be copied
 *Return: the concatenated string
 */
char *_strncpy(char *destination, char *source, int n)
{
    int i, j;
    char *result = destination;

    i = 0;
    while (source[i] != '\0' && i < n - 1)
    {
        destination[i] = source[i];
        i++;
    }
    if (i < n)
    {
        j = i;
        while (j < n)
        {
            destination[j] = '\0';
            j++;
        }
    }
    return (result);
}

/**
 **_strncat - concatenates two strings
 *@destination: the first string
 *@source: the second string
 *@n: the maximum number of bytes to be used
 *Return: the concatenated string
 */
char *_strncat(char *destination, char *source, int n)
{
    int i, j;
    char *result = destination;

    i = 0;
    j = 0;
    while (destination[i] != '\0')
        i++;
    while (source[j] != '\0' && j < n)
    {
        destination[i] = source[j];
        i++;
        j++;
    }
    if (j < n)
        destination[i] = '\0';
    return (result);
}

/**
 **_strchr - locates a character in a string
 *@str: the string to be parsed
 *@character: the character to look for
 *Return: a pointer to the memory area str
 */
char *_strchr(char *str, char character)
{
    do
    {
        if (*str == character)
            return (str);
    } while (*str++ != '\0');

    return (NULL);
}


/**
 * get_history_file - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containing history file
 */
char *get_history_file(info_t *info)
{
    char *buf, *dir;

    dir = _getenv(info, "HOME=");
    if (!dir)
        return (NULL);
    buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
    if (!buf)
        return (NULL);
    buf[0] = 0;
    _strcpy(buf, dir);
    _strcat(buf, "/");
    _strcat(buf, HIST_FILE);
    return (buf);
}

/**
 * write_history - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_history(info_t *info)
{
    ssize_t fd;
    char *filename = get_history_file(info);
    list_t *node = NULL;

    if (!filename)
        return (-1);

    fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
    free(filename);
    if (fd == -1)
        return (-1);
    for (node = info->history; node; node = node->next)
    {
        _putsfd(node->str, fd);
        _putfd('\n', fd);
    }
    _putfd(BUF_FLUSH, fd);
    close(fd);
    return (1);
}

/**
 * read_history - reads history from file
 * @info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int read_history(info_t *info)
{
    int i, last = 0, linecount = 0;
    ssize_t fd, rdlen, fsize = 0;
    struct stat st;
    char *buf = NULL, *filename = get_history_file(info);

    if (!filename)
        return (0);

    fd = open(filename, O_RDONLY);
    free(filename);
    if (fd == -1)
        return (0);
    if (!fstat(fd, &st))
        fsize = st.st_size;
    if (fsize < 2)
        return (0);
    buf = malloc(sizeof(char) * (fsize + 1));
    if (!buf)
        return (0);
    rdlen = read(fd, buf, fsize);
    buf[fsize] = 0;
    if (rdlen <= 0)
        return (free(buf), 0);
    close(fd);
    for (i = 0; i < fsize; i++)
        if (buf[i] == '\n')
        {
            buf[i] = 0;
            build_history_list(info, buf + last, linecount++);
            last = i + 1;
        }
    if (last != i)
        build_history_list(info, buf + last, linecount++);
    free(buf);
    info->histcount = linecount;
    while (info->histcount-- >= HIST_MAX)
        delete_node_at_index(&(info->history), 0);
    renumber_history(info);
    return (info->histcount);
}

/**
 * build_history_list - adds entry to a history linked list
 * @info: parameter struct
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int build_history_list(info_t *info, char *buf, int linecount)
{
    list_t *node = NULL;

    if (info->history)
        node = info->history;
    add_node_end(&node, buf, linecount);

    if (!info->history)
        info->history = node;
    return (0);
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @info: parameter struct
 *
 * Return: the new histcount
 */
int renumber_history(info_t *info)
{
    list_t *node = info->history;
    int i = 0;

    while (node)
    {
        node->num = i++;
        node = node->next;
    }
    return (info->histcount = i);
}


/**
 * is_chain - test if current char in buffer is a chain delimiter
 * @info: the parameter struct
 * @buffer: the char buffer
 * @position: address of current position in buffer
 *
 * Return: 1 if chain delimiter, 0 otherwise
 */
int is_chain(info_t *info, char *buffer, size_t *position)
{
	size_t j = *position;

	if (buffer[j] == '|' && buffer[j + 1] == '|')
	{
		buffer[j] = 0;
		j++;
		info->cmd_buf_type = CMD_OR;
	}
	else if (buffer[j] == '&' && buffer[j + 1] == '&')
	{
		buffer[j] = 0;
		j++;
		info->cmd_buf_type = CMD_AND;
	}
	else if (buffer[j] == ';') /* found end of this command */
	{
		buffer[j] = 0; /* replace semicolon with null */
		info->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*position = j;
	return (1);
}

/**
 * check_chain - checks if we should continue chaining based on last status
 * @info: the parameter struct
 * @buffer: the char buffer
 * @position: address of current position in buffer
 * @start: starting position in buffer
 * @length: length of buffer
 *
 * Return: Void
 */
void check_chain(info_t *info, char *buffer, size_t *position, size_t start, size_t length)
{
	size_t j = *position;

	if (info->cmd_buf_type == CMD_AND)
	{
		if (info->status)
		{
			buffer[start] = 0;
			j = length;
		}
	}
	if (info->cmd_buf_type == CMD_OR)
	{
		if (!info->status)
		{
			buffer[start] = 0;
			j = length;
		}
	}

	*position = j;
}

/**
 * replace_alias - replaces an alias in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_alias(info_t *info)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(info->alias, info->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		info->argv[0] = p;
	}
	return (1);
}

/**
 * replace_vars - replaces variables in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_vars(info_t *info)
{
	int i = 0;
	list_t *node;

	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (!_strcmp(info->argv[i], "$?"))
		{
			replace_string(&(info->argv[i]),
				_strdup(convert_number(info->status, 10, 0)));
			continue;
		}
		if (!_strcmp(info->argv[i], "$$"))
		{
			replace_string(&(info->argv[i]),
				_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(info->env, &info->argv[i][1], '=');
		if (node)
		{
			replace_string(&(info->argv[i]),
				_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		replace_string(&info->argv[i], _strdup(""));

	}
	return (0);
}

/**
 * replace_string - replaces a string
 * @old_str: address of the old string
 * @new_str: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_string(char **old_str, char *new_str)
{
	free(*old_str);
	*old_str = new_str;
	return (1);
}


/**
 * **strtow - splits a string into words. Repeat delimiters are ignored
 * @input_str: the input string
 * @delimiter_str: the delimiter string
 * Return: a pointer to an array of strings, or NULL on failure
 */

char **strtow(char *input_str, char *delimiter_str)
{
    int i, j, k, m, num_words = 0;
    char **result;

    if (input_str == NULL || input_str[0] == 0)
        return (NULL);
    if (!delimiter_str)
        delimiter_str = " ";
    for (i = 0; input_str[i] != '\0'; i++)
        if (!is_delim(input_str[i], delimiter_str) && (is_delim(input_str[i + 1], delimiter_str) || !input_str[i + 1]))
            num_words++;

    if (num_words == 0)
        return (NULL);
    result = malloc((1 + num_words) * sizeof(char *));
    if (!result)
        return (NULL);
    for (i = 0, j = 0; j < num_words; j++)
    {
        while (is_delim(input_str[i], delimiter_str))
            i++;
        k = 0;
        while (!is_delim(input_str[i + k], delimiter_str) && input_str[i + k])
            k++;
        result[j] = malloc((k + 1) * sizeof(char));
        if (!result[j])
        {
            for (k = 0; k < j; k++)
                free(result[k]);
            free(result);
            return (NULL);
        }
        for (m = 0; m < k; m++)
            result[j][m] = input_str[i++];
        result[j][m] = 0;
    }
    result[j] = NULL;
    return (result);
}

/**
 * **strtow2 - splits a string into words
 * @input_str: the input string
 * @delimiter: the delimiter character
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **strtow2(char *input_str, char delimiter)
{
    int i, j, k, m, num_words = 0;
    char **result;

    if (input_str == NULL || input_str[0] == 0)
        return (NULL);
    for (i = 0; input_str[i] != '\0'; i++)
        if ((input_str[i] != delimiter && input_str[i + 1] == delimiter) ||
            (input_str[i] != delimiter && !input_str[i + 1]) || input_str[i + 1] == delimiter)
            num_words++;
    if (num_words == 0)
        return (NULL);
    result = malloc((1 + num_words) * sizeof(char *));
    if (!result)
        return (NULL);
    for (i = 0, j = 0; j < num_words; j++)
    {
        while (input_str[i] == delimiter && input_str[i] != delimiter)
            i++;
        k = 0;
        while (input_str[i + k] != delimiter && input_str[i + k] && input_str[i + k] != delimiter)
            k++;
        result[j] = malloc((k + 1) * sizeof(char));
        if (!result[j])
        {
            for (k = 0; k < j; k++)
                free(result[k]);
            free(result);
            return (NULL);
        }
        for (m = 0; m < k; m++)
            result[j][m] = input_str[i++];
        result[j][m] = 0;
    }
    result[j] = NULL;
    return (result);
}
