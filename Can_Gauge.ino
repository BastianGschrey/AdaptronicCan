#include <LiquidCrystal_I2C.h>

#include <mcp_can.h>
#include <SPI.h>


long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string
int printValueInt = 0;
float printValueFloat = 0.0;

#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);                               // Set CS to pin 10

LiquidCrystal_I2C lcd(0x27,16,2); 


void setup()
{
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK){
    Serial.println("MCP2515 Initialized Successfully!");
    lcd.setCursor(0, 0);
    lcd.print("CAN Init OK");
    delay(250);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MAP:");
    lcd.setCursor(0, 1);
    lcd.print("02:" );
    lcd.setCursor(8, 0);
    lcd.print("MAT:");
    lcd.setCursor(8, 1);
    lcd.print("FuP:");
  }
  else{
    Serial.println("Error Initializing MCP2515...");
     lcd.setCursor(0, 0);
    lcd.print("CAN Init failed");
    }
  
  
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
  
  Serial.println("MCP2515 Library Receive Example...");
}

void loop()
{
  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)



switch(rxId) {
  case 0x361: // MAP
           if(rxId == 865) //search for MAP
           printValueInt = (word(rxBuf[0], rxBuf[1])/10);
      
           lcd.setCursor(4, 0);
           if(printValueInt < 100){
           lcd.print(" ");
           lcd.setCursor(5, 0);
           lcd.print(printValueInt);
           }
           else{
           lcd.print(printValueInt);
           }
           break;
      
 case 0x364: // Lamdba
           printValueFloat = (word(rxBuf[2], rxBuf[3])/1000.00);
      
           lcd.setCursor(3, 1);
       
           lcd.print(printValueFloat);
      
           break;

 case 0x304: // Voltage Battery
          printValueFloat = (word(rxBuf[2], rxBuf[3])/1000.00);
          Serial.println(printValueFloat, 2);
          break;       

 case 0x365:  //MAT
         
          printValueInt = (word(rxBuf[0], rxBuf[1])/10);
          lcd.setCursor(13, 0);
          lcd.print(printValueInt);

          break;      

case 0x366:  //Fuel Pressure
         
          printValueFloat = (word(rxBuf[0], rxBuf[1])/10.0);
          lcd.setCursor(12, 1);
          lcd.print(printValueInt);

          break;
 }
 
  
  }
}
