#pragma once
#include "ThreadObject.h"
#include "IMessageQueueEvents.h"

#include <queue>

class IMessagePutter;
class Writer;
using WriterPtr = std::shared_ptr<Writer>;

enum class State;

// Class Writer
// puts messages to queue
class Writer final: public IMessageQueueEvents,
                    public ThreadObject
{
  Writer(IMessagePutter* ipMessagePutter, const std::queue<std::pair<std::string, int>>& iPriorityMessages);
public:
  // create method with checking input parameters
  static WriterPtr Create(IMessagePutter* ipMessagePutter, const std::queue<std::pair<std::string, int>>& iPriorityMessages);

private:
  // thread function
  void run_internal() override;

  // see IMessageQueueEvents
  void on_start() override;
  void on_stop() override;
  void on_lwm() override;
  void on_hwm() override;

private:
  IMessagePutter* _pMessagePutter = nullptr;
  State _state;

  std::queue<std::pair<std::string, int>> _priorityMessages;
};

