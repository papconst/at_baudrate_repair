/*  After a day of coding I made destructive mistake.
 *  I changed my m590's baudrate to something different from 9600...
 *  It revealed on next reboot: i received crap on my SoftwareSerial. Like squares, dollars etc...
 *  Replace m590 with a new one wasnt an option, so I made this skatch.
 *  Originally it tested on m590, but should work with any AT-cmd based controller. Just turn it on.
 *  
 *  Created 2018.02.15 by Papkov K
 *  papkov.c@gmail.com
 *  
 *  ...and baudrate I set was 9800. Misprint.
 */
#include <SoftwareSerial.h>
#define gsmBoot 5
#define gsmRx 4
#define gsmTx 3
SoftwareSerial gsmSerial(gsmRx, gsmTx);
uint32_t baud_s = 1,baud_c,baud_f = 250000;// I wasnt sure about range, so made it as wide as possible;
                                           // You can make it more narrow to faster search.
                                           // At baudrate 1 it was approx. 1 scan per sec, on 10k few hundreds.
String ok = "";
void setup()
{
  Serial.begin(9600);
  //m590 boot & ready to work. May be unnecessary with different controller.
  pinMode(gsmBoot,OUTPUT);
  digitalWrite(gsmBoot,HIGH);delay(300);
  digitalWrite(gsmBoot,LOW);delay(300);
  digitalWrite(gsmBoot,HIGH);
  delay(30000);

}
String readSerial()
{
  String tmp = "";
  while (gsmSerial.available())
    tmp += char(gsmSerial.read());
  return tmp;
}
void loop()
{
  for(baud_c = baud_s; baud_c <= baud_f; baud_c++)// Scanning all baudrates in range.
                                                  // Perhaps there's a sence to make this cycle downto (i--),
                                                  // but i was pretty sure my baudrate wasnt to big
  {
    gsmSerial.begin(baud_c);
    gsmSerial.println("AT");
    ok = readSerial();
    Serial.print(baud_c);Serial.print(":");Serial.println(ok);
    if(ok.indexOf("OK") > 0)// If we received correct responce...
    {
      gsmSerial.println("AT+IPR=9600");// ... we can set correct baudrate now
      Serial.println("Baud = " + (String)baud_c);// ...print it in console
      while(1){}// ... and do hothing forever. Take a cookie, you're awesome!
    }
    gsmSerial.end();
  }
}
