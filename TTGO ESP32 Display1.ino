
/*  
#ARDUINO IDE 2.0.2 mit TTGO ESP32 Display :1.14 inch color LCD 135x240 display
#https://www.aliexpress.com/item/1005003745502352.html
#Arduino Linux Serial Permissions
Run dmesg to determine idVendor and idProduct of the USB interfaces (in this case there are two.)
Use sudo and your favorite editor to create a file: “/etc/udev/rules.d/50-myusb.rules”
Using the idVendor and idProduct numbers, create two lines in the file using this format (replacing the numbers shown with your numbers):
SUBSYSTEMS==“usb”, ATTRS{idVendor}==“2e8a”, ATTRS{idProduct}==“0004”, GROUP=“plugdev”, MODE=“0660”, TAG+=“uaccess”
Save and close the file then restart your system.
#kleiner Fehler beheben
#"exec: "python": executable file not found in $PATH
#Error compiling for board ESP32 Wrover Module" 
sudo ln -s /usr/bin/python3 /usr/bin/python
#Arduino download
wget https://downloads.arduino.cc/arduino-ide/arduino-ide_2.0.2_Linux_64bit.AppImage
mv arduino-ide_2.0.2_Linux_64bit.AppImage ~/Schreibtisch
chmod u+x ~/Schreibtisch/arduino-ide_2.0.2_Linux_64bit.AppImage
# Starten im Terminal im hintergrund
~/Schreibtisch/arduino-ide_2.0.2_Linux_64bit.AppImage & disown
# Einstellungen
Menü -> File -> Preferences -> Language -> English
Menü -> File -> Preferences -> Show verbose output during compile und upload -> check
Menü -> File -> Preferences -> Compiler warnings -> Default
Menü -> File -> Preferences -> Additional URLs -> https://dl.espressif.com/dl/package_esp32_index.json
Menü -> File -> Preferences -> OK
Libary Manager -> TFT_eSPI -> Suchen und Installieren  TFT_eSPI by Bodmer
Bords Manager -> ESP32 suchen und esp32 by Espressif Sytems instalieren
Tools -> Board -> esp32 -> ESP32 Dev Modul auswählen
Tools -> Port -> /dev/ttyACM0 auswählen -> OK
pluma ~/Arduino/libraries/TFT_eSPI/User_Setup_Select.h
Zeile 30 #include <User_Setup.h>  mit // auskommentieren
Zeile 61 #include <User_Setups/Setup25_TTGO_T_Display.h> "//" löschen und aktivieren
Speichern
#optional
File -> Examples -> Examples from Custom Libary -> TFT_eSPI -> 160x128 -> TFT-Print_Test laden
Compile und Upload -> BINGO
# Weitere Libs installieren
Libary Manager -> TFT_eSPI -> Suchen und Installieren  -> "Adafruit BMP085 Libary by Adafruit" -> INSTALL ALL
Libary Manager -> TFT_eSPI -> Suchen und Installieren  -> "Adafruit MPU6050 by Adafruit" -> INSTALL ALL
Libary Manager -> TFT_eSPI -> Suchen und Installieren  -> "NTPClient by Fabrice Weinberg" -> INSTALL ALL
*/

#include <TFT_eSPI.h>
#include <SPI.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_MPU6050.h>
#include <WiFi.h>
#include <NTPClient.h>

// Replace with your WiFi credentials
const char* ssid = "xxxxxxxxx";
const char* password = "xxxxx";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
Adafruit_BMP085 bmp;
Adafruit_MPU6050 mpu;
TFT_eSPI tft = TFT_eSPI();
int wifiTimeOutCounter = 0;
uint32_t targetTime = 0;
uint32_t chipId = 0;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10);
  Serial.println("Booting...");

  for (int i = 0; i < 17; i = i + 8) chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  Serial.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  Serial.printf("This chip has %d cores\n", ESP.getChipCores());
  Serial.print("Chip ID: ");
  Serial.println(chipId);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifiTimeOutCounter++;
    if (wifiTimeOutCounter >= 60) ESP.restart();
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  timeClient.setTimeOffset(-3600 * 5);

  if (!mpu.begin()) Serial.println("Failed to find MPU6050 chip");
  if (!bmp.begin()) Serial.println("BMP180 Sensor not found ! ! !");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
}

// Leider fehlt das in der Standard-Libary
String getFormattedDate() {
#define LEAP_YEAR(Y) ((Y > 0) && !(Y % 4) && ((Y % 100) || !(Y % 400)))
  unsigned long rawTime = timeClient.getEpochTime() / 86400L;
  unsigned long days = 0, year = 1970;
  uint8_t month;
  static const uint8_t monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  while ((days += (LEAP_YEAR(year) ? 366 : 365)) <= rawTime)
    year++;
  rawTime -= days - (LEAP_YEAR(year) ? 366 : 365);
  days = 0;
  for (month = 0; month < 12; month++) {
    uint8_t monthLength;
    if (month == 1) {
      monthLength = LEAP_YEAR(year) ? 29 : 28;
    } else {
      monthLength = monthDays[month];
    }
    if (rawTime < monthLength) break;
    rawTime -= monthLength;
  }
  String monthStr = ++month < 10 ? "0" + String(month) : String(month);
  String dayStr = ++rawTime < 10 ? "0" + String(rawTime) : String(rawTime);
  //return String(year) + "-" + monthStr + "-" + dayStr;
  return String(dayStr) + "-" + monthStr + "-" + year;
}

void loop() {
  if (targetTime < millis()) {
    targetTime = millis() + 1000;
    render();
  }
}

static void render() {
  String strtmp;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  strtmp = "Gyro XYZ " + String(g.gyro.x, 2) + " "
           + String(g.gyro.y, 2) + " "
           + String(g.gyro.z, 2) + " rad/s    ";
  tft.drawString(strtmp,0,0,2);
  strtmp = "Accel XYZ " + String(a.acceleration.x, 1) + " "
           + String(a.acceleration.y, 1) + " "
           + String(a.acceleration.z, 1) + " m/s^2   ";
  tft.drawString(strtmp,0,22,2);

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  timeClient.update();
  strtmp = timeClient.getFormattedTime() + "  " + getFormattedDate();
  if (timeClient.isTimeSet()) tft.drawString(strtmp, 0, 85, 4);

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  strtmp = String(bmp.readTemperature(), 1) + " C  ";
  strtmp = strtmp + String((float)bmp.readPressure() / 100.0, 1) + " mBar";
  tft.drawString(strtmp, 0, 115, 4);

  Serial.print("AccelX:");
  Serial.print(a.acceleration.x);
  Serial.print(",");
  Serial.print("AccelY:");
  Serial.print(a.acceleration.y);
  Serial.print(",");
  Serial.print("AccelZ:");
  Serial.print(a.acceleration.z);
  Serial.print(", ");
  Serial.print("GyroX:");
  Serial.print(g.gyro.x);
  Serial.print(",");
  Serial.print("GyroY:");
  Serial.print(g.gyro.y);
  Serial.print(",");
  Serial.print("GyroZ:");
  Serial.println(g.gyro.z);

}
