#define led 22
#define ledPull 21
int state=0;
void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  pinMode(ledPull,INPUT_PULLUP);
 
}

void loop() {
 //turns led on or off by reading the state pin
state=digitalRead(ledPull);
if(state==0){
   digitalWrite(led,LOW);
}
else{
  digitalWrite(led,HIGH);
}

 
}
