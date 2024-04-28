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
