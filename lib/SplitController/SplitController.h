// WiiNunchuk Library
// category=Signal Input/Output

#ifndef _SPLITCONTROLLER_H_
#define _SPLITCONTROLLER_H_

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define SPLIT_CONTROLLER_NONE          -1
#define SPLIT_CONTROLLER_NUNCHUCK      0
#define SPLIT_CONTROLLER_CLASSIC       1
#define SPLIT_CONTROLLER_CLASSIC_PRO   2
#define SPLIT_CONTROLLER_DRAWSOME      3
#define SPLIT_CONTROLLER_GUITAR        4
#define SPLIT_CONTROLLER_DRUMS         5
#define SPLIT_CONTROLLER_TURNTABLE     6
#define SPLIT_CONTROLLER_TAIKO         7
#define SPLIT_CONTROLLER_UDRAW         8
#define SPLIT_CONTROLLER_BALANCE_BOARD 9
#define SPLIT_CONTROLLER_MOTION_PLUS   10

#define SPLIT_DATA_TYPE_0             0
#define SPLIT_DATA_TYPE_1             1
#define SPLIT_DATA_TYPE_2             2
#define SPLIT_DATA_TYPE_3             3

#define SPLIT_ANALOG_PRECISION_0      32
#define SPLIT_ANALOG_PRECISION_1      64
#define SPLIT_ANALOG_PRECISION_2      256
#define SPLIT_ANALOG_PRECISION_3      1024

#ifndef SPLIT_CONTROLLER_DEBUG
#define SPLIT_CONTROLLER_DEBUG false
#endif

#ifndef SPLIT_CONTROLLER_DELAY
#define SPLIT_CONTROLLER_DELAY 300
#endif

#ifndef SPLIT_CONTROLLER_TIMEOUT
#define SPLIT_CONTROLLER_TIMEOUT 2
#endif

#ifndef SPLIT_CONTROLLER_CALIBRATION
#define SPLIT_CONTROLLER_CALIBRATION false
#endif

#define SPLIT_ALARM_NUM 0
#define SPLIT_ALARM_IRQ TIMER_IRQ_0

static volatile bool WiiExtension_alarmFired;

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

class SplitController {
  protected:
	uint8_t address;
  public:
    int8_t extensionType = SPLIT_CONTROLLER_NONE;
    int8_t dataType = SPLIT_DATA_TYPE_0;

    bool isReady         = false;

    // Constructor 
	WiiExtension(int sda, int scl, i2c_inst_t *i2cCtl, int32_t speed, uint8_t addr);

    // Methods
    void begin();
	void reset();
  	void start();
  	void poll();
  private:
	
    uint8_t iSDA;
    uint8_t iSCL;
    uint8_t bWire;
    i2c_inst_t *picoI2C;

	int32_t iSpeed;

    int doI2CWrite(uint8_t *pData, int iLen);
    int doI2CRead(uint8_t *pData, int iLen);
    uint8_t doI2CTest();
    void doI2CInit();

    void waitUntil_us(uint64_t us);
    static void alarmIRQ();
};

#endif
