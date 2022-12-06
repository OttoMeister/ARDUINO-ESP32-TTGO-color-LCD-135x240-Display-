# ESP32-TTGO

ARDUINO ESP32 mit TTGO color LCD 135x240 Display 
https://www.aliexpress.com/item/1005003745502352.html

Das habe ich mal zum Test zusammen kopiert. Aber heilige Scheisse, die Arduino IDE ist müll, mehr als ein Blinklicht will man da nicht mit machen.

![](FirstProjectEsp32TTGO.jpeg)

# Ubuntu Anleitung
#ARDUINO IDE 2.0.2 mit TTGO ESP32 Display :1.14 inch color LCD 135x240 display <br>
#https://www.aliexpress.com/item/1005003745502352.html <br>
#Arduino Linux Serial Permissions <br>
Run dmesg to determine idVendor and idProduct of the USB interfaces (in this case there are two.) <br>
Use sudo and your favorite editor to create a file: “/etc/udev/rules.d/50-myusb.rules” <br>
Using the idVendor and idProduct numbers, create two lines in the file using this format (replacing the numbers shown with your numbers): <br>
SUBSYSTEMS==“usb”, ATTRS{idVendor}==“2e8a”, ATTRS{idProduct}==“0004”, GROUP=“plugdev”, MODE=“0660”, TAG+=“uaccess” <br>
Save and close the file then restart your system. <br>
#kleiner Fehler beheben <br>
#"exec: "python": executable file not found in $PATH <br>
#Error compiling for board ESP32 Wrover Module"  <br>
sudo ln -s /usr/bin/python3 /usr/bin/python <br>
#Arduino download <br>
wget https://downloads.arduino.cc/arduino-ide/arduino-ide_2.0.3_Linux_64bit.AppImage <br>
mv arduino-ide_2.0.3_Linux_64bit.AppImage ~/Schreibtisch <br>
chmod u+x ~/Schreibtisch/arduino-ide_2.0.3_Linux_64bit.AppImage <br>
#Starten im Terminal im hintergrund <br>
~/Schreibtisch/arduino-ide_2.0.3_Linux_64bit.AppImage & disown <br>
#Einstellungen <br>
Menü -> File -> Preferences -> Language -> English <br>
Menü -> File -> Preferences -> Show verbose output during compile und upload -> check <br>
Menü -> File -> Preferences -> Compiler warnings -> Default <br>
Menü -> File -> Preferences -> Additional URLs -> https://dl.espressif.com/dl/package_esp32_index.json <br>
Menü -> File -> Preferences -> OK <br>
Libary Manager -> TFT_eSPI -> Suchen und Installieren -> "TFT_eSPI by Bodmer" <br>
Bords Manager -> ESP32 suchen und esp32 by Espressif Sytems instalieren <br>
Tools -> Board -> esp32 -> ESP32 Dev Modul auswählen <br>
Tools -> Port -> /dev/ttyACM0 auswählen -> OK <br>
pluma ~/Arduino/libraries/TFT_eSPI/User_Setup_Select.h <br>
Zeile 30 #include <User_Setup.h>  mit // auskommentieren <br>
Zeile 61 #include <User_Setups/Setup25_TTGO_T_Display.h> "//" löschen und aktivieren <br>
Speichern <br>
#optional <br>
File -> Examples -> Examples from Custom Libary -> TFT_eSPI -> 160x128 -> TFT-Print_Test laden <br>
Compile und Upload -> BINGO <br>
#Weitere Libs installieren <br>
Libary Manager -> TFT_eSPI -> Suchen und Installieren  -> "Adafruit BMP085 Libary by Adafruit" -> INSTALL ALL  <br>
Libary Manager -> TFT_eSPI -> Suchen und Installieren  -> "Adafruit MPU6050 by Adafruit" -> INSTALL ALL <br>
Libary Manager -> TFT_eSPI -> Suchen und Installieren  -> "NTPClient by Fabrice Weinberg" -> INSTALL ALL <br>
