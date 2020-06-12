#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <list>
#include <functional>
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <memory>
#include <tuple>
#include <atomic>
#include <utility>
#include <map>
#include <numeric>
#include "Thread_q.h"
#include "Thread_p.h"

using namespace std;


void f1()
{
   this_thread::sleep_for(1s);
   cout<<"1 running on: "<<this_thread::get_id()<<endl;
}

void f2()
{
   this_thread::sleep_for(1s);
   cout<<"2 running on: "<<this_thread::get_id()<<endl;
}

void f3()
{
   this_thread::sleep_for(1s);
   cout<<"3 running on: "<<this_thread::get_id()<<endl;;
}




int main()
{
   dd::Thread_p<2> th{};

   packaged_task<void()> pt1{f1};
   packaged_task<void()> pt2{f2};
   packaged_task<void()> pt3{f3};
   packaged_task<void()> pt4{f1};
   packaged_task<void()> pt5{f2};
   packaged_task<void()> pt6{f3};


   th.push_task(move(pt1), 0);
   th.push_task(move(pt2), 0);
   th.push_task(move(pt3), 0);
   th.push_task(move(pt4), 0);
   th.push_task(move(pt5), 0);
   th.push_task(move(pt6), 0);


   cout<<"reached!!"<<endl;

   this_thread::sleep_for(10s);
   //th.stop();
   return 0;
}
