#include <i2c_fifo.h>
#include <i2c_slave.h>
#include "pico/stdlib.h"
#include "SplitController.h"

#include <cstring>
#include <cstdio>

SplitController::SplitController(int sda, int scl, i2c_inst_t* i2cCtl, int32_t speed, uint8_t addr) {
	iSDA = sda;
	iSCL = scl;
	picoI2C = i2cCtl;
	bWire = bWire;
	iSpeed = speed;
	address = addr;
}

void SplitController::begin() {
	doI2CInit();
#if SPLIT_CONTROLLER_DEBUG==true
	printf("SplitController::begin\n");
#endif
	isReady = false;
	reset();
}

void SplitController::start() {
	uint8_t idRead[4];
	uint8_t regWrite[16];
	int8_t result;


	doI2CRead(idRead, 4);
	uint32_t slaveButtonState = idRead[0] | (idRead[1] << 8) | (idRead[2] << 16) | (idRead[3] << 24);
}

void SplitController::reset() {
//	uint8_t regWrite[16];
//	int8_t result;
//	bool canContinue = true;
//
//	if (canContinue) {
//		result = doI2C	Test();
//		canContinue = (result == 1);
//	}
//
//#if SPLIT_CONTROLLER_DEBUG==true
//	printf("SplitController::reset\n");
//#endif
//
//	if (canContinue) {
//		regWrite[0] = 0xF0;
//		regWrite[1] = 0x55;
//		result = doI2CWrite(regWrite, 2);
//		canContinue = (result > -1);				
//	}
//
//	if (canContinue) {
//		regWrite[0] = 0xFB;
//		regWrite[1] = 0x00;
//		result = doI2CWrite(regWrite, 2);
//		canContinue = (result > -1);
//	}
//
//	if (canContinue) {
//		// set data format
//		regWrite[0] = 0xFE;
//		regWrite[1] = 0x03;
//		result = doI2CWrite(regWrite, 2);
//		canContinue = (result > -1);
//	}
//
//#if SPLIT_CONTROLLER_DEBUG==true
//	printf("SplitController::reset canContinue? %1d\n", canContinue);
//#endif
//
//	if (canContinue) {
//#if SPLIT_CONTROLLER_DEBUG==true
//		//printf("Reset Sent\n");
//#endif
//		isReady = true;
//	}
//	else {
//#if SPLIT_CONTROLLER_DEBUG==true
//		//printf("Device not found\n");
//#endif
//	}
}

void SplitController::poll() {
	uint8_t reg = _u(0x08);
	uint8_t regWrite[16];
	uint8_t regRead[16];
	int8_t result;

#if SPLIT_CONTROLLER_DEBUG==true
	//printf("SplitController::poll\n");
	//printf("SplitController::poll isReady? %1d\n", isReady);
#endif

//	if (!isReady) return;
//
//	if (extensionType != SPLIT_CONTROLLER_NONE) {
//		switch (dataType) {
//		case SPLIT_DATA_TYPE_1:
//			result = doI2CRead(regRead, 6);
//			break;
//		case SPLIT_DATA_TYPE_2:
//			result = doI2CRead(regRead, 9);
//			break;
//		case SPLIT_DATA_TYPE_3:
//			result = doI2CRead(regRead, 8);
//			break;
//		default:
//			// unknown. TBD
//			result = -1;
//#if SPLIT_CONTROLLER_DEBUG==true
//			printf("SplitController::poll Unknown data type: %1d\n", dataType);
//#endif
//			break;
//		}
//
//		if (result > 0) {
//			switch (extensionType) {
//			case SPLIT_CONTROLLER_NUNCHUCK:
//				joy1X = (regRead[0] & 0xFF);
//				joy1Y = (regRead[1] & 0xFF);
//
//				accelX = (((regRead[2] << 2) | ((regRead[5] >> 2) & 0x03)));
//				accelY = (((regRead[3] << 2) | ((regRead[5] >> 4) & 0x03)));
//				accelZ = (((regRead[4] << 2) | ((regRead[5] >> 6) & 0x03)));
//				buttonZ = (!(regRead[5] & 0x01));
//				buttonC = (!(regRead[5] & 0x02));
//
//#if SPLIT_CONTROLLER_DEBUG==true
//				printf("Joy X=%4d Y=%4d   Acc X=%4d Y=%4d Z=%4d   Btn Z=%1d C=%1d\n", joy1X, joy1Y, accelX, accelY, accelZ, buttonZ, buttonC);
//#endif
//
//				break;
//			case SPLIT_CONTROLLER_CLASSIC:
//			case SPLIT_CONTROLLER_CLASSIC_PRO:
//				// write data format to return
//				// see wiki for data types
//				if (dataType == SPLIT_DATA_TYPE_1) {
//					joy1X = (regRead[0] & 0x3F);
//					joy1Y = (regRead[1] & 0x3F);
//					joy2X = ((regRead[0] & 0xC0) >> 3) | ((regRead[1] & 0xC0) >> 5) | ((regRead[2] & 0x80) >> 7);
//					joy2Y = (regRead[2] & 0x1F);
//
//					triggerLeft = (((regRead[2] & 0x60) >> 2) | ((regRead[3] & 0xE0) >> 5));
//					triggerRight = ((regRead[3] & 0x1F) >> 0);
//
//					directionRight = !((regRead[4] & 0x80) >> 7);
//					directionDown = !((regRead[4] & 0x40) >> 6);
//					buttonLT = !((regRead[4] & 0x20) >> 5);
//					buttonMinus = !((regRead[4] & 0x10) >> 4);
//					buttonHome = !((regRead[4] & 0x08) >> 3);
//					buttonPlus = !((regRead[4] & 0x04) >> 2);
//					buttonRT = !((regRead[4] & 0x02) >> 1);
//
//					buttonZL = !((regRead[5] & 0x80) >> 7);
//					buttonB = !((regRead[5] & 0x40) >> 6);
//					buttonY = !((regRead[5] & 0x20) >> 5);
//					buttonA = !((regRead[5] & 0x10) >> 4);
//					buttonX = !((regRead[5] & 0x08) >> 3);
//					buttonZR = !((regRead[5] & 0x04) >> 2);
//					directionLeft = !((regRead[5] & 0x02) >> 1);
//					directionUp = !((regRead[5] & 0x01) >> 0);
//				}
//				else if (dataType == SPLIT_DATA_TYPE_2) {
//					joy1X = ((regRead[0] << 2) | ((regRead[4] & 0x03) >> 0));
//					joy1Y = ((regRead[2] << 2) | ((regRead[4] & 0x30) >> 4));
//					joy2X = ((regRead[1] << 2) | ((regRead[4] & 0x0C) >> 2));
//					joy2Y = ((regRead[3] << 2) | ((regRead[4] & 0xC0) >> 6));
//
//					triggerLeft = (regRead[5] & 0xFF);
//					triggerRight = (regRead[6] & 0xFF);
//
//					directionRight = !((regRead[7] & 0x80) >> 7);
//					directionDown = !((regRead[7] & 0x40) >> 6);
//					buttonLT = !((regRead[7] & 0x20) >> 5);
//					buttonMinus = !((regRead[7] & 0x10) >> 4);
//					buttonHome = !((regRead[7] & 0x08) >> 3);
//					buttonPlus = !((regRead[7] & 0x04) >> 2);
//					buttonRT = !((regRead[7] & 0x02) >> 1);
//
//					buttonZL = !((regRead[8] & 0x80) >> 7);
//					buttonB = !((regRead[8] & 0x40) >> 6);
//					buttonY = !((regRead[8] & 0x20) >> 5);
//					buttonA = !((regRead[8] & 0x10) >> 4);
//					buttonX = !((regRead[8] & 0x08) >> 3);
//					buttonZR = !((regRead[8] & 0x04) >> 2);
//					directionLeft = !((regRead[8] & 0x02) >> 1);
//					directionUp = !((regRead[8] & 0x01) >> 0);
//				}
//				else if (dataType == SPLIT_DATA_TYPE_3) {
//					joy1X = (regRead[0] & 0xFF);
//					joy1Y = (regRead[2] & 0xFF);
//					joy2X = (regRead[1] & 0xFF);
//					joy2Y = (regRead[3] & 0xFF);
//
//					triggerLeft = (regRead[4] & 0xFF);
//					triggerRight = (regRead[5] & 0xFF);
//
//					directionRight = !((regRead[6] & 0x80) >> 7);
//					directionDown = !((regRead[6] & 0x40) >> 6);
//					buttonLT = !((regRead[6] & 0x20) >> 5);
//					buttonMinus = !((regRead[6] & 0x10) >> 4);
//					buttonHome = !((regRead[6] & 0x08) >> 3);
//					buttonPlus = !((regRead[6] & 0x04) >> 2);
//					buttonRT = !((regRead[6] & 0x02) >> 1);
//
//					buttonZL = !((regRead[7] & 0x80) >> 7);
//					buttonB = !((regRead[7] & 0x40) >> 6);
//					buttonY = !((regRead[7] & 0x20) >> 5);
//					buttonA = !((regRead[7] & 0x10) >> 4);
//					buttonX = !((regRead[7] & 0x08) >> 3);
//					buttonZR = !((regRead[7] & 0x04) >> 2);
//					directionLeft = !((regRead[7] & 0x02) >> 1);
//					directionUp = !((regRead[7] & 0x01) >> 0);
//				}
//				else {
//					// unknown
//				}
//
//#if SPLIT_CONTROLLER_DEBUG==true
//				//if ((_lastRead[0] != regRead[0]) || (_lastRead[1] != regRead[1]) || (_lastRead[2] != regRead[2]) || (_lastRead[3] != regRead[3])) {
//				printf("Joy1 X=%4d Y=%4d  Joy2 X=%4d Y=%4d\n", joy1X, joy1Y, joy2X, joy2Y);
//				//}
//				//printf("Joy1 X=%4d Y=%4d  Joy2 X=%4d Y=%4d  U=%1d D=%1d L=%1d R=%1d TL=%4d TR=%4d\n", joy1X, joy1Y, joy2X, joy2Y, directionUp, directionDown, directionLeft, directionRight, triggerLeft, triggerRight);
//				//printf("A=%1d B=%1d X=%1d Y=%1d ZL=%1d ZR=%1d LT=%1d RT=%1d -=%1d H=%1d +=%1d\n", buttonA, buttonB, buttonX, buttonY, buttonZL, buttonZR, buttonLT, buttonRT, buttonMinus, buttonHome, buttonPlus);
//#endif
//
//				break;
//			case SPLIT_CONTROLLER_GUITAR:
//				if (dataType == SPLIT_DATA_TYPE_1) {
//					joy1X = (regRead[0] & 0x3F);
//					joy1Y = (regRead[1] & 0x3F);
//
//					whammyBar = (regRead[3] & 0x1F);
//					joy2X = (regRead[3] & 0x1F);
//
//					directionDown = !((regRead[4] & 0x40) >> 6);
//					buttonMinus = !((regRead[4] & 0x10) >> 4);
//					buttonPlus = !((regRead[4] & 0x04) >> 2);
//
//					fretOrange = !((regRead[5] & 0x80) >> 7);
//					fretRed = !((regRead[5] & 0x40) >> 6);
//					fretBlue = !((regRead[5] & 0x20) >> 5);
//					fretGreen = !((regRead[5] & 0x10) >> 4);
//					fretYellow = !((regRead[5] & 0x08) >> 3);
//					pedalButton = !((regRead[5] & 0x04) >> 2);
//					directionUp = !((regRead[5] & 0x01) >> 0);
//				}
//				else if (dataType == SPLIT_DATA_TYPE_2) {
//					joy1X = ((regRead[0] << 2) | ((regRead[4] & 0x03) >> 0));
//					joy1Y = ((regRead[2] << 2) | ((regRead[4] & 0x30) >> 4));
//
//					// analog 2 is not used but appears to not change?
//					//joy2X =          ((regRead[1] << 2) | ((regRead[4] & 0x0C) >> 2));
//					//joy2Y =          ((regRead[3] << 2) | ((regRead[4] & 0xC0) >> 6));
//
//					whammyBar = (regRead[6] & 0xFF);
//					joy2X = (regRead[6] & 0xFF);
//
//					directionDown = !((regRead[7] & 0x40) >> 6);
//					buttonMinus = !((regRead[7] & 0x10) >> 4);
//					buttonPlus = !((regRead[7] & 0x04) >> 2);
//
//					fretOrange = !((regRead[8] & 0x80) >> 7);
//					fretRed = !((regRead[8] & 0x40) >> 6);
//					fretBlue = !((regRead[8] & 0x20) >> 5);
//					fretGreen = !((regRead[8] & 0x10) >> 4);
//					fretYellow = !((regRead[8] & 0x08) >> 3);
//					pedalButton = !((regRead[8] & 0x04) >> 2);
//					directionUp = !((regRead[8] & 0x01) >> 0);
//				}
//				else if (dataType == SPLIT_DATA_TYPE_3) {
//					joy1X = (regRead[0] & 0xFF);
//					joy1Y = (regRead[2] & 0xFF);
//
//					// analog 2 is not used but appears to not change?
//					//joy2X =          regRead[1];
//					//joy2Y =          regRead[3];
//
//					whammyBar = (regRead[5] & 0xFF);
//					joy2X = (regRead[5] & 0xFF);
//
//					directionDown = !((regRead[6] & 0x40) >> 6);
//					buttonMinus = !((regRead[6] & 0x10) >> 4);
//					buttonPlus = !((regRead[6] & 0x04) >> 2);
//
//					fretOrange = !((regRead[7] & 0x80) >> 7);
//					fretRed = !((regRead[7] & 0x40) >> 6);
//					fretBlue = !((regRead[7] & 0x20) >> 5);
//					fretGreen = !((regRead[7] & 0x10) >> 4);
//					fretYellow = !((regRead[7] & 0x08) >> 3);
//					pedalButton = !((regRead[7] & 0x04) >> 2);
//					directionUp = !((regRead[7] & 0x01) >> 0);
//				}
//
//#if SPLIT_CONTROLLER_DEBUG==true
//				//                printf("Joy1 X=%4d Y=%4d  Whammy=%4d  U=%1d D=%1d -=%1d +=%1d\n", joy1X, joy1Y, whammyBar, directionUp, directionDown, buttonMinus, buttonPlus);
//				//                printf("Joy1 X=%4d Y=%4d  Whammy=%4d  U=%1d D=%1d -=%1d +=%1d\n", joy1X, joy1Y, whammyBar, directionUp, directionDown, buttonMinus, buttonPlus);
//				//                printf("O=%1d B=%1d Y=%1d R=%1d G=%1d\n", fretOrange, fretBlue, fretYellow, fretRed, fretGreen);
//#endif
//				break;
//			case SPLIT_CONTROLLER_TAIKO:
//				if (dataType == SPLIT_DATA_TYPE_1) {
//					drumLeft = !((regRead[5] & 0x40) >> 6);
//					rimLeft = !((regRead[5] & 0x20) >> 5);
//					drumRight = !((regRead[5] & 0x10) >> 4);
//					rimRight = !((regRead[5] & 0x08) >> 3);
//				}
//				else if (dataType == SPLIT_DATA_TYPE_2) {
//					drumLeft = !((regRead[8] & 0x40) >> 6);
//					rimLeft = !((regRead[8] & 0x20) >> 5);
//					drumRight = !((regRead[8] & 0x10) >> 4);
//					rimRight = !((regRead[8] & 0x08) >> 3);
//				}
//				else if (dataType == SPLIT_DATA_TYPE_3) {
//					drumLeft = !((regRead[7] & 0x40) >> 6);
//					rimLeft = !((regRead[7] & 0x20) >> 5);
//					drumRight = !((regRead[7] & 0x10) >> 4);
//					rimRight = !((regRead[7] & 0x08) >> 3);
//				}
//
//#if SPLIT_CONTROLLER_DEBUG==true
//				//if (_lastRead[0] != regRead[0]) printf("Byte0    " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(regRead[0]));
//				//if (_lastRead[1] != regRead[1]) printf("Byte1    " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(regRead[1]));
//				//if (_lastRead[2] != regRead[2]) printf("Byte2    " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(regRead[2]));
//				//if (_lastRead[3] != regRead[3]) printf("Byte3    " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(regRead[3]));
//				//if (_lastRead[4] != regRead[4]) printf("Byte4    " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(regRead[4]));
//				//if (_lastRead[5] != regRead[5]) printf("Byte5    " BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(regRead[5]));
//				//
//				//if (_lastRead[7] != regRead[7]) {
//				//    printf("DL=%1d RL=%1d DR=%1d RR=%1d\n", drumLeft, rimLeft, drumRight, rimRight);
//				//}
//#endif
//
//				break;
//			}
//
//			// calibrate and remap
//			joy1X = map(
//				calibrate(joy1X, _minX1, _maxX1, _cenX1),
//				0 + _minX1,
//				(_analogPrecision1From - _maxX1),
//				0,
//				(_analogPrecision1To - 1)
//			);
//			joy1Y = map(
//				calibrate(joy1Y, _minY1, _maxY1, _cenY1),
//				0 + _minY1,
//				(_analogPrecision1From - _maxY1),
//				0,
//				(_analogPrecision1To - 1)
//			);
//
//			joy2X = map(
//				calibrate(joy2X, _minX2, _maxX2, _cenX2),
//				0 + _minX2,
//				(_analogPrecision2From - _maxX2),
//				0,
//				(_analogPrecision2To - 1)
//			);
//			joy2Y = map(
//				calibrate(joy2Y, _minY2, _maxY2, _cenY2),
//				0 + _minY2,
//				(_analogPrecision2From - _maxY2),
//				0,
//				(_analogPrecision2To - 1)
//			);
//
//			triggerLeft = map(
//				triggerLeft,
//				0,
//				(_triggerPrecision1From - 1),
//				0,
//				(_triggerPrecision1To - 1)
//			);
//			triggerRight = map(
//				triggerRight,
//				0,
//				(_triggerPrecision2From - 1),
//				0,
//				(_triggerPrecision2To - 1)
//			);
//
//#if SPLIT_CONTROLLER_DEBUG==true
//			//if ((_lastRead[0] != regRead[0]) || (_lastRead[1] != regRead[1]) || (_lastRead[2] != regRead[2]) || (_lastRead[3] != regRead[3])) {
//			//    printf("Joy1 X=%4d Y=%4d  Joy2 X=%4d Y=%4d\n", joy1X, joy1Y, joy2X, joy2Y);
//			//}
//			//printf("Joy1 X=%4d Y=%4d  Joy2 X=%4d Y=%4d  U=%1d D=%1d L=%1d R=%1d TL=%4d TR=%4d\n", joy1X, joy1Y, joy2X, joy2Y, directionUp, directionDown, directionLeft, directionRight, triggerLeft, triggerRight);
//			//printf("A=%1d B=%1d X=%1d Y=%1d ZL=%1d ZR=%1d LT=%1d RT=%1d -=%1d H=%1d +=%1d\n", buttonA, buttonB, buttonX, buttonY, buttonZL, buttonZR, buttonLT, buttonRT, buttonMinus, buttonHome, buttonPlus);
//			for (int i = 0; i < result; ++i) {
//				_lastRead[i] = regRead[i];
//			}
//#endif
//			// continue poll
//			regWrite[0] = 0x00;
//			result = doI2CWrite(regWrite, 1);
//		}
//		else {
//			// device disconnected or invalid read
//			extensionType = SPLIT_CONTROLLER_NONE;
//			reset();
//			start();
//		}
//	}
//	else {
//		reset();
//		start();
//	}
}

uint16_t SplitController::map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t SplitController::calibrate(uint16_t pos, uint16_t min, uint16_t max, uint16_t cen) {
	uint16_t result;

#if SPLIT_CONTROLLER_CALIBRATION==true
	if (pos >= min && pos <= max) {
		result = pos;
	}
	else {
		if (pos < min) {
			result = min;
		}
		else if (pos > max) {
			result = max;
		}
		else {
			result = cen;
		}
	}
#else
	result = pos;
#endif

	return result;
}

int SplitController::doI2CWrite(uint8_t* pData, int iLen) {
	int result = i2c_write_blocking(picoI2C, address, pData, iLen, false);
	waitUntil_us(SPLIT_CONTROLLER_DELAY);
	return result;
}

int SplitController::doI2CRead(uint8_t* pData, int iLen) {
	int result = i2c_read_blocking(picoI2C, address, pData, iLen, false);
	waitUntil_us(SPLIT_CONTROLLER_DELAY);
	return result;
}

uint8_t SplitController::doI2CTest() {
	int result;
	uint8_t rxdata;
	result = doI2CRead(&rxdata, 1);
	return (result >= 0);
}

void SplitController::doI2CInit() {
	if ((iSDA + 2 * i2c_hw_index(picoI2C)) % 4 != 0) return;
	if ((iSCL + 3 + 2 * i2c_hw_index(picoI2C)) % 4 != 0) return;

	i2c_init(picoI2C, iSpeed);
	gpio_set_function(iSDA, GPIO_FUNC_I2C);
	gpio_set_function(iSCL, GPIO_FUNC_I2C);
	gpio_pull_up(iSDA);
	gpio_pull_up(iSCL);

	return;
}

void SplitController::waitUntil_us(uint64_t us) {
	SplitController_alarmFired = false;

	// Enable the interrupt for our alarm (the timer outputs 4 alarm irqs)
	hw_set_bits(&timer_hw->inte, 1u << SPLIT_ALARM_NUM);
	// Set irq handler for alarm irq
	irq_set_exclusive_handler(SPLIT_ALARM_IRQ, alarmIRQ);
	// Enable the alarm irq
	irq_set_enabled(SPLIT_ALARM_IRQ, true);
	// Enable interrupt in block and at processor

	// Alarm is only 32 bits so if trying to delay more
	// than that need to be careful and keep track of the upper
	// bits
	uint64_t target = timer_hw->timerawl + us;

	// Write the lower 32 bits of the target time to the alarm which
	// will arm it
	timer_hw->alarm[SPLIT_ALARM_NUM] = (uint32_t)target;

	while (!SplitController_alarmFired);
}

void SplitController::alarmIRQ() {
	// Clear the alarm irq
	hw_clear_bits(&timer_hw->intr, 1u << SPLIT_ALARM_NUM);

	// Assume alarm 0 has fired
	SplitController_alarmFired = true;
}


//#define BUTTON_MASTER_PIN_COUNT 24
//const uint BUTTON_MASTER_PINS[BUTTON_MASTER_PIN_COUNT] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 26, 27, 28 };

//uint32_t buttonMasterState = 0;

//class ButtonDebouncer {
//public:
//    ButtonDebouncer() : debounceState(0) {
//        for (int i = 0; i < BUTTON_COUNT; i++) {
//            lastDebounceTime[i] = 0;
//        }
//    }
//    void debounce(uint32_t* state);
//
//private:
//    uint32_t debounceState;
//    uint32_t lastDebounceTime[BUTTON_COUNT];
//};
//
//void ButtonDebouncer::debounce(uint32_t* state)
//{
//    uint32_t now = to_ms_since_boot(get_absolute_time());
//
//    for (int i = 0; i < BUTTON_COUNT; i++) {
//        if (((debounceState & (1 << i)) != (*state & (1 << i))) && (now - lastDebounceTime[i]) > DEBOUNCE_DELAY)
//        {
//            debounceState ^= (1 << i);
//            lastDebounceTime[i] = now;
//        }
//    }
//
//    *state = debounceState;
//}

// Function to convert byte to binary string
//void print_binary(uint8_t byte) {
//    for (int i = 7; i >= 0; --i)
//        printf("%d", (byte >> i) & 1);
//}
//
//int main()  
//{
//    stdio_init_all();
//    //tusb_init();
//    i2c_init(i2c0, 400 * 1000);
//    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
//    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
//    gpio_pull_up(I2C_SDA);
//    gpio_pull_up(I2C_SCL);
//
//    //// Button setup
//    //for (int i = 0; i < BUTTON_MASTER_PIN_COUNT; ++i) {
//    //    gpio_init(BUTTON_MASTER_PINS[i]);
//    //    gpio_set_dir(BUTTON_MASTER_PINS[i], GPIO_IN);
//    //    gpio_pull_up(BUTTON_MASTER_PINS[i]);
//    //}
//
//    //ButtonDebouncer debouncer;
//
//    while (1)
//    {
//        //buttonMasterState = gpio_get_all();
//
//        uint8_t buf_i2c[4];
//        int result = i2c_read_blocking(i2c0, I2C_ADDR, buf_i2c, 4, false);
//
//        if (result < 0) {  // if an error occurred during the read operation
//            //printf("Failed to read data from slave. Error code: %d\n", result);
//        }
//        else if (result != 4) {  // if the read operation did not get 4 bytes
//            //printf("Failed to read expected number of bytes. Got %d bytes instead of 4.\n", result);
//        }
//        else {
//            // Cast the byte array to a uint32_t
//            uint32_t data = buf_i2c[0] | (buf_i2c[1] << 8) | (buf_i2c[2] << 16) | (buf_i2c[3] << 24);
//
//            //debouncer.debounce(&data);
//
//            //printf("Slave: ");
//            //for (int i = 31; i >= 0; --i) {
//            //    printf("%d", (data >> i) & 1);
//            //}
//            //printf("\n");
//
//            //printf("Master: ");
//            //for (int i = 31; i >= 0; --i) {
//            //    printf("%d", (buttonMasterState >> i) & 1);
//            //}
//            //printf("\n");
//
//            //printf("AND result: ");
//            //for (int i = 31; i >= 0; --i) {
//            //    printf("%d", ((data & buttonMasterState) >> i) & 1);
//            //}
//            //printf("\n");
//
//
//            //fflush(stdout);  // Make sure the output is printed immediately
//        }
//
//        sleep_ms(500);
//    }
//
//}
