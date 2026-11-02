#include "pzemac.h"
#include "esphome/core/log.h"

namespace esphome::pzemac
{
  void PZEMAC::timed_out()
  {
    // Логуємо таймаут для діагностики
    ESP_LOGW("pzemacv3", "PZEM04Tv3 знеструмлений або відсутній зв'язок (Modbus Timeout)");

    // Скидаємо сенсори в NAN, щоб очистити графіки в Home Assistant
    if (this->voltage_sensor_ != nullptr)
      this->voltage_sensor_->publish_state(NAN);
    if (this->current_sensor_ != nullptr)
      this->current_sensor_->publish_state(NAN);
    if (this->active_power_sensor_ != nullptr)
      this->active_power_sensor_->publish_state(NAN);
    if (this->frequency_sensor_ != nullptr)
      this->frequency_sensor_->publish_state(NAN);
    if (this->power_factor_sensor_ != nullptr)
      this->power_factor_sensor_->publish_state(NAN);

    // Обов'язково викликаємо базовий метод батьківського класу ModbusDevice
    this->ModbusDevice::timed_out();
  }

} // namespace esphome::pzemac
