/************************************************************************
	INCLUDES
************************************************************************/

/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

/* App includes */
#include "ade7880.h"

/************************************************************************
	DECLARATIONS
************************************************************************/
#define DEBUG

/************************************************************************
	FUNCTIONS
************************************************************************/

/* Local functions */

/* Public functions */

int main(int argc, char **argv)
{
    uint32_t data = 0;
    uint16_t address = 0;
    uint32_t aux = 0;
    char *device_address;
    int device_descriptor;
    float avrms, bvrms, cvrms;
    float airms, birms, cirms, nirms;
    float aactivewh, bactivewh, cactivewh;
    float aactivepower, bactivepower, cactivepower;

    device_address = malloc(15);
    sprintf(device_address, "%s", ADE7800_LINUX_DEVICE_ADDRESS);
 
	/* Init ADE7880 */
	if (ADE7880_Init(device_address, &device_descriptor) == ADE7880_ERROR){
		printf("\nError al inicializar el dispositivo");
		return 1;
	}

    ADE7880_InitializeSPI(device_descriptor);
    ADE7880_QuickSetup(device_descriptor);
    ADE7880_DspRun(device_descriptor, ENABLE);
    ADE7880_ReadRegister(device_descriptor, ADE7880_AWATTHR, &aux, 32);
    ADE7880_ReadRegister(device_descriptor, ADE7880_BWATTHR, &aux, 32);
    ADE7880_ReadRegister(device_descriptor, ADE7880_CWATTHR, &aux, 32);
    ADE7880_ReadRegister(device_descriptor, ADE7880_AVAHR, &aux, 32);
    ADE7880_ReadRegister(device_descriptor, ADE7880_BVAHR, &aux, 32);
    ADE7880_ReadRegister(device_descriptor, ADE7880_CVAHR, &aux, 32);
    ADE7880_ReadRegister(device_descriptor, ADE7880_AFWATTHR, &aux, 32);
    ADE7880_ReadRegister(device_descriptor, ADE7880_BFWATTHR, &aux, 32);
    ADE7880_ReadRegister(device_descriptor, ADE7880_CFWATTHR, &aux, 32);
    ADE7880_ReadRegister(device_descriptor, ADE7880_AFVARHR, &aux, 32);
    ADE7880_ReadRegister(device_descriptor, ADE7880_BFVARHR, &aux, 32);
    ADE7880_ReadRegister(device_descriptor, ADE7880_CFVARHR, &aux, 32);
    /*ADE7880_WriteRegister(device_descriptor, ADE7880_ACCMODE, 0x71, 8);*/
    ADE7880_WriteRegister(device_descriptor, ADE7880_CFMODE, 0x70A0, 16);

    ADE7880_ReadPhaseVRMS(device_descriptor, PHASE_A, &avrms, 1, 1);
    printf("AVRMS: %f\n", avrms);
    ADE7880_ReadPhaseVRMS(device_descriptor, PHASE_B, &bvrms, 1, 1);
    printf("BVRMS: %f\n", bvrms);
    ADE7880_ReadPhaseVRMS(device_descriptor, PHASE_C, &cvrms, 1, 1);
    printf("CVRMS: %f\n", cvrms);

    ADE7880_ReadPhaseIRMS(device_descriptor, PHASE_A, &airms, 1, 1);
    printf("AIRMS: %f\n", airms);
    ADE7880_ReadPhaseIRMS(device_descriptor, PHASE_B, &birms, 1, 1);
    printf("BIRMS: %f\n", birms);
    ADE7880_ReadPhaseIRMS(device_descriptor, PHASE_C, &cirms, 1, 1);
    printf("CIRMS: %f\n", cirms);
    ADE7880_ReadPhaseIRMS(device_descriptor, PHASE_N, &nirms, 1, 1);
    printf("NIRMS: %f\n", nirms);
      
    ADE7880_ReadPhaseActiveWH(device_descriptor, PHASE_A, &aactivewh, 1, 1);
    printf("AACTIVEWH: %f\n", aactivewh);
    ADE7880_ReadPhaseActiveWH(device_descriptor, PHASE_B, &bactivewh, 1, 1);
    printf("BACTIVEWH: %f\n", bactivewh);
    ADE7880_ReadPhaseActiveWH(device_descriptor, PHASE_C, &cactivewh, 1, 1);
    printf("CACTIVEWH: %f\n", cactivewh);

    ADE7880_ReadPhaseActivePower(device_descriptor, PHASE_A, &aactivepower, 1, 1);
    printf("AACTIVEPOWER: %f\n", aactivepower);
    ADE7880_ReadPhaseActivePower(device_descriptor, PHASE_B, &bactivepower, 1, 1);
    printf("BACTIVEPOWER: %f\n", bactivepower);
    ADE7880_ReadPhaseActivePower(device_descriptor, PHASE_C, &cactivepower, 1, 1);
    printf("CACTIVEPOWER: %f\n", cactivepower);

    printf("\n");

    ADE7880_Close(device_descriptor);
    free(device_address);
}
