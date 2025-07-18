import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import display
from esphome.const import (
    CONF_ID,
    CONF_LAMBDA,
    CONF_PAGES,
)
from esphome.const import __version__ as ESPHOME_VERSION

DEPENDENCIES = ["esp32"]

CONF_GREYSCALE = "greyscale"
CONF_INVERTCYCLE = "invert_cycle"
CONF_CLEANTIME = "cleantimer"


t547_ns = cg.esphome_ns.namespace("t547")
T547 = t547_ns.class_(
    "T547", cg.PollingComponent, display.DisplayBuffer
)

CONFIG_SCHEMA = cv.All(
    display.FULL_DISPLAY_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(T547),
            cv.Optional(CONF_GREYSCALE, default=False): cv.boolean,
            cv.Optional(CONF_INVERTCYCLE, default=5): cv.int_range(1, 50),
            cv.Optional(CONF_CLEANTIME, default=50): cv.int_range(10, 100),
        }
    )
    .extend(cv.polling_component_schema("5s")),
    cv.has_at_most_one_key(CONF_PAGES, CONF_LAMBDA),
    cv.only_with_arduino,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    if cv.Version.parse(ESPHOME_VERSION) < cv.Version.parse("2023.12.0"):
        await cg.register_component(var, config)
    await display.register_display(var, config)

    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA], [(display.DisplayRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))

    cg.add(var.set_greyscale(config[CONF_GREYSCALE]))
    cg.add(var.set_cycles(config[CONF_INVERTCYCLE]))
    cg.add(var.set_cleantime(config[CONF_CLEANTIME]))

    cg.add_build_flag("-DBOARD_HAS_PSRAM")

    cg.add_library("SPI", version=None)  # required by LilyGoEPD47
    cg.add_library("Wire", version="2.0.0")  # required by LilyGoEPD47
    cg.add_library("LilyGoEPD47", repository="https://github.com/Xinyuan-LilyGO/LilyGo-EPD47", version="v0.3.0")
