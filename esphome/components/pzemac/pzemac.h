#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/modbus/modbus.h"

#include <span>

namespace esphome::pzemac
{

  class PZEMAC final : public PollingComponent, public modbus::ModbusClientDevice
  {
  public:
    void timed_out() override;
  };

} // namespace esphome::pzemac
