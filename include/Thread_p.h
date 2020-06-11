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

template<size_t N>
class Thread_p
{
   public:

      using task = std::packaged_task<void()>;
      using pr = std::pair<size_t, task>;

      Thread_p():th_array{},pr_vec{}
      {
         std::for_each( th_array.begin(), th_array.end(), [](auto& th){ th.run(); } );
      }
      ~Thread_p()
      {
         std::for_each( th_array.begin(), th_array.end(), [](auto& th){ th.stop(); } );
      }

      void push_task(task&& tsk, size_t prio)
      {
         pr_vec.emplace_back(prio, std::move(tsk));
         std::push_heap( pr_vec.begin(), pr_vec.end(), dd::task_pair_comp<pr>{} );
      }


   protected:

   private:
   static constexpr size_t max_threads = N;
   std::array<dd::Thread_q<task>,N> th_array;
   std::vector<pr> pr_vec;
};

}

#endif // THREAD_P_H
