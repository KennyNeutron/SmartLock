#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

SoftwareSerial BTSerial(2, 3);  // RX, TX

#define SS_PIN 10
#define RST_PIN 9

byte readCard[4];

String tagID = "";
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define LED0 4
#define LED1 5

#define BT_status 6

#define Relay0 7
#define Relay1 8

bool UnlockNFC = false;
bool UnlockBT = false;
bool DeviceUnlock = false;

uint32_t bt_last_millis = 0;
void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  SPI.begin();

  mfrc522.PCD_Init();
  delay(4);
  //Show details of PCD - MFRC522 Card Reader
  mfrc522.PCD_DumpVersionToSerial();

  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(BT_status, INPUT);
  pinMode(Relay0, OUTPUT);
  pinMode(Relay1, OUTPUT);

  LockRelay0();
  LockRelay1();
  LED0_OFF();
  LED1_OFF();

  // EEPROM.write(0x10, false);
  // EEPROM.write(0x20, false);
  // EEPROM.write(0x30, false);
  // EEPROM.write(0x40, false);

  // EEPROM.write(0x10, true);
  // WriteIDToEEPROM(0x11, MasterTag);

  Serial.print("EEPROM tag1 Reg:");
  Serial.println(EEPROM.read(0x10));
  Serial.print("EEPROM tag1 ID:");
  Serial.println(GetIDFromEEPROM(0x11));

  Serial.println("--------------------------");
  Serial.println(" Access Control ");
  Serial.println("Scan Your Card>>");
  bt_last_millis = millis();
}



void loop() {
  //----------------------------------------------------------------------
  //Wait until new tag is available
  while (getID()) {
    Serial.print("ID Read Success\ntagID:");
    Serial.println(tagID);
    int tagNum = 0x10;
    for (int a = 0; a < 4; a++) {
      Serial.print("Read ID at 0x");
      Serial.println(tagNum, HEX);
      if (EEPROM.read(tagNum) == true && tagID == GetIDFromEEPROM(tagNum + 1)) {
        UnlockNFCmode();
      }
      tagNum += 0x10;
    }


    delay(2000);
    Serial.println(" Access Control ");
    Serial.println("Scan Your Card>>");
  }
  //----------------------------------------------------------------------

  if (!BTstatus) {
    UnlockBT = false;
  }
  if (UnlockBT) {
    LED1_ON();
    UnlockRelay1();
  } else {
    UnlockNFC = false;
    LED1_OFF();
    LockRelay1();
  }


  if (UnlockNFC) {
    LED0_ON();
    UnlockRelay0();
  } else {
    LED0_OFF();
    LockRelay0();
  }


  if (BTSerial.available() > 0) {
    uint8_t UARTdata = BTSerial.read();

    Serial.println("BT msg recieved!");
    Serial.print("DATA:");
    Serial.println(UARTdata);
    if (UARTdata >= 'A' && UARTdata <= 'D') {
      Register((UARTdata - 64) * 0x10);
    } else if (UARTdata >= 'a' & UARTdata <= 'd') {
      EEPROM.write((UARTdata - 96) * 0x10, false);
    } else if (UARTdata == 'E') {
      UnlockBT = true;
    } else if (UARTdata == 'e') {
      UnlockBT = false;
    }
  }


  if (UnlockNFC == true && UnlockBT == true) {
    DeviceUnlock = true;
  } else {
    DeviceUnlock = false;
  }

  if (millis() - bt_last_millis >= 1500) {
    BT_SendNFC_Reg();
    bt_last_millis = millis();
  }
}


void UnlockNFCmode() {
  Serial.println(" Access Granted!");
  Serial.println("--------------------------");
  LED0_ON();
  UnlockRelay0();
  UnlockNFC = true;
}



bool BTstatus() {
  return digitalRead(BT_status);
}


void LockRelay0() {
  digitalWrite(Relay0, 1);
}

void UnlockRelay0() {
  digitalWrite(Relay0, 0);
}

void LockRelay1() {
  digitalWrite(Relay1, 1);
}

void UnlockRelay1() {
  digitalWrite(Relay1, 0);
}