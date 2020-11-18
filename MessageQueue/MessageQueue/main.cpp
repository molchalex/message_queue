#include "MessageQueue.h"
#include "Writer.h"
#include "Reader.h"
#include "Printer.h"
#include <iostream>

// generate random messages with random priorities
std::queue<std::pair<std::string, int>> GenerateMessages()
{
  const int count = rand() % int(100);

  std::queue<std::pair<std::string, int>> res;
  for (int i = 0; i < count; i++)
  {
    int priority = rand() % int(1000);
    std::string message = std::to_string(priority);
    res.push(std::pair<std::string, int>(message, priority));
  }
  return res;
}

// generate readers
std::vector<ReaderPtr> CreateReaders(IMessageGetter* ipMessageGetter, IPrinter* ipHandleMessage)
{
  std::vector<ReaderPtr> readers;
  const int count = rand() % int(10);
  for (int i = 0; i < count; i++)
  {
    auto pReader = Reader::Create(ipMessageGetter, ipHandleMessage);
    if (pReader)
      readers.push_back(pReader);
  }
  return readers;
}

// generate writers
std::vector<WriterPtr> CreateWriters(IMessagePutter* ipMessagePutter)
{
  std::vector<WriterPtr> writers;
  const int count = rand() % int(10);
  for (int i = 0; i < count; i++)
  {
    auto pWriter = Writer::Create(ipMessagePutter, GenerateMessages());
    if (pWriter)
      writers.push_back(pWriter);
  }
  return writers;
}

class MessageQueueWrapper: public IMessageQueue
{
public:
  bool init(unsigned iQueueMaxSize, unsigned iHwm, unsigned iLwm)
  {
    clear();

    _pMessageQueue = MessageQueue::Create(iQueueMaxSize, iHwm, iLwm);
    if (!_pMessageQueue)
      return false;

    // create vector of writers
    _writers = CreateWriters(_pMessageQueue.get());

    // add writer listeners
    for (auto& pWriter : _writers)
      _pMessageQueue->addListener(pWriter.get());

    // create vector of readers
    _readers = CreateReaders(_pMessageQueue.get(), &_printer);

    // add reader listeners
    for (auto& pReader : _readers)
      _pMessageQueue->addListener(pReader.get());

    return true;
  }

  void run() override
  {
    if (_pMessageQueue)
      _pMessageQueue->run();
  }
  
  void stop() override
  {
    if (_pMessageQueue)
      _pMessageQueue->stop();
  }

private:
  void clear()
  {
    _readers.clear();
    _writers.clear();

    _pMessageQueue.reset();
  }

  MessageQueuePtr _pMessageQueue;
  std::vector<WriterPtr> _writers;
  std::vector<ReaderPtr> _readers;

  Printer _printer;
};


int main()
{
  MessageQueueWrapper messageQueue;

  for (int i = 0; i < 10; i++)
  {
    const int maxSize = 100;
    const int hwm = rand() % int(100);
    const int lwm = rand() % int(100);

    std::cout << "Message queue init: " << maxSize << " " << hwm << " " << lwm << std::endl;

    if (!messageQueue.init(maxSize, hwm, lwm))
    {
      std::cout << "Error: invalid input parameters!" << std::endl;
      continue;
    }

    std::cout << "Message queue start: " << std::endl;
    messageQueue.run();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Message queue stop." << std::endl;
    messageQueue.stop();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}