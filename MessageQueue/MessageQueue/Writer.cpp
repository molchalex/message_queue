#include "Writer.h"

#include "IMessageQueue.h"
#include "State.h"

Writer::Writer(IMessagePutter* ipMessagePutter, const std::queue<std::pair<std::string, int>>& iPriorityMessages)
  : _pMessagePutter(ipMessagePutter)
  , _priorityMessages(iPriorityMessages)
  , _state(State::STOPPED)
{
  
}

WriterPtr Writer::Create(IMessagePutter* ipMessagePutter, const std::queue<std::pair<std::string, int>>& iPriorityMessages)
{
  // check input
  if (!ipMessagePutter ||
    iPriorityMessages.empty())
    return nullptr;

  return WriterPtr(new Writer(ipMessagePutter, iPriorityMessages));
}

void Writer::run_internal()
{
  // thread works while input message is not empty
  while(!_priorityMessages.empty())
  {
    if (_state != State::RUNNING)
      continue;

    auto message = _priorityMessages.front();

    // try to put message to queue
    const RetCodes retCode = _pMessagePutter->put(message.first, message.second);
    if (retCode == RetCodes::OK)
      _priorityMessages.pop();

    std::this_thread::sleep_for(std::chrono::milliseconds(_delay));
  }
}

void Writer::on_start()
{
  _state = State::RUNNING;

  // run thread
  run();
}

void Writer::on_stop()
{
  _state = State::STOPPED;
}

void Writer::on_lwm()
{
  _state = State::RUNNING;
}

void Writer::on_hwm()
{
  _state = State::STOPPED;
}