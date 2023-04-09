#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

enum OPT
{
    OPT_a,
    OPT_d,
    OPT_s,
    OPT_f,
    OPT_num
};

static int g_opts[OPT_num] = {0};
static int deepth = 0;
static char *g_file_path = new char[50];

int get_option(char *opt)
{
    while (*opt != '\0')
    {
        switch (*opt)
        {
        case 'a':
            g_opts[OPT_a] = 1;
            break;
        case 'd':
            g_opts[OPT_d] = 1;
            break;
        case 's':
            g_opts[OPT_s] = 1;
            break;
        case 'f':
            g_opts[OPT_f] = 1;
            break;
        default:
            return -1;
        }
        opt++;
    }
    return 0;
}

void print_file(const char *name, const struct stat *statptr)
{
    if (g_opts[OPT_d] == 1)
    {
        if (S_ISDIR(statptr->st_mode) == 0)
            return;
        
    }
    for (int i = 0; i < deepth - 1; i++)
    {
        std::cout << "| ";
    }
    std::cout << "|--";

    if(g_opts[OPT_s]==1){
        std::cout<<"[ "<<statptr->st_size<<" ]";
    }

    if (g_opts[OPT_f] == 0)
    {
        std::cout << name << std::endl;
    }
    else
    {
        std::cout << g_file_path << std::endl;
    }
}

void dopath(const char *name)
{
    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;
    int n;

    // get file stat
    if (lstat(g_file_path, &statbuf) < 0)
    {
        std::cout << "lstat error: " << g_file_path << std::endl;
        return;
    }
    // not a directory
    if (S_ISDIR(statbuf.st_mode) == 0)
    {
        print_file(name, &statbuf);
        return;
    }
    print_file(name, &statbuf);
    n = strlen(g_file_path);
    g_file_path[n++] = '/';
    g_file_path[n] = '\0';
    dp = opendir(g_file_path);
    deepth++;
    while ((dirp = readdir(dp)) != nullptr)
    {

        if (g_opts[OPT_a] == 0 && dirp->d_name[0] == '.')
        {
            continue;
        }

        if (strcmp(dirp->d_name, ".") == 0 ||
            strcmp(dirp->d_name, "..") == 0)
            continue;

        strcpy(&g_file_path[n], dirp->d_name);
        dopath(dirp->d_name);
    }
    deepth--;
    g_file_path[n - 1] = '\0';
    closedir(dp);
}

int main(int argc, char *argv[])
{
    g_file_path[0] = '.';
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (get_option(argv[i] + 1) == -1)
            {
                std::cout << "bad options" << std::endl;
                return -1;
            }
        }
        else
        {
            strcpy(g_file_path, argv[i]);
            struct stat file_stat;
            if (stat(g_file_path, &file_stat) < 0)
            {
                std::cout << "no file or dir" << g_file_path << std::endl;
                return -1;
            }
        }
    }
    dopath(g_file_path);
    return 0;
}
