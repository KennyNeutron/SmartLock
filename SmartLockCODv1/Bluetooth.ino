void BT_SendNFC_Reg() {
  bool tag1 = EEPROM.read(0x10);
  bool tag2 = EEPROM.read(0x20);
  bool tag3 = EEPROM.read(0x30);
  bool tag4 = EEPROM.read(0x40);

  String msg = "A," + String(tag1) + "," + String(tag2) + "," + String(tag3) + "," + String(tag4) + "," + String(DeviceUnlock) + ",B";
  Serial.print("BT Written:");
  Serial.println(msg);
  BTSerial.print(msg);
}