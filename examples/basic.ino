/*
* Demo program to demonstrate the features of the app.
*/

#include <SoftwareSerial.h>
#include <ArduinoBLE.h>

// PINS
#define BLUETOOTH_TX 8
#define BLUETOOTH_RX 7

// VARIABLES

// 49 is the still value for the throttle and steering
int prevThrottle = 49, prevSteering = 49,
        throttle, steering, prevMillis, sliderVal, button, sliderId;

String str;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBLE phone(bluetooth); // pass reference of bluetooth object to ArduinoCommander.

// Setup code runs once after program starts
void setup() {
    // Start serial monitor at 9600 bps
    Serial.begin(9600);

    // Start bluetooth serial at 9600 bps
    bluetooth.begin(9600);

    // delay just in case bluetooth module needs time to "get ready"
    delay(100);

    Serial.println("setup complete");
    prevMillis = millis();
}

// Put your main code here, to run repeatedly:
void loop() {
    // ID of the button pressed pressed
    button = phone.getButton();

    // Returns the text data sent from the phone.
    // After it returns the latest data, empty string "" is sent in subsequent
    // calls until text data is sent again.
    str = phone.getText();

    // throttle and steering values go from 0 to 99.
    throttle = phone.getThrottle();
    steering = phone.getSteering();

    // ID of the slider moved
    sliderId = phone.getSliderId();

    // slider value goes from 0 to 200
    sliderVal = phone.getSliderVal();

    // display button data whenever its pressed
    if (button != -1) {
        Serial.print("Button: ");
        Serial.println(button);
    }

    // display slider data when slider moves
    if (sliderId != -1) {
        Serial.print("Slider ID: ");
        Serial.print(sliderId);
        Serial.print("\tValue: ");
        Serial.println(sliderVal);
    }

    // display throttle and steering data every half second when if steering or throttle value is changed
    if (abs(millis() - prevMillis) > 500
        && (prevThrottle != throttle || prevSteering != steering)) {
        Serial.println("----------------------------");
        Serial.print("Throttle: ");
        Serial.println(throttle);
        Serial.print("Steering: ");
        Serial.println(steering);
        prevThrottle = throttle;
        prevSteering = steering;
        prevMillis = millis();
    }

    if (str != "") {
        Serial.print(str);
        Serial.print("\n");
    }

    // Send string from serial command line to the phone. This will alert the user.
    if (Serial.available()) {
        Serial.write("usb: ");
        String str = Serial.readString();
        phone.sendMessage(str);
        Serial.print(str);
        Serial.write('\n');
    }
}
