#include <ODriveUART.h>
// #include <SoftwareSerial.h>

// SoftwareSerial odrive_serial(8, 9); // RX (ODrive TX), TX (ODrive RX)

ODriveUART odrive(Serial5);           //Connected on Serial 5

int vel , axis = 0;
int pos;


void setup() {
  Serial5.begin(115200);
  Serial.begin(115200);
  
  odrive.closedLoopState(); // Accessing Closed Loop
}

void loop() {
  Serial.print("Enter the Speed : ");
  while(Serial.available()==0);
  vel = Serial.parseInt();
  odrive.SetVelocity(axis,vel);
  Serial.println(vel);

  // Serial.print("Enter the Speed : ");
  // while(Serial.available()==0);
  // vel = Serial.parseInt();
  // Serial.println(vel);
  // Serial.print("Enter the Position : ");
  // while(Serial.available()==0);
  // pos = Serial.parseInt();
  // Serial.println(pos);
  // odrive.setPosition(pos,vel);

}
