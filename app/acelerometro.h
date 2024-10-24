#ifndef I2C_H
#define I2C_H

#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

// i2c base address
#define I2C_BASE 0xFFC04000
#define I2C_SPAN 0x00000100
#define XL345_RANGE_16G 0b11
#define XL345_FULL_RESOLUTION 0x08
#define ADXL345_REG_DATA_FORMAT 0x31
#define ADXL345_REG_BW_RATE 0x2C
#define XL345_RATE_200 0x0B
#define ADXL345_REG_THRESH_ACT 0x24
#define ADXL345_REG_THRESH_INACT 0x25
#define ADXL345_REG_TIME_INACT 0x26
#define ADXL345_REG_ACT_INACT_CTL 0x27
#define ADXL345_REG_INT_ENABLE 0x2E
#define ADXL345_REG_POWER_CTL 0x2D
#define ADXL345_REG_INT_SOURCE 0x30
#define XL345_ACTIVITY 0x10
#define XL345_INACTIVITY 0x08 
#define XL345_STANDBY 0x00
#define XL345_MEASURE 0x08

// address of registers
#define I2C_CON 0x0
#define I2C_TAR 0x4
#define I2C_DATA_CMD 0x10
#define IC_FD_SCL_HCNT 0x1C
#define IC_FS_SCL_LCNT 0x20
#define IC_CLR_INTR 0x40
#define IC_ENABLE 0x6C
#define IC_TXFLR 0x74
#define IC_RXFLR 0x78
#define IC_ENABLE_STATUS 0x9C

// struct to hold mapped I2C register pointers
typedef struct {
	volatile int *i2c0_con;
	volatile int *i2c0_tar;
	volatile int *i2c0_data_cmd;
	volatile int *ic_fd_scl_hcnt;
	volatile int *ic_fs_scl_lcnt;
	volatile int *ic_clr_intr;
	volatile int *ic_enable;
	volatile int *ic_txflr;
	volatile int *ic_rxflr;
	volatile int *ic_enable_status;
} I2C_Registers;

int open_fd();

I2C_Registers map_i2c(int fd);

void unmap_i2c(void *i2c_virtual);

void I2C0_Init(I2C_Registers *regs);

void accelerometer_reg_multi_read(uint8_t address, uint8_t values[], uint8_t len, I2C_Registers *regs);

void accelerometer_init(I2C_Registers regs);

void accelerometer_reg_read(uint8_t address, uint8_t *value, I2C_Registers *regs);

void accelerometer_reg_write(uint8_t address, uint8_t value, I2C_Registers *regs);

void accelerometer_x_read(int16_t szData[2], I2C_Registers regs);

int accelerometer_isDataReady(I2C_Registers regs);

#endif
