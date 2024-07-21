
#ifndef ODriveArduino_h
#define ODriveArduino_h

#include "Arduino.h"
#include "ODriveEnums.h"

struct ODriveFeedback {
    float pos;
    float vel;
};

class ODriveArduino {
public:
    /**
     * @brief Constructs an ODriveArduino instance that will communicate over
     * the specified serial port.
     */
    ODriveArduino(Stream& serial);

    /**
     * @brief Clears the error status of the ODrive and restarts the brake
     * resistor if it was disabled due to an error.
     */
    void clearErrors();

    /**
     * @brief Sends a new position setpoint.
     */
    void setPosition(float position);

    /**
     * @brief Sends a new position setpoint with a velocity feedforward term.
     */
    void setPosition(float position, float velocity_feedforward);

    /**
     * @brief Sends a new position setpoint with velocity and torque feedforward terms.
     */
    void setPosition(float position, float velocity_feedforward, float torque_feedforward);

    /**
     * @brief Sends a new velocity setpoint.
     */
    void SetVelocity(int motor_number, float velocity);

    /**
     * @brief Sends a new velocity setpoint with a torque feedforward term.
     */
    void SetVelocity(int motor_number, float velocity, float current_feedforward);

    /**
     * @brief Sends a new torque setpoint.
     */
    void setTorque(float torque);

    /**
     * @brief Puts the ODrive into trapezoidal trajectory mode and sends a new
     * position setpoint.
     */
    void trapezoidalMove(float position);

    /**
     * @brief Requests the latest position and velocity estimates.
     * 
     * Returns pos = 0.0 and vel = 0.0 in case of a communication error.
     */
    ODriveFeedback getFeedback();

    /**
     * @brief Requests the latest position estimate.
     * 
     * Returns 0.0 in case of a communication error.
     */
    float getPosition() { return getFeedback().pos; }

    /**
     * @brief Requests the latest velocity estimate.
     * 
     * Returns 0.0 in case of a communication error.
     */
    float getVelocity() { return getFeedback().vel; }

    // Generic parameter access
    String getParameterAsString(const String& path);
    long getParameterAsInt(const String& path) { return getParameterAsString(path).toInt(); }
    float getParameterAsFloat(const String& path) { return getParameterAsString(path).toFloat(); }
    void setParameter(const String& path, const String& value);
    void setParameter(const String& path, long value) { setParameter(path, String(value)); }

    /**
     * @brief Tells the ODrive to change its axis state.
     */
    void setState(ODriveAxisState requested_state);

    /**
     * @brief Requests the current axis state from the ODrive.
     * 
     * Returns AXIS_STATE_UNDEFINED in case of a communication error.
     */
    ODriveAxisState getState();

private:
    String readLine(unsigned long timeout_ms = 10);

    Stream& serial_;
};

#endif //ODriveArduino_h
