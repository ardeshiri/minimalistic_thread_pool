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
   //this_thread::sleep_for(1s);

   cout<<"111111"<<endl;
}

void f2()
{
   //this_thread::sleep_for(3s);

   cout<<"222222"<<endl;
}

void f3()
{
  // this_thread::sleep_for(1s);

   cout<<"333333"<<endl;
}

void f4()
{
   this_thread::sleep_for(5s);

   cout<<"+==============================+"<<endl;
}

void xc(int x)
{
   cout<<x<<endl;
}

int main()
{
   packaged_task<void()> pt1{f1};
   packaged_task<void()> pt2{f2};
   packaged_task<void()> pt3{f3};
   packaged_task<void()> pt4{f4};

   dd::Thread_p<10> th{};
   th.push_task(move(pt1), 14);




   this_thread::sleep_for(10s);

   return 0;
}
