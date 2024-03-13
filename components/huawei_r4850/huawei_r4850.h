#pragma once

#include "esphome/components/canbus/canbus.h"
#include "esphome/components/number/number.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/switch/switch.h"
#include "esphome/core/component.h"


#define R48XX_CURRENT_SCALLER 20.0


#define R48XX_DATA_SET_VOLTAGE (0x0100)
#define R48XX_DATA_SET_VOLTAGE_DEFAULT (0x0101)
#define R48XX_DATA_SET_OVERVOLTAGE_PROTECT (0x0102)
#define R48XX_DATA_SET_CURRENT (0x0103)
#define R48XX_DATA_SET_CURRENT_DEFAULT (0x0104)
#define R48XX_DATA_SET_INPUT_AC_CURRENT (0x0109)
#define R48xx_DATA_POWER_STATE (0x0132)

namespace esphome {
namespace huawei_r4850 {

class HuaweiR4850Component : public PollingComponent {
public:
  HuaweiR4850Component(canbus::Canbus *canbus);
  void setup() override;
  void update() override;

  void set_output_voltage(float value, bool offline = false);
  void set_max_output_current(float value, bool offline = false);
  void set_power(bool value);
  void set_offline_values();
  void set_value_uint32(uint16_t functioncode, bool enable,
                                            uint32_t value);

  void set_operation_hours_sensor(sensor::Sensor *operation_hours_sensor) {
    operation_hours_sensor_ = operation_hours_sensor;
  }
  void set_input_voltage_sensor(sensor::Sensor *input_voltage_sensor) {
    input_voltage_sensor_ = input_voltage_sensor;
  }
  void set_input_frequency_sensor(sensor::Sensor *input_frequency_sensor) {
    input_frequency_sensor_ = input_frequency_sensor;
  }
  void set_input_current_sensor(sensor::Sensor *input_current_sensor) {
    input_current_sensor_ = input_current_sensor;
  }
  void set_input_power_sensor(sensor::Sensor *input_power_sensor) {
    input_power_sensor_ = input_power_sensor;
  }
  void set_input_temp_sensor(sensor::Sensor *input_temp_sensor) {
    input_temp_sensor_ = input_temp_sensor;
  }
  void set_efficiency_sensor(sensor::Sensor *efficiency_sensor) {
    efficiency_sensor_ = efficiency_sensor;
  }
  void set_output_voltage_sensor(sensor::Sensor *output_voltage_sensor) {
    output_voltage_sensor_ = output_voltage_sensor;
  }
  void set_output_current_sensor(sensor::Sensor *output_current_sensor) {
    output_current_sensor_ = output_current_sensor;
  }
  void
  set_max_output_current_sensor(sensor::Sensor *max_output_current_sensor) {
    max_output_current_sensor_ = max_output_current_sensor;
  }
  void set_output_power_sensor(sensor::Sensor *output_power_sensor) {
    output_power_sensor_ = output_power_sensor;
  }
  void set_output_temp_sensor(sensor::Sensor *output_temp_sensor) {
    output_temp_sensor_ = output_temp_sensor;
  }
  void set_alarm_state_sensor(sensor::Sensor *alarm_state_sensor) {
    alarm_state_sensor_ = alarm_state_sensor;
  }

  void set_output_voltage_number(number::Number *output_voltage_number) {
    output_voltage_number_ = output_voltage_number;
  }
  void set_output_voltage_default_number(number::Number *output_voltage_default_number) {
    output_voltage_default_number_ = output_voltage_default_number;
  }
  void
  set_output_current_number(number::Number *output_current_number) {
    output_current_number_ = output_current_number;
  }
  void set_output_current_default_number(number::Number *output_current_default_number) {
    output_current_default_number_ = output_current_default_number;
  }

  void set_power_switch(switch_::Switch *power_switch) {
    power_switch_ = power_switch;
  }

protected:
  canbus::Canbus *canbus;
  uint32_t lastUpdate_;

  sensor::Sensor *operation_hours_sensor_{nullptr};
  sensor::Sensor *input_voltage_sensor_{nullptr};
  sensor::Sensor *input_frequency_sensor_{nullptr};
  sensor::Sensor *input_current_sensor_{nullptr};
  sensor::Sensor *input_power_sensor_{nullptr};
  sensor::Sensor *input_temp_sensor_{nullptr};
  sensor::Sensor *efficiency_sensor_{nullptr};
  sensor::Sensor *output_voltage_sensor_{nullptr};
  sensor::Sensor *output_current_sensor_{nullptr};
  sensor::Sensor *max_output_current_sensor_{nullptr};
  sensor::Sensor *output_power_sensor_{nullptr};
  sensor::Sensor *output_temp_sensor_{nullptr};
  sensor::Sensor *alarm_state_sensor_{nullptr};

  number::Number *output_voltage_number_{nullptr};
  number::Number *output_voltage_default_number_{nullptr};
  number::Number *output_current_number_{nullptr};
  number::Number *output_current_default_number_{nullptr};
  

  switch_::Switch *power_switch_{nullptr};

  void on_frame(uint32_t can_id, bool rtr, std::vector<uint8_t> &data);

  void publish_sensor_state_(sensor::Sensor *sensor, float value);
  void publish_number_state_(number::Number *number, float value);
  void publish_switch_state_(switch_::Switch *pswitch, const bool &state);
};

} // namespace huawei_r4850
} // namespace esphome
