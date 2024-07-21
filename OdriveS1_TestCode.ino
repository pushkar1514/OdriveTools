#include <ODriveArduino.h>
// #include <SoftwareSerial.h>

// SoftwareSerial odrive_serial(8, 9); // RX (ODrive TX), TX (ODrive RX)

ODriveArduino odrive(Serial3);

int vel , axis = 0;
int pos;


void setup() {
  Serial3.begin(115200);

  Serial.begin(115200);
  

  Serial.println("Waiting for ODrive...");
  while (odrive.getState() == AXIS_STATE_UNDEFINED) {
    delay(10);
  }
  
  Serial.print("DC voltage: ");
  Serial.println(odrive.getParameterAsFloat("vbus_voltage"));
  
  Serial.println("Enabling closed loop control...");
  while (odrive.getState() != AXIS_STATE_CLOSED_LOOP_CONTROL) {
    odrive.clearErrors();
    odrive.setState(AXIS_STATE_CLOSED_LOOP_CONTROL);
    delay(10);
  }
  
  Serial.println("ODrive running!");
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
  // Serial.print(vel);
  // Serial.print("                      Enter the Position : ");
  // while(Serial.available()==0);
  // pos = Serial.parseInt();
  // Serial.println(pos);
  // odrive.setPosition(pos,vel);

}
