#pragma once

// Interface class for queue events
class IMessageQueueEvents
{
public:
  virtual ~IMessageQueueEvents() = default;

  virtual void on_start() = 0;
  virtual void on_stop() = 0;
  virtual void on_lwm() = 0;
  virtual void on_hwm() = 0;
};