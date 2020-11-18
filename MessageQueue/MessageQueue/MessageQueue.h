#pragma once

#include "IMessageQueue.h"

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

class Message;
class MessageQueue;
using MessageQueuePtr = std::shared_ptr<MessageQueue>;

enum class State;

// Class MessageQueue
//  - creates priority queue
//  - blocks 'get' method for async threads
//  - supports LWM/HWM mechanism
class MessageQueue final: public IMessageQueue,
                          public IMessagePutter,
                          public IMessageGetter
{
  // constructor by max queue size & high/low water mark values
  MessageQueue(unsigned iQueueMaxSize, unsigned iHwm, unsigned iLwm);
public:
  // destructor
  virtual ~MessageQueue();

  static MessageQueuePtr Create(unsigned iQueueMaxSize, unsigned iHwm, unsigned iLwm);

  // put message, see IMessagePutter
  RetCodes put(const std::string& iMessage, int iPriority) override;

  // get message, see IMessageGetter
  RetCodes get(std::string& oMessage) override;

  // run queue
  void run() override;

  // stop queue
  void stop() override;

  // add one more queue listener
  void addListener(IMessageQueueEvents* ipEvents);

private:
  unsigned _queueMaxSize = 0;
  unsigned _hwmValue = 0;
  unsigned _lwmValue = 0;

  State _state;

  std::vector<IMessageQueueEvents*> _listeners;

  std::priority_queue<Message> _messages;

  mutable std::recursive_mutex _rmutex;
  std::condition_variable_any _cvBlockingGet;

  using Locker = std::unique_lock<std::recursive_mutex>;
};

