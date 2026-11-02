#pragma once

#include "pzemac.h"

namespace esphome {
namespace pzemac_withoffline {

class PZEMACWithOffline : public pzemac::PZEMAC {
 public:
  void timed_out() override {
    ESP_LOGW("pzemac_withoffline", "PZEM04Tv3 знеструмлений (Modbus Timeout)");

    // Скидаємо сенсори, які ми успадкували від оригінального класу
    if (this->voltage_sensor_ != nullptr) this->voltage_sensor_->publish_state(NAN);
    if (this->current_sensor_ != nullptr) this->current_sensor_->publish_state(NAN);
    if (this->active_power_sensor_ != nullptr) this->active_power_sensor_->publish_state(NAN);
    if (this->frequency_sensor_ != nullptr) this->frequency_sensor_->publish_state(NAN);
    if (this->power_factor_sensor_ != nullptr) this->power_factor_sensor_->publish_state(NAN);

    // Обов'язково передаємо таймаут далі в оригінальний Modbus
    this->ModbusDevice::timed_out();
  }
};

}  // namespace pzemac_withoffline
}  // namespace esphome
