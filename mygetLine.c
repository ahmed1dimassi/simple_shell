#include "myshell.h"

/**
 * input_buf - buffers chained commands
 * @info: parameter structure
 * @buffer: address of buffer
 * @length: address of length variable
 *
 * Return: bytes read
 */
ssize_t input_buf(info_t *info, char **buffer, size_t *length)
{
    ssize_t bytes_read = 0;
    size_t current_length = 0;

    if (!*length) /* If nothing left in the buffer, fill it */
    {
        /* bfree((void **)info->cmd_buf); */
        free(*buffer);
        *buffer = NULL;
        signal(SIGINT, sigintHandler);
#if USE_GETLINE
        bytes_read = getline(buffer, &current_length, stdin);
#else
        bytes_read = _getline(info, buffer, &current_length);
#endif
        if (bytes_read > 0)
        {
            if ((*buffer)[bytes_read - 1] == '\n')
            {
                (*buffer)[bytes_read - 1] = '\0'; /* Remove trailing newline */
                bytes_read--;
            }
            info->linecount_flag = 1;
            remove_comments(*buffer);
            build_history_list(info, *buffer, info->histcount++);
            /* If (_strchr(*buffer, ';')) is this a command chain? */
            {
                *length = bytes_read;
                info->cmd_buf = buffer;
            }
        }
    }
    return bytes_read;
}

/**
 * get_input - gets a line minus the newline
 * @info: parameter structure
 *
 * Return: bytes read
 */
ssize_t get_input(info_t *info)
{
    static char *buffer; /* The ';' command chain buffer */
    static size_t i, j, length;
    ssize_t bytes_read = 0;
    char **buffer_ptr = &(info->arg), *p;

    _putchar(BUF_FLUSH);
    bytes_read = input_buf(info, &buffer, &length);
    if (bytes_read == -1) /* EOF */
        return -1;
    if (length) /* We have commands left in the chain buffer */
    {
        j = i; /* Initialize a new iterator to the current buffer position */
        p = buffer + i; /* Get a pointer for return */

        check_chain(info, buffer, &j, i, length);
        while (j < length) /* Iterate to semicolon or end */
        {
            if (is_chain(info, buffer, &j))
                break;
            j++;
        }

        i = j + 1; /* Increment past nulled ';'' */
        if (i >= length) /* Reached the end of the buffer? */
        {
            i = length = 0; /* Reset position and length */
            info->cmd_buf_type = CMD_NORM;
        }

        *buffer_ptr = p; /* Pass back a pointer to the current command position */
        return _strlen(p); /* Return the length of the current command */
    }

    *buffer_ptr = buffer; /* Else not a chain, pass back the buffer from _getline() */
    return bytes_read; /* Return the length of the buffer from _getline() */
}

/**
 * read_buf - reads a buffer
 * @info: parameter structure
 * @buffer: buffer
 * @size: size
 *
 * Return: bytes read
 */
ssize_t read_buf(info_t *info, char *buffer, size_t *size)
{
    ssize_t bytes_read = 0;

    if (*size)
        return 0;
    bytes_read = read(info->readfd, buffer, READ_BUF_SIZE);
    if (bytes_read >= 0)
        *size = bytes_read;
    return bytes_read;
}

/**
 * _getline - gets the next line of input from STDIN
 * @info: parameter structure
 * @ptr: address of a pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: bytes read
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
    static char buf[READ_BUF_SIZE];
    static size_t i, len;
    size_t k;
    ssize_t r = 0, s = 0;
    char *p = NULL, *new_p = NULL, *c;

    p = *ptr;
    if (p && length)
        s = *length;
    if (i == len)
        i = len = 0;

    r = read_buf(info, buf, &len);
    if (r == -1 || (r == 0 && len == 0))
        return -1;

    c = _strchr(buf + i, '\n');
    k = c ? 1 + (unsigned int)(c - buf) : len;
    new_p = _realloc(p, s, s ? s + k : k + 1);
    if (!new_p) /* MALLOC FAILURE! */
        return (p ? free(p), -1 : -1);

    if (s)
        _strncat(new_p, buf + i, k - i);
    else
        _strncpy(new_p, buf + i, k - i + 1);

    s += k - i;
    i = k;
    p = new_p;

    if (length)
        *length = s;
    *ptr = p;
    return s;
}

/**
 * sigintHandler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused)) int sig_num)
{
    _puts("\n");
    _puts("$ ");
    _putchar(BUF_FLUSH);
}
