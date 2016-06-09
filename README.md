#Smart Home IoT Test Bench

##Introduction
**Smart Home IoT Test Bench** is a project light version of MQTT. It is not a kind of MQTT, but made to transfer data easy and visualization like IoT devices.

- **Project name :** Capstone Design - Micro Processor 
- **Development period :** 2016Y 03M 02D ~ 2016Y 06M 10D

##Features
![alt tag](https://github.com/janghe11/Smart_Home_IoT_Test_Bench/blob/master/Pictures/Smart_Home_IoT_Test_Bench_Structures.jpg)

##Installation
- **AVR ATmega8535 :** Build CD_ATmega8535/main.c file and burn into your ATmega8535 or use CD_ATmega8535/Debug/Exe/CD_ATmega8535.hex file
- **Raspberry Pi 2 or 3 :** Execute CD_RaspberryPi2/install.sh in /home/pi/ after expand filesystem and time zone set.(sudo permission and internet connection is needed. Ex) sudo sh install.sh)
- **Android :** Build android projects in CD_Android folder and install apk file in your phone.

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
※ All command characters are unsigned char.

| **Command character**              | **Command name**        | **Available command functions**                                                                                                 |
| ----------------------------------------- | :-------------------------------: | :----------------------------------------------------------------------------------------------------------: |
| 0 ~ f (0x30 ~ 0x39, 0x61 ~ 0x66) | Screen boiler temperature | rs232_get_command(‘0 ~ f’) / set_rs232_data(‘0 ~ f’)                                                               |
| g (0x67)                                               | Loosen gas valve                   | rs232_get_command(‘g’)  / stepmotor_spin(‘g’) / set_rs232_data(‘g’)                                  |
| l (0x6c)                                                 | Lock door                                | rs232_get_command(‘l’) / door_lock_unlock('l') / avr_sound(‘l’) / set_rs232_data(‘l’)      |
| p (0x70)                                               | 4x4 Keypad is pressed         | avr_sound(‘p’) / set_rs232_data(‘p’)                                                                                                 |
| u (0x75)                                               | Unlock door                           | rs232_get_command(‘u’) / door_lock_unlock('u') / avr_sound(‘u’) / set_rs232_data(‘u’) |
| v (0x76)                                                | Fasten gas valve                   | rs232_get_command(‘v’) / stepmotor_spin(‘v’) / set_rs232_data(‘v’)                                      |
| w (0x77)                                              | Process error                         | set_rs232_data(‘w’)                                                                                                                               |


####ATmega8535 main.c function information (17 functions)

####Raspberry Pi 2 structures
![alt tag](https://github.com/janghe11/Smart_Home_IoT_Test_Bench/blob/master/Pictures/Smart%20Home%20Iot%20Test%20Bench%20Structures%20-%20Raspberry%20Pi.jpg)

####Raspberry Pi 2(avr_daemon.c) flowchart
![alt tag](https://github.com/janghe11/Smart_Home_IoT_Test_Bench/blob/master/Pictures/Smart%20Home%20IoT%20Test%20Bench%20Flowchart%20-%20Raspberry%20Pi%20(avr_daemon.c).jpg)

####Raspberry Pi 2(control.php) flowchart
![alt tag](https://github.com/janghe11/Smart_Home_IoT_Test_Bench/blob/master/Pictures/Smart%20Home%20IoT%20Test%20Bench%20Flowchart%20-%20Raspberry%20Pi%20(control.php).jpg)

####control.php data protocol information

