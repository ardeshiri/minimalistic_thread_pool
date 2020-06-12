#ifndef Thread_q_H
#define Thread_q_H

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>
#include <utility>
#include <chrono>
#include <algorithm>
#include "Error.h"

using namespace std::chrono;

namespace dd{

template<typename T>
class Thread_q
{
    public:

        using pool = T;
        using task_t = typename pool::task;

        Thread_q();
        Thread_q(const Thread_q&) = delete;
        Thread_q& operator=(const Thread_q&) = delete;
        Thread_q(Thread_q&&) = delete;
        Thread_q& operator=(Thread_q&&) = delete;
        ~Thread_q();

        void enq( task_t&& );
        void vec_enq( std::vector<task_t>& );
        void run();
        void stop();
        void initialize(pool*);
        size_t qsize();

    protected:
    private:
        std::vector<task_t> task_q;
        pool* thread_p_ptr;
        std::atomic<bool> running;
        std::thread th;
        void loop();
};


template<typename T>
Thread_q<T>::Thread_q():task_q{}, thread_p_ptr{nullptr}, running{false}, th{}
{}


template<typename T>
Thread_q<T>::~Thread_q()
{
   stop();
   if(th.joinable())
   {
      th.join();
   }
}


template<typename T>
size_t Thread_q<T>::qsize()
{
   return task_q.size();
}


template<typename T>
void Thread_q<T>::enq( task_t&& t)
{
   task_q.push_back(std::move(t));
}


template<typename T>
void Thread_q<T>::vec_enq( std::vector<task_t>& tv)
{
   std::move(tv.begin(), tv.end(),std::back_inserter(task_q));
}


template<typename T>
void Thread_q<T>::loop()
{
   running = true;
   while(running)
   {
      if(task_q.empty())
      {
         thread_p_ptr->enq_to(this);
      }
      std::for_each(task_q.begin(),task_q.end(),[](auto& t){ t(); });
      task_q.clear();
      if(!running)
      {
         return;
      }
   }
}

template<typename T>
void Thread_q<T>::run()
{
   if(thread_p_ptr == nullptr)
   {
      throw Error{"uninitialized thread_p_ptr."};
   }
   th = std::thread{&Thread_q::loop, this};
}

template<typename T>
void Thread_q<T>::stop()
{
   running = false;
}

template<typename T>
void Thread_q<T>::initialize(T* t_ptr)
{
   thread_p_ptr = t_ptr;
}

}


#endif // Thread_q_H
