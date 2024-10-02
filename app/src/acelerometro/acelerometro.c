#include "acelerometro.h"

int open_physical(int fd)
{
	if (fd == -1)
	{
		if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1)
		{
			printf("ERROR: could not open \"/dev/mem\"...\n");
			return (-1);
		}
	}

	return fd;
}

void *map_physical(int fd, unsigned int base, unsigned int span)
{
	void *virtual_base;

	// Get a mapping from physical addresses to virtual addresses
	virtual_base = mmap(NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
	if (virtual_base == NULL)
	{
		printf("ERROR: mmap() failed...\n");
		close(fd);
		return (NULL);
	}

	return virtual_base;
}

int unmap_physical(void *virtual_base, unsigned int span)
{
	if (munmap(virtual_base, span) != 0)
	{
		printf("ERROR: munmap() failed...\n");
		return (-1);
	}
	return 0;
}

void close_physical(int fd)
{
	close(fd);
}

/* Set mux to connect ADXL345 to I2C0 Controller */
void mux_init()
{

	volatile unsigned int *gpio7_ptr, *gpio8_ptr, *i2c0fpga_ptr; // Mux pointer

	gpio7_ptr = sysmgr_base_ptr + SYSMGR_GENERALIO7;
	gpio8_ptr = sysmgr_base_ptr + SYSMGR_GENERALIO8;
	i2c0fpga_ptr = sysmgr_base_ptr + SYSMGR_I2C0USEFPGA;

	*i2c0fpga_ptr = 0;
	*gpio7_ptr = 1;
	*gpio8_ptr = 1;
	printf("gio7: %#x\ngio8: %#x\ni2c0fpga: %#x\n", *gpio7_ptr, *gpio8_ptr, *i2c0fpga_ptr);
}

/* Enabling I2C0 with polling */
int I2C0_onoff(unsigned int onoff)
{
	int ti2c_poll = 2500;
	int MAX_T_POLL_COUNT = 100;
	int poll_count = 0;

	int good = 0;
	*(i2c0_base_ptr + I2C0_ENABLE) = onoff;

	while (((*(i2c0_base_ptr + I2C0_ENABLE_STATUS) & 0x1) == (onoff - 1)) & (poll_count < MAX_T_POLL_COUNT))
	{
		poll_count++;
		usleep(ti2c_poll);
		*(i2c0_base_ptr + I2C0_ENABLE) = onoff;
	}
	if (poll_count < 10)
		good = 1;

	return good;
}

int I2C0_Init()
{

	// Abort tranmission and disable Controller for config
	printf("%d\n%d\n", I2C0_TAR, I2C0_FS_SCL_HCNT * 1);
	printf("%p\n%p\n", i2c0_base_ptr, i2c0_base_ptr + (I2C0_FS_SCL_HCNT * 1));
	*(i2c0_base_ptr + I2C0_ENABLE) = 0x2;
	// Wait for disable status

	if (!(I2C0_onoff(2)))
	{
		printf("Unable to disable\n");
		return 0;
	}
	printf("Configuring\n");

	// 7-Bit addressing, FastMode (400kb/s), Master Mode
	*(i2c0_base_ptr + I2C0_CON) = 0x65;

	// Set target address to be ADXL345 devid 0x53 and 7bit addressing
	*(i2c0_base_ptr + I2C0_TAR) = 0x53;

	// Minimum period is to be 2.5us but minimum high period is 0.6us
	// and minimum low period is 1.3us so 0.3us is added to both.
	*(i2c0_base_ptr + I2C0_FS_SCL_HCNT) = 60 + 30;
	*(i2c0_base_ptr + I2C0_FS_SCL_LCNT) = 130 + 30;

	// Enable the controller
	printf("Renabling controller\n");
	if (!(I2C0_onoff(1)))
	{
		printf("Unable to enable\n");
		return 0;
	}
	printf("ic_tar: %#x\nic_con: %#x\n", *(i2c0_base_ptr + I2C0_TAR), *(i2c0_base_ptr + I2C0_CON));
	return 1;
}

/* Single Byte Read */
void ADXL345_REG_READ(uint8_t address, uint8_t *value)
{

	// Send address and start signal
	*(i2c0_base_ptr + I2C0_DATA_CMD) = address + 0x400;

	// send read signal
	*(i2c0_base_ptr + I2C0_DATA_CMD) = 0x100;

	// wait for response

	while (*(i2c0_base_ptr + I2C0_RXFLR) == 0)
	{
		continue;
	}
	*value = *(i2c0_base_ptr + I2C0_DATA_CMD) & 0xFF;
}

/* Single byte Write */
void ADXL345_REG_WRITE(uint8_t address, uint8_t value)
{

	*(i2c0_base_ptr + I2C0_DATA_CMD) = address + 0x400;
	*(i2c0_base_ptr + I2C0_DATA_CMD) = value;
}

/* Multiple Byte Write */
void ADXL345_REG_MULTI_READ(uint8_t address, uint8_t values[], uint8_t len)
{

	int i = 0;
	int nth_byte = 0;
	*(i2c0_base_ptr + I2C0_DATA_CMD) = address + 0x400;

	// send read signal multiple times to prevent overwritten data at
	// inconsistent times

	for (i = 0; i < len; i++)
		*(i2c0_base_ptr + I2C0_DATA_CMD) = 0x100;

	while (len)
	{
		if (*(i2c0_base_ptr + I2C0_RXFLR) > 0)
		{
			values[nth_byte] = *(i2c0_base_ptr + I2C0_DATA_CMD) & 0xFF;
			nth_byte++;
			len--;
		}
	}
}

void ADXL345_init()
{

	uint8_t data_format = 0x03;
	uint8_t bw_rate = 0x07;

	//+-16 range, 10 bits
	ADXL345_REG_WRITE(ADXL345_DATA_FORMAT, data_format);
	ADXL345_REG_WRITE(ADXL345_BW_RATE, bw_rate);

	// Using threshold for new data
	ADXL345_REG_WRITE(ADXL345_THRESH_ACT, 0x04);
	ADXL345_REG_WRITE(ADXL345_THRESH_INACT, 0x02);
	ADXL345_REG_WRITE(ADXL345_TIME_INACT, 0x02);
	ADXL345_REG_WRITE(ADXL345_ACT_INACT_CTL, 0xFF);
	ADXL345_REG_WRITE(ADXL345_INT_ENABLE, 0x18);

	// Reset Measurement config
	ADXL345_REG_WRITE(ADXL345_POWER_CTL, 0x00); // standby
	ADXL345_REG_WRITE(ADXL345_POWER_CTL, 0x08);
}

int ADXL345_IsDataReady()
{
	int bReady = 0;
	uint8_t data8;

	ADXL345_REG_READ(ADXL345_INT_SOURCE, &data8);
	if (data8 & ADXL345_ACTIVITY)
	{
		ADXL345_REG_READ(ADXL345_INT_SOURCE, &data8);
		if (data8 & ADXL345_DATAREADY)
		{
			bReady = 1;
		}
	}
	return bReady;
}

// Read acceleration data of all three axes

void ADXL345_XYZ_Read(int16_t szData16[3])
{
	uint8_t szData8[6];
	ADXL345_REG_MULTI_READ(0x32, (uint8_t *)&szData8, sizeof(szData8));

	szData16[0] = (szData8[1] << 8) | szData8[0];
	szData16[1] = (szData8[3] << 8) | szData8[2];
	szData16[2] = (szData8[5] << 8) | szData8[4];
}

void ADXL345_IdRead(uint8_t *pId)
{
	ADXL345_REG_READ(ADXL345_DEVID, pId);
}
