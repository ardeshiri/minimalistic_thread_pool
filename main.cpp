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

   cout<<"1 running on: "<<this_thread::get_id()<<endl;
   this_thread::sleep_for(10ms);
}

void f2()
{
   cout<<"2 running on: "<<this_thread::get_id()<<endl;
   this_thread::sleep_for(30ms);
}

void f3()
{
   cout<<"3 running on: "<<this_thread::get_id()<<endl;
   this_thread::sleep_for(50ms);
}


int main()
{
   dd::Thread_p<3> th{};
    // packaged_task<void()> pt1{f1};
    // th.push_task(move(pt1), 0);
   for(int i = 0 ; i < 100; i++)
   {
      packaged_task<void()> pt2{f1};
      packaged_task<void()> pt3{f2};
      packaged_task<void()> pt5{f3};
     // packaged_task<void()> pt6{f3};

      th.push_task(move(pt2), 10);
      th.push_task(move(pt3), 10);
      th.push_task(move(pt5), 10);
     // if(i == 80) th.stop(2);
     // th.push_task(move(pt6), 10);
   }

   packaged_task<void()> ptx{f1};
   auto du = ptx.get_future();
   th.push_task(move(ptx), 0);
   du.get();
    // this_thread::sleep_for(4s);

   return 0;
}
