void printSpecifiedLines(String dirName, EthernetClient client, long startingLine, long numLines) {
  File myFile = SD.open(dirName);
  if (myFile) {
    Serial.print(F("STARTINGLINE: "));
    Serial.println(startingLine);
    Serial.print(F("NUMLINES: "));
    Serial.println(numLines);
    int printedLines = 0;
    // seek starting byte
    long startingByte = startingLine * (RECORD_LENGTH + 2);
    if (myFile.seek(startingByte)) {
      Serial.println(F("SUCCESSFUL SEEK"));
      printHeader(client, 200);
      client.println(F("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime"));
      Serial.println(F("time,tankid,temp,temp setpoint,pH,pH setpoint,onTime"));
    } else {
      Serial.println(F("FAILED SEEK"));
      printHeader(client, 416);
      // if the starting byte is greater than file size, print an error:
      client.print(F("starting byte is greater than file size at file: "));
      client.println(dirName);
      // close the file:
      myFile.close();
      return;
    }
    // print specified lines
    while (printedLines++ < numLines) {
      // fill up buffer
      char line[100];
      memset(line, 0, 100);
      for (int i = 0; i < 100 && myFile.available(); i++) {
        byte read = myFile.read();
        if ((char)read == '\n') {
          break;
        }
        line[i] = read;
      }
      // write buffer to client
      client.write(line, strlen(line));
      Serial.write(line, strlen(line));
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    printHeader(client, 404);
    client.print(F("error opening file at directory"));
    client.println(dirName);
  }
}