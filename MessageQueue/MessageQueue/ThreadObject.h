#pragma once

#include <thread>

// base class for thread object
class ThreadObject
{
public:
  virtual ~ThreadObject();

protected:
  void run();

  virtual void run_internal() = 0;

private:
  std::unique_ptr<std::thread> _pThread;
};