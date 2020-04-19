
void sendCommand(char* c)
{
  char i=0;
  while(c[i] != '\0')
  {
    Serial1.write(c[i]); //WriteUSART(c[i]);
    //while (BusyUSART()); // attendo che il dato venga trasmesso
    delayMicroseconds(50);
    i++;
  }

  Serial1.write('\0');
  Serial1.write(0x0D);
  Serial1.write(0x0A);

  delay(1000);
}


void setBaudRate_9600N81()
{
  // Baud Rate
  // AT+UART=9600,0,0 ----set the baud rate to be 9600N81
  char command[] = { 'A', 'T', '+', 'U', 'A', 'R', 'T', '=', '9', '6', '0', '0', ',', '0', ',', '0', '\0' };   
  sendCommand(command);
}


void setDeviceName(char* name)
{
  // Device Name
  // FORMATO "AT+NAME=nome";   Es: "AT+NAME=ProgPIC"
  char i, j;
  char command[30];

  command[0]='A'; command[1]='T'; command[2]='+'; command[3]='N'; command[4]='A'; command[5]='M'; command[6]='E'; // command[7]='='; 
  
  i=7;
  j=0;
  while(name[j] != '\0')     // Concateno il nome ricevuto come parametro al comando
  {
    command[i] = name[j];
    i++;
    j++;
  }
  command[i] = '\0';
  sendCommand(command);
}


void setDevicePIN(char* pin)
{
  // Device PIN
  // FORMATO "AT+PINxxxx";   Es: "AT+PIN1234"
  char i, j;
  char command[13];

  command[0]='A'; command[1]='T'; command[2]='+'; command[3]='P'; command[4]='S'; command[5]='W'; command[6]='D'; // command[7]='=';
  
  i=7;
  j=0;
  while(pin[j] != '\0')     // Concateno il pin ricevuto come parametro al comando
  {
    command[i] = pin[j];
    i++;
    j++;
  }
  command[i] = '\0';
  sendCommand(command);
}



void Init_BluetoothDevice()
{
  char deviceName[] = "ProgPIC";
  char devicePIN[]  = "1234";
  setBaudRate_9600N81();
  setDeviceName(deviceName);
  setDevicePIN(devicePIN);
}


void setup()
{
  Serial1.begin(9600);
  Serial.begin(9600);
  delay(500);
  Init_BluetoothDevice();
}

void loop()
{
  if (Serial.available())
  {
    byte a=Serial.read();
    Serial1.write(a);
  }

  if (Serial1.available())
  {
    byte data=Serial1.read();
    if (data >= 'a')
      data -= 'a'-'A';
    delay(500);
    Serial1.write(data);
    Serial.write(data);
  }  
}
