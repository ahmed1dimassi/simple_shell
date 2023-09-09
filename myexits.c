#include "myshell.h"

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
