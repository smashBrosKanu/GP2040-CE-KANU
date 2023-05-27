/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#include "storagemanager.h"

#include "BoardConfig.h"
#include "AnimationStorage.hpp"
#include "Effects/StaticColor.hpp"
#include "FlashPROM.h"
#include "hardware/watchdog.h"
#include "Animation.hpp"
#include "CRC32.h"

#include "addons/analog.h"
#include "addons/board_led.h"
#include "addons/bootsel_button.h"
#include "addons/buzzerspeaker.h"
#include "addons/dualdirectional.h"
#include "addons/extra_button.h"
#include "addons/i2canalog1219.h"
#include "addons/i2cdisplay.h"
#include "addons/jslider.h"
#include "addons/neopicoleds.h"
#include "addons/playernum.h"
#include "addons/ps4mode.h"
#include "addons/pleds.h"
#include "addons/reverse.h"
#include "addons/turbo.h"
#include "addons/slider_socd.h"
#include "addons/wiiext.h"
#include "addons/tilt.h"
#include "addons/snes_input.h"

#include "config_utils.h"

#include "bitmaps.h"

#include "helper.h"

Storage::Storage()
{
	EEPROM.start();
	critical_section_init(&animationOptionsCs);
	ConfigUtils::load(config);
}

bool Storage::save()
{
	return ConfigUtils::save(config);
}

static void updateAnimationOptionsProto(const AnimationOptions& options)
{
	AnimationOptions_Proto& optionsProto = Storage::getInstance().getAnimationOptions();

	optionsProto.baseAnimationIndex			= options.baseAnimationIndex;
	optionsProto.brightness					= options.brightness;
	optionsProto.staticColorIndex			= options.staticColorIndex;
	optionsProto.buttonColorIndex			= options.buttonColorIndex;
	optionsProto.chaseCycleTime				= options.chaseCycleTime;
	optionsProto.rainbowCycleTime			= options.rainbowCycleTime;
	optionsProto.themeIndex					= options.themeIndex;
	optionsProto.hasCustomTheme				= options.hasCustomTheme;
	optionsProto.customThemeUp				= options.customThemeUp;
	optionsProto.customThemeDown			= options.customThemeDown;
	optionsProto.customThemeLeft			= options.customThemeLeft;
	optionsProto.customThemeRight			= options.customThemeRight;
	optionsProto.customThemeB1				= options.customThemeB1;
	optionsProto.customThemeB2				= options.customThemeB2;
	optionsProto.customThemeB3				= options.customThemeB3;
	optionsProto.customThemeB4				= options.customThemeB4;
	optionsProto.customThemeL1				= options.customThemeL1;
	optionsProto.customThemeR1				= options.customThemeR1;
	optionsProto.customThemeL2				= options.customThemeL2;
	optionsProto.customThemeR2				= options.customThemeR2;
	optionsProto.customThemeS1				= options.customThemeS1;
	optionsProto.customThemeS2				= options.customThemeS2;
	optionsProto.customThemeA1				= options.customThemeA1;
	optionsProto.customThemeA2				= options.customThemeA2;
	optionsProto.customThemeL3				= options.customThemeL3;
	optionsProto.customThemeR3				= options.customThemeR3;
	optionsProto.customThemeUpPressed		= options.customThemeUpPressed;
	optionsProto.customThemeDownPressed		= options.customThemeDownPressed;
	optionsProto.customThemeLeftPressed		= options.customThemeLeftPressed;
	optionsProto.customThemeRightPressed	= options.customThemeRightPressed;
	optionsProto.customThemeB1Pressed		= options.customThemeB1Pressed;
	optionsProto.customThemeB2Pressed		= options.customThemeB2Pressed;
	optionsProto.customThemeB3Pressed		= options.customThemeB3Pressed;
	optionsProto.customThemeB4Pressed		= options.customThemeB4Pressed;
	optionsProto.customThemeL1Pressed		= options.customThemeL1Pressed;
	optionsProto.customThemeR1Pressed		= options.customThemeR1Pressed;
	optionsProto.customThemeL2Pressed		= options.customThemeL2Pressed;
	optionsProto.customThemeR2Pressed		= options.customThemeR2Pressed;
	optionsProto.customThemeS1Pressed		= options.customThemeS1Pressed;
	optionsProto.customThemeS2Pressed		= options.customThemeS2Pressed;
	optionsProto.customThemeA1Pressed		= options.customThemeA1Pressed;
	optionsProto.customThemeA2Pressed		= options.customThemeA2Pressed;
	optionsProto.customThemeL3Pressed		= options.customThemeL3Pressed;
	optionsProto.customThemeR3Pressed		= options.customThemeR3Pressed;
}

void Storage::performEnqueuedSaves()
{
	if (animationOptionsSavePending.load())
	{
		critical_section_enter_blocking(&animationOptionsCs);
		updateAnimationOptionsProto(animationOptionsToSave);
		save();
		animationOptionsSavePending.store(false);
		critical_section_exit(&animationOptionsCs);
	}
}

void Storage::enqueueAnimationOptionsSave(const AnimationOptions& animationOptions)
{
	const uint32_t crc = CRC32::calculate(&animationOptions);
	critical_section_enter_blocking(&animationOptionsCs);
	if (crc != animationOptionsCrc)
	{
		animationOptionsToSave = animationOptions;
		animationOptionsCrc = crc;
		animationOptionsSavePending.store(true);
	}
	critical_section_exit(&animationOptionsCs);
}

/* Board stuffs */
void Storage::initBoardOptions() {
<<<<<<< HEAD
	EEPROM.get(BOARD_STORAGE_INDEX, boardOptions);
	uint32_t lastCRC = boardOptions.checksum;
	boardOptions.checksum = CHECKSUM_MAGIC;
	if (lastCRC != CRC32::calculate(&boardOptions)) {
		setDefaultBoardOptions();
	}
}

void Storage::initAddonOptions() {
	EEPROM.get(ADDON_STORAGE_INDEX, addonOptions);
	uint32_t lastCRC = addonOptions.checksum;
	addonOptions.checksum = CHECKSUM_MAGIC;
	if (lastCRC != CRC32::calculate(&addonOptions)) {
		setDefaultAddonOptions();
	}
}

void Storage::initSplashImage() {
	EEPROM.get(SPLASH_IMAGE_STORAGE_INDEX, splashImage);
	uint32_t lastCRC = splashImage.checksum;
	splashImage.checksum = CHECKSUM_MAGIC;
	if (lastCRC != CRC32::calculate(&splashImage)) {
		setDefaultSplashImage();
	}
}

void Storage::initPS4Options() {
	EEPROM.get(PS4_STORAGE_INDEX, ps4Options);
	if (ps4Options.checksum != NOCHECKSUM_MAGIC) {
		setDefaultPS4Options();
	}
}

void Storage::setDefaultBoardOptions()
{
	// Set GP2040 version string and 0 mem after
	boardOptions.hasBoardOptions   = false;
	boardOptions.pinDpadUp         = PIN_DPAD_UP;
	boardOptions.pinDpadDown       = PIN_DPAD_DOWN;
	boardOptions.pinDpadLeft       = PIN_DPAD_LEFT;
	boardOptions.pinDpadRight      = PIN_DPAD_RIGHT;
	boardOptions.pinButtonB1       = PIN_BUTTON_B1;
	boardOptions.pinButtonB2       = PIN_BUTTON_B2;
	boardOptions.pinButtonB3       = PIN_BUTTON_B3;
	boardOptions.pinButtonB4       = PIN_BUTTON_B4;
	boardOptions.pinButtonL1       = PIN_BUTTON_L1;
	boardOptions.pinButtonR1       = PIN_BUTTON_R1;
	boardOptions.pinButtonL2       = PIN_BUTTON_L2;
	boardOptions.pinButtonR2       = PIN_BUTTON_R2;
	boardOptions.pinButtonS1       = PIN_BUTTON_S1;
	boardOptions.pinButtonS2       = PIN_BUTTON_S2;
	boardOptions.pinButtonL3       = PIN_BUTTON_L3;
	boardOptions.pinButtonR3       = PIN_BUTTON_R3;
	boardOptions.pinButtonA1       = PIN_BUTTON_A1;
	boardOptions.pinButtonA2       = PIN_BUTTON_A2;
	boardOptions.buttonLayout      = BUTTON_LAYOUT;
	boardOptions.buttonLayoutRight = BUTTON_LAYOUT_RIGHT;
	boardOptions.splashMode        = SPLASH_MODE;
	boardOptions.splashChoice      = SPLASH_CHOICE;
	boardOptions.splashDuration    = SPLASH_DURATION;
	boardOptions.i2cSDAPin         = I2C_SDA_PIN;
	boardOptions.i2cSCLPin         = I2C_SCL_PIN;
	boardOptions.i2cBlock          = (I2C_BLOCK == i2c0) ? 0 : 1;
	boardOptions.i2cSpeed          = I2C_SPEED;
	boardOptions.hasI2CDisplay     = HAS_I2C_DISPLAY;
	boardOptions.displayI2CAddress = DISPLAY_I2C_ADDR;
	boardOptions.displaySize       = DISPLAY_SIZE;
	boardOptions.displayFlip       = DISPLAY_FLIP;
	boardOptions.displayInvert     = DISPLAY_INVERT;
	boardOptions.displaySaverTimeout     = DISPLAY_SAVER_TIMEOUT;

	ButtonLayoutParams params = {
		.layout = BUTTON_LAYOUT,
		.startX = 8,
		.startY = 28,
		.buttonRadius = 8,
		.buttonPadding = 2
	};
	boardOptions.buttonLayoutCustomOptions.params = params;

	ButtonLayoutParams paramsRight = {
		.layoutRight = BUTTON_LAYOUT_RIGHT,
		.startX = 8,
		.startY = 28,
		.buttonRadius = 8,
		.buttonPadding = 2
	};
	boardOptions.buttonLayoutCustomOptions.paramsRight = paramsRight;

	strncpy(boardOptions.boardVersion, GP2040VERSION, strlen(GP2040VERSION));
	setBoardOptions(boardOptions);
}

void Storage::setBoardOptions(BoardOptions options)
{
	if (memcmp(&options, &boardOptions, sizeof(BoardOptions)) != 0)
	{
		options.checksum = CHECKSUM_MAGIC; // set checksum to magic number
		options.checksum = CRC32::calculate(&options);
		EEPROM.set(BOARD_STORAGE_INDEX, options);
		EEPROM.commit();
		memcpy(&boardOptions, &options, sizeof(BoardOptions));
	}
}

void Storage::setDefaultAddonOptions()
{
	addonOptions.pinButtonTurbo    		= PIN_BUTTON_TURBO;
	addonOptions.pinButtonReverse  		= PIN_BUTTON_REVERSE;
	addonOptions.pinSliderLS       		= PIN_SLIDER_LS;
	addonOptions.pinSliderRS       		= PIN_SLIDER_RS;
	addonOptions.pinSliderSOCDOne     	= PIN_SLIDER_SOCD_ONE;
	addonOptions.pinSliderSOCDTwo    	= PIN_SLIDER_SOCD_TWO;
	addonOptions.pinDualDirDown    		= PIN_DUAL_DIRECTIONAL_DOWN;
	addonOptions.pinDualDirUp      		= PIN_DUAL_DIRECTIONAL_UP;
	addonOptions.pinDualDirLeft    		= PIN_DUAL_DIRECTIONAL_LEFT;
	addonOptions.pinDualDirRight   		= PIN_DUAL_DIRECTIONAL_RIGHT;
	addonOptions.pinTilt1 = PIN_TILT_1;
	addonOptions.pinTilt2 = PIN_TILT_2;
	addonOptions.pinTiltFunction = PIN_TILT_FUNCTION;
	addonOptions.pinTiltLeftAnalogDown = PIN_TILT_LEFT_ANALOG_DOWN;
	addonOptions.pinTiltLeftAnalogUp = PIN_TILT_LEFT_ANALOG_UP;
	addonOptions.pinTiltLeftAnalogLeft = PIN_TILT_LEFT_ANALOG_LEFT;
	addonOptions.pinTiltLeftAnalogRight = PIN_TILT_LEFT_ANALOG_RIGHT;
	addonOptions.pinTiltRightAnalogDown = PIN_TILT_RIGHT_ANALOG_DOWN;
	addonOptions.pinTiltRightAnalogUp = PIN_TILT_RIGHT_ANALOG_UP;
	addonOptions.pinTiltRightAnalogLeft = PIN_TILT_RIGHT_ANALOG_LEFT;
	addonOptions.pinTiltRightAnalogRight = PIN_TILT_RIGHT_ANALOG_RIGHT;
	addonOptions.turboShotCount    		= DEFAULT_SHOT_PER_SEC;
	addonOptions.pinTurboLED       		= TURBO_LED_PIN;
	addonOptions.pinReverseLED     		= REVERSE_LED_PIN;
	addonOptions.reverseActionUp        = REVERSE_UP_DEFAULT;
	addonOptions.reverseActionDown      = REVERSE_DOWN_DEFAULT;
	addonOptions.reverseActionLeft      = REVERSE_LEFT_DEFAULT;
	addonOptions.reverseActionRight     = REVERSE_RIGHT_DEFAULT;
	addonOptions.i2cAnalog1219SDAPin    = I2C_ANALOG1219_SDA_PIN;
	addonOptions.i2cAnalog1219SCLPin    = I2C_ANALOG1219_SCL_PIN;
	addonOptions.i2cAnalog1219Block     = (I2C_ANALOG1219_BLOCK == i2c0) ? 0 : 1;
	addonOptions.i2cAnalog1219Speed     = I2C_ANALOG1219_SPEED;
	addonOptions.i2cAnalog1219Address   = I2C_ANALOG1219_ADDRESS;
	addonOptions.onBoardLedMode			= BOARD_LED_TYPE;
	addonOptions.dualDirDpadMode        = DUAL_DIRECTIONAL_STICK_MODE;
	addonOptions.dualDirCombineMode     = DUAL_DIRECTIONAL_COMBINE_MODE;
	addonOptions.tiltSOCDMode = TILT_SOCD_MODE;
	addonOptions.analogAdcPinX      	= ANALOG_ADC_VRX;
	addonOptions.analogAdcPinY      	= ANALOG_ADC_VRY;
	addonOptions.bootselButtonMap		= BOOTSEL_BUTTON_MASK;
	addonOptions.buzzerPin              = BUZZER_PIN;
	addonOptions.buzzerVolume           = BUZZER_VOLUME;
	addonOptions.extraButtonMap		    = EXTRA_BUTTON_MASK;
	addonOptions.extraButtonPin		    = EXTRA_BUTTON_PIN;
	addonOptions.playerNumber           = PLAYER_NUMBER;
	addonOptions.shmupMode = TURBO_SHMUP_MODE; // Turbo SHMUP Mode
	addonOptions.shmupMixMode = SHMUP_MIX_MODE; // How we mix turbo and non-turbo buttons
	addonOptions.shmupAlwaysOn1 = SHMUP_ALWAYS_ON1;
	addonOptions.shmupAlwaysOn2 = SHMUP_ALWAYS_ON2;
	addonOptions.shmupAlwaysOn3 = SHMUP_ALWAYS_ON3;
	addonOptions.shmupAlwaysOn4 = SHMUP_ALWAYS_ON4;
	addonOptions.pinShmupBtn1 = PIN_SHMUP_BUTTON1;
	addonOptions.pinShmupBtn2 = PIN_SHMUP_BUTTON2;
	addonOptions.pinShmupBtn3 = PIN_SHMUP_BUTTON3;
	addonOptions.pinShmupBtn4 = PIN_SHMUP_BUTTON4;
	addonOptions.shmupBtnMask1 = SHMUP_BUTTON1;
	addonOptions.shmupBtnMask2 = SHMUP_BUTTON2;
	addonOptions.shmupBtnMask3 = SHMUP_BUTTON3;
	addonOptions.shmupBtnMask4 = SHMUP_BUTTON4;
	addonOptions.pinShmupDial = PIN_SHMUP_DIAL;
  addonOptions.sliderSOCDModeOne = SLIDER_SOCD_SLOT_ONE;
  addonOptions.sliderSOCDModeTwo  = SLIDER_SOCD_SLOT_TWO;
  addonOptions.sliderSOCDModeDefault = SLIDER_SOCD_SLOT_DEFAULT;
	addonOptions.wiiExtensionSDAPin    = WII_EXTENSION_I2C_SDA_PIN;
	addonOptions.wiiExtensionSCLPin    = WII_EXTENSION_I2C_SCL_PIN;
	addonOptions.wiiExtensionBlock     = (WII_EXTENSION_I2C_BLOCK == i2c0) ? 0 : 1;
	addonOptions.wiiExtensionSpeed     = WII_EXTENSION_I2C_SPEED;
	addonOptions.AnalogInputEnabled     = ANALOG_INPUT_ENABLED;
	addonOptions.BoardLedAddonEnabled   = BOARD_LED_ENABLED;
	addonOptions.BootselButtonAddonEnabled = BOOTSEL_BUTTON_ENABLED;
	addonOptions.BuzzerSpeakerAddonEnabled = BUZZER_ENABLED;
	addonOptions.DualDirectionalInputEnabled = DUAL_DIRECTIONAL_ENABLED;
	addonOptions.TiltInputEnabled = TILT_ENABLED;
	addonOptions.ExtraButtonAddonEnabled = EXTRA_BUTTON_ENABLED;
	addonOptions.I2CAnalog1219InputEnabled = I2C_ANALOG1219_ENABLED;
	addonOptions.JSliderInputEnabled    = JSLIDER_ENABLED;
	addonOptions.SliderSOCDInputEnabled    = SLIDER_SOCD_ENABLED;
	addonOptions.PlayerNumAddonEnabled  = PLAYERNUM_ADDON_ENABLED;
	addonOptions.PS4ModeAddonEnabled    = PS4MODE_ADDON_ENABLED;
	addonOptions.ReverseInputEnabled    = REVERSE_ENABLED;
	addonOptions.TurboInputEnabled      = TURBO_ENABLED;
	addonOptions.WiiExtensionAddonEnabled      = WII_EXTENSION_ENABLED;
	setAddonOptions(addonOptions);
}

void Storage::setAddonOptions(AddonOptions options)
{
	if (memcmp(&options, &addonOptions, sizeof(AddonOptions)) != 0)
	{
		options.checksum = CHECKSUM_MAGIC; // set checksum to magic number
		options.checksum = CRC32::calculate(&options);
		EEPROM.set(ADDON_STORAGE_INDEX, options);
		EEPROM.commit();
		addonOptions = options;
	}
}

void Storage::setDefaultSplashImage()
{
	memcpy(&splashImage.data, &splashImageMain, sizeof(splashImageMain));
	setSplashImage(splashImage);
}

void Storage::setSplashImage(const SplashImage& image)
{
	if (memcmp(&splashImage, &image, sizeof(SplashImage)) != 0)
	{
		memcpy(&splashImage, &image, sizeof(SplashImage));
		splashImage.checksum = CHECKSUM_MAGIC; // set checksum to magic number
		splashImage.checksum = CRC32::calculate(&splashImage);

		EEPROM.set(SPLASH_IMAGE_STORAGE_INDEX, splashImage);
		EEPROM.commit();

		// Reset, so that the memcmp gives the correct result on the next call to this function
		splashImage.checksum = CHECKSUM_MAGIC;
	}
}

/* LED stuffs */
void Storage::initLEDOptions()
{
	EEPROM.get(LED_STORAGE_INDEX, ledOptions);
	uint32_t lastCRC = ledOptions.checksum;
	ledOptions.checksum = CHECKSUM_MAGIC;
	if (lastCRC != CRC32::calculate(&ledOptions)) {
		setDefaultLEDOptions();
	}
}

void Storage::setDefaultLEDOptions()
{
	ledOptions.dataPin = BOARD_LEDS_PIN;
	ledOptions.ledFormat = LED_FORMAT;
	ledOptions.ledLayout = BUTTON_LAYOUT;
	ledOptions.ledsPerButton = LEDS_PER_PIXEL;
	ledOptions.brightnessMaximum = LED_BRIGHTNESS_MAXIMUM;
	ledOptions.brightnessSteps = LED_BRIGHTNESS_STEPS;
	ledOptions.indexUp = LEDS_DPAD_UP;
	ledOptions.indexDown = LEDS_DPAD_DOWN;
	ledOptions.indexLeft = LEDS_DPAD_LEFT;
	ledOptions.indexRight = LEDS_DPAD_RIGHT;
	ledOptions.indexB1 = LEDS_BUTTON_B1;
	ledOptions.indexB2 = LEDS_BUTTON_B2;
	ledOptions.indexB3 = LEDS_BUTTON_B3;
	ledOptions.indexB4 = LEDS_BUTTON_B4;
	ledOptions.indexL1 = LEDS_BUTTON_L1;
	ledOptions.indexR1 = LEDS_BUTTON_R1;
	ledOptions.indexL2 = LEDS_BUTTON_L2;
	ledOptions.indexR2 = LEDS_BUTTON_R2;
	ledOptions.indexS1 = LEDS_BUTTON_S1;
	ledOptions.indexS2 = LEDS_BUTTON_S2;
	ledOptions.indexL3 = LEDS_BUTTON_L3;
	ledOptions.indexR3 = LEDS_BUTTON_R3;
	ledOptions.indexA1 = LEDS_BUTTON_A1;
	ledOptions.indexA2 = LEDS_BUTTON_A2;
	setLEDOptions(ledOptions);
}

void Storage::setLEDOptions(LEDOptions options)
{
	if (memcmp(&options, &ledOptions, sizeof(LEDOptions)) != 0)
	{
		options.checksum = CHECKSUM_MAGIC; // set checksum to magic number
		options.checksum = CRC32::calculate(&options);
		EEPROM.set(LED_STORAGE_INDEX, options);
		EEPROM.commit();
		memcpy(&ledOptions, &options, sizeof(LEDOptions));
	}
}

void Storage::savePS4Options()     // PS4 Options
{
	ps4Options.checksum = NOCHECKSUM_MAGIC;
	EEPROM.set(PS4_STORAGE_INDEX, ps4Options);
	EEPROM.commit();
}

void Storage::setDefaultPS4Options()
{
	// Zero everything out
	memset(&ps4Options, 0, sizeof(PS4Options));
}

PS4Options * Storage::getPS4Options()
{
	return &ps4Options;
=======
	setDefaultBoardOptions();
>>>>>>> upstream/main
}

void Storage::ResetSettings()
{
	EEPROM.reset();
	watchdog_reboot(0, SRAM_END, 2000);
}

void Storage::SetConfigMode(bool mode) { // hack for config mode
	CONFIG_MODE = mode;
	previewDisplayOptions = config.displayOptions;
}

bool Storage::GetConfigMode()
{
	return CONFIG_MODE;
}

void Storage::SetGamepad(Gamepad * newpad)
{
	gamepad = newpad;
}

Gamepad * Storage::GetGamepad()
{
	return gamepad;
}

void Storage::SetProcessedGamepad(Gamepad * newpad)
{
	processedGamepad = newpad;
}

Gamepad * Storage::GetProcessedGamepad()
{
	return processedGamepad;
}

void Storage::SetFeatureData(uint8_t * newData)
{
	memcpy(newData, featureData, sizeof(uint8_t)*sizeof(featureData));
}

void Storage::ClearFeatureData()
{
	memset(featureData, 0, sizeof(uint8_t)*sizeof(featureData));
}

uint8_t * Storage::GetFeatureData()
{
	return featureData;
}

/* Animation stuffs */
AnimationOptions AnimationStorage::getAnimationOptions()
{
	AnimationOptions options;
	const AnimationOptions_Proto& optionsProto = Storage::getInstance().getAnimationOptions();

	options.checksum				= 0;
	options.baseAnimationIndex		= std::min<uint32_t>(optionsProto.baseAnimationIndex, 255);
	options.brightness				= std::min<uint32_t>(optionsProto.brightness, 255);
	options.staticColorIndex		= std::min<uint32_t>(optionsProto.staticColorIndex, 255);
	options.buttonColorIndex		= std::min<uint32_t>(optionsProto.buttonColorIndex, 255);
	options.chaseCycleTime			= std::min<int32_t>(optionsProto.chaseCycleTime, 65535);
	options.rainbowCycleTime		= std::min<int32_t>(optionsProto.rainbowCycleTime, 65535);
	options.themeIndex				= std::min<uint8_t>(optionsProto.themeIndex, 255);
	options.hasCustomTheme			= optionsProto.hasCustomTheme;
	options.customThemeUp			= optionsProto.customThemeUp;
	options.customThemeDown			= optionsProto.customThemeDown;
	options.customThemeLeft			= optionsProto.customThemeLeft;
	options.customThemeRight		= optionsProto.customThemeRight;
	options.customThemeB1			= optionsProto.customThemeB1;
	options.customThemeB2			= optionsProto.customThemeB2;
	options.customThemeB3			= optionsProto.customThemeB3;
	options.customThemeB4			= optionsProto.customThemeB4;
	options.customThemeL1			= optionsProto.customThemeL1;
	options.customThemeR1			= optionsProto.customThemeR1;
	options.customThemeL2			= optionsProto.customThemeL2;
	options.customThemeR2			= optionsProto.customThemeR2;
	options.customThemeS1			= optionsProto.customThemeS1;
	options.customThemeS2			= optionsProto.customThemeS2;
	options.customThemeA1			= optionsProto.customThemeA1;
	options.customThemeA2			= optionsProto.customThemeA2;
	options.customThemeL3			= optionsProto.customThemeL3;
	options.customThemeR3			= optionsProto.customThemeR3;
	options.customThemeUpPressed	= optionsProto.customThemeUpPressed;
	options.customThemeDownPressed	= optionsProto.customThemeDownPressed;
	options.customThemeLeftPressed	= optionsProto.customThemeLeftPressed;
	options.customThemeRightPressed	= optionsProto.customThemeRightPressed;
	options.customThemeB1Pressed	= optionsProto.customThemeB1Pressed;
	options.customThemeB2Pressed	= optionsProto.customThemeB2Pressed;
	options.customThemeB3Pressed	= optionsProto.customThemeB3Pressed;
	options.customThemeB4Pressed	= optionsProto.customThemeB4Pressed;
	options.customThemeL1Pressed	= optionsProto.customThemeL1Pressed;
	options.customThemeR1Pressed	= optionsProto.customThemeR1Pressed;
	options.customThemeL2Pressed	= optionsProto.customThemeL2Pressed;
	options.customThemeR2Pressed	= optionsProto.customThemeR2Pressed;
	options.customThemeS1Pressed	= optionsProto.customThemeS1Pressed;
	options.customThemeS2Pressed	= optionsProto.customThemeS2Pressed;
	options.customThemeA1Pressed	= optionsProto.customThemeA1Pressed;
	options.customThemeA2Pressed	= optionsProto.customThemeA2Pressed;
	options.customThemeL3Pressed	= optionsProto.customThemeL3Pressed;
	options.customThemeR3Pressed	= optionsProto.customThemeR3Pressed;

	return options;
}

void AnimationStorage::save()
{
	Storage::getInstance().enqueueAnimationOptionsSave(AnimationStation::options);
}
