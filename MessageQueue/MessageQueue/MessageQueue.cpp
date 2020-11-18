#include "MessageQueue.h"

#include "IMessageQueueEvents.h"
#include "State.h"

// private message class for priority queue data
// contains message(std::string) and priority(int)
class Message
{
public:
  Message(const std::string& iMessage, int iPriority)
    : _message(iMessage)
    , _priority(iPriority)
  {}

  const std::string& str() const
  {
    return _message;
  }

  bool operator<(const Message& iOther) const
  {
    return _priority < iOther._priority;
  }
  
  bool operator>(const Message& iOther) const
  {
    return _priority > iOther._priority;
  }

private:
  std::string _message;
  int _priority;
};

MessageQueue::MessageQueue(unsigned iQueueMaxSize, unsigned iHwm, unsigned iLwm)
  : _queueMaxSize(iQueueMaxSize)
  , _hwmValue(iHwm)
  , _lwmValue(iLwm)
  , _state(State::STOPPED)
{
  
}

MessageQueue::~MessageQueue()
{
  stop();
}

MessageQueuePtr MessageQueue::Create(unsigned iQueueMaxSize, unsigned iHwm, unsigned iLwm)
{
  // check input parameters
  if (iHwm > iQueueMaxSize ||
      iLwm > iQueueMaxSize || 
      iHwm < iLwm)
    return nullptr;

  return MessageQueuePtr(new MessageQueue(iQueueMaxSize, iHwm, iLwm));
}

RetCodes MessageQueue::put(const std::string& iMessage, int iPriority)
{
  Locker lock(_rmutex);

  if (_state == State::STOPPED)
    return RetCodes::STOPPED;

  if (_messages.size() >= _hwmValue)
  {
    for (auto& pListener : _listeners)
      pListener->on_hwm();

    return _messages.size() == _queueMaxSize ? RetCodes::NO_SPACE : RetCodes::HWM;
  }

  _messages.push( { iMessage, iPriority } );
  if (!_messages.empty())
    _cvBlockingGet.notify_one();
  
  return RetCodes::OK;
}

RetCodes MessageQueue::get(std::string& oMessage)
{
  Locker lock(_rmutex);

  if (_state == State::STOPPED)
    return RetCodes::STOPPED;

  while (_messages.empty())
  {
    _cvBlockingGet.wait(lock);

    if (_state == State::STOPPED)
      return RetCodes::STOPPED;
  }

  oMessage = _messages.top().str();
  _messages.pop();

  if (_messages.size() <= _lwmValue)
    for (auto& pListener : _listeners)
      pListener->on_lwm();

  return RetCodes::OK;
}

void MessageQueue::stop()
{
  if (_state == State::STOPPED)
    return;

  _state = State::STOPPED;

  for (auto& pListener : _listeners)
    pListener->on_stop();

  _cvBlockingGet.notify_all();
}

void MessageQueue::run()
{
  if (_state == State::RUNNING)
    return;

  _state = State::RUNNING;

  for (auto& pListener : _listeners)
    pListener->on_start();
}

void MessageQueue::addListener(IMessageQueueEvents* ipEvents)
{
  if (ipEvents)
    _listeners.push_back(ipEvents);
}
