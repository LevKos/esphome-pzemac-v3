import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, modbus
# Імпортуємо саме модуль сенсора оригінального pzemac
from esphome.components.pzemac import sensor as pzemac_sensor
from esphome.const import CONF_ID

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

    # 4. Створюємо екземпляр НАШОГО C++ класу замість оригінального
    var = cg.new_Pvariable(config[CONF_ID])

    # 5. Реєструємо компонент у системі та підключаємо Modbus (копіюємо логіку оригіналу)
    await cg.register_component(var, config)
    await modbus.register_modbus_device(var, config)

    # 6. Прив'язуємо всі сенсори, які користувач вказав у YAML
    # Перевіряємо кожен можливий сенсор PZEM04T і реєструємо його
    for prop in ["voltage", "current", "active_power", "frequency", "power_factor"]:
        if prop in config:
            sens = await sensor.new_sensor(config[prop])
            cg.add(getattr(var, f"set_{prop}_sensor")(sens))
            
    # Окремо реєструємо лічильник енергії, бо в коді pzemac він має назву "energy"
    if "energy" in config:
        sens = await sensor.new_sensor(config["energy"])
        cg.add(var.set_energy_sensor(sens))

   # --- ДОДАТИ ЦІ ДВА РЯДКИ В КІНЕЦЬ ФУНКЦІЇ ---
    # Змушує компілятор шукати файли в оригінальній папці pzemac
    cg.add_build_flag("-Isrc/esphome/components/pzemac")
    cg.add_build_flag("-Isrc/esphome") # про всяк випадок для глобальних шляхів

    # Передаємо конфіг в оригінальну функцію, щоб вона налаштувала всі сенсори та Modbus
#    await pzemac_sensor.to_code_base()

    # Викликаємо оригінальну генерацію коду pzemac.
    # Вона сама налаштує UART, Modbus та прив'яже всі сенсори (напругу, струм і т.д.) до нашого var.
#    await pzemac_sensor.to_code(config)
    
    # Переписуємо тип створеної змінної у фінальному C++ файлі на наш клас
#    cg.add(var)