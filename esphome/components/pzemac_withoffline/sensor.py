import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, modbus
# Імпортуємо саме модуль сенсора оригінального pzemac
from esphome.components.pzemac import esphome.const import CONF_ID

# Беремо схему валідації та базову функцію генерації з оригінального сенсора
CONFIG_SCHEMA = pzemac_sensor.CONFIG_SCHEMA

# Оголошуємо наш новий простір імен та клас
pzemac_withoffline_ns = cg.esphome_ns.namespace("pzemac_withoffline")

# Оголошуємо C++ клас. Він наслідується від оригінального pzemac::PZEMAC
PZEMACWithOffline = pzemac_withoffline_ns.class_(
    "PZEMACWithOffline", pzemac_sensor.PZEMAC
)

# PZEMACWithOffline = pzemac_withoffline_ns.class_(
#    "PZEMACWithOffline", pzemac_sensor.PZEMAC, cg.C#omponent, modbus.ModbusDevice
#)

async def to_code(config):
    # Створюємо екземпляр нашого C++ класу
#    var = cg.new_PvM(PZEMACWithOffline)
    # Правильний метод ESPHome для створення нового екземпляра об'єкта в C++
    var = cg.new_Pvariable(config[CONF_ID])
    # Передаємо конфіг в оригінальну функцію, щоб вона налаштувала всі сенсори та Modbus
    await pzemac_sensor.to_code_base(var, config)
