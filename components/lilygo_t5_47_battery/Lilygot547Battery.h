#pragma once
#include <Arduino.h>
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/hal.h"

#include <driver/adc.h>
#include "esp_adc_cal.h"

#ifndef EPD_DRIVER
#define EPD_DRIVER
#include "epd_driver.h"
#endif

namespace esphome {
namespace lilygo_t5_47_battery {

class Lilygot547Battery : public PollingComponent {
 public:
  sensor::Sensor *voltage{nullptr};

  void setup() override;
  void update() override;
  void update_battery_info();
  void correct_adc_reference();

  void set_voltage_sensor(sensor::Sensor *voltage_sensor) { voltage = voltage_sensor; }

 protected:
  esp_adc_cal_characteristics_t cal_characteristics_;
};
}  // namespace lilygo_t5_47_battery
}  // namespace esphome
