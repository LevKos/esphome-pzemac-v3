#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "../mymodbus/mymodbus.h"

#include <vector>

namespace esphome
{
  namespace pzemacv3
  {

    template <typename... Ts>
    class ResetEnergyAction;

    class PZEMACV3 : public PollingComponent, public mymodbus::MyModbusDevice
    {

    public:
      void set_voltage_sensor(sensor::Sensor *voltage_sensor) { voltage_sensor_ = voltage_sensor; }
      void set_current_sensor(sensor::Sensor *current_sensor) { current_sensor_ = current_sensor; }
      void set_power_sensor(sensor::Sensor *power_sensor) { power_sensor_ = power_sensor; }
      void set_energy_sensor(sensor::Sensor *energy_sensor) { energy_sensor_ = energy_sensor; }
      void set_frequency_sensor(sensor::Sensor *frequency_sensor) { frequency_sensor_ = frequency_sensor; }
      void set_power_factor_sensor(sensor::Sensor *power_factor_sensor) { power_factor_sensor_ = power_factor_sensor; }

      void update() override;

      //      void on_offline();
      void on_modbus_offline() override;

      void on_modbus_data(const std::vector<uint8_t> &data) override;

      void dump_config() override;

    protected:
      template <typename... Ts>
      friend class ResetEnergyAction;
      sensor::Sensor *voltage_sensor_{nullptr};
      sensor::Sensor *current_sensor_{nullptr};
      sensor::Sensor *power_sensor_{nullptr};
      sensor::Sensor *energy_sensor_{nullptr};
      sensor::Sensor *frequency_sensor_{nullptr};
      sensor::Sensor *power_factor_sensor_{nullptr};

      void reset_energy_();
    };

    template <typename... Ts>
    class ResetEnergyAction : public Action<Ts...>
    {
    public:
      ResetEnergyAction(PZEMACV3 *pzemac) : pzemac_(pzemac) {}

      void play(const Ts &...x) override { this->pzemac_->reset_energy_(); }

    protected:
      PZEMACV3 *pzemac_;
    };

  } // namespace pzemacv3
} // namespace esphome
