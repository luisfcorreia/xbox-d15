/*
  Example sketch for the Xbox 360 USB library - developed by Kristian Lauszus
  For more information visit my blog: http://blog.tkjelectronics.dk/ or
  send me an e-mail:  kristianl@tkjelectronics.com
*/

#include <XBOXUSB.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
XBOXUSB Xbox(&Usb);

float xpos;
float ypos;
int xpos_d15;
int ypos_d15;

// MCP4231 related stuff
byte Xaddress = B00010000;
byte Yaddress = B00000000;
int CS = 6;

#define ABUTTON A0
#define BBUTTON A1

int potWrite(byte address, int value)
{
  digitalWrite(CS, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS, HIGH);
}


void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXBOX USB Library Started"));

  pinMode(ABUTTON, OUTPUT);
  pinMode(BBUTTON, OUTPUT);

  pinMode (CS, OUTPUT);
  SPI.begin();

}

void loop() {
  Usb.Task();
  if (Xbox.Xbox360Connected) {
    if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500) {
      if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) {
        xpos = Xbox.getAnalogHat(LeftHatX);
        if (xpos >= 1) {
          // got positive value
          xpos_d15 = map(xpos, 7500, 32768, 128, 255);
        } else {
          // got negative value
          xpos_d15 = map(xpos, -32767, -7500, 0, 127);
        }
        potWrite(Xaddress, xpos_d15);
        Serial.print(F("LeftHatX: "));
        Serial.print(xpos);
        Serial.print("\t");
        Serial.print(xpos_d15);
        Serial.print("\t");
      }
      if (Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500) {
        ypos = Xbox.getAnalogHat(LeftHatY);
        if (ypos >= 1) {
          // got positive value
          ypos_d15 = map(ypos, 7500, 32768, 128, 255);
        } else {
          // got negative value
          ypos_d15 = map(ypos, -32767, -7500, 0, 127);
        }
        potWrite(Yaddress, ypos_d15);
        Serial.print(F("LeftHatY: "));
        Serial.print(ypos);
        Serial.print("\t");
        Serial.print(xpos_d15);
        Serial.print("\t");
      }
      Serial.println();
    }

    // Process button presses
    if (Xbox.getButtonClick(A)) {
      digitalWrite(ABUTTON, HIGH);
      Serial.println("A");
    } else {
      digitalWrite(ABUTTON, LOW);
    }

    if (Xbox.getButtonClick(B)) {
      digitalWrite(BBUTTON, HIGH);
      Serial.println("B");
    } else {
      digitalWrite(BBUTTON, LOW);
    }
  }
  delay(1);
}
