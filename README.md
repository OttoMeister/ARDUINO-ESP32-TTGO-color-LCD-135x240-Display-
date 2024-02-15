# ESP32-TTGO

ARDUINO ESP32 mit TTGO color LCD 135x240 Display 
https://www.aliexpress.com/item/1005003745502352.html

![](FirstProjectEsp32TTGO.jpeg)


## How to install ARDUINO IDE 2.0.3 with ESP32 support on Linux
erase all old traces, then download, install and start it
```
rm -rf ~/Schreibtisch/arduino-ide* ~/.arduinoIDE ~/.arduino15 ~/.config/arduino* ~/.config/Arduino* ~/Arduino*
wget https://downloads.arduino.cc/arduino-ide/arduino-ide_2.0.3_Linux_64bit.AppImage
mv arduino-ide_2.0.3_Linux_64bit.AppImage ~/Schreibtisch
chmod u+x ~/Schreibtisch/arduino-ide_2.0.3_Linux_64bit.AppImage
~/Schreibtisch/arduino-ide_2.0.3_Linux_64bit.AppImage & disown
```

## Install Arduino IDE 2.3.0 on Windows 
Download from here:
```
https://downloads.arduino.cc/arduino-ide/arduino-ide_2.3.0_Windows_64bit.exe
```
Start the installer, klick yes to everything.

## Settings in Arduino IDE:<br>
Menü -> File -> Preferences -> Language -> English <br>
Menü -> File -> Preferences -> Show verbose output during compile und upload -> check <br>
Menü -> File -> Preferences -> Compiler warnings -> Default <br>
Menü -> File -> Preferences -> Additional URLs -> `https://dl.espressif.com/dl/package_esp32_index.json` <br>
Menü -> File -> Preferences -> OK <br>
Libary Manager -> TFT_eSPI -> search and install: TFT_eSPI by Bodmer ver 2.5.34 <br>
Libary Manager -> TFT_eSPI -> search and install: LovyanGFX by lovyan03 ver 1.1.12 <br>
Libary Manager -> TFT_eSPI -> search and install: NTPClient by F.Weinberg ver 3.2.1 <br>
Libary Manager -> TFT_eSPI -> search and install: HTTPClient by A.McEwen ver 2.2.0 <br>
Libary Manager -> TFT_eSPI -> search and install: ArduinoJson.h by B.Blanchon ver 7.0.3 <br>
Bords Manager -> ESP32 search and install esp32 by Espressif Sytems instalieren <br>
Select Board and Port -> ESP32 Dev Module
-CPU Frequency : “240MHz (WiFi/BT)” <br>
-Core Degug Level : “Verbose” <br>
-Flash Frequency : “80MHz” <br>
-Flash Mode : “QIO” or “DIO” <br>
-Flash Size : “4MB (32Mb)” <br>
-Partition Scheme : “Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)” <br>
-PSRAM : “Disabled” <br>
-Upload Speed: “921600”  <br>

## config library:<br>
Linux: pluma ~/Arduino/libraries/TFT_eSPI/User_Setup_Select.h <br>
Windows: notepad %USERPROFILE%\Documents\Arduino\libraries\TFT_eSPI\User_Setup_Select.h <br>
Line 30 #include <User_Setup.h> comment out with // <br>
Line 61 #include <User_Setups/Setup25_TTGO_T_Display.h> delete "//" and activate <br>
Save <br>
