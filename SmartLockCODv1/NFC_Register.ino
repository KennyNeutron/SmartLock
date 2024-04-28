String GetIDFromEEPROM(int addrOffset) {
  int newStrLen = 8;
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++) {
    data[i] = EEPROM.read(addrOffset + i);
  }
  data[newStrLen] = '\0';  // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)
  return String(data);
}

void WriteIDToEEPROM(int AddressToSet, const String &strToWrite) {
  EEPROM.write(AddressToSet, strToWrite[0]);
  EEPROM.write(AddressToSet + 1, strToWrite[1]);
  EEPROM.write(AddressToSet + 2, strToWrite[2]);
  EEPROM.write(AddressToSet + 3, strToWrite[3]);
  EEPROM.write(AddressToSet + 4, strToWrite[4]);
  EEPROM.write(AddressToSet + 5, strToWrite[5]);
  EEPROM.write(AddressToSet + 6, strToWrite[6]);
  EEPROM.write(AddressToSet + 7, strToWrite[7]);
}

void Register(int AddressToStore) {
  UnlockNFC = false;
  while (!getID()) {
    LED0_ON();
    LED1_OFF();
    delay(50);
    LED0_OFF();
    LED1_ON();
    delay(50);
  }

  EEPROM.write(AddressToStore, true);
  WriteIDToEEPROM(AddressToStore+1, tagID);

  delay(2000);
}
