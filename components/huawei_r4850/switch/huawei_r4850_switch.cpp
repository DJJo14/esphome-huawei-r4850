#include "huawei_r4850_switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace huawei_r4850 {

static const char *TAG = "huawei_r4850.switch";

// void EmptySwitch::setup() {

// }

void HuaweiR4850Switch::write_state(bool state)
{
    parent_->set_power(state);
}

// void EmptySwitch::dump_config(){
//     ESP_LOGCONFIG(TAG, "Empty custom switch");
// }

} //namespace huawei_r4850
} //namespace esphome