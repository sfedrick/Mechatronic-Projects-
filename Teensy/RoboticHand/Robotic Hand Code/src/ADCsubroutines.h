// sets the reference voltage and activates the desired pins for ADC
void ADCsetup(int activate[]){
  // step 1: set the reference voltage
  ADMUX |=1<<(REFS0);
  ADMUX &=~(1<<(REFS1));
  //Step 2: Set the ADC Frequency to be 125 khz
  ADCSRA|=1<<(ADPS0);
  ADCSRA|=1<<(ADPS1);
  ADCSRA|=1<<(ADPS2);

  // step 3:
  int i;
  int activePins[12]={ADC0D,ADC1D,ADC4D,ADC5D,ADC6D,ADC7D,ADC8D,ADC9D,ADC10D,ADC11D,ADC12D,ADC13D};
  //activates all the pin in the activate array
  for(i=0;i<12;i=i+1){
  	if(i<6){
  		if(activate[i]==1){
  			DIDR0 |=1<<(activePins[i]);
  		}
  		else{
  			DIDR0 &= ~(1<<(activePins[i]));
  		}
  	}
  	else if(i>6){
  		if(activate[i]==1){
  			DIDR2 |=1<<(activePins[i]);
  		}
  		else{
  			DIDR2 &= ~(1<<(activePins[i]));
  		}
  	}

  }

}
//converts any number into binary and outputs the binary result into
//an array
int * getbinary(int Num, int array[]){
  int i=0;
  while(Num>0){
    if(Num%2!=0){
      array[i]=1;
    }
    Num=Num/2;
    i=i+1;
  }
  return array;
}


// This lets you choose a channel
// you pass in a size four array binary array corresponding
// to the channel you would like to check
void choosechannel(int *Bits){
    //step 5 choose ADC channel

    if(*(Bits+3)==0){
      ADCSRB &=~(1<<(MUX5));
    }
    else{
      ADCSRB |=(1<<(MUX5));
    }

    if(*(Bits+2)==0){
      ADMUX &=~(1<<(MUX2));
    }
    else{
      ADMUX |= (1<<(MUX2));
    }

    if(*(Bits+1)==0){
      ADMUX &=~(1<<(MUX1));
    }
    else{
      ADMUX |= (1<<(MUX1));
    }

    if(*(Bits)==0){
      ADMUX &=~(1<<(MUX0));
    }
    else{
      ADMUX |= (1<<(MUX0));
    }
//begin ADC
    ADCSRA|=1<<(ADEN);
  	ADCSRA|=1<<(ADSC);
}

//spits out the adc result if it is ready
int grabADC(void){
  if((ADCSRA & (1<<ADIF))!=0){
    int result=ADC;
		ADCSRA|=(1<<ADIF);
		return result;
	}
  else{
    return -1;
  }
}
