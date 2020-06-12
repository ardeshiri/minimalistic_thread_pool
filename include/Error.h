#ifndef ERROR_H
#define ERROR_H
#include <iostream>
#include <time.h>
#include <stdio.h>

using std::string;
namespace dd{

    class Error
    {
        public:
            Error(string comment="", int mo=0, int smo=0, int errn=0, int prio=0);
            virtual ~Error();
            void what() const noexcept;

        protected:

        private:
            int module;
            int submodule;
            int errornumber;
            int priority;
            time_t ntime;
            string comment;
            string stime;
    };
}
#endif // ERROR_H
