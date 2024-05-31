
# **WiFiExe: ESP32-S3 based BadUSB**

WifiExe is an ESP32S3 based project that demonstrates HID (Human Interface Device) payload execution capabilities. It allows users to remotely execute keyboard payloads via a simple web interface. The project uses combination of batch scripts, and cmd prompts along with libraries developed by other authors in order to achieve it's goal. 

> Disclaimer: This project is for educational and research purposes only. The authors are not responsible for misuse or damage. Unauthorized access or data extraction without permission is illegal. Ensure compliance with laws and obtain proper authorization before use.


## Features

- Default Payload- Extract Browser Data (Password, Cookies, History, Bookmarks)

- Web Based Interface- All operations are controlled through a webserver running on ESP32

- Custom Payloads- Web Interface has an option to run custom batch files with admin priviledges


## Demo

Payload Execution-

https://github.com/ravssh/WifiExe/assets/171224699/97702358-c742-4b4d-b562-b0061a265265



Post-Exploitation-

![files](https://github.com/ravssh/WifiExe/assets/171224699/f72a5d64-d573-4d56-985d-311117c3d6ce)


brave_default_password.csv-

![brave_pass](https://github.com/ravssh/WifiExe/assets/171224699/0fc31668-0f31-4114-a204-dd71d9e742e6)



## Limitations

- May not work on  < Windows 11

- Due to processing bottleneck, it is comparitively slower as there are multiple tasks running simultaneously

- Slow speed of USB 1.1, thus exploit takes long time to run

- May crash if overloaded with web requests



## Requirements

- ESP32S3 based board

- VSCode & PlatformIO
  

## Usage

1) Install PlatformIO extention for VSCode - ```https://platformio.org/```

1) Clone the Github repo - ```git clone https://github.com/ravssh/WifiExe/tree/main/```

    ( Note - You have to configure platformio.ini file according to  your board )
    ([Getting Started with PlatformIO and ESP32](https://docs.platformio.org/en/stable/tutorials/espressif32/arduino_debugging_unit_testing.html#setting-up-the-project))

4) Upload the project to your ESP32S3 board
   
5) Make a file inside sd_card named "file_temp_check.txt" ( This file will be used to determine the sd-msc Drive location) 

6) Connect to WifiAP 'WifiExe' with password 'password'

7) Open web browser, go to 192.168.4.1



##  Web Interface Functions

- **View Root Directory** : Allows the user to go through filesystem (May not be stable)
  - To execute custom batch files, open any batch file within the file system, and click on "EXECUTE".
  - Note : IF you use "EXECUTE" , you may need to Clear Traces after execution


- **Execute Payload** : Executes a batch file to gain admin priviledges.
  -After gaining admin priviledges, the device reconnects as a USB Flash Drive (USing on USB-MSC feature of ESP32). The batch script waits for sometime and executes the .exe file.
  - Note : This file has been developed by - @moonD4rk
  - Visit : https://github.com/moonD4rk/HackBrowserData

- **Disconnect & Restart** : A simple function which ends the HID and/or MSC connection between the PC and the controller and reboots the device.
  - Can be used if device is buggy during execution

- **Mount Storage** : Ends HID interface and connects the device as a USB Flash Drive.
  - This function can be used to retrieve data from the SDCarD.
  - Note : This feature is developed by - @atomic14
  - Visit : https://github.com/atomic14/esp32-sdcard-msc

- **Clear Traces** : This feature allows the user to clear any left over files from the custom execution done using "Execute" button from the File Menu.
  - Note : Not needed to be run if Payload is executed


## Tested on

- ESP32S3-Pico by Waveshare
- PlatformIO (Arduino Framework)
- Windows 11 (23H2)
- MicroSD Card - 2GB (SDSC mode, with a SPI frequency of 20 MHz)
Total Runtime for Payload ~ 2 min


## Credits 

- Browser Data Decrypted using [HackBrowserData](https://github.com/moonD4rk/HackBrowserData) developed by [moonD4rk](https://github.com/moonD4rk)
- SD-USB-MSC class is implemented using [esp32-sdcard-msc](https://github.com/atomic14/esp32-sdcard-msc) developed by [atomic14](https://github.com/atomic14/esp32-sdcard-msc)