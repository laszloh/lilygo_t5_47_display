#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/version.h"
#include "esphome/components/display/display_buffer.h"

#include "epd_driver.h"

namespace esphome {
namespace t547 {

#if ESPHOME_VERSION_CODE >= VERSION_CODE(2023, 12, 0)
class T547 : public display::DisplayBuffer {
#else
class T547 : public PollingComponent, public display::DisplayBuffer {
#endif  // VERSION_CODE(2023, 12, 0)
 public:
  void set_greyscale(bool greyscale) { this->greyscale_ = greyscale; }
  void set_cycles(size_t cycles) { this->cycles_ = cycles; }
  void set_cleantime(size_t cleantime) {this->cleantime_ = cleantime; }

  float get_setup_priority() const override;

  void dump_config() override;

  void display();
  void clean();
  void update() override;

  void setup() override;

  uint8_t get_panel_state() const { return this->panel_on_; }
  bool get_greyscale() const { return this->greyscale_; }
  size_t get_cycles() const { return this->cycles_; }
  size_t get_cleantime() const { return this->cleantime_; }

#if ESPHOME_VERSION_CODE >= VERSION_CODE(2022,6,0)
  display::DisplayType get_display_type() override {
    return get_greyscale() ? display::DisplayType::DISPLAY_TYPE_GRAYSCALE : display::DisplayType::DISPLAY_TYPE_BINARY;
  }
#endif

 protected:
  void draw_absolute_pixel_internal(int x, int y, Color color) override;

  void eink_off_();
  void eink_on_();


  int get_width_internal() override { return 960; }

  int get_height_internal() override { return 540; }

  size_t get_buffer_length_();


  uint8_t panel_on_ = 0;
  uint8_t temperature_;

  bool greyscale_;
  size_t cycles_;
  size_t cleantime_;

};

}  // namespace T547
}  // namespace esphome
