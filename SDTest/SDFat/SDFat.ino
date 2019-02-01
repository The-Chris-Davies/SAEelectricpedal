/*
 * Example use of two SPI ports on an STM32 board.
 * Note SPI speed is limited to 18 MHz.
 * Pins:
 * CS = A4
 * CLK = A5
 * MISO = A6
 * MOSI = A7
 */
#include <SPI.h>
#include "SdFat.h"
#include "FreeStack.h"

// set ENABLE_EXTENDED_TRANSFER_CLASS non-zero to use faster EX classes

// Use first SPI port
SdFat sd1;
// SdFatEX sd1;
const uint8_t SD1_CS = PA4;  // chip select for sd1

const uint8_t BUF_DIM = 100;
uint8_t buf[BUF_DIM];

const uint32_t FILE_SIZE = 1000;
const uint16_t NWRITE = FILE_SIZE/BUF_DIM;
//------------------------------------------------------------------------------
// print error msg, any SD error codes, and halt.
// store messages in flash
#define errorExit(msg) errorHalt(F(msg))
#define initError(msg) initErrorHalt(F(msg))
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  // Wait for USB Serial 
  while (!Serial) {
  }

  // fill buffer with known data
  for (size_t i = 0; i < sizeof(buf); i++) {
    buf[i] = i;
  }

  Serial.println(F("type any character to start"));
  while (!Serial.available()) {
  }
  Serial.print(F("FreeStack: "));
  Serial.println(FreeStack());

  // initialize the first card
  if (!sd1.begin(SD1_CS, SD_SCK_MHZ(18))) {
    sd1.initError("sd1:");
  }
  // create Dir1 on sd1 if it does not exist
  if (!sd1.exists("/Dir1")) {
    if (!sd1.mkdir("/Dir1")) {
      sd1.errorExit("sd1.mkdir");
    }
  }
  
  // list root directory on both cards
  Serial.println(F("------sd1 root-------"));
  sd1.ls();
  
  // make /Dir1 the default directory for sd1
  if (!sd1.chdir("/Dir1")) {
    sd1.errorExit("sd1.chdir");
  }
  // remove test.bin from /Dir1 directory of sd1
  if (sd1.exists("test.bin")) {
    if (!sd1.remove("test.bin")) {
      sd1.errorExit("remove test.bin");
    }
  }
  
  // list current directory on both cards
  Serial.println(F("------sd1 Dir1-------"));
  sd1.ls();
  Serial.println(F("---------------------"));

  // set the current working directory for open() to sd1
  sd1.chvol();

  // create or open /Dir1/test.bin and truncate it to zero length
  SdFile file1;
  if (!file1.open("test.bin", O_RDWR | O_CREAT | O_TRUNC)) {
    sd1.errorExit("file1");
  }
  Serial.println(F("Writing test.bin to sd1"));

  // write data to /Dir1/test.bin on sd1
  for (uint16_t i = 0; i < NWRITE; i++) {
    if (file1.write(buf, sizeof(buf)) != sizeof(buf)) {
      sd1.errorExit("sd1.write");
    }
  }
  
  // read file1
  file1.rewind();
  uint32_t t = millis();
  Serial.println(F("Reading from SD"));
  while (1) {
    int n = file1.read(buf, sizeof(buf));
    if (n < 0) {
      sd1.errorExit("read1");
    }
    if (n == 0) {
      break;
    }
    for(uint8_t i = 0; i < n; i++){
      Serial.print(buf[i]);
    }
  }
  Serial.println(F("---------------------"));
  t = millis() - t;
  Serial.print(F("File size: "));
  Serial.println(file1.fileSize());
  Serial.print(F("Copy time: "));
  Serial.print(t);
  Serial.println(F(" millis"));
  // close test.bin
  file1.close();
  // list current directory on both cards
  Serial.println(F("------sd1 -------"));
  sd1.ls("/", LS_R | LS_DATE | LS_SIZE);
  Serial.println(F("---------------------"));
  Serial.println(F("Done"));
}
//------------------------------------------------------------------------------
void loop() {}
