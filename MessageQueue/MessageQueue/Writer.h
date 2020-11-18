#pragma once
#include "ThreadObject.h"
#include "IMessageQueueEvents.h"

#include <queue>

class IMessagePutter;
class Writer;
using WriterPtr = std::shared_ptr<Writer>;

enum class State;
// Class Writer
class Writer final: public IMessageQueueEvents,
                    public ThreadObject
{
  Writer(IMessagePutter* ipMessagePutter, const std::queue<std::pair<std::string, int>>& iPriorityMessages);
public:
  static WriterPtr Create(IMessagePutter* ipMessagePutter, const std::queue<std::pair<std::string, int>>& iPriorityMessages);

private:
  void run_internal() override;

  void on_start() override;
  void on_stop() override;
  void on_lwm() override;
  void on_hwm() override;

private:
  IMessagePutter* _pMessagePutter = nullptr;
  State _state;

  std::queue<std::pair<std::string, int>> _priorityMessages;
};

