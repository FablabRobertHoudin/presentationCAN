// Arduino Due - Transmet les valeurs RGB reçues par BT (Serial1) vers le CAN
// Par Emmanuel Goudot (Emmanuel.Goudot@gmail.com)
// basé sur CANmanager demo...

// Required libraries
#include "variant.h"
#include <due_can.h>

#define SerialBT Serial1

boolean isBT = false;

CAN_FRAME frame;

void setup()
{

  //Serial.begin(115200);
  Serial.begin(57600);
  SerialBT.begin(57600); // Bluetooth port
  Serial.println("CAN manager");
  
  // Initialize CAN0, Set the proper baud rates here
  CAN.init(CAN_BPS_125K);
  
  //By default there are 7 RX mailboxes for each device
  //extended
  //syntax is mailbox, ID, mask, extended
  CAN.setRXFilter(0, 0x700, 0x700, false);
  CAN.setRXFilter(1, 0x100000, 0x1FFFFFFF, true);
  CAN.setRXFilter(2, 0, 0, true); //catch all mailbox

  //now register all of the callback functions.
  //CAN.setCallback(0, gotFrame7xx);
  //CAN.setCallback(1, gotFrameAppli);
  //CAN.setGeneralCallback(gotFrame);

  pinMode(2, INPUT);

}

unsigned long t, t1 = 0;
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

byte maxR=255, maxG=255, maxB=255;

void loop(){ 
  static int pos = 0, pos2, n;
  static char line[255];
  char inByte = 0;
  int a0;

  t = millis();
  if(t - t1 >= 1000) {
    t1 = t;
    a0 = analogRead(A0);
    Serial.print("A0: ");
    Serial.println(a0);
    }

  if (SerialBT.available()) {
    // Interface entre BT et LED RGB...
    inByte = SerialBT.read();
    if (inByte=='=') {
      // recup des valeurs RGV
      maxR = SerialBT.parseInt(); 
      maxG = SerialBT.parseInt(); 
      maxB = SerialBT.parseInt(); 
      Serial.print("Valeurs: ");
      Serial.print(maxR);
      Serial.print(", ");
      Serial.print(maxG);
      Serial.print(", ");
      Serial.print(maxB);
      Serial.print("\n");
      // Construction trame CAN, id 20h
          frame.data.byte[0] = maxR;
          frame.data.byte[1] = maxG;
          frame.data.byte[2] = maxB;
          frame.id = 0x00000020;
          frame.extended = 1;
          frame.length = 3;
          CAN.sendFrame(frame); // Et envoi...
      }
    
  }

 }

