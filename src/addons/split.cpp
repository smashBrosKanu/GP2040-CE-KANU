#include "addons/split.h"
#include "storagemanager.h"
#include "hardware/gpio.h"

bool SplitControllerInput::available() {
    const BoardOptions& boardOptions = Storage::getInstance().getBoardOptions();
    AddonOptions options = Storage::getInstance().getAddonOptions();

    return (!boardOptions.hasI2CDisplay && (options.SplitControllerAddonEnabled &&
        options.splitControllerSDAPin != (uint8_t)-1 &&
        options.splitControllerSCLPin != (uint8_t)-1));
}

void SplitControllerInput::setup() {
    AddonOptions options = Storage::getInstance().getAddonOptions();
    nextTimer = getMillis();

#if SPLIT_CONTROLLER_DEBUG==true
    stdio_init_all();
#endif

    uIntervalMS = 0;
    
    split = new SplitController(
        options.splitControllerSDAPin,
        options.splitControllerSCLPin,
        options.splitControllerBlock == 0 ? i2c0 : i2c1,
        options.splitControllerSpeed,
        SPLIT_CONTROLLER_I2C_ADDR);
    split->begin();
    split->start();
}

void SplitControllerInput::process() {
	//
}
