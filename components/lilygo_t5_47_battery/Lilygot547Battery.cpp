#include "Lilygot547Battery.h"
#include "esphome/core/log.h"
#include "epd_driver.h"

#define TAG "Lilygot547Battery"


namespace esphome {
namespace lilygo_t5_47_battery {

void Lilygot547Battery::setup() {
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
  correct_adc_reference();
}

void Lilygot547Battery::update() {
  epd_poweron();
  // wait for voltage to stabilise
  delay(100);
  this->update_battery_info();
  epd_poweroff();
}

void Lilygot547Battery::update_battery_info() {
  static constexpr size_t samples = 8;
  uint32_t aggregator = 0;

  for(uint8_t i = 0; i < samples; i++) {
    int raw = adc1_get_raw(ADC1_CHANNEL_0);   // GPIO36
    if(raw == -1) {
      ESP_LOGE(TAG, "ADC measurement failed!");
      if (this->voltage != nullptr)
        this->voltage->publish_state(NAN);
      return;
    }
    aggregator += raw;
  }
  uint32_t mv = esp_adc_cal_raw_to_voltage(aggregator / samples, &this->cal_characteristics_);

  float battery_voltage = mv * 2.0f / 1000.0f;
  if (this->voltage != nullptr)
    this->voltage->publish_state(battery_voltage);
}

void Lilygot547Battery::correct_adc_reference() {
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &this->cal_characteristics_);
  switch (val_type) {
    case ESP_ADC_CAL_VAL_EFUSE_VREF:
      ESP_LOGV(TAG, "Using eFuse Vref for calibration");
      break;
    case ESP_ADC_CAL_VAL_EFUSE_TP:
      ESP_LOGV(TAG, "Using two-point eFuse Vref for calibration");
      break;
    case ESP_ADC_CAL_VAL_DEFAULT_VREF:
    default:
      break;
  }
}

}  // namespace lilygo_t5_47_battery
}  // namespace esphome
