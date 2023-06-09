#include "addons/analog.h"
#include "storagemanager.h"
#include "helper.h"
#include "config.pb.h"

#include "hardware/adc.h"

#define ADC_MAX ((1 << 12) - 1)
#define ANALOG_CENTER 0.5f // 0.5f is center
#define ANALOG_DEADZONE 0.05f // move to config (future release)

bool AnalogInput::available() {
    return Storage::getInstance().getAddonOptions().analogOptions.enabled;
}

void AnalogInput::setup() {
    const AnalogOptions& options = Storage::getInstance().getAddonOptions().analogOptions;
	analogAdcPinX = options.analogAdcPinX;
	analogAdcPinY = options.analogAdcPinY;

    // Make sure GPIO is high-impedance, no pullups etc
    if ( isValidPin(analogAdcPinX) )
        adc_gpio_init(analogAdcPinX);
    if ( isValidPin(analogAdcPinY) )
        adc_gpio_init(analogAdcPinY);
}

void AnalogInput::process()
{
    Gamepad * gamepad = Storage::getInstance().GetGamepad();
    float adc_x = ANALOG_CENTER;
    float adc_y = ANALOG_CENTER;
    if ( isValidPin(analogAdcPinX) ) {
        adc_select_input(analogAdcPinX-26); // ANALOG-X
        adc_x = ((float)adc_read())/ADC_MAX;
    }
    if ( isValidPin(analogAdcPinY) ) {
        adc_select_input(analogAdcPinY-26); // ANALOG-Y
        adc_y = ((float)adc_read())/ADC_MAX;
    }
    if ( abs(adc_x - ANALOG_CENTER) < ANALOG_DEADZONE ) // deadzones
        adc_x = ANALOG_CENTER;
    if ( abs(adc_y - ANALOG_CENTER) < ANALOG_DEADZONE ) // deadzones
        adc_y = ANALOG_CENTER;

    // Convert to 16-bit value
    gamepad->state.lx = (uint16_t)(65535.0f*adc_x);
    gamepad->state.ly = (uint16_t)(65535.0f*adc_y);
}
