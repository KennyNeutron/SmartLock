#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

byte readCard[4];
String MasterTag = "40263755";
String tagID = "";
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define LED0 4
#define LED1 5

#define BT_status 6


void setup() {
  Serial.begin(9600);
  SPI.begin();

  mfrc522.PCD_Init();
  delay(4);
  //Show details of PCD - MFRC522 Card Reader
  mfrc522.PCD_DumpVersionToSerial();

  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(BT_status, INPUT);

  LED0_OFF();
  LED1_OFF();

  Serial.println("--------------------------");
  Serial.println(" Access Control ");
  Serial.println("Scan Your Card>>");
}


/**********************************************************************************************
 * loop() function
**********************************************************************************************/
void loop() {
  //----------------------------------------------------------------------
  //Wait until new tag is available
  while (getID()) {
    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
    if (tagID == MasterTag) {
      Serial.println(" Access Granted!");
      Serial.println("--------------------------");
      LED0_ON();
      //You can write any code here like, opening doors,
      //switching ON a relay, lighting up an LED etc...
    }
    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
    else {
      Serial.println(" Access Denied!");
      Serial.println("--------------------------");
    }
    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
    delay(2000);
    Serial.println(" Access Control ");
    Serial.println("Scan Your Card>>");
  }
  //----------------------------------------------------------------------

  Serial.print("BT:");
  Serial.println(BTstatus());

  if(BTstatus()){
    LED1_ON();
  }else{
    LED1_OFF();
  }
}




/**********************************************************************************************
 * getID() function
 * Read new tag if available
**********************************************************************************************/
boolean getID() {
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  // Getting ready for Reading PICCs
  //If a new PICC placed to RFID reader continue
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Since a PICC placed get Serial and continue
  if (!mfrc522.PICC_ReadCardSerial()) {
    return false;
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  tagID = "";
  // The MIFARE PICCs that we use have 4 byte UID
  for (uint8_t i = 0; i < 4; i++) {
    //readCard[i] = mfrc522.uid.uidByte[i];
    // Adds the 4 bytes in a single String variable
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  tagID.toUpperCase();
  Serial.print("ID:");
  Serial.println(tagID);
  mfrc522.PICC_HaltA();  // Stop reading
  return true;
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
}


bool BTstatus() {
  return digitalRead(BT_status);
}
