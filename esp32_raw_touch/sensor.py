import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_NAME, CONF_ID, CONF_PIN, ESP_PLATFORM_ESP32
from esphome.pins import validate_gpio_pin
from . import esp32_raw_touch_ns, ESP32RawTouchComponent

ESP_PLATFORMS = [ESP_PLATFORM_ESP32]
DEPENDENCIES = ['esp32_raw_touch']

CONF_ESP32_TOUCH_ID = 'esp32_touch_id'

TOUCH_PADS = {
    4: cg.global_ns.TOUCH_PAD_NUM0,
    0: cg.global_ns.TOUCH_PAD_NUM1,
    2: cg.global_ns.TOUCH_PAD_NUM2,
    15: cg.global_ns.TOUCH_PAD_NUM3,
    13: cg.global_ns.TOUCH_PAD_NUM4,
    12: cg.global_ns.TOUCH_PAD_NUM5,
    14: cg.global_ns.TOUCH_PAD_NUM6,
    27: cg.global_ns.TOUCH_PAD_NUM7,
    33: cg.global_ns.TOUCH_PAD_NUM8,
    32: cg.global_ns.TOUCH_PAD_NUM9,
}

def validate_touch_pad(value):
    value = validate_gpio_pin(value)
    if value not in TOUCH_PADS:
        raise cv.Invalid(f"Pin {value} does not support touch pads.")
    return value

#esp32_raw_touch_ns = cg.esphome_ns.namespace('esp32_raw_touch')
#ESP32RawTouchComponent = esp32_raw_touch_ns.class_('ESP32RawTouchComponent', sensor.Sensor, cg.PollingComponent)

ESP32RawTouchSensor = esp32_raw_touch_ns.class_('ESP32RawTouchSensor', sensor.Sensor, cg.PollingComponent)

#CONF_RAWTOUCH = 'rawtouch'

CONFIG_SCHEMA = sensor.SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(ESP32RawTouchSensor),
    cv.GenerateID(CONF_ESP32_TOUCH_ID): cv.use_id(ESP32RawTouchComponent),
    cv.Required(CONF_PIN): validate_touch_pad,
}).extend(cv.polling_component_schema('60s'))


def to_code(config):
    hub = yield cg.get_variable(config[CONF_ESP32_TOUCH_ID])
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_NAME], TOUCH_PADS[config[CONF_PIN]])
    yield sensor.register_sensor(var, config)
    cg.add(hub.register_touch_pad(var))