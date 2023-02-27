#include<stdio.h>
#include<string>
#include<vector>

enum OPT{ 
    OPT_n,OPT_b,OPT_s,OPT_num
};
static int g_opts[OPT_num]={0};

static std::vector<std::string> in_files_str;
static std::string outfile_str;

int get_option(char * opt){
    while (*opt!='\0')
    {
        switch (*opt)
        {
        case 'n':
            g_opts[OPT_n]=1;
            break;
        case 'b':
            g_opts[OPT_b]=1;
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

int cat_file_to_file(FILE *infile,FILE *outfile){
    char buf[1024];
    int i=1;
    bool mark=false;
    while (fgets(buf,1024,infile)!=NULL)
    {  
      
        if(g_opts[OPT_s]==1){
            if(mark){
                if(buf[0]=='\n'){
                continue;
                }  
            }
        }
        
        if(g_opts[OPT_n]==1){
            fputs((std::to_string(i)+"   ").c_str(),outfile);
        }else if(g_opts[OPT_b]==1){
            if(buf[0]=='\n'){
                continue;
            }
            else{
                fputs((std::to_string(i)+"   ").c_str(),outfile);
            }           
        }
        
        if(buf[0]=='\n'){
            mark=true;
        }else{
            mark=false;
        }
        if (fputs(buf,outfile)==EOF)
        {
            printf("output error\n");
        }
        i++;
    }
    return 0;
}

int main(int argc,char *argv[]){
    for(int i=1;i<argc;i++){
        if(argv[i][0]=='-'){
            if(get_option(argv[i]+1)==-1){
                printf("bad options\n");
                return -1;
            }
        }
        else if(argv[i][0]=='>'){
            i++;
            outfile_str=argv[i];
            break;
        }else{
            in_files_str.push_back(argv[i]);
        } 
    }

    FILE * outfile;
    if(outfile_str.empty()){
        outfile=stdout;
    }else{
        if((outfile=fopen(outfile_str.c_str(),"wb"))==NULL){
            printf("open outfile error\n");
            return 0;
        }
    }
    
    for(auto it=in_files_str.begin();it!=in_files_str.end();it++){
        FILE * infile;
        if((infile=fopen((*it).c_str(),"rb"))==NULL){
            printf("open infile error\n");
            return 0;
        }
        cat_file_to_file(infile,outfile);
    }
    return 0;
}
