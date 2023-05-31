#include "addons/tilt.h"
#include "storagemanager.h"
#include "gamepad.h"

SplitController* myController0 = new SplitController(0, 1, i2c0, 400000, 0x17);

bool TiltInput::available() {
	AddonOptions options = Storage::getInstance().getAddonOptions();
	pinTilt1 = options.pinTilt1;
	pinTilt2 = options.pinTilt2;
	pinTiltFunction = options.pinTiltFunction;
	pinTiltLeftAnalogDown = options.pinTiltLeftAnalogDown;
	pinTiltLeftAnalogUp = options.pinTiltLeftAnalogUp;
	pinTiltLeftAnalogLeft = options.pinTiltLeftAnalogLeft;
	pinTiltLeftAnalogRight = options.pinTiltLeftAnalogRight;
	pinTiltRightAnalogDown = options.pinTiltRightAnalogDown;
	pinTiltRightAnalogUp = options.pinTiltRightAnalogUp;
	pinTiltRightAnalogLeft = options.pinTiltRightAnalogLeft;
	pinTiltRightAnalogRight = options.pinTiltRightAnalogRight;

	return options.TiltInputEnabled;
}

void TiltInput::setup() {
	AddonOptions options = Storage::getInstance().getAddonOptions();
	tiltSOCDMode = options.tiltSOCDMode;

	// Setup Tilt Key
	uint8_t pinTilt[11] = { pinTilt1,
											pinTilt2,
											pinTiltFunction,
											pinTiltLeftAnalogDown,
											pinTiltLeftAnalogUp,
											pinTiltLeftAnalogLeft,
											pinTiltLeftAnalogRight,
											pinTiltRightAnalogDown,
											pinTiltRightAnalogUp,
											pinTiltRightAnalogLeft,
											pinTiltRightAnalogRight };

	for (int i = 0; i < 11; i++) {
		if (pinTilt[i] != (uint8_t)-1) {
			gpio_init(pinTilt[i]);             // Initialize pin
			gpio_set_dir(pinTilt[i], GPIO_IN); // Set as INPUT
			gpio_pull_up(pinTilt[i]);          // Set as PULLUP
		}
	}

	maskTilt1 = 1UL << pinTilt1;
	maskTilt2 = 1UL << pinTilt2;
	maskTiltFunction = 1UL << pinTiltFunction;
	maskTiltLeftAnalogDown = 1UL << pinTiltLeftAnalogDown;
	maskTiltLeftAnalogUp = 1UL << pinTiltLeftAnalogUp;
	maskTiltLeftAnalogLeft = 1UL << pinTiltLeftAnalogLeft;
	maskTiltLeftAnalogRight = 1UL << pinTiltLeftAnalogRight;
	maskTiltRightAnalogDown = 1UL << pinTiltRightAnalogDown;
	maskTiltRightAnalogUp = 1UL << pinTiltRightAnalogUp;
	maskTiltRightAnalogLeft = 1UL << pinTiltRightAnalogLeft;
	maskTiltRightAnalogRight = 1UL << pinTiltRightAnalogRight;

	myController0 = new SplitController(0, 1, i2c0, 400000, 0x17);

	dDebLeftState = 0;
	dDebRightState = 0;
	tiltLeftState = 0;
	tiltRightState = 0;

	lastGPUD = DIRECTION_NONE;
	lastGPLR = DIRECTION_NONE;

	lastTiltUD = DIRECTION_NONE;
	lastTiltLR = DIRECTION_NONE;

	uint32_t now = getMillis();
	for (int i = 0; i < 4; i++) {
		dpadTime[i] = now;
	}
}

void TiltInput::debounce()
{
	uint32_t now = getMillis();
	Gamepad* gamepad = Storage::getInstance().GetGamepad();

	for (int i = 0; i < 4; i++)
	{
		if ((dDebLeftState & dpadMasks[i]) != (tiltLeftState & dpadMasks[i]) && (now - dpadTime[i]) > gamepad->debounceMS)
		{
			dDebLeftState ^= dpadMasks[i];
			dpadTime[i] = now;
		}
	}
	tiltLeftState = dDebLeftState;

	for (int i = 0; i < 4; i++)
	{
		if ((dDebRightState & dpadMasks[i]) != (tiltRightState & dpadMasks[i]) && (now - dpadTime[i]) > gamepad->debounceMS)
		{
			dDebRightState ^= dpadMasks[i];
			dpadTime[i] = now;
		}
	}
	tiltRightState = dDebRightState;
}

void TiltInput::preprocess()
{
	Gamepad* gamepad = Storage::getInstance().GetGamepad();

	// Need to invert since we're using pullups
	uint32_t masterButtonState = ~gpio_get_all();
	uint32_t slaveButtonState = ~myController0->getSlaveButtonState();

	values = masterButtonState | slaveButtonState;

	// Need to invert since we're using pullups
	tiltLeftState = 0;
	if (pinTiltLeftAnalogUp != (uint8_t)-1) {
		tiltLeftState |= ((values & maskTiltLeftAnalogUp) ? gamepad->mapDpadUp->buttonMask : 0);
	}
	if (pinTiltLeftAnalogDown != (uint8_t)-1) {
		tiltLeftState |= ((values & maskTiltLeftAnalogDown) ? gamepad->mapDpadDown->buttonMask : 0);
	}
	if (pinTiltLeftAnalogLeft != (uint8_t)-1) {
		tiltLeftState |= ((values & maskTiltLeftAnalogLeft) ? gamepad->mapDpadLeft->buttonMask : 0);
	}
	if (pinTiltLeftAnalogRight != (uint8_t)-1) {
		tiltLeftState |= ((values & maskTiltLeftAnalogRight) ? gamepad->mapDpadRight->buttonMask : 0);
	}

	tiltRightState = 0;
	if (pinTiltRightAnalogUp != (uint8_t)-1) {
		tiltRightState |= ((values & maskTiltRightAnalogUp) ? gamepad->mapDpadUp->buttonMask : 0);
	}
	if (pinTiltRightAnalogDown != (uint8_t)-1) {
		tiltRightState |= ((values & maskTiltRightAnalogDown) ? gamepad->mapDpadDown->buttonMask : 0);
	}
	if (pinTiltRightAnalogLeft != (uint8_t)-1) {
		tiltRightState |= ((values & maskTiltRightAnalogLeft) ? gamepad->mapDpadLeft->buttonMask : 0);
	}
	if (pinTiltRightAnalogRight != (uint8_t)-1) {
		tiltRightState |= ((values & maskTiltRightAnalogRight) ? gamepad->mapDpadRight->buttonMask : 0);
	}

	// Debounce our directional pins
	debounce();

	// Convert gamepad from process() output to uint8 value
	uint8_t gamepadState = gamepad->state.dpad;

	gamepad->state.dpad = gamepadState;
}

void TiltInput::process()
{
	SOCDTiltClean(tiltSOCDMode);

	Gamepad* gamepad = Storage::getInstance().GetGamepad();
	uint8_t tiltLeftOut = tiltLeftState;
	uint8_t tiltRightOut = tiltRightState;

	// Set Tilt Output
	OverrideGamepad(gamepad, tiltLeftOut, tiltRightOut);
}

void TiltInput::OverrideGamepad(Gamepad* gamepad, uint8_t dpad1, uint8_t dpad2) {

	// For left analog
	//if(pinButtonL2Pressed || pinButtonR2Pressed){
	//	if (pinTilt1Pressed) {
	//		gamepad->state.lx = dpadToAnalogX(dpad1) + (GAMEPAD_JOYSTICK_MID - dpadToAnalogX(dpad1)) * 0.3;
	//		gamepad->state.ly = dpadToAnalogY(dpad1) + (GAMEPAD_JOYSTICK_MID - dpadToAnalogY(dpad1)) * 0.3;
	//	}
	//	else if (pinTilt2Pressed) {
	//		gamepad->state.lx = dpadToAnalogX(dpad1) + (GAMEPAD_JOYSTICK_MID - dpadToAnalogX(dpad1)) * 0.27;
	//		gamepad->state.ly = dpadToAnalogY(dpad1) + (GAMEPAD_JOYSTICK_MID - dpadToAnalogY(dpad1)) * 0.27;
	//	}
	//	else {
	//		gamepad->state.lx = dpadToAnalogX(dpad1);
	//		gamepad->state.ly = dpadToAnalogY(dpad1);
	//	}
	//}
	if (values & maskTilt1) {
		gamepad->state.lx = dpadToAnalogX(dpad1) + (GAMEPAD_JOYSTICK_MID - dpadToAnalogX(dpad1)) * 0.35;
		gamepad->state.ly = dpadToAnalogY(dpad1) + (GAMEPAD_JOYSTICK_MID - dpadToAnalogY(dpad1)) * 0.45;
	}
	else if (values & maskTilt2) {
		gamepad->state.lx = dpadToAnalogX(dpad1) + (GAMEPAD_JOYSTICK_MID - dpadToAnalogX(dpad1)) * 0.65;
		gamepad->state.ly = dpadToAnalogY(dpad1) + (GAMEPAD_JOYSTICK_MID - dpadToAnalogY(dpad1)) * 0.35;
	}
	else {
		gamepad->state.lx = dpadToAnalogX(dpad1);
		gamepad->state.ly = dpadToAnalogY(dpad1);
	}

	// For right analog
//Hold Function button turn on Home, minus, plus, capture.
	if (values & maskTiltFunction) {
		gamepad->state.buttons |= (dpad2 & GAMEPAD_MASK_LEFT) ? GAMEPAD_MASK_S1 : 0;
		gamepad->state.buttons |= (dpad2 & GAMEPAD_MASK_DOWN) ? GAMEPAD_MASK_A1 : 0;
		gamepad->state.buttons |= (dpad2 & GAMEPAD_MASK_RIGHT) ? GAMEPAD_MASK_S2 : 0;
		gamepad->state.buttons |= (dpad2 & GAMEPAD_MASK_UP) ? GAMEPAD_MASK_A2 : 0;
	}
	else if ((values & maskTilt1) && (values & maskTilt2)) {
		gamepad->state.dpad = dpad2; //Hold tilt1 + tilt2 turn on D-Pad
	}
	else if (values & maskTilt1) {
		if (dpad2 & (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT)) {
			gamepad->state.rx = dpadToAnalogX(dpad2) + (GAMEPAD_JOYSTICK_MID - dpadToAnalogX(dpad2)) * 0.3;
			gamepad->state.ry = GAMEPAD_JOYSTICK_MID * 1.7;
		}
		else {
			gamepad->state.rx = dpadToAnalogX(dpad2);
			gamepad->state.ry = dpadToAnalogY(dpad2);
		}
	}
	else if (values & maskTilt2) {
		if (dpad2 & (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT)) {
			gamepad->state.rx = dpadToAnalogX(dpad2) + (GAMEPAD_JOYSTICK_MID - dpadToAnalogX(dpad2)) * 0.3;
			gamepad->state.ry = GAMEPAD_JOYSTICK_MID * 0.3;
		}
		else {
			gamepad->state.rx = dpadToAnalogX(dpad2);
			gamepad->state.ry = dpadToAnalogY(dpad2);
		}
	}
	else {
		gamepad->state.rx = dpadToAnalogX(dpad2);
		gamepad->state.ry = dpadToAnalogY(dpad2);
	}
}

void TiltInput::SOCDTiltClean(SOCDMode socdMode) {
	// Tilt SOCD Last-Win Clean
	switch (tiltLeftState & (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN)) {
	case (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN): // If last state was Up or Down, exclude it from our gamepad
		if (socdMode == SOCD_MODE_UP_PRIORITY) {
			tiltLeftState ^= GAMEPAD_MASK_DOWN; // Remove Down
			lastTiltUD = DIRECTION_UP; // We're in UP mode
		}
		else if (socdMode == SOCD_MODE_SECOND_INPUT_PRIORITY && lastTiltUD != DIRECTION_NONE) {
			tiltLeftState ^= (lastTiltUD == DIRECTION_UP) ? GAMEPAD_MASK_UP : GAMEPAD_MASK_DOWN;
		}
		else {
			tiltLeftState ^= (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN); // Remove UP and Down in Neutral
			lastTiltUD = DIRECTION_NONE;
		}
		break;
	case GAMEPAD_MASK_UP:
		lastTiltUD = DIRECTION_UP;
		break;
	case GAMEPAD_MASK_DOWN:
		lastTiltUD = DIRECTION_DOWN;
		break;
	default:
		lastTiltUD = DIRECTION_NONE;
		break;
	}
	switch (tiltLeftState & (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT)) {
	case (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT):
		if (socdMode == SOCD_MODE_UP_PRIORITY || socdMode == SOCD_MODE_NEUTRAL) {
			tiltLeftState ^= (GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT); // Remove L + R to Neutral
			lastTiltLR = DIRECTION_NONE;
		}
		else if (socdMode == SOCD_MODE_SECOND_INPUT_PRIORITY) {
			if (lastTiltLR != DIRECTION_NONE)
				tiltLeftState ^= (lastTiltLR == DIRECTION_LEFT) ? GAMEPAD_MASK_LEFT : GAMEPAD_MASK_RIGHT; // Last Win
			else
				lastTiltLR = DIRECTION_NONE;
		}
		break;
	case GAMEPAD_MASK_LEFT:
		lastTiltLR = DIRECTION_LEFT;
		break;
	case GAMEPAD_MASK_RIGHT:
		lastTiltLR = DIRECTION_RIGHT;
		break;
	default:
		lastTiltLR = DIRECTION_NONE;
		break;
	}
}

uint8_t TiltInput::gpadToBinary(DpadMode dpadMode, GamepadState state) {
	uint8_t out = 0;
	switch (dpadMode) { // Convert gamepad to tilt if we're in mixed
	case DPAD_MODE_DIGITAL:
		out = state.dpad;
		break;
	case 3:
		if (state.lx == GAMEPAD_JOYSTICK_MIN) {
			out = out | GAMEPAD_MASK_LEFT;
		}
		else if (state.lx == GAMEPAD_JOYSTICK_MAX) {
			out = out | GAMEPAD_MASK_RIGHT;
		}
		if (state.ly == GAMEPAD_JOYSTICK_MIN) {
			out = out | GAMEPAD_MASK_UP;
		}
		else if (state.ly == GAMEPAD_JOYSTICK_MAX) {
			out = out | GAMEPAD_MASK_DOWN;
		}
		break;
	case DPAD_MODE_RIGHT_ANALOG:
		if (state.rx == GAMEPAD_JOYSTICK_MIN) {
			out = out | GAMEPAD_MASK_LEFT;
		}
		else if (state.rx == GAMEPAD_JOYSTICK_MAX) {
			out = out | GAMEPAD_MASK_RIGHT;
		}
		if (state.ry == GAMEPAD_JOYSTICK_MIN) {
			out = out | GAMEPAD_MASK_UP;
		}
		else if (state.ry == GAMEPAD_JOYSTICK_MAX) {
			out = out | GAMEPAD_MASK_DOWN;
		}
		break;
	}

	return out;
}

