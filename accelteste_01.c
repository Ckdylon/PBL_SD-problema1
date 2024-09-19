#	ifndef accel_H
#define accel_H
//#include "ADXL345.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ADXL345.h"


// ADXL345 Functions
void ADXL345_Init();
void ADXL345_Calibrate();
bool ADXL345_IsDataReady();
bool ADXL345_WasActivityUpdated();
void ADXL345_XYZ_Read(int16_t szData16[3]);
void ADXL345_IdRead(uint8_t *pId);
void ADXL345_REG_READ(uint8_t address, uint8_t *value);
void ADXL345_REG_WRITE(uint8_t address, uint8_t value);
void ADXL345_REG_MULTI_READ(uint8_t address, uint8_t values[], uint8_t len);

// I2C0 Functions
void I2C0_Init();
void I2C0_Enable_FPGA_Access();

// Pinmux Functions
void Pinmux_Config();

void Pinmux_Config(){
        *SYSMGR_I2C0USEFPGA = 0;
        *SYSMGR_GENERALIO7 = 1;
        *SYSMGR_GENERALIO8 = 1;
 }

void I2C0_Init(){

 // Abort any ongoing transmits and disable I2C0.
 *I2C0_ENABLE = 2;

 // Wait until I2C0 is disabled
 while(((*I2C0_ENABLE_STATUS)&0x1) == 1){}

 // Configure the config reg with the desired setting (act as
 // a master, use 7bit addressing, fast mode (400kb/s)).
         *I2C0_CON = 0x65;

 // Set target address (disable special commands, use 7bit addressing)
         *I2C0_TAR = 0x53;

 // Set SCL high/low counts (Assuming default 100MHZ clock input toI2C0 Controller).
 // The minimum SCL high period is 0.6us, and the minimum SCL lowperiod is 1.3us,
 // However, the combined period must be 2.5us or greater, so add 0.3usto each.
         *I2C0_FS_SCL_HCNT = 60 + 30; // 0.6us + 0.3us
         *I2C0_FS_SCL_LCNT = 130 + 30; // 1.3us + 0.3us
         
 // Enable the controller
         *I2C0_ENABLE = 1;

 // Wait until controller is powered on
         while(((*I2C0_ENABLE_STATUS)&0x1) == 0){}
 }


void ADXL345_REG_READ(uint8_t address, uint8_t *value){

         // Send reg address (+0x400 to send START signal)
         *I2C0_DATA_CMD = address + 0x400;

         // Send read signal
         *I2C0_DATA_CMD = 0x100;

         // Read the response (first wait until RX buffer contains data)
         while (*I2C0_RXFLR == 0){}
         *value = *I2C0_DATA_CMD;

}

// Write value to internal register at address
void ADXL345_REG_WRITE(uint8_t address, uint8_t value){

    // Send reg address (+0x400 to send START signal)
    *I2C0_DATA_CMD = address + 0x400;

    // Send value
    *I2C0_DATA_CMD = value;
}

// Initialize the ADXL345 chip
 void ADXL345_Init(){

 // +- 16g range, full resolution
 ADXL345_REG_WRITE(ADXL345_REG_DATA_FORMAT, XL345_RANGE_16G |
XL345_FULL_RESOLUTION);

 // Output Data Rate: 200Hz
 ADXL345_REG_WRITE(ADXL345_REG_BW_RATE, XL345_RATE_200);

 // The DATA_READY bit is not reliable. It is updated at a much higher rate than the Data Rate
 // Use the Activity and Inactivity interrupts as indicators for new data.
 ADXL345_REG_WRITE(ADXL345_REG_THRESH_ACT, 0x04); //activity threshold
 ADXL345_REG_WRITE(ADXL345_REG_THRESH_INACT, 0x02); //inactivity threshold
 ADXL345_REG_WRITE(ADXL345_REG_TIME_INACT, 0x02); //time for inactivity
 ADXL345_REG_WRITE(ADXL345_REG_ACT_INACT_CTL, 0xFF); //Enables AC coupling for thresholds
 ADXL345_REG_WRITE(ADXL345_REG_INT_ENABLE, XL345_ACTIVITY | XL345_INACTIVITY ); //enable interrupts

 // stop measure
ADXL345_REG_WRITE(ADXL345_REG_POWER_CTL, XL345_STANDBY);

 // start measure
 ADXL345_REG_WRITE(ADXL345_REG_POWER_CTL, XL345_MEASURE);
 }


// Return true if there was activity since the last read (checks ACTIVITY bit).
bool ADXL345_WasActivityUpdated(){
        bool bReady = false;
    uint8_t data8;

    ADXL345_REG_READ(ADXL345_REG_INT_SOURCE,&data8);
    if (data8 & XL345_ACTIVITY)
        bReady = true;

    return bReady;
}

// Read acceleration data of all three axes
void ADXL345_XYZ_Read(int16_t szData16[3]){
    uint8_t szData8[6];
    ADXL345_REG_MULTI_READ(0x32, (uint8_t *)&szData8, sizeof(szData8));

    szData16[0] = (szData8[1] << 8) | szData8[0];
    szData16[1] = (szData8[3] << 8) | szData8[2];
    szData16[2] = (szData8[5] << 8) | szData8[4];
}

// Read multiple consecutive internal registers
void ADXL345_REG_MULTI_READ(uint8_t address, uint8_t values[], uint8_t len){

    // Send reg address (+0x400 to send START signal)
    *I2C0_DATA_CMD = address + 0x400;

    // Send read signal len times
    int i;
    for (i=0;i<len;i++)
        *I2C0_DATA_CMD = 0x100;

    // Read the bytes
    int nth_byte=0;
    while (len){
        if ((*I2C0_RXFLR) > 0){
            values[nth_byte] = *I2C0_DATA_CMD;
            nth_byte++;
            len--;
        }
    }
}

int main(void){
    uint8_t devid;
    int16_t mg_per_lsb = 4;
    int16_t XYZ[3];

    // Configure Pin Muxing
    Pinmux_Config();

    // Initialize I2C0 Controller
    I2C0_Init();

    // 0xE5 is read from DEVID(0x00) if I2C is functioning correctly
    ADXL345_REG_READ(0x00, &devid);

    // Correct Device ID
    if (devid == 0xE5){
        // Initialize accelerometer chip
        ADXL345_Init();

        while(1){
            if (ADXL345_WasActivityUpdated()){
                ADXL345_XYZ_Read(XYZ);
                printf("X=%d mg, Y=%d mg, Z=%d mg\n", XYZ[0]*mg_per_lsb, XYZ[1]*mg_per_lsb, XYZ[2]*mg_per_lsb);
            }
        }
    } else {
        printf("Incorrect device ID\n");
    }

    return 0;
}

#endif
                   
                                                                                                                                                                                          1,1  

