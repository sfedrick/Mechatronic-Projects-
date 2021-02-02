/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"
#include "PulseWidthModulation.h"
//set to 1 to activate dying mode
int dying=1;
//set to 1 for asymetric
int a=0;
// set both dying and a to zero for heart

int main(void)
{
//simple pulse array goes to full intensity and comes back down
	float simple[3][2]={{0,0},{1,1},{2,0}};
//asymetric pulse array
	float asymetric[3][2]={{0,0},{0.3,1},{1,0}};
// heart pulse array
float heart[11][2]={
	{0,0},
	{0.1,1},
	{0.5,0},
	{0.6,0.5},
	{1.0,0},
	{3.0,0},
	{3.1,1},
	{3.5,0},
	{3.6,0.5},
	{4.0,0},
	{6.0,0}
};

float dyingheart[10][2]={
	{0,0},
	{0.1,1},
	{0.5,0},
	{0.6,0.5},
	{1.0,0},
	{2.0,0}
};

	while(1){
		if(dying){
			//progressively decrease intensity to simulate a dying heart
			float dyingsteps=20;
			float maxdecline=1.0/dyingsteps;
			float max=1.0;
			int i;
			for (i=0; i<=dyingsteps;i+=1){
				pulse(dyingheart,6, 100,500,max);
				max=max-maxdecline;
			};

		}
		else if(a){
				pulse(asymetric,3, 100, 500,1.0);
		}
		else{
			pulse(heart,11, 100, 500,1.0);
		}


			// this is used for dying heart

	}

	return 0;
}
