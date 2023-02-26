#include<sys/stat.h>
#include<dirent.h>
#include<iostream>
#include<string>
#include<vector>

enum OPT{ 
    OPT_p,OPT_num
};
static int g_opts[OPT_num]={0};

static std::vector<std::string> dir_path;

int get_option(char * opt){
    while (*opt!='\0')
    {
        switch (*opt)
        {
        case 'p':
            g_opts[OPT_p]=1;
            break;
        default:
            return -1;
        }
        opt++;
    }
    return 0;
}

void get_dir_path_p(char * path){
    std::string dir;
    for(int i=0;path[i]!='\0';i++){
        if(path[i]!='/'){
            dir.push_back(path[i]);
        }else{
            dir_path.push_back(dir);
            dir.push_back('/');
        }   
    }
    dir_path.push_back(path);
}

int main(int argc,char *argv[]){
    for(int i=1;i<argc;i++){
        if(argv[i][0]=='-'){
            if(get_option(argv[i]+1)==-1){
                std::cout<<"bad options"<<std::endl;
                return -1;
            }
        }
        else{
            if(g_opts[OPT_p]==1){
                get_dir_path_p(argv[i]);
            }else{
                dir_path.push_back(argv[i]);
            }
        }
    }

    for(auto it=dir_path.begin();it!=dir_path.end();it++){
        auto path=(*it).c_str();
        if(NULL==opendir(path)){
            if (int ret=mkdir(path,S_IRWXU)<0){
                std::cout<<"mkdir "<<path<<" error"<<std::endl;
            }
        }
    }

    return 0;
}
