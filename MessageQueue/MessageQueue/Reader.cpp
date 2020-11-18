#include "Reader.h"

#include "IMessageQueue.h"
#include "Printer.h"
#include "State.h"

Reader::Reader(IMessageGetter* ipMessageGetter, IPrinter* ipHandleMessage)
  : _pMessageGetter(ipMessageGetter)
  , _pPrinter(ipHandleMessage)
  , _state(State::STOPPED)
{
  
}

ReaderPtr Reader::Create(IMessageGetter* ipMessageGetter, IPrinter* ipHandleMessage)
{
  if (!ipMessageGetter || !ipHandleMessage)
    return nullptr;

  return ReaderPtr(new Reader(ipMessageGetter, ipHandleMessage));
}

void Reader::handle_message(const std::string& iMessage) const
{
  _pPrinter->print_message(iMessage);
}

void Reader::run_internal()
{
  // check state
  while (_state == State::RUNNING)
  {
    // get message
    std::string message;
    const RetCodes retCode = _pMessageGetter->get(message);

    // print message
    if (retCode == RetCodes::OK)
      handle_message(message);

    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
}

void Reader::on_start()
{
  _state = State::RUNNING;

  // run thread
  run();
}

void Reader::on_stop()
{
  _state = State::STOPPED;
}

void Reader::on_lwm()
{
  // nothing
}

void Reader::on_hwm()
{
  // nothing
}