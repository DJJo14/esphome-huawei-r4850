#include "huawei_r4850.h"
#include "esphome/core/application.h"
#include "esphome/core/base_automation.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace huawei_r4850 {

static const char *const TAG = "huawei_r4850";

static const uint32_t CAN_ID_REQUEST = 0x108040FE;
static const uint32_t CAN_ID_DATA = 0x1081407F;
static const uint32_t CAN_ID_INFO_REQUEST = 0x1080D2FE; //0x108050FE;
static const uint32_t CAN_ID_SET = 0x108180FE;
static const uint32_t CAN_ID_MASK = 0x0000FF00;

static const uint16_t R48xx_DATA_SET_Output_VOLTAGE = 0x0100;

static const uint16_t R48xx_DATA_OPERATION_TIME = 0x010E;
static const uint16_t R48xx_DATA_INPUT_POWER = 0x0170;
static const uint16_t R48xx_DATA_INPUT_FREQ = 0x0171;
static const uint16_t R48xx_DATA_INPUT_CURRENT = 0x0172;
static const uint16_t R48xx_DATA_OUTPUT_POWER = 0x0173;
static const uint16_t R48xx_DATA_EFFICIENCY = 0x0174;
static const uint16_t R48xx_DATA_OUTPUT_VOLTAGE = 0x0175;
static const uint16_t R48xx_DATA_OUTPUT_CURRENT_MAX = 0x0176;
static const uint16_t R48xx_DATA_INPUT_VOLTAGE = 0x0178;
static const uint16_t R48xx_DATA_OUTPUT_TEMPERATURE = 0x017F;
static const uint16_t R48xx_DATA_INPUT_TEMPERATURE = 0x0180;
static const uint16_t R48xx_DATA_OUTPUT_CURRENT = 0x0181;
static const uint16_t R48xx_DATA_OUTPUT_CURRENT1 = 0x0182;
static const uint16_t R48xx_DATA_ALARM_STATE = 0x0183;

HuaweiR4850Component::HuaweiR4850Component(canbus::Canbus *canbus) { this->canbus = canbus; }

void HuaweiR4850Component::setup() {
  Automation<std::vector<uint8_t>, uint32_t, bool> *automation;
  LambdaAction<std::vector<uint8_t>, uint32_t, bool> *lambdaaction;
  canbus::CanbusTrigger *canbus_canbustrigger;

  canbus_canbustrigger = new canbus::CanbusTrigger(this->canbus, 0, 0, true);
  canbus_canbustrigger->set_component_source("canbus");
  App.register_component(canbus_canbustrigger);
  automation = new Automation<std::vector<uint8_t>, uint32_t, bool>(canbus_canbustrigger);
  auto cb = [=](std::vector<uint8_t> x, uint32_t can_id, bool remote_transmission_request) -> void {
    this->on_frame(can_id, remote_transmission_request, x);
  };
  lambdaaction = new LambdaAction<std::vector<uint8_t>, uint32_t, bool>(cb);
  automation->add_actions({lambdaaction});
}

void HuaweiR4850Component::update() {
  ESP_LOGD(TAG, "Sending request message");
  std::vector<uint8_t> data = {0, 0, 0, 0, 0, 0, 0, 0};
  this->canbus->send_data(CAN_ID_REQUEST, true, data);

  // ESP_LOGI(TAG, "voltage %f", this->output_voltage_number_->state);
  // if (this->output_voltage_number_->has_state() == false)
  // {
  //   std::vector<uint8_t> data = {0, 0, 0, 0, 0, 0, 0, 0};
  //   data[0] = (R48xx_DATA_SET_Output_VOLTAGE &0xFF00)>>8;
  //   data[1] = (R48xx_DATA_SET_Output_VOLTAGE &0xFF);
  //   this->canbus->send_data(CAN_ID_INFO_REQUEST, true, data);
  //   ESP_LOGI(TAG, "request voltage");
  // }
  // no new value for 5* intervall -> set sensors to NAN)
  if (millis() - lastUpdate_ > this->update_interval_ * 5)
  {
    this->publish_sensor_state_(this->operation_hours_sensor_, NAN);
    this->publish_sensor_state_(this->input_power_sensor_, NAN);
    this->publish_sensor_state_(this->input_voltage_sensor_, NAN);
    this->publish_sensor_state_(this->input_current_sensor_, NAN);
    this->publish_sensor_state_(this->input_temp_sensor_, NAN);
    this->publish_sensor_state_(this->input_frequency_sensor_, NAN);
    this->publish_sensor_state_(this->output_power_sensor_, NAN);
    this->publish_sensor_state_(this->output_current_sensor_, NAN);
    this->publish_sensor_state_(this->max_output_current_sensor_, NAN);
    this->publish_sensor_state_(this->output_voltage_sensor_, NAN);
    this->publish_sensor_state_(this->output_temp_sensor_, NAN);
    this->publish_sensor_state_(this->efficiency_sensor_, NAN);
    this->publish_number_state_(this->max_output_current_number_, NAN);
    this->publish_sensor_state_(this->alarm_state_sensor_, NAN);
  }
}

void HuaweiR4850Component::set_output_voltage(float value, bool offline) {
  uint8_t functionCode = 0x0;
  if (offline)
    functionCode += 1;
  int32_t raw = 1024.0 * value;
  std::vector<uint8_t> data = {
      0x1, functionCode, 0x0, 0x0, (uint8_t) (raw >> 24), (uint8_t) (raw >> 16), (uint8_t) (raw >> 8), (uint8_t) raw};
  this->canbus->send_data(CAN_ID_SET, true, data);
}

void HuaweiR4850Component::set_max_output_current(float value, bool offline) {
  uint8_t functionCode = 0x3;
  if (offline)
    functionCode += 1;
  int32_t raw = 20.0 * value;
  std::vector<uint8_t> data = {
      0x1, functionCode, 0x0, 0x0, (uint8_t) (raw >> 24), (uint8_t) (raw >> 16), (uint8_t) (raw >> 8), (uint8_t) raw};
  this->canbus->send_data(CAN_ID_SET, true, data);
}

void HuaweiR4850Component::set_offline_values() {
  if (output_voltage_number_) {
    set_output_voltage(output_voltage_number_->state, true);
  };
  if (max_output_current_number_) {
    set_max_output_current(max_output_current_number_->state, true);
  }
}

void HuaweiR4850Component::on_frame(uint32_t can_id, bool rtr, std::vector<uint8_t> &data) {
  uint16_t signal_id = data[1] + ((data[0] & 0xF) << 8);
  if ((can_id & CAN_ID_MASK) == (CAN_ID_DATA & CAN_ID_MASK) ||
      (can_id & CAN_ID_MASK) == (CAN_ID_INFO_REQUEST & CAN_ID_MASK)) 
  {
    uint32_t value = (data[4] << 24) + (data[5] << 16) + (data[6] << 8) + data[7];
    float conv_value = 0;
    
    switch (signal_id) {
      case R48xx_DATA_OPERATION_TIME:
        conv_value = value;
        this->publish_sensor_state_(this->operation_hours_sensor_, conv_value);
        ESP_LOGI(TAG, "Operation time: %f hour", conv_value);
        break;
      
      case R48xx_DATA_INPUT_POWER:
        conv_value = value / 1024.0;
        this->publish_sensor_state_(this->input_power_sensor_, conv_value);
        ESP_LOGI(TAG, "Input power: %f", conv_value);
        break;

      case R48xx_DATA_INPUT_FREQ:
        conv_value = value / 1024.0;
        this->publish_sensor_state_(this->input_frequency_sensor_, conv_value);
        ESP_LOGI(TAG, "Input frequency: %f", conv_value);
        break;

      case R48xx_DATA_INPUT_CURRENT:
        conv_value = value / 1024.0;
        this->publish_sensor_state_(this->input_current_sensor_, conv_value);
        ESP_LOGI(TAG, "Input current: %f", conv_value);
        break;

      case R48xx_DATA_OUTPUT_POWER:
        conv_value = value / 1024.0;
        this->publish_sensor_state_(this->output_power_sensor_, conv_value);
        ESP_LOGI(TAG, "Output power: %f", conv_value);
        break;

      case R48xx_DATA_EFFICIENCY:
        conv_value = value / 1024.0 * 100;
        this->publish_sensor_state_(this->efficiency_sensor_, conv_value);
        ESP_LOGI(TAG, "Efficiency: %f", conv_value);
        break;

      case R48xx_DATA_OUTPUT_VOLTAGE:
        conv_value = value / 1024.0;
        this->publish_sensor_state_(this->output_voltage_sensor_, conv_value);
        ESP_LOGI(TAG, "Output voltage: %f", conv_value);
        break;

      case R48xx_DATA_OUTPUT_CURRENT_MAX:
        conv_value = value / 20.0;
        this->publish_number_state_(this->max_output_current_number_, conv_value);
        this->publish_sensor_state_(this->max_output_current_sensor_, conv_value);
        ESP_LOGI(TAG, "Max Output current: %f", conv_value);
        break;

      case R48xx_DATA_INPUT_VOLTAGE:
        conv_value = value / 1024.0;
        this->publish_sensor_state_(this->input_voltage_sensor_, conv_value);
        ESP_LOGI(TAG, "Input voltage: %f", conv_value);
        break;

      case R48xx_DATA_OUTPUT_TEMPERATURE:
        conv_value = value / 1024.0;
        this->publish_sensor_state_(this->output_temp_sensor_, conv_value);
        ESP_LOGI(TAG, "Output temperature: %f", conv_value);
        break;

      case R48xx_DATA_INPUT_TEMPERATURE:
        conv_value = value / 1024.0;
        this->publish_sensor_state_(this->input_temp_sensor_, conv_value);
        ESP_LOGI(TAG, "Input temperature: %f", conv_value);
        break;

      case R48xx_DATA_OUTPUT_CURRENT1:
        // printf("Output Current(1) %.02fA\r\n", value / 1024.0);
        // output_current = value / 1024.0;
        break;

      case R48xx_DATA_OUTPUT_CURRENT:
        conv_value = value / 1024.0;
        this->publish_sensor_state_(this->output_current_sensor_, conv_value);
        ESP_LOGI(TAG, "Output current: %f", conv_value);
        break;

      case R48xx_DATA_ALARM_STATE:
        conv_value = value;
        this->publish_sensor_state_(this->alarm_state_sensor_, conv_value);
        ESP_LOGI(TAG, "Alarm state: %08X", value);

        // if (this->output_voltage_number_->has_state() == false)
        {
          static uint16_t req_val = 0x1000;
          req_val++;
          std::vector<uint8_t> send_data = {(uint8_t)(req_val>>8), (uint8_t)(req_val&0xFF), 0, 0, 0, 0, 0, 0};
          this->canbus->send_data(CAN_ID_INFO_REQUEST, true, send_data);
          ESP_LOGI(TAG, "request  %02X %02X", send_data[0], send_data[1]);
        }
        // this usually is the last message
        break;

      default:
        ESP_LOGI(TAG, "Unknown parameter 0x%8X 0x%04X 0x%02X 0x%02X 0x%08X", can_id, signal_id, data[2], data[3], value);
        break;
    }
    this->lastUpdate_ = millis();
  }
  else
  {
    ESP_LOGI(TAG, "Unknown ID 0x%8X, 0x%04X %02X %02X %02X %02X %02X %02X", can_id, signal_id, data[2], data[3], data[4], data[5], data[6], data[7]);
  }
}

void HuaweiR4850Component::publish_sensor_state_(sensor::Sensor *sensor, float value) {
  if (sensor) {
    sensor->publish_state(value);
  }
}

void HuaweiR4850Component::publish_number_state_(number::Number *number, float value) {
  if (number) {
    number->publish_state(value);
  }
}

}  // namespace huawei_r4850
}  // namespace esphome
