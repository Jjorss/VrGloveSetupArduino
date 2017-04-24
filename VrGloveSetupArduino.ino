//#include <SerialCommand.h>
#include <SoftwareSerial.h>

int incomingByte = 0;   // for incoming serial data

int LHandIndex_1 = 0;   //First Knuckle
int LHandIndex_2 = 0;   //Second Knuckle

int LHandIndex_1_Min = 1023;   //Calibration Values
int LHandIndex_1_Max = 0;      

int LHandIndex_2_Min = 1023;   //Calibration Values   
int LHandIndex_2_Max = 0;   

boolean connected = false;
boolean flag = false;

int buff[5]= {0,0,0,0,0};
int pos = 0;
//SerialCommand SCmd;

int Index_1 = A4;
int Index_2 = A5;
void addToBuff(int c){
  buff[pos]=c;
  pos++;
  pos = (pos)%5;
}


int normalize(int c){
  addToBuff(c);
  return((buff[0]+buff[1]+buff[2]+buff[3]+buff[4])/5);
}
void setup() {
  Serial.begin(115200);
  // this statement does nothing on the UNO
  //while(!Serial);
  //SCmd.addCommand("P",pingHandler);
  //SCmd.addCommand("C",calibrateHandler);
  //SCmd.addCommand("D",dataHandler);
  //Serial.println("Ready");
}
void dataHandler(){

 
      LHandIndex_1 = map(analogRead(Index_1),LHandIndex_1_Min,LHandIndex_1_Max,0,90);
      LHandIndex_2 = map(analogRead(Index_2),LHandIndex_2_Min,LHandIndex_2_Max,0,90);
      String msg= "";
      msg = msg + String(LHandIndex_1) + ","+String(LHandIndex_2)+",";
      Serial.println(msg);
  

}

void loop() {  
  if(Serial.available()>0) {
    //SCmd.readSerial();
    // read the incoming byte:
    incomingByte = Serial.read();
    // say what you got:
    //Serial.print("I received: ");
    //Serial.println(incomingByte, DEC);
    parseCommand(incomingByte);
  }

}

void parseCommand(int byte){
  switch(byte){
    case 80:
    pingHandler();
    break;
    
    case 67:
    calibrateHandler();
    break;
    
    case 68:
    dataHandler();
    break;
    
    default:
    break;
  }
}

void pingHandler(){
  Serial.println("PONG");
}

void calibrateHandler(){
  flag = 0;
  while (!flag){
    if (Serial.available()>0){
      char r = Serial.read();
      if (r=='F' || r=='f'){
        LHandIndex_1_Min = analogRead(Index_1);
        LHandIndex_2_Min = analogRead(Index_2);
        flag = 1;
        Serial.println("Cal Min done");
      }
    }
    
  }
  flag = 0;
  while (!flag){
    if (Serial.available()>0){
      char r = Serial.read();
      if (r=='F' || r=='f'){
        LHandIndex_1_Max = analogRead(Index_1);
        LHandIndex_2_Max = analogRead(Index_2);
        flag = 1;
        Serial.println("Cal Max done");
      }
    }
    
  }
  Serial.flush();
}


