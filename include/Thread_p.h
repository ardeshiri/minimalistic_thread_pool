#ifndef THREAD_P_H
#define THREAD_P_H
#include "Thread_q.h"
#include <array>

namespace dd{

template<typename T>
struct task_pair_comp
{
   bool operator()( const T& lv, const T& rv )
   {
      return ( lv.first < rv.first );
   }
};

template<size_t N, typename TSK = std::packaged_task<void()>>
class Thread_p
{
   public:

      using task = TSK;
      using pr = std::pair<size_t, task>;

      Thread_p();
      ~Thread_p();
      void push_task(task&& tsk, size_t prio);
      void stop();
      void stop(size_t th_num);

      template<typename T>
      void enq_to(T*);

   protected:
   private:
   std::condition_variable thread_p_cv;
   static constexpr size_t max_threads = N;
   std::array<dd::Thread_q<Thread_p<N>>,N> thread_array;
   std::vector<pr> pair_vec;
   std::mutex mtx_over_vec;
   std::atomic<bool> running;
};

template<size_t N,typename TSK>
void Thread_p<N,TSK>::stop(size_t th_num)
{
   if( th_num <  max_threads)
      thread_array[th_num].stop();
}

template<size_t N,typename TSK>
Thread_p<N,TSK>::Thread_p():thread_p_cv{},thread_array{},pair_vec{},mtx_over_vec{},running{false}
{
   std::for_each( thread_array.begin(), thread_array.end(), [this](auto& th){ th.initialize(this); } );
   std::for_each( thread_array.begin(), thread_array.end(), [](auto& th){ th.run(); } );
   running = true;
}


template<size_t N,typename TSK>
Thread_p<N,TSK>::~Thread_p()
{
   stop();
}


template<size_t N,typename TSK>
void Thread_p<N,TSK>::push_task(task&& tsk, size_t prio)
{
   prio = (prio > 10) ? 10 : prio;
   std::lock_guard<std::mutex> lg{mtx_over_vec};
   {
      std::for_each( pair_vec.begin(), pair_vec.end(), [](auto& p){ p.first += 1; } );
      pair_vec.emplace_back(prio, std::move(tsk));
      std::push_heap( pair_vec.begin(), pair_vec.end(), dd::task_pair_comp<pr>{} );
   }
   thread_p_cv.notify_one();
}


template<size_t N,typename TSK>
void Thread_p<N,TSK>::stop()
{
   std::for_each( thread_array.begin(), thread_array.end(), [](auto& th){ th.stop(); } );
   running = false;
   thread_p_cv.notify_all();
}


template<size_t N,typename TSK>
template<typename T>
void Thread_p<N,TSK>::enq_to(T* th_q)
{
   std::unique_lock<std::mutex> ul{mtx_over_vec};
   thread_p_cv.wait(ul, [this](){return !pair_vec.empty() || !running;});
   if(!running)
   {
      return;
   }
   std::pop_heap( pair_vec.begin(), pair_vec.end(), dd::task_pair_comp<pr>{} );
   th_q->enq(std::move(pair_vec.back().second));
   pair_vec.pop_back();
}



}

#endif // THREAD_P_H
