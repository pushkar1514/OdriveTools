// Author: ODrive Robotics Inc.
// License: MIT
// Documentation: https://docs.odriverobotics.com/v/latest/guides/arduino-guide.html

#include "Arduino.h"
#include "ODriveSwerve.h"

static const int kMotorNumber = 0;

// Print with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

ODriveSwerve::ODriveSwerve(Stream& serial)
    : serial_(serial) {}

void ODriveSwerve::clearErrors() {
    serial_ << "sc\n";
}

void ODriveSwerve::closedLoopState(){
    Serial.println("Waiting for ODrive...");
    while (getState() == AXIS_STATE_UNDEFINED) {
        delay(10);
    }
    Serial.print("DC voltage: ");
    Serial.println(getParameterAsFloat("vbus_voltage"));
    
    Serial.println("Enabling closed loop control...");
    while (getState() != AXIS_STATE_CLOSED_LOOP_CONTROL) {
        clearErrors();
        setState(AXIS_STATE_CLOSED_LOOP_CONTROL);
        delay(10);
    }
    Serial.println("ODrive running!");
    delay(500); 
}

void ODriveSwerve::setPosition(float position) {
    setPosition(position, 0.0f, 0.0f);
}

void ODriveSwerve::setPosition(float position, float velocity_feedforward) {
    setPosition(position, velocity_feedforward, 0.0f);
}

void ODriveSwerve::setPosition(float position, float velocity_feedforward, float torque_feedforward) {
    serial_ << "p " << kMotorNumber  << " " << position << " " << velocity_feedforward << " " << torque_feedforward << "\n";
}

void ODriveSwerve::setVelocity(int motor_number, float velocity) {
   setVelocity(motor_number, velocity, 0.0f);
}

void ODriveSwerve::setVelocity(int motor_number, float velocity, float current_feedforward) {
    setMode(CONTROL_MODE_VELOCITY_CONTROL);
    setParameter("axis0.controller.config.input_mode", INPUT_MODE_VEL_RAMP);
    serial_ << "v " << motor_number  << " " << velocity << " " << current_feedforward << "\n";
}

void ODriveSwerve::setTorque(float torque) {
    serial_ << "c " << kMotorNumber << " " << torque << "\n";
}

void ODriveSwerve::trapezoidalMove(float position) {
    serial_ << "t " << kMotorNumber << " " << position << "\n";
}

void ODriveSwerve::goTo(float position, float velocity){
    setMode(CONTROL_MODE_POSITION_CONTROL);
    setParameter("axis0.trap_traj.config.vel_limit", velocity);
    setParameter("axis0.trap_traj.config.accel_limit", 10);
    setParameter("axis0.trap_traj.config.decel_limit", 10);
    setParameter("axis0.controller.config.input_mode", INPUT_MODE_TRAP_TRAJ);
    trapezoidalMove(position);

}

ODriveFeedback ODriveSwerve::getFeedback() {
    // Flush RX
    while (serial_.available()) {
        serial_.read();
    }

    serial_ << "f " << kMotorNumber << "\n";

    String response = readLine();

    int spacePos = response.indexOf(' ');
    if (spacePos >= 0) {
        return {
            response.substring(0, spacePos).toFloat(),
            response.substring(spacePos+1).toFloat()
        };
    } else {
        return {0.0f, 0.0f};
    }
}

String ODriveSwerve::getParameterAsString(const String& path) {
    serial_ << "r " << path << "\n";
    return readLine();
}

void ODriveSwerve::setParameter(const String& path, const String& value) {
    serial_ << "w " << path << " " << value << "\n";
}

void ODriveSwerve::setState(ODriveAxisState requested_state) {
    setParameter("axis0.requested_state", String((long)requested_state));
}

void ODriveSwerve::setMode(ODriveControlMode requested_mode){
    Serial.println((String)"Setting contorl MOde to "+requested_mode);
    setParameter("axis0.controller.config.control_mode",String((long)requested_mode));
}

ODriveAxisState ODriveSwerve::getState() {
    return getParameterAsInt("axis0.current_state");
}


String ODriveSwerve::readLine(unsigned long timeout_ms) {
    String str = "";
    unsigned long timeout_start = millis();
    for (;;) {
        while (!serial_.available()) {
            if (millis() - timeout_start >= timeout_ms) {
                return str;
            }
        }
        char c = serial_.read();
        if (c == '\n')
            break;
        str += c;
    }
    return str;
}
