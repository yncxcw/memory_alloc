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


using namespace std;



void _memory_(std::vector<long long>& latencys){


int size=M;

for(int index=0; index<100000; index++){
//set up high resolution clock
auto start=std::chrono::high_resolution_clock::now();
//alocate 100M memory pages
char* p_array=new char[size];
//access to force allocation
p_array[0]='a';
p_array[1]='a';
p_array[2]='a';
p_array[size-1]='a';
auto elapsed = std::chrono::high_resolution_clock::now() - start;
long long _seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
latencys.push_back(_seconds);
//deallocate the array
delete p_array;

usleep(10);

}

return;
}

int main(int argc, char* argv[]){

if(argc != 2)
{
 std::cout<<"1 parameter for the # of thread";
 return 0;
} 

unsigned num_cpus = std::thread::hardware_concurrency();
std::cout<<"available cores: "<<num_cpus<<std::endl;
int N = atoi(argv[1]); 

if(N > num_cpus)
{
std::cout<<"# of threads should be less than "<<num_cpus<<std::endl;
return 0;
}

std::vector<thread> threads(N);
std::vector<std::vector<long long>> latency_array(N);
std::cout<<"vector size: "<<latency_array.size()<<std::endl;
for(int i=0;i<N;i++){
  //create the thread
  threads[i]=thread(&_memory_,std::ref(latency_array[i]));
  //set CPU affinity
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(i, &cpuset);
  int rc = pthread_setaffinity_np(threads[i].native_handle(), sizeof(cpu_set_t), &cpuset);
  if (rc != 0) {
      std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
  }
}

for(auto& th : threads){
 th.join();
}

std::cout<<"join finish"<<std::endl;

//write out the latency to file
std::ofstream fwrite("result.txt");

for(auto latencys: latency_array){
  for(auto latency : latencys){
      fwrite<<latency<<" ";
   }
  fwrite<<std::endl; 
}

fwrite.close();

std::cout<<"writes finish"<<std::endl;
return 0;

}
