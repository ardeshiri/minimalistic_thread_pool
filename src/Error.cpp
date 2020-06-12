#include "Error.h"

dd::Error::Error( string cmnt, int mo, int smo, int errn, int prio):module(mo),submodule(smo),errornumber(errn),priority(prio),ntime{},comment(cmnt),stime{}
{
    time(&ntime);
    stime = ctime(&ntime);
}

dd::Error::~Error()
{
    //dtor
}

void dd::Error::what() const noexcept
{
    std::cerr<<"error occured at: "<<stime<<std::endl;
    std::cerr<<comment<<std::endl;
}
