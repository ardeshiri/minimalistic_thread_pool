#ifndef Thread_q_H
#define Thread_q_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>
#include <utility>
#include <chrono>
#include <algorithm>
using namespace std::chrono;

namespace dd{

template<typename T>
class Thread_q
{
    public:

        using task = T;

        Thread_q();
        ~Thread_q();

        void enq( task&& );
        void vec_enq( std::vector<task>& );
        void run();
        void stop();
        bool is_busy();

    protected:

    private:
        std::queue<task> task_q;
        std::mutex mtx;
        std::condition_variable cv;
        std::atomic<bool> running;
        std::atomic<bool> busy;
        std::thread th;
        void loop();
};


template<typename T>
Thread_q<T>::Thread_q():task_q{}, mtx{}, cv{}, running{false}, busy{false}, th{}
{
    //ctor
}

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
void Thread_q<T>::enq( task&& t)
{
   std::lock_guard<std::mutex> lg{mtx};
   task_q.push(std::move(t));
   cv.notify_one();
}

template<typename T>
void Thread_q<T>::vec_enq( std::vector<task>& tv)
{
   std::lock_guard<std::mutex> lg{mtx};
   std::for_each( tv.begin(), tv.end(), [this](auto& t){ task_q.push(std::move(t));} );
   cv.notify_one();
}

template<typename T>
void Thread_q<T>::loop()
{
   running = true;
   busy = true;
   std::vector<task> tv{};
   while(running)
   {
      tv.clear();
      {
         busy = false;
         std::unique_lock ul(mtx);
         cv.wait(ul , [this](){ return ((!task_q.empty()) || !running ) ; });
         busy = true;
         while(!task_q.empty())
         {
            tv.push_back(std::move(task_q.front()));
            task_q.pop();
         }
      }
      std::for_each(tv.begin(),tv.end(),[](auto& t){ t(); });
   }
}

template<typename T>
void Thread_q<T>::run()
{
   th = std::thread{&Thread_q::loop, this};
   std::this_thread::sleep_for(1ms);
   cv.notify_one();
}

template<typename T>
void Thread_q<T>::stop()
{
   running = false;
   cv.notify_one();
}

template<typename T>
bool Thread_q<T>::is_busy()
{
   return busy;
}


}


#endif // Thread_q_H
