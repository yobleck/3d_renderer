//g++ -pthread -o mt_test_exe mt_test.cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
using namespace std;

mutex m;  //https://nrecursions.blogspot.com/2014/08/mutex-tutorial-and-example.html

void func(int split, int start, int end){
    //cout << "id: " << this_thread::get_id() << endl;
    double num_divisor, big_divisor, modulo;
    for(int i=start;i<=end;i++)//number being checked
    {
        num_divisor = 0;
        for(int j=1;j<=i;j++)//divisor being checked
        {
            modulo = i%j;
            if(modulo == 0)//modulo to check divisibility
            {
               num_divisor++;
            }
        }
        if(num_divisor > big_divisor)
        {
            big_divisor = num_divisor;
            m.lock();
            cout << i << endl;
            m.unlock();
        }
    }
}

int main()
{
    int counter = 100000;

    auto start = chrono::high_resolution_clock::now();
    double num_divisor, big_divisor, modulo;
    for(int i=1;i<=counter;i++)//number being checked
    {
        num_divisor = 0;
        for(int j=1;j<=i;j++)//divisor being checked
        {
            modulo = i%j;
            if(modulo == 0)//modulo to check divisibility
            {
               num_divisor++;
            }
        }
        if(num_divisor > big_divisor)
        {
            big_divisor = num_divisor;
            cout << i << endl;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "time: " << elapsed.count() << endl;
    

    ///////////////////////////////////////////////////////
    
    
    auto start1 = chrono::high_resolution_clock::now();
    int thread_count = 4;
    vector<thread> threads;
    for(int i=0;i<thread_count;i++){
        threads.push_back(thread(func,thread_count,(i* counter/thread_count)+1,(i+1)* counter/thread_count));
    }
    for(thread & t: threads){
        if(t.joinable()==true){
            t.join();
        }
    }
    auto end1 = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed1 = end1 - start1;
    cout << "time1: " << elapsed1.count() << endl;
    return 0;
}
