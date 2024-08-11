/************************************************************************
	INCLUDES
************************************************************************/
#include "ade7880.h"
#include <fcntl.h>
#include <string.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>

/************************************************************************
	LOCAL DEFINES AND TYPES
************************************************************************/
/*#define DEBUG*/

/************************************************************************
	DECLARATIONS
************************************************************************/

/* Private Global Variables */

/* Private Functions */

/************************************************************************
	FUNCTIONS
************************************************************************/
/* Private Functions */

static uint8_t ADE7880_MirrorData(uint8_t data)
{
  uint8_t ret;
  uint8_t i;

  ret = 0;
  for(i=0; i<8; i++) {
    ret |= ((data>>i)&0x01)<<(7-i);
  }

  return ret;
}

/* Public Functions */

int8_t ADE7880_Init(char* devPath, int *device)
{
	int8_t status = ADE7880_ERROR;

#ifdef DEBUG
    printf("%s\n", __func__);
#endif

	*device = open(devPath, O_RDWR);
	if(*device != 0) {
		status = ADE7880_OK;
	}

	return status;
}

int8_t ADE7880_Close(int device)
{
	return close(device);
}

int8_t ADE7880_WriteRegister(int device, uint16_t address, uint32_t data, int bits)
{
	struct spi_ioc_transfer	xfer[XFER_LEN];
	int8_t status = ADE7880_ERROR;
	uint8_t tx[ADE7880_TRANSFER_SIZE] = {0};

	/* Create the message */
	memset(xfer, 0, sizeof xfer);
	memset(tx, 0, sizeof tx);
	tx[0] = ADE7880_WRITE_REG;
	tx[1] = (uint8_t)(address >> 8) & 0xFF;
	tx[2] = (uint8_t)address & 0xFF;
    switch (bits) {
      case 8:
        tx[3] = data & 0xFF;
        xfer[0].len = 4;
        break;
      case 16:
        tx[3] = (data >> 8) & 0xFF;
        tx[4] = data & 0xFF;
        xfer[0].len = 5;
        break;
      case 24:
        tx[3] = (data >> 16) & 0xFF;
        tx[4] = (data >> 8) & 0xFF;
        tx[5] = data & 0xFF;
        xfer[0].len = 6;
        break;
      case 32:
        tx[3] = (data >> 24) & 0xFF;
        tx[4] = (data >> 16) & 0xFF;
        tx[5] = (data >> 8) & 0xFF;
        tx[6] = data & 0xFF;
        xfer[0].len = 7;
        break;
      default:
        status = ADE7880_ERROR;
        goto error_write;
    }

	xfer[0].tx_buf = (unsigned long)tx;

	/* Send it */
#ifdef DEBUG
    printf("%s: 0x%x = 0x%x\n", __func__, address, data);
#endif
	status = ioctl(device, SPI_IOC_MESSAGE(1), xfer);

	if( status == xfer[0].len)
	{
		status = ADE7880_OK;
	}

error_write:
	return status;
}

int8_t ADE7880_ReadRegister(int device, uint16_t address, uint32_t *data, int bits)
{
	struct spi_ioc_transfer	xfer[XFER_LEN];
	int8_t status = ADE7880_ERROR;
	uint8_t tx_cmd[ADE7880_READ_CMD_SIZE] = {0};
	uint8_t rx[4] = {0};

	/* Create the message */
	memset(xfer, 0, sizeof xfer);
	tx_cmd[0] = ADE7880_READ_REG;
    tx_cmd[1] = (address >> 8) & 0xFF;
    tx_cmd[2] = address & 0xFF;

	/* First we send the address to be read. */
	xfer[0].tx_buf = (unsigned long)tx_cmd;
    xfer[0].bits_per_word = 8;
	xfer[0].len = ADE7880_READ_CMD_SIZE;

	/* Now we heard the response */
	xfer[1].rx_buf = (unsigned long)rx;
    xfer[1].bits_per_word = 8;
	xfer[1].len = bits / 8;

	status = ioctl(device, SPI_IOC_MESSAGE(2), xfer);

	if( status >= 0)
	{
      switch (bits) {
        case 8:
          *data = rx[0];
          break;
        case 16:
          *data = (rx[0] << 8) | rx[1];
          break;
        case 24:
          *data = (rx[0] << 16) | (rx[1] << 8) | rx[2];
          break;
        case 32:
          *data = (rx[0] << 24) | (rx[1] << 16) | (rx[2] << 8) | rx[3];
          break;
      }
	  status = ADE7880_OK;
    } 
#ifdef DEBUG
    printf("%s: 0x%x = 0x%x\n", __func__, address, *data);
#endif
	return status;
}

int8_t ADE7880_InitializeSPI(int device)
{
  uint8_t i = 0;

  for(i=0; i<3; i++) {
    ADE7880_WriteRegister(device, ADE7880_DUMMY, 0x00, 8);
  }
};

int8_t ADE7880_RamProtect(int device, uint8_t cmd)
{
  uint8_t value;

  if(cmd == ENABLE)
    value = 0x80;
  else
    value = 0x00;

  ADE7880_WriteRegister(device, 0xE7FE, 0xAD, 8);
  ADE7880_WriteRegister(device, 0xe7e3, value, 8);
} 

int8_t ADE7880_QuickSetup(int device)
{
  uint16_t gain;
  uint8_t i;
  uint32_t aux;

  ADE7880_ReadRegister(device, ADE7880_CONFIG, &aux, 16);
  aux &= 0xffff;
  aux |= 0x0080;
  ADE7880_WriteRegister(device, ADE7880_CONFIG, aux, 16);
  usleep(1000000);

  ADE7880_RamProtect(device, DISABLE);
  gain = (PHASE_VOLTAGES_GAIN << 6) | (NEUTRAL_CURRENTS_GAIN << 3) | PHASE_CURRENTS_GAIN;
  ADE7880_WriteRegister(device, ADE7880_Gain, gain, 16);

  /*ADE7880_WriteRegister(device, ADE7880_WTHR, WTHR_DEFAULT, 8);*/
  /*ADE7880_WriteRegister(device, ADE7880_VARTHR, VARTHR_DEFAULT, 8);*/
  /*ADE7880_WriteRegister(device, ADE7880_VATHR, VATHR_DEFAULT, 8);*/
  /*for (i=0; i<3; i++) {*/
  /*ADE7880_WriteRegister(device, ADE7880_VLEVEL, VLEVEL_DEFAULT, 32);*/
  /*}*/
  ADE7880_WriteRegister(device, ADE7880_COMPMODE, 0x0001, 16);
  ADE7880_WriteRegister(device, ADE7880_CFMODE, 0x70A0, 16);
  ADE7880_WriteRegister(device, ADE7880_LCYCMODE, 0x3f, 8);
  //ADE7880_WriteRegister(device, ADE7880_LCYCMODE, 0x38, 8);

  ADE7880_WriteRegister(device, ADE7880_AIGAIN, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_AVGAIN, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_BIGAIN, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_BVGAIN, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_CIGAIN, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_CVGAIN, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_NIGAIN, 0x00000000, 32);

  ADE7880_WriteRegister(device, ADE7880_APGAIN, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_AWATTOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_BPGAIN, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_BWATTOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_CPGAIN, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_CWATTOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_AIRMSOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_AVRMSOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_BIRMSOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_BVRMSOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_CIRMSOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_CVRMSOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_NIRMSOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_HPGAIN, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_ISUMLVL, 0x00000000, 32);

  ADE7880_WriteRegister(device, ADE7880_AFWATTOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_BFWATTOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_CFWATTOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_AFVAROS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_BFVAROS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_CFVAROS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_AFIRMSOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_BFIRMSOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_CFIRMSOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_HXWATTOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_HYWATTOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_HZWATTOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_HXVAROS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_HYVAROS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_HZVAROS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_HXIRMSOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_HYIRMSOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_HZIRMSOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_HXVRMSOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_HYVRMSOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_HZVRMSOS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_AIRMS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_AVRMS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_BIRMS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_BVRMS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_CIRMS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_CVRMS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_NIRMS, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_ISUM, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_ISUM, 0x00000000, 32);
  ADE7880_WriteRegister(device, ADE7880_ISUM, 0x00000000, 32);

  ADE7880_WriteRegister(device, ADE7880_WTHR, WTHR_DEFAULT, 8);
  ADE7880_WriteRegister(device, ADE7880_VARTHR, VARTHR_DEFAULT, 8);
  ADE7880_WriteRegister(device, ADE7880_VATHR, VATHR_DEFAULT, 8);
  ADE7880_WriteRegister(device, ADE7880_VLEVEL, VLEVEL_DEFAULT, 32);
  ADE7880_WriteRegister(device, ADE7880_VNOM, VNOM_DEFAULT, 32);
  ADE7880_WriteRegister(device, ADE7880_CF1DEN, CFXDEN_VAL, 16);
  ADE7880_WriteRegister(device, ADE7880_CF2DEN, CFXDEN_VAL, 16);
  ADE7880_WriteRegister(device, ADE7880_CF3DEN, CFXDEN_VAL, 16);
  /*ADE7880_WriteRegister(device, ADE7880_COMPMODE, 0x0001, 16);*/
  /*ADE7880_WriteRegister(device, ADE7880_CFMODE, 0x70A0, 16);*/
  /*ADE7880_WriteRegister(device, ADE7880_LCYCMODE, 0x3f, 8);*/

  ADE7880_RamProtect(device, ENABLE); 

  ADE7880_ReadRegister(device, ADE7880_AIGAIN, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_AVGAIN, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_BIGAIN, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_BVGAIN, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_CIGAIN, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_CVGAIN, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_NIGAIN, &aux, 32);

  ADE7880_ReadRegister(device, ADE7880_APGAIN, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_AWATTOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_BPGAIN, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_BWATTOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_CPGAIN, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_CWATTOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_AIRMSOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_AVRMSOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_BIRMSOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_BVRMSOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_CIRMSOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_CVRMSOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_NIRMSOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_HPGAIN, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_ISUMLVL, &aux, 32);

  ADE7880_ReadRegister(device, ADE7880_AFWATTOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_BFWATTOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_CFWATTOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_AFVAROS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_BFVAROS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_CFVAROS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_AFIRMSOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_BFIRMSOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_CFIRMSOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_HXWATTOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_HYWATTOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_HZWATTOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_HXVAROS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_HYVAROS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_HZVAROS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_HXIRMSOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_HYIRMSOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_HZIRMSOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_HXVRMSOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_HYVRMSOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_HZVRMSOS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_AIRMS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_AVRMS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_BIRMS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_BVRMS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_CIRMS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_CVRMS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_NIRMS, &aux, 32);
  ADE7880_ReadRegister(device, ADE7880_ISUM, &aux, 32);
}

int8_t ADE7880_DspRun(int device, uint8_t cmd)
{
  uint8_t i;

  for(i=0; i<3; i++) {
    if (cmd == ENABLE) {
      ADE7880_WriteRegister(device, ADE7880_RUN, 0x0001, 16);
    }
    else {
      ADE7880_WriteRegister(device, ADE7880_RUN, 0x0000, 16);
    }
  }
}

int8_t ADE7880_WaitNewConversion(int device)
{
  int32_t result = 0;
  int8_t cc = 0;

  ADE7880_ReadRegister(device, ADE7880_STATUS0, &result, 32);

  if (result&0x20000 == 0x20000) {
    ADE7880_RamProtect(device, DISABLE);
    result&=0xD0000;
    ADE7880_WriteRegister(device, ADE7880_STATUS0, result, 32);
    ADE7880_ReadRegister(device, ADE7880_STATUS0, &result, 32);
    ADE7880_RamProtect(device, ENABLE);

    while(((result&0x20000) == 0) && (cc++ < 10)) {
      ADE7880_ReadRegister(device, ADE7880_STATUS0, &result, 32);
    }  
  }
}

void ADE7880_ReadPhaseVRMS(int device, uint8_t channel, float *measure, float offset, float divider)
{
  uint16_t target_reg;
  int32_t measure_temp = 0;

  switch(channel) {
    case PHASE_A: target_reg = ADE7880_AVRMS; break;
    case PHASE_B: target_reg = ADE7880_BVRMS; break;
    case PHASE_C: target_reg = ADE7880_CVRMS; break;
  }

  ADE7880_WaitNewConversion(device);
  ADE7880_ReadRegister(device, target_reg, &measure_temp, 32);
  *measure = ((float)measure_temp - offset) / divider;
}

void ADE7880_ReadPhaseIRMS(int device, uint8_t channel, float *measure, float offset, float divider)
{
  uint16_t target_reg;
  int32_t measure_temp = 0;

  switch(channel) {
    case PHASE_A: target_reg = ADE7880_AIRMS; break;
    case PHASE_B: target_reg = ADE7880_BIRMS; break;
    case PHASE_C: target_reg = ADE7880_CIRMS; break;
    case PHASE_N: target_reg = ADE7880_NIRMS; break;
  }

  ADE7880_WaitNewConversion(device);
  ADE7880_ReadRegister(device, target_reg, &measure_temp, 32);
  *measure = ((float)measure_temp - offset) / divider;
}

void ADE7880_ReadPhaseActiveWH(int device, uint8_t channel, float *measure, float offset, float divider)
{
  uint16_t target_reg;
  int32_t measure_temp = 0;

  switch(channel) {
    case PHASE_A: target_reg = ADE7880_AWATTHR; break;
    case PHASE_B: target_reg = ADE7880_BWATTHR; break;
    case PHASE_C: target_reg = ADE7880_CWATTHR; break;
  }

  ADE7880_WaitNewConversion(device);
  ADE7880_ReadRegister(device, target_reg, &measure_temp, 32);
  *measure = ((float)measure_temp - offset) / divider;
}

void ADE7880_ReadPhaseActivePower(int device, uint8_t channel, float *measure, float offset, float divider)
{
  uint16_t target_reg;
  int32_t measure_temp = 0;

  switch(channel) {
    case PHASE_A: target_reg = ADE7880_AWATT; break;
    case PHASE_B: target_reg = ADE7880_BWATT; break;
    case PHASE_C: target_reg = ADE7880_CWATT; break;
  }

  /*ADE7880_WaitNewConversion(device);*/
  ADE7880_ReadRegister(device, target_reg, &measure_temp, 32);
  *measure = ((float)measure_temp - offset) / divider;
}

void ADE7880_ReadPhaseAparentPower(int device, uint8_t channel, float *measure, float offset, float divider)
{
  uint16_t target_reg;
  int32_t measure_temp = 0;

  switch(channel) {
    case PHASE_A: target_reg = ADE7880_AVA; break;
    case PHASE_B: target_reg = ADE7880_BVA; break;
    case PHASE_C: target_reg = ADE7880_CVA; break;
  }
  /*ADE7880_WaitNewConversion(device);*/
  ADE7880_ReadRegister(device, target_reg, &measure_temp, 32);
  *measure = ((float)measure_temp - offset) / divider;
}

void ADE7880_ReadPhasePowerFactor(int device, uint8_t channel, float *measure, float offset, float divider)
{
  uint16_t target_reg;
  int32_t measure_temp = 0;
  float tmp;

  switch(channel) {
    case PHASE_A: target_reg = ADE7880_APF; break;
    case PHASE_B: target_reg = ADE7880_BPF; break;
    case PHASE_C: target_reg = ADE7880_CPF; break;
  }
  /*ADE7880_WaitNewConversion(device);*/
  ADE7880_ReadRegister(device, target_reg, &measure_temp, 16);
  tmp = ((float)measure_temp - offset) / divider;
  if (tmp > 1.0) {
	  tmp = 1.0;
  }

  *measure = tmp;
}
