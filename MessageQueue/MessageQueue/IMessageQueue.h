#pragma once

#include <string>

class IMessageQueueEvents;

// return codes of message queue
enum class RetCodes
{
  OK = 0,
  HWM = -1,
  NO_SPACE = -2,
  STOPPED = -3
};

// interface class for put message to queue
class IMessagePutter
{
public:
  virtual ~IMessagePutter() = default;

  virtual RetCodes put(const std::string& iMessage, int iPriority) = 0;
};

// interface class for get message from queue
class IMessageGetter
{
public:
  virtual ~IMessageGetter() = default;

  virtual RetCodes get(std::string& oMessage) = 0;
};

// interface class for control message queue
class IMessageQueue
                    
{
public:
  virtual ~IMessageQueue() = default;

  virtual void run() = 0;
  virtual void stop() = 0;
};

using IMessageQueuePtr = std::shared_ptr<IMessageQueue>;