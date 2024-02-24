import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID

from .. import HuaweiR4850Component, huawei_r4850_ns, CONF_HUAWEI_R4850_ID

HuaweiR4850Switch = huawei_r4850_ns.class_(
    "HuaweiR4850Switch", switch.Switch, cg.Component
)

CONF_POWER = "power"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_HUAWEI_R4850_ID): cv.use_id(HuaweiR4850Component),
        cv.Optional(CONF_POWER): switch.SWITCH_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(HuaweiR4850Switch)
            }
        ).extend(cv.COMPONENT_SCHEMA),
    }
)

async def to_code(config):
    hub = await cg.get_variable(config[CONF_HUAWEI_R4850_ID])
    conf = config[CONF_POWER]
    var = cg.new_Pvariable(conf[CONF_ID])
    await cg.register_component(var, conf)
    await switch.register_switch(var, conf)
    cg.add(getattr(hub, "set_power_switch")(var))
    cg.add(var.set_parent(hub))



# async def to_code(config):
#     hub = await cg.get_variable(config[CONF_JBD_BMS_ID])
#     for key, address in SWITCHES.items():
#         if key in config:
#             conf = config[key]
#             var = cg.new_Pvariable(conf[CONF_ID])
#             await cg.register_component(var, conf)
#             await switch.register_switch(var, conf)
#             cg.add(getattr(hub, f"set_{key}_switch")(var))
#             cg.add(var.set_parent(hub))
#             cg.add(var.set_holding_register(address[0]))
#             cg.add(var.set_bitmask(address[1]))