#include<unistd.h>
#include<fcntl.h>
#include<iostream>
#include<vector>
enum OPT{ 
    OPT_f,OPT_i,OPT_n,OPT_s,OPT_num
};


static int g_opts[OPT_num]={0};
static std::vector<std::string> g_path;
int get_option(char * opt){
    while (*opt!='\0')
    {
        switch (*opt)
        {
        case 'f':
            g_opts[OPT_f]=1;
            break;
        case 'i':
            g_opts[OPT_i]=1;
            break;
        case 'n':
            g_opts[OPT_n]=1;
            break;
        case 's':
            g_opts[OPT_s]=1;
            break;
        default:
            return -1;
        }
        opt++;
    }
    return 0;
}

int main(int argc ,char **argv){
    for( int i=1;i<argc;i++){
        if(argv[i][0]=='-'){
            if(get_option(argv[i]+1)==-1){
                std::cout<<"bad options"<<std::endl;
                return -1;
            }
        }else{
            g_path.push_back(argv[i]);
        }
    }

    if(g_path.size()<=0){
        std::cout<<"have no input filepath"<<std::endl; 
        return 0;
    }

    if(g_path.size()==1){
        if(access(g_path[0].c_str(),F_OK)==-1){
            std::cout<<"source file : "<<g_path[0]<<" not existence"<<std::endl;
            return 0;
        }
         char nowpath[100];
         getcwd(nowpath,99);
         g_path.push_back(nowpath);
    }

    for(auto str:g_path){
        std::cout<<str<<std::endl;
    }

    for(int i=1;i<g_path.size();i++){
        if(access(g_path[i].c_str(),F_OK)!=-1){
            std::cout<<"distance file : "<<g_path[i]<<" has existenced"<<std::endl;
            break;
        }
        if(g_opts[OPT_s]){
            symlink(g_path[0].c_str(),g_path[i].c_str());
        }else{
            link(g_path[0].c_str(),g_path[i].c_str());
        }
    }

    return 0;

}
