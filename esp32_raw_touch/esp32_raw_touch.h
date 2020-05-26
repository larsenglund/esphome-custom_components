#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

#ifdef ARDUINO_ARCH_ESP32

namespace esphome {
namespace esp32_raw_touch {

class ESP32RawTouchSensor;

class ESP32RawTouchComponent : public sensor::Sensor, public PollingComponent {
 public:
  void register_touch_pad(ESP32RawTouchSensor *pad) { children_.push_back(pad); }
  
  void set_setup_mode(bool setup_mode) { setup_mode_ = setup_mode; }

  void set_iir_filter(uint32_t iir_filter) { iir_filter_ = iir_filter; }

  void set_sleep_duration(uint16_t sleep_duration) { sleep_cycle_ = sleep_duration; }

  void set_measurement_duration(uint16_t meas_cycle) { meas_cycle_ = meas_cycle; }

  void set_low_voltage_reference(touch_low_volt_t low_voltage_reference) {
    low_voltage_reference_ = low_voltage_reference;
  }

  void set_high_voltage_reference(touch_high_volt_t high_voltage_reference) {
    high_voltage_reference_ = high_voltage_reference;
  }

  void set_voltage_attenuation(touch_volt_atten_t voltage_attenuation) { voltage_attenuation_ = voltage_attenuation; }
  
  /// Update touch values.
  void update() override;
  
  /// Setup touch
  void setup() override;
  
  void dump_config() override;
  
  float get_setup_priority() const override { return setup_priority::DATA; }
  
  // void loop() override;
  void on_shutdown() override;

 protected:
 /// Is the IIR filter enabled?
  bool iir_filter_enabled_() const { return iir_filter_ > 0; }

  uint16_t sleep_cycle_{};
  uint16_t meas_cycle_{65535};
  touch_low_volt_t low_voltage_reference_{};
  touch_high_volt_t high_voltage_reference_{};
  touch_volt_atten_t voltage_attenuation_{};
  std::vector<ESP32RawTouchSensor *> children_;
  bool setup_mode_{false};
  uint32_t setup_mode_last_log_print_{};
  uint32_t iir_filter_{0};
  
  touch_pad_t touch_pad_;
  uint16_t value_;
};

/// Simple helper class to expose a touch pad value as a sensor.
class ESP32RawTouchSensor : public sensor::Sensor {
 public:
  ESP32RawTouchSensor(const std::string &name, touch_pad_t touch_pad);

  touch_pad_t get_touch_pad() const { return touch_pad_; }
  uint16_t get_value() const { return value_; }

 protected:
  friend ESP32RawTouchComponent;

  touch_pad_t touch_pad_;
  uint16_t value_;
};

}  // namespace esp32_raw_touch
}  // namespace esphome

#endif