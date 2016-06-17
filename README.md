#Smart Home IoT Test Bench

##Introduction
**Smart Home IoT Test Bench** is a project light version of MQTT. It is not a kind of MQTT, but made to transfer data easy and visualization like IoT devices.

- **Project name :** Capstone Design - Micro Processor 
- **Development period :** 2016Y 03M 02D ~ 2016Y 06M 10D
- **Development Part :** Jongwoo Jun → LCD, Boiler in AVR / Android App | Taehee Jang → Door lock, Gas valve, sound in AVR / Raspberry Pi

##Features
![alt tag](https://github.com/janghe11/Smart_Home_IoT_Test_Bench/blob/master/Pictures/Smart_Home_IoT_Test_Bench_Structures.jpg)
[Video](https://www.youtube.com/watch?v=-Crt2v5ujaE "See system overall")

###AVR ATmega8535
[Video](http:// "AVR Door lock - Overall")
[Video](http:// "AVR Door lock - Type password")
[Video](http:// "AVR Door lock - Unlock")
[Video](http:// "AVR Door lock - Password fail")
[Video](http:// "AVR Door lock - Modify password")
[Video](http:// "AVR Boiler - Overall")
[Video](http:// "AVR Boiler - Temerature control")
[Video](http:// "AVR Gas valve - Valve(Step motor) on/off")
###Raspberry Pi2 or 3

###Android(Since 4.1 Jelly Bean)
[Video](http:// "Android to Raspberry Pi and cloud instance")

##Installation
**1.** Folk this project to your repository.
**2. AVR ATmega8535 :** Insert CD_ATmega8535.hex file (Location : CD_ATmega8535/Debug/Exe/CD_ATmega8535.hex) to your AVR device.
**3. Raspberry Pi 2 or 3 :** 
**3-1.** Change #define MARIA_CLOUD "Your domain here" to your cloud domain in **avr_daemon.c** 18 line and commit.
**3-2.** Change WiFi SSID and Password as your wish in **install.sh** 97 ~ 98 line and commit.
**3-3.** Change git repository address to yours in **install.sh** 61 line and commit.
**3-4.** Boot raspbian OS and do "sudo raspi-config". Execute "1. Expand file system" and "4. International Option" → Select time zone, Change Localization. Reboot.
**3-5.** Clone this project in your repository or Copy install.sh by FTP client. And connect USB to RS232-C cable. Execute **"sudo sh install.sh"** [Video](http:// "Raspberry Pi install.sh installation video")
**4. Android :** 
**4-1.** Change Raspberry Pi domain or local IP in **MainActivity.java** 231line .
**4-2.** Build android projects in CD_Android folder and install apk file in your phone.

##Details
###Hardware platform and devices
- AVR → MIDAS Engineering MDA-Multi MICOM (ATmega8535, http://www.midaseng.com/)
- Raspberry Pi 2 (Compatible to RPi3)
- Android (Since 4.1 Jelly Bean is available)
- Wireless router(Tested in ipTime A2004R)
- USB to RS-232C cable

###Development environment and tools
- **Compiler  :** *AVR →* IAR Embedded Workbench Atmel AVR C / *Raspberry Pi →* arm-gcc C Compiler, PHP 5.6.11
- **Operating system :** *Raspberry Pi →* Raspbian Jessie 03-18-2016 / *Android →* Android 4.1 Jellybean or later
- **Development environment :** *AVR development →* Windows XP or 7 32bit / *Raspberry Pi →* Ubuntu 15.10 64bit or later / *Android →* Android Studio 1.5 or later
- **Applications :** Apache2, mariaDB, PHPMyAdmin, etc.

###AVR ATmega8535
####Brief of Smart Home IoT Test Bench structures
![alt tag](https://github.com/janghe11/Smart_Home_IoT_Test_Bench/blob/master/Pictures/Smart%20Home%20IoT%20Test%20Bench%20Structures.jpg)

####Details of Smart Home IoT Test Bench structures
![alt tag](https://github.com/janghe11/Smart_Home_IoT_Test_Bench/blob/master/Pictures/Smart%20Home%20IoT%20Test%20Bench%20Structures(Details).jpg)

####AVR(ATmega8535 main.c) flowchart
![alt tag](https://github.com/janghe11/Smart_Home_IoT_Test_Bench/blob/master/Pictures/Smart%20Home%20IoT%20Test%20Bench%20Flowchart%20-%20ATmega8535%20(main.c).jpg)

####DDR pin map
![alt tag](https://github.com/janghe11/Smart_Home_IoT_Test_Bench/blob/master/Pictures/AVR-DDRA.png)
![alt tag](https://github.com/janghe11/Smart_Home_IoT_Test_Bench/blob/master/Pictures/AVR-DDRB.png)
![alt tag](https://github.com/janghe11/Smart_Home_IoT_Test_Bench/blob/master/Pictures/AVR-DDRC.png)
![alt tag](https://github.com/janghe11/Smart_Home_IoT_Test_Bench/blob/master/Pictures/AVR-DDRD.png)

####Data transfer protocol information(RPi ↔ AVR)
※ All command characters(input parameters) are unsigned char.

| **Command character**              | **Command name**        | **Available command functions**                                                                                                 |
| ----------------------------------------- | :-------------------------------: | :----------------------------------------------------------------------------------------------------------: |
| 0 ~ f (0x30 ~ 0x39, 0x61 ~ 0x66) | Screen boiler temperature | rs232_get_command(‘0 ~ f’) / set_rs232_data(‘0 ~ f’)                                                               |
| g (0x67)                                               | Loosen gas valve                   | rs232_get_command(‘g’)  / stepmotor_spin(‘g’) / set_rs232_data(‘g’)                                  |
| l (0x6c)                                                 | Lock door                                | rs232_get_command(‘l’) / door_lock_unlock('l') / avr_sound(‘l’) / set_rs232_data(‘l’)      |
| p (0x70)                                               | 4x4 Keypad is pressed         | avr_sound(‘p’) / set_rs232_data(‘p’)                                                                                                 |
| u (0x75)                                               | Unlock door                           | rs232_get_command(‘u’) / door_lock_unlock('u') / avr_sound(‘u’) / set_rs232_data(‘u’) |
| v (0x76)                                                | Fasten gas valve                   | rs232_get_command(‘v’) / stepmotor_spin(‘v’) / set_rs232_data(‘v’)                                      |
| w (0x77)                                              | Process error                         | set_rs232_data(‘w’)                                                                                                                               |


####ATmega8535 main.c function information (18 functions)

| **Function name**  | **Parameters** | **Return datatype** | **Return value** | **Description**                                                                                    |
| -------------------------- | --------------------- | --------------------------- | ----------------------- | ---------------------------------------------------------------------------- |
| delay                               |                                  | int                                    | 0                                | Delay 65000 counts.                                                                             |
| doorlock                        |                                  | void                                  |                                    | Show password input screen on LCD.                                             |
| door_lock_unlock        | u, l                            | int                                    | 0                                 | Show password match or stay out mode screen on LCD.          |
| stepmotor_spin            | v, g                           | int                                    | 0                                 | Rotate step motor left(v) or right(g).                                              |
| init_rs232                      |                                  | int                                     | 0                                 | Initialize RS232 registers.                                                                   |
| set_rs232_data             | unsigned char      | unsigned char               | 0                                 | Send 1byte unsigned char data from AVR via RS-232C cable. |
| get_rs232_data            |                                  | unsigned char               | 0                                 |
| encryption                      |
| boiler                               |
| SCAN ~ SCAN4            |                                   | void                                  |                                     |
| init_devices                    |                                  | int                                     | 0                                 |
| avr_sound                      | w, u, l, p                 | int                                     | 0                                  |
| rs232_get_command | unsigned char       |int                                      | 0
| encryption                      | void                         |
| password_checker       |                                  | int                                      | 0, 1                             |

####ATmega8535 main.c interrupts information

| **Interrupt name**    | **Description** |
| ---------------------------- | --------------------- |
| TIMER1_COMPA_vect |                                 |

####Raspberry Pi 2 structures
![alt tag](https://github.com/janghe11/Smart_Home_IoT_Test_Bench/blob/master/Pictures/Smart%20Home%20Iot%20Test%20Bench%20Structures%20-%20Raspberry%20Pi.jpg)

####Raspberry Pi 2(avr_daemon.c) flowchart
![alt tag](https://github.com/janghe11/Smart_Home_IoT_Test_Bench/blob/master/Pictures/Smart%20Home%20IoT%20Test%20Bench%20Flowchart%20-%20Raspberry%20Pi%20(avr_daemon.c).jpg)

####Raspberry Pi 2(control.php) flowchart
![alt tag](https://github.com/janghe11/Smart_Home_IoT_Test_Bench/blob/master/Pictures/Smart%20Home%20IoT%20Test%20Bench%20Flowchart%20-%20Raspberry%20Pi%20(control.php).jpg)

####control.php data protocol information

| **Command variables** | **Command character** | **Command name**                        | **Command function**                                   | **_POST variables** |
| -------------------------------- | -------------------------------- | -------------------------------------------- | ------------------------------------------------------ | --------------------------- |
| $andClientId                        | KEY_VALUE                           | Match ID between android and RPi | strcmp($andClientId, $rpiClientId);                | andClientId                    |
| $andModeGet                     | 1, 2                                          | Set command to RPi whether get database or set command to AVR | 1 → andGetMaria(~); / 2 → rpiAndCommCheck(~); | andModeSet |
| $andCommGet                   | 0 ~ f, u, l, g, v, o                    | Request specific function of AVR      | avrSetComm($andCommGet, $avrDevId); | andCommSet                |
