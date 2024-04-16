#include "huawei_r4850_switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace huawei_r4850 {

static const char *TAG = "huawei_r4850.switch";

void HuaweiR4850Switch::write_state(bool state)
{
  int32_t raw = 0;
  switch (this->functionCode_) {
  case R48XX_DATA_SET_INPUT_AC_CURRENT:
    raw = parent_->get_input_currentlimit_number();
    parent_->set_value_uint32(this->functionCode_, state, raw);
    break;
  case R48xx_DATA_POWER_STATE:
    parent_->set_value_uint32(this->functionCode_, state, raw);
    // parent_->set_power(state);
    break;

  default:
    break;
  }
}


} //namespace huawei_r4850
} //namespace esphome