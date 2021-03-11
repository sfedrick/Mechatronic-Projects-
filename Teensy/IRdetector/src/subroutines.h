//wait for the input capture to change and come back
int WaitForChange(void){
  unsigned int oldtime=0;
  unsigned int newtime=0;
  int stopper=0;
  //records the old pinstate
  int Oldpinstate=PINC & (1<<7);
  //records the newest pinstate
  int Newpinstate=Oldpinstate;
  //continue until input capture value changes twice
  while(stopper<2){
    //check for when the pinstate changes
    Newpinstate=PINC & (1<<7);
    if(Newpinstate!=Oldpinstate){
      //if it's the first change set oldtime to input capture
      if(stopper==0){
        oldtime=(unsigned int)ICR3;
        stopper+=1;
      //  m_usb_tx_string("\n first stop: \n");
      //  m_usb_tx_int(oldtime);
      }
      //if it is the second change set newtime to input capture
      else if(stopper==1){
        newtime=(unsigned int)ICR3;
        stopper+=1;
      //  m_usb_tx_string("\n second stop : \n");
      //  m_usb_tx_int(newtime);
      }
      Oldpinstate=Newpinstate;
    }

  }
  // once input capture changes twice return the difference between the two times
  int dt=newtime-oldtime;
  return dt;

}
