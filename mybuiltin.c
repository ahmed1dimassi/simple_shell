#include "myshell.h"

/**
 * _myexit - exits the shell
 * @new_info: Structure containing potential arguments. Used to maintain
 *            constant function prototype.
 * Return: exits with a given exit status
 *         (0) if new_info->argv[0] != "exit"
 */
int _myexit(info_t *new_info)
{
    int exitcheck;

    if (new_info->argv[1])  /* If there is an exit argument */
    {
        exitcheck = _erratoi(new_info->argv[1]);
        if (exitcheck == -1)
        {
            new_info->status = 2;
            print_error(new_info, "Illegal number: ");
            _eputs(new_info->argv[1]);
            _eputchar('\n');
            return (1);
        }
        new_info->err_num = _erratoi(new_info->argv[1]);
        return (-2);
    }
    new_info->err_num = -1;
    return (-2);
}

/**
 * _mycd - changes the current directory of the process
 * @new_info: Structure containing potential arguments. Used to maintain
 *            constant function prototype.
 * Return: Always 0
 */
int _mycd(info_t *new_info)
{
    char *s, *dir, buffer[1024];
    int chdir_ret;

    s = getcwd(buffer, 1024);
    if (!s)
        _puts("TODO: >>getcwd failure emsg here<<\n");
    if (!new_info->argv[1])
    {
        dir = _getenv(new_info, "HOME=");
        if (!dir)
            chdir_ret = /* TODO: what should this be? */
                chdir((dir = _getenv(new_info, "PWD=")) ? dir : "/");
        else
            chdir_ret = chdir(dir);
    }
    else if (_strcmp(new_info->argv[1], "-") == 0)
    {
        if (!_getenv(new_info, "OLDPWD="))
        {
            _puts(s);
            _putchar('\n');
            return (1);
        }
        _puts(_getenv(new_info, "OLDPWD=")), _putchar('\n');
        chdir_ret = /* TODO: what should this be? */
            chdir((dir = _getenv(new_info, "OLDPWD=")) ? dir : "/");
    }
    else
        chdir_ret = chdir(new_info->argv[1]);
    if (chdir_ret == -1)
    {
        print_error(new_info, "can't cd to ");
        _eputs(new_info->argv[1]), _eputchar('\n');
    }
    else
    {
        _setenv(new_info, "OLDPWD", _getenv(new_info, "PWD="));
        _setenv(new_info, "PWD", getcwd(buffer, 1024));
    }
    return (0);
}

/**
 * _myhelp - changes the current directory of the process
 * @new_info: Structure containing potential arguments. Used to maintain
 *            constant function prototype.
 * Return: Always 0
 */
int _myhelp(info_t *new_info)
{
    char **arg_array;

    arg_array = new_info->argv;
    _puts("help call works. Function not yet implemented \n");
    if (0)
        _puts(*arg_array); /* temp att_unused workaround */
    return (0);
}
