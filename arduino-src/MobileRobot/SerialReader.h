class SerialReader {
  private:
    byte dataLen;
    byte strLen;

  public:
    SerialReader(byte dataLen, byte strLen) {
      this->dataLen = dataLen;
      this->strLen = strLen;
    }

    /*
    Recieves and processes incoming serial data: <xAxis, yAxis, kp, ki, kd, mSpeed>
    Returns whether or not the complete string of data was recieved
    */
    bool read(float data[]) {
      char incomingChar = 0;
      if (Serial.available() > 0) {
        char buffer[dataLen][strLen+1];
        incomingChar = Serial.read();
        if (incomingChar == '<') {
          for (byte i = 0; i < dataLen; i++) {
            for (byte j = 0; j < strLen; j++) {
              incomingChar = Serial.read();
              if (incomingChar == ',' || incomingChar == '>') {
                buffer[i][j] = '\0';
                break;
              }
              else buffer[i][j] = incomingChar;
            }
            data[i] = atof(buffer[i]);
          }
        }
      }
      return incomingChar == '>' ? true : false;
    }

    //Returns an integer value based on the data that has changed
    int getCmd(float data[], float prevData[]) {
      for (byte i = 0; i < dataLen; i++)
        if (prevData[i] != data[i])
          return i == 0 ? i : i - 1;
      return -1;
    }

    //Updates current data to keep track of changes
    void update(float data[], float prevData[]) {
      for (byte i = 0; i < dataLen; i++) 
        prevData[i] = data[i];
    }  

    //Resets a bounded range of values in the data array to zero
    void reset(float data[], float prevData[], byte lowerBound, byte upperBound) {
      for (byte i = lowerBound; i < upperBound; i++) {
        data[i] = 0;
        prevData[i] = 0;
      }
    }

    //Flushes garbage data
    void flush() {
      while (Serial.available() > 0) Serial.read();
    }
};