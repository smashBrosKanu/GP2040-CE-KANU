#ifndef _SplitControllerAddon_H
#define _SplitControllerAddon_H

#include <string>
#include <stdint.h>
#include <hardware/i2c.h>
#include "BoardConfig.h"
#include "gpaddon.h"
#include "gamepad.h"
#include "storagemanager.h"
#include "SplitController.h"

// SplitControllern Module Name
#define SplitControllernName "SplitControllern"

#ifndef SPLIT_CONTROLLER_ENABLED
#define SPLIT_CONTROLLER_ENABLED 1
#endif

#ifndef SPLIT_CONTROLLER_I2C_ADDR
#define SPLIT_CONTROLLER_I2C_ADDR 0x17
#endif

#ifndef SPLIT_CONTROLLER_I2C_SDA_PIN
#define SPLIT_CONTROLLER_I2C_SDA_PIN -1
#endif

#ifndef SPLIT_CONTROLLER_I2C_SCL_PIN
#define SPLIT_CONTROLLER_I2C_SCL_PIN -1
#endif

#ifndef SPLIT_CONTROLLER_I2C_BLOCK
#define SPLIT_CONTROLLER_I2C_BLOCK i2c1
#endif

#ifndef SPLIT_CONTROLLER_I2C_SPEED
#define SPLIT_CONTROLLER_I2C_SPEED 400000
#endif

class SplitControllernInput : public GPAddon {
public:
	virtual bool available();
	virtual void setup();       // SplitControllern Setup
	virtual void process();     // SplitControllern Process
	virtual void preprocess() {}
	virtual std::string name() { return SplitControllernName; }
private:
    SplitController * split;
};

#endif  // _SplitControllernAddon_H
