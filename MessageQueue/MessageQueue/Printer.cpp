#include "Printer.h"
#include <iostream>
#include <thread>

void Printer::print_message(const std::string& iMessage)
{
  std::lock_guard<std::mutex> lock(_coutMutex);
  std::cout << "message: " << iMessage << std::endl;
}
