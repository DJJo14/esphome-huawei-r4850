#include "huawei_r4850_number.h"
#include "esphome/core/log.h"

namespace esphome {
namespace huawei_r4850 {


void HuaweiR4850Number::control(float value) {
  int32_t raw;
  switch (this->functionCode_) {
  case R48XX_DATA_SET_VOLTAGE:
  case R48XX_DATA_SET_VOLTAGE_DEFAULT:
    raw = 1024.0 * value;
    parent_->set_value_uint32(this->functionCode_, 0, raw);;
    break;
  case R48XX_DATA_SET_CURRENT:
  case R48XX_DATA_SET_CURRENT_DEFAULT:
    raw = R48XX_CURRENT_SCALLER * value;
    parent_->set_value_uint32(this->functionCode_, 0, raw);
    break;

  default:
    break;
  }
}

} // namespace huawei_r4850
} // namespace esphome
