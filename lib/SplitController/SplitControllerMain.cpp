#include <i2c_fifo.h>
#include <i2c_slave.h>
#include "pico/stdlib.h"
#include <cstdio>
#include "tusb.h"
#include "SplitController.h"

#define I2C_ADDR  0x17
#define I2C_SDA   0
#define I2C_SCL   1
#define DEBOUNCE_DELAY 4 // Debounce delay in milliseconds
#define BUTTON_COUNT 32 // Update this based on your specific project
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
void print_binary(uint8_t byte) {
    for (int i = 7; i >= 0; --i)
        printf("%d", (byte >> i) & 1);
}

int main()  
{
    stdio_init_all();
    tusb_init();
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    //// Button setup
    //for (int i = 0; i < BUTTON_MASTER_PIN_COUNT; ++i) {
    //    gpio_init(BUTTON_MASTER_PINS[i]);
    //    gpio_set_dir(BUTTON_MASTER_PINS[i], GPIO_IN);
    //    gpio_pull_up(BUTTON_MASTER_PINS[i]);
    //}

    //ButtonDebouncer debouncer;

    while (1)
    {
        //buttonMasterState = gpio_get_all();

        uint8_t buf_i2c[4];
        int result = i2c_read_blocking(i2c0, I2C_ADDR, buf_i2c, 4, false);

        if (result < 0) {  // if an error occurred during the read operation
            printf("Failed to read data from slave. Error code: %d\n", result);
        }
        else if (result != 4) {  // if the read operation did not get 4 bytes
            printf("Failed to read expected number of bytes. Got %d bytes instead of 4.\n", result);
        }
        else {
            // Cast the byte array to a uint32_t
            uint32_t data = buf_i2c[0] | (buf_i2c[1] << 8) | (buf_i2c[2] << 16) | (buf_i2c[3] << 24);

            //debouncer.debounce(&data);

            printf("Slave: ");
            for (int i = 31; i >= 0; --i) {
                printf("%d", (data >> i) & 1);
            }
            printf("\n");

            //printf("Master: ");
            //for (int i = 31; i >= 0; --i) {
            //    printf("%d", (buttonMasterState >> i) & 1);
            //}
            //printf("\n");

            //printf("AND result: ");
            //for (int i = 31; i >= 0; --i) {
            //    printf("%d", ((data & buttonMasterState) >> i) & 1);
            //}
            //printf("\n");


            fflush(stdout);  // Make sure the output is printed immediately
        }

        sleep_ms(500);
    }

}
