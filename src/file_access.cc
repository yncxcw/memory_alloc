#include<iostream>
#include<thread>
#include<vector>
#include<ctime>
#include<sys/time.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<sys/types.h>
#include<stdio.h>
#include<fstream>
//size of memory in terms of GB
#define S 1*1024
//size of 1MB
#define M 1024*1024
//default is 8
//number of threads
#define N 15


using namespace std;


void allocate(int tid,char** p){

cout<<tid<<" start allocate file"<<endl;

const char* fname=string("/tmp/file_"+to_string(tid)).c_str();

FILE* fd=fopen(fname,"w+");

if(fd==NULL){
  cout<<"file open error at allocating"<<endl;
}


for(int i=0;i<S*10;i++){
  char *_tows=new char[M];
  for(int j=0;j<M;j++){
    _tows[j]='a';
 }

 fwrite(_tows,sizeof(char),M,fd);
 delete _tows;

}
fclose(fd);

}

void do_access(int tid,char** p){

cout<<tid<<" start access"<<endl;

const char* fname=string("/tmp/file_"+to_string(tid)).c_str();

FILE* fd=fopen(fname,"r");

if(fd==NULL){
  cout<<"file open error at accessing"<<endl;
}

int loop=0;

//default is 2
while(loop<5){
  char _ch;
  while((_ch=fgetc(fd))!=EOF){
   //sequentialy read file
  }
  //move the file pointer to the start
  fseek(fd,0,SEEK_SET);
  
 loop++;
}


fclose(fd);
}
void _memory_(int index){

//sieze of 1K pointer
char *p[S];
allocate(index,p);
do_access(index,p);

return;
}

int main(){
int pid=getpid();
int memcg_id=syscall(333,pid);
std::cout<<"pid: "<<pid<<"memcg_id: "<<memcg_id<<endl;

//allocation
timespec ts,te;
clock_gettime(CLOCK_REALTIME, &ts);
std::cout<<"start"<<std::endl;
std::vector<thread> threads;
for(int i=1;i<=N;i++){
 threads.push_back(thread(&_memory_,i));
}

for(auto& th : threads){
 th.join();
}
clock_gettime(CLOCK_REALTIME, &te);
std::cout<<"finish "<<double(te.tv_sec-ts.tv_sec)<<std::endl;
return 0;

}
