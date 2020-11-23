#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "Pinmap.h"

SDLib::File myFile;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial){;}
  Serial.print("Initializing SD card...");
  if (!SD.begin(sd_CS))
  {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  myFile = SD.open("motion1.txt", FILE_WRITE);
  if (myFile) {
    Serial.println("File created successfully.");
    myFile.println("030
60
90
120
150
")
    return 1;
  } else {
    Serial.println("Error while creating file.");
    return 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}