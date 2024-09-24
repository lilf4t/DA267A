#include "accelerometer.h"
#include "circular_buffer.h"
#include <math.h>

// If AD0 is LOW, the I2C address of the board will be 0x68. Otherwise, the address will be 0x69.
#define MPU6050_ADDR 0x68
/*
Register (Hex): 6B
Bit7 DEVICE_RESET
Bit6 SLEEP
Bit5 CYCLE
Bit4 -
Bit3 TEMP_DIS
Bit2, Bit1, Bit0 CLKSEL[2:0]
*/
#define MPU6050_PWR_MGMT_1 0x6B

/*
Register (Hex) 19
Bit7, Bit6, Bit5, Bit4, Bit3, Bit2, Bit1, Bit0: SMPLRT_DIV[7:0]
*/
#define MPU6050_SMPLRT_DIV 0x19
/*
From Datasheet, page 45:
Register (Hex): 75
Bit7 -
Bit6, Bit5, Bit4, Bit3, Bit2, Bit1: WHO_AM_I[6:1]
Bit0: -
*/
#define MPU6050_WHO_AM_I 0x75

/*
 * From Datasheet, page 29:
 * Register (Hex) 3B | Bit 7-0: ACCEL_XOUT[15:8]
 * Register (Hex) 3C | Bit 7-0: ACCEL_XOUT[7:0]
 * Register (Hex) 3D | Bit 7-0: ACCEL_YOUT[15:8]
 * Register (Hex) 3E | Bit 7-0: ACCEL_YOUT[7:0]
 * Register (Hex) 3F | Bit 7-0: ACCEL_ZOUT[15:8]
 * Register (Hex) 40 | Bit 7-0: ACCEL_ZOUT[7:0]
 */
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40
// #define MPU6050_ACCEL_LSB 16384.f

i2c_cmd_handle_t cmd;
esp_err_t res;

void init()
{
    // configure and install driver
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;             // ESP32 acts as master
    conf.sda_io_num = 33;                    // pin used for SDA
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE; // The SDA and SCL lines are active low, so they should be pulled up with resistors
    conf.scl_io_num = 32;                    // SCL pin number
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE; // enables pullup on SDA
    conf.master.clk_speed = 100000;          // Standard mode (100 Kbit/s)

    // configure I2C controller 0
    esp_err_t res = i2c_param_config(I2C_NUM_0, &conf);
    ESP_ERROR_CHECK(res);

    // install driver, no buffers needed in master mode nor special interrupts config
    res = i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
    ESP_ERROR_CHECK(res);

    // configure power mode
    // here we set all bits of the PWR_MGMT_1 register to 0
    // create command
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    // start command
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    // set address + write and check for ack
    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_WRITE, 1);
    ESP_ERROR_CHECK(res);
    // write the register address and check for ack
    res = i2c_master_write_byte(cmd, MPU6050_PWR_MGMT_1, 1);
    ESP_ERROR_CHECK(res);
    // write value of the regiter: 0, and check for ack
    res = i2c_master_write_byte(cmd, 0x00, 1);

    ESP_ERROR_CHECK(res);
    // end of command
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    // send the command, 1 second timeout
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(res);
    // delete command now that it's not needed
    i2c_cmd_link_delete(cmd);

    // set the sampling frequency
    // the sampling freq is gyro sampling freq / (1 + divider)
    // setting divider to 250 leads to sampling freq. of 32 Hz
    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);

    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_WRITE, 1); // WRITE bit set!
    ESP_ERROR_CHECK(res);

    res = i2c_master_write_byte(cmd, MPU6050_SMPLRT_DIV, 1); // write to SMPLRT_DIV
    ESP_ERROR_CHECK(res);

    res = i2c_master_write_byte(cmd, 250, 1); // set SMPLRT_DIV to 250
    ESP_ERROR_CHECK(res);

    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);

    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    // ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);
}

u_int32_t getMagnitude()
{
    double ACCEL_X = readAccelerometer(MPU6050_ACCEL_XOUT_H, MPU6050_ACCEL_XOUT_L);
    double ACCEL_Y = readAccelerometer(MPU6050_ACCEL_YOUT_H, MPU6050_ACCEL_YOUT_L);
    double ACCEL_Z = readAccelerometer(MPU6050_ACCEL_ZOUT_H, MPU6050_ACCEL_ZOUT_L);

    // Base 2 because of the binary numeral system
    u_int32_t returnVal = sqrt(pow(ACCEL_X, 2) + pow(ACCEL_Y, 2) + pow(ACCEL_Z, 2));

    return returnVal;
}

int16_t readAccelerometer(uint16_t highRegister, uint16_t lowRegister)
{
    // read the latest sampled temperature
    // we need to combine TEMP_OUT_L and TEMP_OUT_H into one 16-bit signed integer
    // and then coonvert that into C using the formula: t = temp_out /340 + 36.53
    // create a little buffer where to store the answer
    uint8_t buffer;
    // holder of the acceleration
    int16_t accRaw = 0;

    // read low register
    // send just the register number with no other data
    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);

    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_WRITE, 1); // WRITE bit set!
    ESP_ERROR_CHECK(res);

    res = i2c_master_write_byte(cmd, lowRegister, 1); // read low first
    ESP_ERROR_CHECK(res);

    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);

    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(res);

    i2c_cmd_link_delete(cmd);

    // wait a little
    vTaskDelay(pdMS_TO_TICKS(10));

    // now read the answer
    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);

    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_READ, 1); // READ bit set!
    ESP_ERROR_CHECK(res);

    res = i2c_master_read(cmd, &buffer, 1, I2C_MASTER_NACK);
    ESP_ERROR_CHECK(res);

    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);

    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(res);

    i2c_cmd_link_delete(cmd);
    accRaw = buffer;

    // read high register
    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);

    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_WRITE, 1); // WRITE bit set!
    ESP_ERROR_CHECK(res);

    res = i2c_master_write_byte(cmd, highRegister, 1); // read high
    ESP_ERROR_CHECK(res);

    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);

    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(res);

    i2c_cmd_link_delete(cmd);
    vTaskDelay(pdMS_TO_TICKS(10));

    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);

    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_READ, 1); // READ bit set!
    ESP_ERROR_CHECK(res);

    res = i2c_master_read(cmd, &buffer, 1, I2C_MASTER_NACK);
    ESP_ERROR_CHECK(res);

    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);

    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(res);

    i2c_cmd_link_delete(cmd);

    // combine high and low registers into a signed integer
    accRaw |= ((int16_t)buffer) << 8;
    //printf("acceleration raw is: %d\n", accRaw);
    return accRaw;
}