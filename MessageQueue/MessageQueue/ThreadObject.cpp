#include "ThreadObject.h"

ThreadObject::~ThreadObject()
{
  if (_pThread)
    _pThread->join();

  _pThread.reset();
}

void ThreadObject::run()
{
  if (_pThread)
    _pThread->join();

  _pThread.reset(new std::thread(&ThreadObject::run_internal, this));
}