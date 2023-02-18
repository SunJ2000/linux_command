#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

enum OPT{ 
    OPT_a,OPT_i,OPT_t,OPT_h,OPT_l,OPT_num
};

static std::string g_nowpath="";
static int g_opts[OPT_num]={0};
static std::vector<std::string> g_path;
static std::vector<std::string> g_filename;
static std::vector<struct stat> g_stats;

int get_option(char * opt){
    while (*opt!='\0')
    {
        switch (*opt)
        {
        case 'a':
            g_opts[OPT_a]=1;
            break;
        case 'h':
            g_opts[OPT_h]=1;
            break;
        case 'i':
            g_opts[OPT_i]=1;
            break;
        case 'l':
            g_opts[OPT_l]=1;
            break;
        case 't':
            g_opts[OPT_t]=1;
            break;
        default:
            return -1;
        }
        opt++;
    }
    return 0;
}

int get_file_names(std::string path){
    DIR *dir=opendir(path.c_str());
    if(dir==NULL){
        std::cout<<"opendir "<<path<<" error"<<std::endl;
        return -1;
    }
    dirent * entry;
    while((entry=readdir(dir))!=NULL)
    {
        g_filename.push_back(entry->d_name);
    }
    std::sort(g_filename.begin(),g_filename.end());
    return 0;
}

int get_file_stats(){
    struct stat buf;
    for(auto it=g_filename.begin();it!=g_filename.end();it++){
        int ret=0;

        ret=lstat((g_nowpath+"/"+(*it)).c_str(),&buf);
        if(ret==0){
            g_stats.push_back(buf);
        }else{
            std::cout<<"get file \"" <<*it<<"\" stat error"<<std::endl;
            return -1;
        }
    }
    return 0;
}

std::string get_fileinodenumber_string(ino_t ino){
    std::string inonumber_string="";
    if(g_opts[OPT_i]==1){
        inonumber_string=std::to_string(ino)+"  ";
    }
    return inonumber_string;
}

std::string get_filemode_string(mode_t mode){
    std::string file_type;
    //mode_t tmpmod=mode;
    std::string file_power={};
    if(S_ISDIR(mode)){
        file_type="d";
    }
    else if(S_ISREG(mode)){
        file_type="-";
    }
    else if(S_ISBLK(mode)){
        file_type="b";
    }
    else if(S_ISCHR(mode)){
        file_type="c";
    }
    else if(S_ISSOCK(mode)){
        file_type="s";
    }
    else if(S_ISLNK(mode)){
        file_type="l";
    }
    else {
        file_type="p";
    }
    
    for(int i=3;i>0;i--){
        int pos=i*3-1;
        if(mode&(1<<pos)){
            file_power.push_back('r');
        }else{
            file_power.push_back('-');
        }
        pos--;
        if(mode&(1<<pos)){
            file_power.push_back('w');
        }else{
            file_power.push_back('-');
        }
        pos--;
        if(mode&(1<<pos)){
            file_power.push_back('x');
        }else{
            file_power.push_back('-');
        }        
    }

    return file_type+file_power+"  ";
}

std::string get_filelink_string(nlink_t n){
    return std::to_string(n)+"  ";
}

std::string get_fileusername_string(uid_t uid){
    auto user=getpwuid(uid);
    return (std::string)user->pw_name+"  ";
}

std::string get_filegroupname_string(gid_t gid){
    return (std::string)(getgrgid(gid)->gr_name)+"  ";
}

std::string get_filesize_string(off_t size){
    
    if(g_opts[OPT_h]==1){
        int tmp=size;
        if(size>1024*1024*1024){
            tmp/=1024*1024*1024;
            return std::to_string(tmp)+"G  ";
        }
        else if(size>1024*1024){
            tmp/=1024*1024;
            return std::to_string(tmp)+"M  ";
        }
        else if(size>1024){
            tmp/=1024;
            return std::to_string(tmp)+"K  ";
        }
    }
    return std::to_string(size)+"  ";
}

std::string get_filetime_string(struct timespec filetime){
    char stime[30]={'\0'};
    snprintf(stime,13,"%s",4+ctime(&filetime.tv_sec));
    return (std::string)stime+"  ";
}

std::string get_l_string(const struct stat *file){
    
    return get_fileinodenumber_string(file->st_ino)+get_filemode_string(file->st_mode)
            +get_filelink_string(file->st_nlink)+get_fileusername_string(file->st_uid)
            +get_filegroupname_string(file->st_gid)+get_filesize_string(file->st_size)
            +get_filetime_string(file->st_mtim);

}

void sortFile_byt(){
    size_t n=g_filename.size();
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            if(g_stats[j].st_mtim.tv_sec<g_stats[j+1].st_mtim.tv_sec){
                g_filename[j].swap(g_filename[j+1]);
                 auto tmp=g_stats[j+1];
                 g_stats[j+1]=g_stats[j];
                 g_stats[j]=tmp;
            }
        }
    }
}

int show_ls_onepath(){
    if(g_opts[OPT_l]==1){
        if(get_file_stats()==-1){
            return -1;
        }  
    }
    if(g_opts[OPT_t]==1){
        if(g_opts[OPT_l]==0){
           if(get_file_stats()==-1){
            return -1;
            }  
        }
        sortFile_byt();
    }
    for(std::size_t i=0;i<g_filename.size();i++){
        //std::cout<<g_filename.size()<<" "<<g_stats.size()<<std::endl;
        if(g_opts[OPT_a]==0){
            if(g_filename[i][0]=='.'){
                continue;
            }
        }
        if(g_opts[OPT_l]==1){
             std::cout<<get_l_string(&g_stats[i]);
        }
        std::cout<<g_filename[i]<<std::endl;
    }
    return 0;
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
            g_path.push_back(argv[i]);
        }
    }

    if(g_path.size()==0){
        g_path.push_back(".");
    }
    //test get_option()
    // for(int i=0;i<OPT_num;i++){
    //     if(g_opts[i]==1){
    //         std::cout<<i<<std::endl;
    //     }
    // }
    //test get file_path
    // for(auto it=g_path.begin();it!=g_path.end();it++){
    //     std::cout<<*it<<std::endl;
    // }
    for(auto it=g_path.cbegin();it!=g_path.cend();it++){
        g_nowpath=*it;
        g_filename.clear();
        g_stats.clear();
        std::cout<<*it<<": "<<std::endl;
        get_file_names(*it);
        show_ls_onepath();
        std::cout<<std::endl;
    }
    return 0;
}
