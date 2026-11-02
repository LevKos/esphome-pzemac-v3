import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import pzemac, sensor
from esphome.const import CONF_ID

# Беремо всю схему валідації з оригінального компонента
CONFIG_SCHEMA = pzemac.CONFIG_SCHEMA

pzemac_withoffline_ns = cg.esphome_ns.namespace("pzemac_withoffline")
PZEMACWithOffline = pzemac_withoffline_ns.class_(
    "PZEMACWithOffline", pzemac.PZEMAC
)

async def to_code(config):
    # Генеруємо код на основі логіки оригінального компонента, 
    # але створюємо саме наш обгорнутий клас PZEMACWithOffline
    var = cg.new_PVM(PZEMACWithOffline)
    await pzemac.to_code_base(var, config)
