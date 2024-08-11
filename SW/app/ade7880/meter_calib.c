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
#include <time.h>

/* App includes */
#include "ade7880_calib.h"

/************************************************************************
	DECLARATIONS
************************************************************************/
#define DEBUG

float avrms = 0, bvrms = 0, cvrms = 0;
float airms = 0, birms = 0, cirms = 0, nirms = 0;
float aactivewh = 0, bactivewh = 0, cactivewh = 0;
float aactivepower= 0, bactivepower = 0, cactivepower = 0;
float aaparentpower = 0, baparentpower = 0, caparentpower = 0;
float apf = 0, bpf = 0, cpf = 0;

/************************************************************************
	FUNCTIONS
************************************************************************/

/* Local functions */

/* Public functions */

int main(int argc, char *argv[])
{
    uint32_t data = 0;
    uint16_t address = 0;
    uint32_t aux = 0;
    char *device_address;
    int device_descriptor;
//    float avrms = 0, bvrms = 0, cvrms = 0;
//    float airms = 0, birms = 0, cirms = 0, nirms = 0;
//    float aactivewh = 0, bactivewh = 0, cactivewh = 0;
//    float aactivepower= 0, bactivepower = 0, cactivepower = 0;
    int i;
    float tmp;
    int RUNS = 128;
    float runs;

    setbuf(stdout, NULL);

    device_address = malloc(15);
    sprintf(device_address, "%s", ADE7800_LINUX_DEVICE_ADDRESS);

    if (argc == 1)
    {
	    printf("Se usa 128 como número de pasadas\n");
	    RUNS = 128;
    }
    else {
	    RUNS = atoi(argv[1]);
	    printf("Se usa %d como número de pasadas\n", RUNS);
    }
 
	/* Init ADE7880 */
	if (ADE7880_Init(device_address, &device_descriptor) == ADE7880_ERROR){
		printf("\nError al inicializar el dispositivo");
		return 1;
	}

    ADE7880_InitializeSPI(device_descriptor);

    for(i = 0; i < RUNS; i++)
    {
      ADE7880_ReadPhaseVRMS(device_descriptor, PHASE_A, &tmp);
      avrms += tmp;
      tmp = 0;
      ADE7880_ReadPhaseVRMS(device_descriptor, PHASE_B, &tmp);
      bvrms += tmp;
      tmp = 0;
      ADE7880_ReadPhaseVRMS(device_descriptor, PHASE_C, &tmp);
      cvrms += tmp;
      tmp = 0;
      
      ADE7880_ReadPhaseIRMS(device_descriptor, PHASE_A, &tmp);
      airms += tmp;
      tmp = 0;
      ADE7880_ReadPhaseIRMS(device_descriptor, PHASE_B, &tmp);
      birms += tmp;
      tmp = 0;
      ADE7880_ReadPhaseIRMS(device_descriptor, PHASE_C, &tmp);
      cirms += tmp;
      tmp = 0;
      ADE7880_ReadPhaseIRMS(device_descriptor, PHASE_N, &tmp);
      nirms += tmp;
      tmp = 0;
     
      ADE7880_ReadPhaseActivePower(device_descriptor, PHASE_A, &tmp);
      aactivepower += tmp;
      tmp = 0;
      ADE7880_ReadPhaseActivePower(device_descriptor, PHASE_B, &tmp);
      bactivepower += tmp;
      tmp = 0;
      ADE7880_ReadPhaseActivePower(device_descriptor, PHASE_C, &tmp);
      cactivepower += tmp;
      tmp = 0;


      ADE7880_ReadPhaseActiveWH(device_descriptor, PHASE_A, &tmp);
      aactivewh += tmp;
      tmp = 0;
      ADE7880_ReadPhaseActiveWH(device_descriptor, PHASE_B, &tmp);
      bactivewh += tmp;
      tmp = 0;
      ADE7880_ReadPhaseActiveWH(device_descriptor, PHASE_C, &tmp);
      aactivewh += tmp;
      tmp = 0;

      ADE7880_ReadPhaseAparentPower(device_descriptor, PHASE_A, &tmp);
      aaparentpower += tmp;
      tmp = 0;
      ADE7880_ReadPhaseAparentPower(device_descriptor, PHASE_B, &tmp);
      baparentpower += tmp;
      tmp = 0;
      ADE7880_ReadPhaseAparentPower(device_descriptor, PHASE_C, &tmp);
      caparentpower += tmp;
      tmp = 0;

      ADE7880_ReadPhasePowerFactor(device_descriptor, PHASE_A, &tmp);
      apf += tmp;
      tmp = 0;
      ADE7880_ReadPhasePowerFactor(device_descriptor, PHASE_B, &tmp);
      bpf += tmp;
      tmp = 0;
      ADE7880_ReadPhasePowerFactor(device_descriptor, PHASE_C, &tmp);
      cpf += tmp;
      tmp = 0;

      sleep(1);
      printf("#");
      if (i%10 == 0)
      {
	      printf(" %d ",i);
      }
    }

    printf("\n");
    runs = RUNS*1.0;

    printf("avrms: %f\n", avrms/runs);
    printf("bvrms: %f\n", bvrms/runs);
    printf("cvrms: %f\n", cvrms/runs);

    printf("airms: %f\n", airms/runs);
    printf("birms: %f\n", birms/runs);
    printf("cirms: %f\n", cirms/runs);
    printf("nirms: %f\n", nirms/runs);

    printf("aactivepower: %f\n",aactivepower/runs);
    printf("bactivepower: %f\n",bactivepower/runs);
    printf("cactivepower: %f\n",cactivepower/runs);

    printf("aactivewh: %f\n",aactivewh/runs);
    printf("bactivewh: %f\n",bactivewh/runs);
    printf("cactivewh: %f\n",cactivewh/runs);

    printf("aaparentpower: %f\n",aaparentpower/runs);
    printf("baparentpower: %f\n",baparentpower/runs);
    printf("caparentpower: %f\n",caparentpower/runs);

    printf("apf: %f\n",apf/runs);
    printf("bpf: %f\n",bpf/runs);
    printf("cpf: %f\n",cpf/runs);

    ADE7880_Close(device_descriptor);
    free(device_address);
}
