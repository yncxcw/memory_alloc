import numpy as np
import  matplotlib.pyplot as pl
import subprocess


"""
execute a benchmark to plot graph
"""

def read_results(file_name):
    results=[]
    with open(file_name,"r") as fread:
        for line in fread.readlines():
            items=line.strip().split()
            results.extend([int(item) for item in items])
    return results



def execute_benchmark(commands):
    subprocess.call(commands)

def get_cdf(datas):
    datas = sorted(datas)
    datas = datas[0:int(0.999*len(datas))]
    #print datas[int(0.99*len(datas))]
    datas = np.array(datas)
    count,bv=np.histogram(datas,bins=100)
    cdf=np.cumsum(count)
    cdf=cdf/float(count.sum())
    return bv[1:],cdf

def draw_cdf(results):
    X,Y=get_cdf(results)
    pl.plot(X,Y)
    pl.show()


if __name__=="__main__":
    """
    Benchmarking the memory allocation latency, usage
    memory_alloc_thread num_threads
    """
    results=[]
    for num_thread in range(1,6,2):
        ##1. execute the benchmark   
        commands=["./memory_alloc_thread",str(num_thread)]
        execute_benchmark(commands) 

        ##2 read the results
        results.append(read_results("result.txt"))

    num_threads=1
    print len(results)
    for result in results:
        print "reuslt len",len(result)
        X,Y=get_cdf(result)
        pl.plot(X,Y,label=str(num_threads))
        num_threads = num_threads + 2

    pl.legend()
    pl.show()
