#ifndef ADE7880_H_
#define ADE7880_H_

/************************************************************************
	INCLUDES
************************************************************************/
#include <stdint.h>

/**************************************************************************
 * CONSTANTS
 *************************************************************************/

/* SPI Configuration constants */
#define XFER_LEN                        2
#define ADE7880_TRANSFER_SIZE			8
#define ADE7880_READ_CMD_SIZE			3
#define ADE7880_READ_REG                0x1
#define ADE7880_WRITE_REG               0x0
#define ADE7800_LINUX_DEVICE_ADDRESS    "/dev/spidev1.0"

/* Status constants */
#define ADE7880_OK						0
#define ADE7880_ERROR					(-1)

#define ADE7880_AIGAIN                  0x4380
#define ADE7880_AVGAIN                  0x4381
#define ADE7880_BIGAIN                  0x4382
#define ADE7880_BVGAIN                  0x4383
#define ADE7880_CIGAIN                  0x4384
#define ADE7880_CVGAIN                  0x4385
#define ADE7880_NIGAIN                  0x4386
#define ADE7880_DICOEFF                 0x4388
#define ADE7880_APGAIN                  0x4389
#define ADE7880_AWATTOS                 0x438A
#define ADE7880_BPGAIN                  0x438B
#define ADE7880_BWATTOS                 0x438C
#define ADE7880_CPGAIN                  0x438D
#define ADE7880_CWATTOS                 0x438E
#define ADE7880_AIRMSOS                 0x438F
#define ADE7880_AVRMSOS                 0x4390
#define ADE7880_BIRMSOS                 0x4391
#define ADE7880_BVRMSOS                 0x4392
#define ADE7880_CIRMSOS                 0x4393
#define ADE7880_CVRMSOS                 0x4394
#define ADE7880_NIRMSOS                 0x4395
#define ADE7880_HPGAIN                  0x4398
#define ADE7880_ISUMLVL                 0x4399
#define ADE7880_VLEVEL                  0x439F
#define ADE7880_AFWATTOS                0x43A2
#define ADE7880_BFWATTOS                0x43A3
#define ADE7880_CFWATTOS                0x43A4
#define ADE7880_AFVAROS                 0x43A5
#define ADE7880_BFVAROS                 0x43A6
#define ADE7880_CFVAROS                 0x43A7
#define ADE7880_AFIRMSOS                0x43A8
#define ADE7880_BFIRMSOS                0x43A9
#define ADE7880_CFIRMSOS                0x43AA
#define ADE7880_AFVRMSOS                0x43AB
#define ADE7880_BFVRMSOS                0x43AC
#define ADE7880_CFVRMSOS                0x43AD
#define ADE7880_HXWATTOS                0x43AE
#define ADE7880_HYWATTOS                0x43AF
#define ADE7880_HZWATTOS                0x43B0
#define ADE7880_HXVAROS                 0x43B1
#define ADE7880_HYVAROS                 0x43B2
#define ADE7880_HZVAROS                 0x43B3
#define ADE7880_HXIRMSOS                0x43B4
#define ADE7880_HYIRMSOS                0x43B5
#define ADE7880_HZIRMSOS                0x43B6
#define ADE7880_HXVRMSOS                0x43B7
#define ADE7880_HYVRMSOS                0x43B8
#define ADE7880_HZVRMSOS                0x43B9
#define ADE7880_AIRMS                   0x43C0
#define ADE7880_AVRMS                   0x43C1
#define ADE7880_BIRMS                   0x43C2
#define ADE7880_BVRMS                   0x43C3
#define ADE7880_CIRMS                   0x43C4
#define ADE7880_CVRMS                   0x43C5
#define ADE7880_NIRMS                   0x43C6
#define ADE7880_ISUM                    0x43C7
#define ADE7880_RUN                     0xE228
#define ADE7880_AWATTHR                 0xE400
#define ADE7880_BWATTHR                 0xE401
#define ADE7880_CWATTHR                 0xE402
#define ADE7880_AFWATTHR                0xE403
#define ADE7880_BFWATTHR                0xE404
#define ADE7880_CFWATTHR                0xE405
#define ADE7880_AFVARHR                 0xE409
#define ADE7880_BFVARHR                 0xE40A
#define ADE7880_CFVARHR                 0xE40B
#define ADE7880_AVAHR                   0xE40C
#define ADE7880_BVAHR                   0xE40D
#define ADE7880_CVAHR                   0xE40E
#define ADE7880_IPEAK                   0xE500
#define ADE7880_VPEAK                   0xE501
#define ADE7880_STATUS0                 0xE502
#define ADE7880_STATUS1                 0xE503
#define ADE7880_AIMAV                   0xE504
#define ADE7880_BIMAV                   0xE505
#define ADE7880_CIMAV                   0xE506
#define ADE7880_OILVL                   0xE507
#define ADE7880_OVLVL                   0xE508
#define ADE7880_SAGLVL                  0xE509
#define ADE7880_MASK0                   0xE50A
#define ADE7880_MASK1                   0xE50B
#define ADE7880_IAWV                    0xE50C
#define ADE7880_IBWV                    0xE50D
#define ADE7880_ICWV                    0xE50E
#define ADE7880_INWV                    0xE50F
#define ADE7880_VAWV                    0xE510
#define ADE7880_VBWV                    0xE511
#define ADE7880_VCWV                    0xE512
#define ADE7880_AWATT                   0xE513
#define ADE7880_BWATT                   0xE514
#define ADE7880_CWATT                   0xE515
#define ADE7880_AFVAR                   0xE516
#define ADE7880_BFVAR                   0xE517
#define ADE7880_CFVAR                   0xE518
#define ADE7880_AVA                     0xE519
#define ADE7880_BVA                     0xE51A
#define ADE7880_CVA                     0xE51B
#define ADE7880_CHECKSUM                0xE51F
#define ADE7880_VNOM                    0xE520
#define ADE7880_LAST_RWDATA_24bit       0xE5FF
#define ADE7880_PHSTATUS                0xE600
#define ADE7880_ANGLE0                  0xE601
#define ADE7880_ANGLE1                  0xE602
#define ADE7880_ANGLE2                  0xE603
#define ADE7880_PHNOLOAD                0xE608
#define ADE7880_LINECYC                 0xE60C
#define ADE7880_ZXTOUT                  0xE60D
#define ADE7880_COMPMODE                0xE60E
#define ADE7880_Gain                    0xE60F
#define ADE7880_CFMODE                  0xE610
#define ADE7880_CF1DEN                  0xE611
#define ADE7880_CF2DEN                  0xE612
#define ADE7880_CF3DEN                  0xE613
#define ADE7880_APHCAL                  0xE614
#define ADE7880_BPHCAL                  0xE615
#define ADE7880_CPHCAL                  0xE616
#define ADE7880_PHSIGN                  0xE617
#define ADE7880_CONFIG                  0xE618
#define ADE7880_MMODE                   0xE700
#define ADE7880_ACCMODE                 0xE701
#define ADE7880_LCYCMODE                0xE702
#define ADE7880_PEAKCYC                 0xE703
#define ADE7880_SAGCYC                  0xE704
#define ADE7880_CFCYC                   0xE705
#define ADE7880_HSDC_CFG                0xE706
#define ADE7880_Version                 0xE707
#define ADE7880_LAST_RWDATA_8bit        0xE7FD
#define ADE7880_FVRMS                   0xE880
#define ADE7880_FIRMS                   0xE881
#define ADE7880_FWATT                   0xE882
#define ADE7880_FVAR                    0xE883
#define ADE7880_FVA                     0xE884
#define ADE7880_FPF                     0xE885
#define ADE7880_VTHDN                   0xE886
#define ADE7880_ITHDN                   0xE887
#define ADE7880_HXVRMS                  0xE888
#define ADE7880_HXIRMS                  0xE889
#define ADE7880_HXWATT                  0xE88A
#define ADE7880_HXVAR                   0xE88B
#define ADE7880_HXVA                    0xE88C
#define ADE7880_HXPF                    0xE88D
#define ADE7880_HXVHD                   0xE88E
#define ADE7880_HXIHD                   0xE88F
#define ADE7880_HYVRMS                  0xE890
#define ADE7880_HYIRMS                  0xE891
#define ADE7880_HYWATT                  0xE892
#define ADE7880_HYVAR                   0xE893
#define ADE7880_HYVA                    0xE894
#define ADE7880_HYPF                    0xE895
#define ADE7880_HYVHD                   0xE896
#define ADE7880_HYIHD                   0xE897
#define ADE7880_HZVRMS                  0xE898
#define ADE7880_HZIRMS                  0xE899
#define ADE7880_HZWATT                  0xE89A
#define ADE7880_HZVAR                   0xE89B
#define ADE7880_HZVA                    0xE89C
#define ADE7880_HZPF                    0xE89D
#define ADE7880_HZVHD                   0xE89E
#define ADE7880_HZIHD                   0xE89F
#define ADE7880_HCONFIG                 0xE900
#define ADE7880_APF                     0xE902
#define ADE7880_BPF                     0xE903
#define ADE7880_CPF                     0xE904
#define ADE7880_APERIOD                 0xE905
#define ADE7880_BPERIOD                 0xE906
#define ADE7880_CPERIOD                 0xE907
#define ADE7880_APNOLOAD                0xE908
#define ADE7880_VARNOLOAD               0xE909
#define ADE7880_VANOLOAD                0xE90A
#define ADE7880_LAST_ADD                0xE9FE
#define ADE7880_LAST_RWDATA_16bit       0xE9FF
#define ADE7880_CONFIG3                 0xEA00
#define ADE7880_LAST_OP                 0xEA01
#define ADE7880_WTHR                    0xEA02
#define ADE7880_VARTHR                  0xEA03
#define ADE7880_VATHR                   0xEA04
#define ADE7880_HX_reg                  0xEA08
#define ADE7880_HY_reg                  0xEA09
#define ADE7880_HZ_reg                  0xEA0A
#define ADE7880_LPOILVL                 0xEC00
#define ADE7880_CONFIG2                 0xEC01
#define ADE7880_DUMMY                   0xEBFF

/************************************************************************
	GLOBAL DEFINES AND TYPES
************************************************************************/

#define PHASE_CURRENTS_GAIN             0
#define NEUTRAL_CURRENTS_GAIN           0
#define PHASE_VOLTAGES_GAIN             0   

#define WTHR_DEFAULT    0x03
#define VARTHR_DEFAULT  0x03
#define VATHR_DEFAULT   0x03
#define VLEVEL_DEFAULT  0x38000
#define VNOM_DEFAULT    0x23C354
#define CFXDEN_VAL      0x0DB3 
#define DICOEFF         0xFFF8000

#define ENABLE      1
#define DISABLE     0

#define PHASE_A     0
#define PHASE_B     1
#define PHASE_C     2
#define PHASE_N     3

/*#define TRANSFORMER_RATIO   20.56621005*/

/*#define VLSB_CONST          211184.71337579617834394904458599*/
//#define VLSB_CONST          11031.70006
//#define ILSB_CONST          24408
//#define WHLSB_CONST         (67.28)*2.275
//#define WATTLSB_CONST       32.310626703

/**************************************************************************
 * FUNCTIONS
 *************************************************************************/
int8_t ADE7880_Init(char* devPath, int *device);
int8_t ADE7880_Close(int device);
int8_t ADE7880_WriteRegister(int device, uint16_t address, uint32_t data, int bits);
int8_t ADE7880_ReadRegister(int device, uint16_t address, uint32_t *data, int bits);
int8_t ADE7880_InitializeSPI(int device);
int8_t ADE7880_QuickSetup(int device);
int8_t ADE7880_DspRun(int device, uint8_t cmd);
void ADE7880_ReadPhaseVRMS(int device, uint8_t channel, float *measure, float offset, float divider);
void ADE7880_ReadPhaseIRMS(int device, uint8_t channel, float *measure, float offset, float divider);
void ADE7880_ReadPhaseActiveWH(int device, uint8_t channel, float *measure, float offset, float divider);
void ADE7880_ReadPhaseActivePower(int device, uint8_t channel, float *measure, float offset, float divider);
void ADE7880_ReadPhaseAparentPower(int device, uint8_t channel, float *measure, float offset, float divider);
void ADE7880_ReadPhasePowerFactor(int device, uint8_t channel, float *measure, float offset, float divider);

#endif /* ADE7880_H_   */
