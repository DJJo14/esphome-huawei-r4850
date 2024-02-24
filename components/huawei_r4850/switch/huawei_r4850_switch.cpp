#include "huawei_r4850_switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace huawei_r4850 {

static const char *TAG = "huawei_r4850.switch";

void HuaweiR4850Switch::write_state(bool state)
{
    parent_->set_power(state);
}


} //namespace huawei_r4850
} //namespace esphome