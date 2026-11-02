#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/pzemac/pzemac.h"

namespace esphome {
namespace pzemac_withoffline {

class PZEMACWithOffline : public pzemac::PZEMAC {
 protected:
  uint32_t last_successful_read_{0};
  bool is_offline_{false};
  // Максимальний час очікування відповіді (наприклад, 10 секунд)
  const uint32_t TIMEOUT_MS = 10000; 

 public:
  // Коли приходять валідні дані від PZEM, оригінальний клас викликає on_modbus_data
  void on_modbus_data(const std::vector<uint8_t> &data) override {
    // Спочатку викликаємо оригінальний обробник, щоб оновити сенсори числами
    pzemac::PZEMAC::on_modbus_data(data);
    
    // Фіксуємо час успішного отримання пакету
    this->last_successful_read_ = millis();
    this->is_offline_ = false;
  }

  // Метод loop() викликається ESPHome на кожній ітерації процесора
  void loop() override {
    pzemac::PZEMAC::loop();

    // Якщо пристрій ще жодного разу не відповів з моменту старту, чекаємо першого запиту
    if (this->last_successful_read_ == 0) {
      this->last_successful_read_ = millis();
      return;
    }

    // Перевіряємо, чи не перевищено таймаут очікування і чи ми ще не в статусі офлайн
    if (!this->is_offline_ && (millis() - this->last_successful_read_ > TIMEOUT_MS)) {
      this->is_offline_ = true;
      
      ESP_LOGW("pzemac_offline", "PZEM04Tv3 не відповідає більше %d мс. Скидання сенсорів в NAN.", TIMEOUT_MS);

      // Примусово відправляємо NAN, щоб Home Assistant розірвав графіки
      if (this->voltage_sensor_ != nullptr) this->voltage_sensor_->publish_state(NAN);
      if (this->current_sensor_ != nullptr) this->current_sensor_->publish_state(NAN);
      if (this->active_power_sensor_ != nullptr) this->active_power_sensor_->publish_state(NAN);
      if (this->frequency_sensor_ != nullptr) this->frequency_sensor_->publish_state(NAN);
      if (this->power_factor_sensor_ != nullptr) this->power_factor_sensor_->publish_state(NAN);
    }
  }
};

}  // namespace pzemac_withoffline
}  // namespace esphome
