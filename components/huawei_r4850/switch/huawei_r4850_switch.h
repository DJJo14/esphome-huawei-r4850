#pragma once
#include "../huawei_r4850.h"
#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace huawei_r4850 {

class HuaweiR4850Switch : public switch_::Switch, public Component {
 public:
 void set_parent(HuaweiR4850Component *parent) {
    this->parent_ = parent;
  };
  void write_state(bool state) override;

 protected:
  HuaweiR4850Component *parent_;
};

} //namespace huawei_r4850
} //namespace esphome
