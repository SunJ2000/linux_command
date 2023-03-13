#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <vector>

enum OPT
{
    OPT_b,
    OPT_i,
    OPT_f,
    OPT_n,
    OPT_u,
    OPT_num
};

static int g_opts[OPT_num] = {0};

static std::vector<std::string> g_path;

int get_option(char *opt)
{
    while (*opt != '\0')
    {
        switch (*opt)
        {
        case 'b':
            g_opts[OPT_b] = 1;
            break;
        case 'i':
            g_opts[OPT_i] = 1;
            break;
        case 'f':
            g_opts[OPT_f] = 1;
            break;
        case 'n':
            g_opts[OPT_n] = 1;
            break;
        case 'u':
            g_opts[OPT_u] = 1;
            break;
        default:
            return -1;
        }
        opt++;
    }
    return 0;
}

//-1:not exist;  0:floder;  1:file
int floder_or_not(std::string path)
{
    int ret = 0;
    struct stat file_stat;
    if (ret = stat(path.c_str(), &file_stat) < 0)
    {
        return -1;
    }
    if (S_ISDIR(file_stat.st_mode))
    {
        return 0;
    }
    return 1;
}

void mv_file_to_path(const std::string source_file, const std::string dest_file)
{
    if (floder_or_not(source_file) == -1)
    {
        std::cout << "error:sorce_file \'" << source_file << "\' is not existed" << std::endl;
        return;
    }

    if (floder_or_not(dest_file) != -1)
    {
        if (g_opts[OPT_i] == 1)
        {
            std::cout << "overwrite \'" << dest_file << "\'? ";
            char agree;
            std::cin >> agree;
            if (agree == 'y')
            {
                rename(source_file.c_str(), dest_file.c_str());
                return;
            }
            else
            {
                return;
            }
        }
        else if (g_opts[OPT_f] == 1)
        {
            rename(source_file.c_str(), dest_file.c_str());
            return;
        }
        else if (g_opts[OPT_n] == 1)
        {
            return;
        }
    }else{
        rename(source_file.c_str(), dest_file.c_str());
    }
}

int main(int argc, char *argv[])
{
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
            g_path.push_back(argv[i]);
        }
    }

    if (g_path.size() < 2)
    {
        std::cout << "error:no target file" << std::endl;
        return -1;
    }

    std::string last_file = g_path.back();
    std::string dest_file;

    if (g_path.size() > 2)
    {
        if (floder_or_not(last_file) != 0)
        {
            std::cout << "error:target \'" << last_file << "\' is not a directory" << std::endl;
        }
    }

    // dest_file is a directory
    if (floder_or_not(last_file) == 0)
    {
        for (auto it = g_path.begin(); it != g_path.end()-1; it++)
        {
            dest_file = last_file + "/" + (*it);
            mv_file_to_path(*it,dest_file);
        }
        return 0;
    }
    std::string source_file = *g_path.begin();
    dest_file = last_file;
    mv_file_to_path(source_file,dest_file);
    return 0;
}