import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, modbus
# Імпортуємо саме модуль сенсора оригінального pzemac
from esphome.components.pzemac import sensor as pzemac_sensor

# Беремо схему валідації та базову функцію генерації з оригінального сенсора
CONFIG_SCHEMA = pzemac_sensor.CONFIG_SCHEMA

# Оголошуємо наш новий простір імен та клас
pzemac_withoffline_ns = cg.esphome_ns.namespace("pzemac_withoffline")
PZEMACWithOffline = pzemac_withoffline_ns.class_(
    "PZEMACWithOffline", pzemac_sensor.PZEMAC, cg.Component, modbus.ModbusDevice
)

async def to_code(config):
    # Створюємо екземпляр нашого C++ класу
    var = cg.new_PVM(PZEMACWithOffline)
    # Передаємо конфіг в оригінальну функцію, щоб вона налаштувала всі сенсори та Modbus
    await pzemac_sensor.to_code_base(var, config)
