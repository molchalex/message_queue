#pragma once
#include "ThreadObject.h"
#include "IMessageQueueEvents.h"

#include <mutex>

class IPrinter;
class IMessageGetter;
enum class State;

class Reader;
using ReaderPtr = std::shared_ptr<Reader>;

// Class Reader
// gets and prints messages
class Reader final: public IMessageQueueEvents, 
                    public ThreadObject
{
  Reader(IMessageGetter* ipMessageGetter, IPrinter* ipHandleMessage);
public:
  // create method with checking input parameters
  static ReaderPtr Create(IMessageGetter* ipMessageGetter, IPrinter* ipHandleMessage);

private:
  // process message
  void handle_message(const std::string& iMessage) const;

  // thread function
  void run_internal() override;

  // see IMessageQueueEvents
  void on_start() override;
  void on_stop() override;
  void on_lwm() override;
  void on_hwm() override;

private:
  IMessageGetter* _pMessageGetter;
  IPrinter* _pPrinter;

  State _state;
};

