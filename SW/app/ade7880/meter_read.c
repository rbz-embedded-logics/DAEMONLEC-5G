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
#include "ade7880.h"

/************************************************************************
	DECLARATIONS
************************************************************************/
#define DEBUG

float V_offset = 3587.0;
float V_divider = 10965.6;
float I_offset = 973.1;
float I_divider = 34709.6;
float Active_offset = 0.0;
float Active_divider = 58.7;
float Aparent_offset = 0.0;
float Aparent_divider = 341.6;
float PF_offset	= 0.0;
float PF_divider = 33281.0;


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
    float aaparentpower, baparentpower, caparentpower;
    float aaparentwh, baparentwh, caparentwh;
    float aPF, bPF, cPF;
    time_t timestamp;
    char *json_string, *csv_string; 
    int i;
    FILE *fp = fopen("/tmp/data_energy.json", "w");
    FILE *csv = fopen("/tmp/data_energy.csv", "w"); 

    if (fp == NULL) { 
	        printf("Error opening json file!\n"); 
		    return 1; // Exit the program with an error code 
    } 

    if (csv == NULL) {
	        printf("Error opening csv file!\n");
		return 1; // Exit the program with an error code
    }

    device_address = malloc(15);
    sprintf(device_address, "%s", ADE7800_LINUX_DEVICE_ADDRESS);
 
	/* Init ADE7880 */
	if (ADE7880_Init(device_address, &device_descriptor) == ADE7880_ERROR){
		printf("\nError al inicializar el dispositivo");
		return 1;
	}

    json_string = malloc(4096);
    csv_string = malloc(4096);
    ADE7880_InitializeSPI(device_descriptor);

    timestamp = time(NULL);
    ADE7880_ReadPhaseVRMS(device_descriptor, PHASE_A, &avrms, V_offset, V_divider);
    ADE7880_ReadPhaseVRMS(device_descriptor, PHASE_B, &bvrms, V_offset, V_divider);
    ADE7880_ReadPhaseVRMS(device_descriptor, PHASE_C, &cvrms, V_offset, V_divider);

    ADE7880_ReadPhaseIRMS(device_descriptor, PHASE_A, &airms, I_offset, I_divider);
    ADE7880_ReadPhaseIRMS(device_descriptor, PHASE_B, &birms, I_offset, I_divider);
    ADE7880_ReadPhaseIRMS(device_descriptor, PHASE_C, &cirms, I_offset, I_divider);
    ADE7880_ReadPhaseIRMS(device_descriptor, PHASE_N, &nirms, I_offset, I_divider);
     
    aactivewh = 0;
    bactivewh = 0;
    cactivewh = 0; 
    for( i = 0; i < 60; i++){
	float tmp;
	ADE7880_ReadPhaseActivePower(device_descriptor, PHASE_A, &tmp, Active_offset, Active_divider);
	aactivewh += tmp;
	tmp = 0;
	ADE7880_ReadPhaseActivePower(device_descriptor, PHASE_B, &tmp, Active_offset, Active_divider);
	bactivewh += tmp;
	tmp = 0;
        ADE7880_ReadPhaseActivePower(device_descriptor, PHASE_C, &tmp, Active_offset, Active_divider);
        cactivewh += tmp;

	ADE7880_ReadPhaseAparentPower(device_descriptor, PHASE_A, &tmp, Aparent_offset, Aparent_divider);
        aaparentwh += tmp;
        tmp = 0;
        ADE7880_ReadPhaseAparentPower(device_descriptor, PHASE_B, &tmp, Aparent_offset, Aparent_divider);
        baparentwh += tmp;
        tmp = 0;
        ADE7880_ReadPhaseAparentPower(device_descriptor, PHASE_C, &tmp, Aparent_offset, Aparent_divider);
        caparentwh += tmp;
	sleep(1);
    }

    aactivepower = aactivewh/60;
    bactivepower = bactivewh/60;
    cactivepower = cactivewh/60;
    aaparentpower = aaparentwh/60;
    baparentpower = baparentwh/60;
    caparentpower = caparentwh/60;

    aactivewh = aactivewh/3600;
    bactivewh = bactivewh/3600;
    cactivewh = cactivewh/3600;
    aaparentwh = aaparentwh/3600;
    baparentwh = baparentwh/3600;
    caparentwh = caparentwh/3600;

    //ADE7880_ReadPhaseActivePower(device_descriptor, PHASE_A, &aactivepower);
    //ADE7880_ReadPhaseActivePower(device_descriptor, PHASE_B, &bactivepower);
    //ADE7880_ReadPhaseActivePower(device_descriptor, PHASE_C, &cactivepower);
    //ADE7880_ReadPhaseAparentPower(device_descriptor, PHASE_A, &aaparentpower, Aparent_offset, Aparent_divider);
    //ADE7880_ReadPhaseAparentPower(device_descriptor, PHASE_B, &baparentpower, Aparent_offset, Aparent_divider);
    //ADE7880_ReadPhaseAparentPower(device_descriptor, PHASE_C, &caparentpower, Aparent_offset, Aparent_divider);

    aPF = aactivepower / (aactivepower + aaparentpower);
    bPF = bactivepower / (bactivepower + baparentpower);
    cPF = cactivepower / (cactivepower + caparentpower);

    //ADE7880_ReadPhasePowerFactor(device_descriptor, PHASE_A, &aPF, PF_offset, PF_divider);
    //ADE7880_ReadPhasePowerFactor(device_descriptor, PHASE_B, &bPF, PF_offset, PF_divider);
    //ADE7880_ReadPhasePowerFactor(device_descriptor, PHASE_C, &cPF, PF_offset, PF_divider);

    sprintf(json_string,"{avrms=%f, bvrms= %f, cvrms=%f, airms=%f, birms=%f, cirms=%f, nirms=%f, aactivewh=%f, bactivewh=%f, cactivewh=%f, aactivepower=%f, bactivepower=%f, cactivepower=%f, aaparentpower=%f, baparentpower=%f, caparentpower=%f, aaparentwh=%f, baparentwh=%f, caparentwh=%f, aPF=%f, bPF=%f, cPF=%f}", avrms, bvrms, cvrms, airms, birms, cirms, nirms, aactivewh, bactivewh, cactivewh, aactivepower, bactivepower, cactivepower, aaparentpower, baparentpower, caparentpower, aaparentwh, baparentwh, caparentwh, aPF, bPF, cPF);
    printf("%ld %s\n", timestamp, json_string);

    sprintf(csv_string, "%ld; %f; %f; %f\n", timestamp, aactivepower, bactivepower, cactivepower);

    printf("%ld %s\n", timestamp, csv_string);


    int result = fputs(json_string, fp);
    result = fputs(csv_string, csv); 
    fclose(fp); 
    fclose(csv);


    ADE7880_Close(device_descriptor);
    free(device_address);
}
