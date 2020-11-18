#pragma once
#include <string>
#include <mutex>

// Interface class for printer
class IPrinter
{
public:
  virtual ~IPrinter() = default;

  virtual void print_message(const std::string& iMessage) = 0;
};

// Class Printer prints message to console
class Printer final: public IPrinter
{
public:
  void print_message(const std::string& iMessage) override;

private:
  std::mutex _coutMutex;
};

