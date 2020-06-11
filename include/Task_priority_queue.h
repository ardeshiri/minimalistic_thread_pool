#ifndef TASK_PRIORITY_QUEUE_H
#define TASK_PRIORITY_QUEUE_H
#include <queue>


template<typename T, typename C, typename A>
class Task_priority_queue:public std::priority_queue<T,C,A>
{
   public:
      Task_priority_queue();
      virtual ~Task_priority_queue();

      template<typename TS>
      void insert_task(TS&& tsk)
      {
         this.
      }

   protected:

   private:
};

#endif // TASK_PRIORITY_QUEUE_H
