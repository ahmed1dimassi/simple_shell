#include "myshell.h"

/**
 * bfree - frees a pointer and NULLs the address
 * @ptr_address: address of the pointer to free
 *
 * Return: 1 if freed, otherwise 0.
 */
int bfree(void **ptr_address)
{
    if (ptr_address && *ptr_address)
    {
        free(*ptr_address);
        *ptr_address = NULL;
        return (1);
    }
    return (0);
}
