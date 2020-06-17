#include <iostream>
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

   for(int i = 0 ; i < 100; i++)
   {
      packaged_task<void()> pt1{f1};
      packaged_task<void()> pt2{f2};
      packaged_task<void()> pt3{f3};
      th.push_task(move(pt1), 10);
      th.push_task(move(pt2), 10);
      th.push_task(move(pt3), 10);
   }
   packaged_task<void()> ptx{f1};
   auto fut = ptx.get_future();
   th.push_task(move(ptx), 0);
   fut.get();

   return 0;
}
