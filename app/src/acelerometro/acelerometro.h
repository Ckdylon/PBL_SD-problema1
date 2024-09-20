#	ifndef accel_H
#define accel_H
#include <stdint.h>
#include <stdbool.h>

/* Prototypes for functions used to access physical memory addresses */
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);

// SYSMGR Pin Muxing Registers
/* GENERALIO7 (trace_d6): 
    0 : Pin is connected to GPIO/LoanIO number 55. 
    1 : Pin is connected to Peripheral signal I2C0.SDA. 
    2 : Pin is connected to Peripheral signal SPIS1.SS0. 
    3 : Pin is connected to Peripheral signal TRACE.D6. */
#define SYSMGR_GENERALIO7       ((volatile unsigned int *) 0xffd0849C)
/* GENERALIO8 (trace_d7): 
    0 : Pin is connected to GPIO/LoanIO number 56. 
    1 : Pin is connected to Peripheral signal I2C0.SCL. 
    2 : Pin is connected to Peripheral signal SPIS1.MISO. 
    3 : Pin is connected to Peripheral signal TRACE.D7. */
#define SYSMGR_GENERALIO8       ((volatile unsigned int *) 0xffd084A0)
/* I2C0USEFPGA: 
    0 : I2C0 uses HPS Pins. 
    1 : I2C0 uses the FPGA Inteface. */
#define SYSMGR_I2C0USEFPGA      ((volatile unsigned int *) 0xffd08704)

/* I2C0 Controller Registers
#define I2C0_CON                ((volatile unsigned int *) 0xffc04000)
#define I2C0_TAR                ((volatile unsigned int *) 0xffc04004)
#define I2C0_DATA_CMD           ((volatile unsigned int *) 0xffc04010)
#define I2C0_RXFLR              ((volatile unsigned int *) 0xffc04078)
#define I2C0_ENABLE             ((volatile unsigned int *) 0xffc0406c)
#define I2C0_ENABLE_STATUS      ((volatile unsigned int *) 0xffc0409c)
#define I2C0_FS_SCL_HCNT        ((volatile unsigned int *) 0xffc0401c)
#define I2C0_FS_SCL_LCNT        ((volatile unsigned int *) 0xffc04020)
*/
// ADXL345 Register List
#define ADXL345_REG_DATA_FORMAT 	0x31
#define ADXL345_REG_THRESH_ACT		0x24
#define ADXL345_REG_THRESH_INACT	0x25
#define ADXL345_REG_TIME_INACT		0x26
#define ADXL345_REG_ACT_INACT_CTL	0x27
#define ADXL345_REG_INT_ENABLE  	0x2E  // default value: 0x00
#define ADXL345_REG_POWER_CTL   	0x2D
#define ADXL345_REG_INT_SOURCE  	0x30  // default value: 0x02

/* Bit values in DATA_FORMAT */
#define XL345_RANGE_16G            0x03
#define XL345_FULL_RESOLUTION      0x08
#define ADXL345_REG_BW_RATE     	0x2C


/* Bit values in BW_RATE   */
#define XL345_RATE_200        0x0b

/* Bit values in INT_ENABLE, INT_MAP, and INT_SOURCE are identical
   use these bit values to read or write any of these registers.        */
#define XL345_ACTIVITY             0x10
#define XL345_INACTIVITY           0x08

/* Bit values in POWER_CTL                                              */
#define XL345_STANDBY              0x00
#define XL345_MEASURE              0x08

// I2C0 Functions
void I2C0_Init();
void I2C0_Enable_FPGA_Access();

// ADXL345 Functions
int IniciarAcelerometro();
void ADXL345_Init();
bool ADXL345_WasActivityUpdated();
void ADXL345_XYZ_Read(int16_t szData16[3]);
void ADXL345_REG_READ(uint8_t address, uint8_t *value);
void ADXL345_REG_WRITE(uint8_t address, uint8_t value);
void ADXL345_REG_MULTI_READ(uint8_t address, uint8_t values[], uint8_t len);

// Pinmux Functions
void Pinmux_Config();

#endif