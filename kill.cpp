#include<iostream>
#include<string>
#include<vector>

#include<signal.h>

std::vector<std::string> g_sigs{ "","SIGHUP","SIGINT",	" SIGQUIT"	, 
"SIGILL",	" SIGTRAP","SIGABRT" ,"SIGBUS"	,"SIGFPE","SIGKILL", "SIGUSR1",
"SIGSEGV"	, "SIGUSR2"	, "SIGPIPE",	"SIGALRM","SIGTERM","SIGSTKFLT", 
"SIGCHLD"	,"SIGCONT",	 "SIGSTOP", "SIGTSTP","SIGTTIN","SIGTTOU"	, "SIGURG", 
"SIGXCPU"	,"SIGXFSZ","SIGVTALRM", "SIGPROF", "SIGWINCH","SIGIO","SIGPWR",
"SIGSYS"	, "SIGRTMIN"};

enum OPT{ 
    OPT_l,OPT_s,OPT_num
};
static int g_opts[OPT_num]={0};

static std::vector<int>g_nums;
static int g_signal;

int get_option(char * opt){
    if(*opt=='l'){
        g_opts[OPT_l]=1;
    }else if(*opt=='s'){
        g_opts[OPT_s]=1;
    }else if(*opt>'0'&&*opt<'9'){
        g_signal=atoi(opt);
    }else{
        return -1;
    }
    return 0;
}

void showAllSignals(){
    for(int i=1;i<g_sigs.size();i++){
        std::cout<<i<<") "<<g_sigs[i]<<"  ";
    }
    std::cout<<std::endl;
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
            g_nums.push_back(atoi(argv[i]));
        }
    }

    if(g_opts[OPT_l]==1){
        if(g_nums.empty()){
            showAllSignals();
        }
        else {
            for(auto it=g_nums.begin();it!=g_nums.end();it++){
                if(*it<g_sigs.size()){
                    std::cout<<*it<<") "<<g_sigs[*it]<<" ";
                }else{
                    std::cout<<"signal "<<*it<<" not exitence";
                }
            }
            std::cout<<std::endl;
        }

        return 0;
    }

    if(g_opts[OPT_s]==1){
        auto it=g_nums.begin();
        g_signal=*it;
        if(*it<g_sigs.size()){
           for(it++;it!=g_nums.end();it++){
            if(auto ret=kill(*it,g_signal)<0){
                std::cout<<"kill "<< g_signal<<" to pid "<<*it <<"error"<<std::endl;
            }
           }
           
        }else{
            std::cout<<"signal "<<*it<<" not exitence"<<std::endl;
            
        }
        return 0;
    }

    if(g_opts[OPT_s]==0&&g_opts[OPT_l]==0){
         for(auto it=g_nums.begin();it!=g_nums.end();it++){
            if(auto ret=kill(*it,SIGKILL)<0){
                std::cout<<"kill "<< SIGKILL<<" to pid "<<*it <<"error"<<std::endl;
            }
           }
    }
    return 0;
}