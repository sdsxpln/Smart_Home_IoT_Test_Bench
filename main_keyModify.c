/*
* Switch / 4x4 Keypad / UART
*  Created on: 2016. 3. 4.
*      Author: Taehee Jang
*
* LCD / Rotary Switch
*  Created on: 2016. 3. 16.
*      Author: Jongwoo Jun
* 
* Compiler : IAR
*/

#include "io8535.h"
// Switch input pin
#define X0 PINB_Bit4
#define X1 PINB_Bit5
#define X2 PINB_Bit6
#define X3 PINB_Bit7
// RS232 UART setting pin
#define    UDRE     USR_Bit5
#define    TXC      USR_Bit6
#define    RXC      USR_Bit7

#define SCAN_SPEED      290 // Doorlock Keyscan speed control
#define ENABLE	        1
#define DISABLE			0

// Key Matrix values
__flash unsigned char KCODE[16] = {0x00, 0x04, 0x08, 0x0c, 0x01, 0x05, 0x09, 0x0d, 0x02, 0x06, 0x0a, 0x0e, 0x03, 0x07, 0x0b, 0x0f};
// Step motor angle (each 1.8 degree)
__flash unsigned char SPINANGLE[8] = {0x10, 0x30, 0x20, 0x60, 0x40, 0xc0, 0x80, 0x90};

// LCD �ʱ� ��� ȭ��
unsigned char msg1[]="**** Hello **** ";
unsigned char msg2[]="PUSH Button Plz ";

// 1.����� OPEN LCD ��� ȭ��
__flash unsigned char Door_lock1[] = "1-Door Lock Fun?";
__flash unsigned char Door_lock2[] = "2-PASSWORD :    ";

// 3.���Ϸ� LCD ��� ȭ��
__flash unsigned char Boiler1[] = "1-Boiler Fun?   ";
__flash unsigned char Boiler2[] = "2-Temperature:  ";


#include "LCD4.h"
// Rotary ��ƮB�� �Է��� ��巹�� �޴� ������ �����ϴ� ����
unsigned char r;
unsigned char LCD[16] = { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f };

unsigned char data = 0; // RS232 UART UDR data input / output

unsigned char modify_password = 0; // password modify mode
// 4�ڸ� Ű�� ������� �����ϱ� ���� ����
unsigned char insert_array = 0; // Var for input password arrays
unsigned char KEY; // Save 1 key Row
unsigned char FLAG; // Check specific key code
unsigned char KEY2;
unsigned int delay_count = SCAN_SPEED; // Delay count for key scan (280 is ideal)
unsigned char check_password[] = {0x0f, 0x0f, 0x0f, 0x0f};
unsigned char set_password[] = {0x01, 0x02, 0x03, 0x04}; // Password doorlock keeps
int passwordWrong; // Check Password (+1 if password is wrong)
// ��й�ȣ * ǥ�� ���� ����
int number = 0;

unsigned int spinCount, spinStep; // Step Motor counts

int delay(unsigned int i) {
  while (i--);
  return 0;
}

/* 
* ====================RS232 control functions====================
* init_rs232 -> Initialize UART
* set_rs232_data -> UART transmission (only send status informations)
* get_rs232_data -> UART receive (get data and do instructions)
* ====================RS232 data variables====================
* unsigned char 'u' : Doorlock unlock 
* unsigned char 'l' : Doorlock lock
* unsigned char 't' : Boiler temp increase
* unsigned char 'b' : Boiler temp decrease
* unsigned char 'g' : Loosen gas valve
* unsigned char 'v' : Fasten gas valve
*/
int init_rs232(void)
{
  UBRR = 23; //UART Baud Rate Register  9600bps in 3.6854MHz
  UCR = 0x18; //UART Control Register -> RXEN, TXEN Enable
  
  return 0;
}

unsigned char set_rs232_data(unsigned char data)
{
  // Wait until data is received
  // Transmit data
  if(UDRE)
    UDR = data;
  
  return 0; 
}

unsigned char get_rs232_data(void)
{
  //  When data recieve complete
  // Get data from UDR
  if(RXC)
    return UDR;
  else
    return 0;
}

int rs232_get_command(unsigned char data)
{
  switch(data) {
  case 'u': // Doorlock unlock
    set_rs232_data('u');
    break;
  case 'l': // Doorlock lock
    set_rs232_data('l');
    break;
  case 't': // Boiler temp increase
    r += 1;
    set_rs232_data('t');
    break;
  case 'b': // Boiler temp increase
    r -= 1;
    set_rs232_data('b');
    break;
  case 'g': // Loosen gas valve
    spinRight();
    set_rs232_data('g');
    break;
  case 'v': // Fasten gas valve
    spinLeft();
    set_rs232_data('v');
    break;
  default:
    asm("nop");
    
  }
  
  return 0;
}

// ��й�ȣ **** lcd ��� �Լ�
int encryption(void) {
  if(number==0){
    COMMAND(0xcc);
    CHAR_O(0x2A);
    delay(65000);
  }
  else if(number==1) {
    COMMAND(0xcd);
    CHAR_O(0x2A);
    delay(65000);
  }
  else if(number==2) {
    COMMAND(0xce);
    CHAR_O(0x2A);
    delay(65000);
  }
  else { // (number==3)
    COMMAND(0xcf);
    CHAR_O(0x2A);
    delay(65000);
  }
  return 0;
}

void boiler(void) {
  unsigned char i;
  // ���÷��� Ŭ����
  COMMAND(0x01);
  // 1���� ������ ���
  COMMAND(0x02);
  for (i = 0; i < 16; i++) {
    CHAR_O(Boiler1[i]);
  }
  // 2���� ������ ���
  COMMAND(0xc0);
  for (i = 0; i < 16; i++) {
    CHAR_O(Boiler2[i]);
  }
  // �µ� ���� �ڸ�,���� �ڸ� �⺻�� ���
  COMMAND(0xce);
  CHAR_O(0x31);
  //CHAR_O(temperature1);
  COMMAND(0xcf);
  CHAR_O(0x38);
  //CHAR_O(temperature2);
}

// Rotate Step Motor 180 left
int spinLeft(void) {
  spinCount = 200;
  spinStep = 7;
  do {
    PORTD = SPINANGLE[spinStep];
    spinStep--;
    spinStep &= 0x07;
    delay(60000);
  }while(spinCount--);
  
  return 0;
}

// Rotate Step Motor 180 right
int spinRight(void) {
  spinCount = 200;
  spinStep = 0;
  do {
    PORTD = SPINANGLE[spinStep];
    spinStep++;
    spinStep &= 0x07;
    delay(60000);
  }while(spinCount--);
  
  return 0;
}

// Catch 4x4 Hex Keypad Input
void SCAN(void)
{
  unsigned char i, temp, key1;
  KEY = key1 = 0;
  FLAG = 1;
  PORTA = 0xfe;
  delay(6000);
  
  temp = PINA;
  temp = (temp >> 4) | 0xf0;
  for (i=0; i<4; i++) {
    if (!(temp & 0x01)) {
      key1 = KEY; 
      FLAG = 0;
    }
    temp >>= 1;
    KEY++;
  }
  KEY = key1 & 0x0f;
  // Key�� ���۵����� �ι� ���� ���� ���� ���� delay
  //encryption();
  // ��й�ȣ �ڸ� ���� �ϱ� ���� ����
  //number++;
}

void SCAN2(void)
{
  unsigned char i, temp, key1;
  KEY = key1 = 4;
  FLAG = 1;
  PORTA = 0xfd;
  delay(6000);
  
  temp = PINA;
  temp = (temp >> 4) | 0xf0;
  for (i=0; i<4; i++) {
    if (!(temp & 0x01)) {
      key1 = KEY; FLAG = 0;
    }
    temp >>= 1;
    KEY++;
  }
  KEY = key1 & 0x0f;
  //encryption();
  //number++;
}

unsigned char SCAN3(void)
{
  unsigned char i, temp, key1;
  KEY = key1 = 8;
  FLAG = 1;
  PORTA = 0xfb;
  delay(6000);
  
  temp = PINA;
  temp = (temp >> 4) | 0xf0;
  for (i=0; i<4; i++) {
    if (!(temp & 0x01)) {
      key1 = KEY; FLAG = 0;
    }
    temp >>= 1;
    KEY++;
  }
  KEY = key1 & 0x0f;
  //encryption();
  //number++;
  
  return KCODE[KEY];
}

void SCAN4(void)
{
  unsigned char i, temp, key1;
  KEY = key1 = 12;
  FLAG = 1;
  PORTA = 0xf7;
  delay(6000);
  
  temp = PINA;
  temp = (temp >> 4) | 0xf0;
  for (i=0; i<4; i++) {
    if (!(temp & 0x01)) {
      key1 = KEY; FLAG = 0;
    }
    temp >>= 1;
    KEY++;
  }
  KEY = key1 & 0x0f;
  //encryption();
  //number++;
}

int password_checker(void)
{
  passwordWrong = 0;
  insert_array = 0;
  // Check insert_array 4 times
  for (unsigned char pwd_check_array = 0; pwd_check_array < 4; pwd_check_array++) {
    if(check_password[pwd_check_array] == set_password[pwd_check_array]) {
      passwordWrong = 1;
      PORTD = 0xf7;
      delay(60000);
      PORTD = 0xf7;
      delay(60000);
      PORTD = 0xff;
      delay(60000);
      PORTD = 0xff;
      delay(60000);
    } else {
      passwordWrong = 0;
      break;
    }
  }
  
  // Insert dummy values after checking
  for (pwd_check_array = 0; pwd_check_array < 4; pwd_check_array++)  {
    check_password[pwd_check_array] = 'f';
  }
  
  // Password correct = 1
  if(!(passwordWrong)) {
    set_rs232_data('e');
    return 0;  
  } else {
    PORTD = 0xf3;
    delay(60000);
    PORTD = 0xf3;
    delay(60000);
    PORTD = 0xfb;
    delay(60000);
    PORTD = 0xfb;
    delay(60000);
    set_rs232_data('u');
    spinRight();  
    return 1;
  }
}

// Initialize and check devices
int init_devices(void)
{
  unsigned int delay_time = 60000;
  // Check Debug LED
  PORTD = 0xf3;
  while(delay_time--);
  PORTD = PORTD << 1;
  delay_time = 60000;
  while(delay_time--);
  PORTD = PORTD << 1;
  
  // Check step motor left and right
  spinCount = 10;
  spinStep = 7;
  delay_time = 100;
  do {
    PORTD = SPINANGLE[spinStep];
    spinStep--;
    spinStep &= 0x07;
    while(delay_time--);
  }while(spinCount--);
  
  spinCount = 10;
  spinStep = 0;
  delay_time = 100;
  do {
    PORTD = SPINANGLE[spinStep];
    spinStep++;
    spinStep &= 0x07;
    while(delay_time--);
  }while(spinCount--);
  
  return 0;
}

int main(void) {
  /*
  * DDRA : Keypad C0 ~ C3 (Output PA0 ~ PA3), L0 ~ L3 (Input PA4 ~ PA7)
  * DDRB : Rotary Switch (Input PB0 ~ PB3), Switch (Input PB4 ~ PB7) 
  * DDRC : Character LCD (Output (D4~D7) PC0 ~ PC3  (RW,RW,E) PC5 ~ PC7 ), Speaker (Output PC4) 
  * DDRD : Step motor (Output PD7 ~ PD4), Debug LED (Output PD3, PD2), UART (TXD Output PD1 RXD Input PD0)
  */
  DDRA = 0x0f;
  DDRB = 0x00;
  DDRC = 0xff;
  DDRD = 0xfe;
  // Initialize and check LED / step motor
  init_devices();
  // Initialize RS232 Communication
  init_rs232();
  //DISPLAY function for MDA_Multi (LCD4.H)
  L_INIT();
  DISPLAY();
  
  do {
    // Get data from UART and command informaions  
    data = get_rs232_data();
    rs232_get_command(data);
    r = PINB; // r�̶�� ����� ��ƮB�� �Է��� ��巹���� �ִ´�.
    //1. Doorlock & Step Motor Open Process
    if (X0) {
      unsigned char k;
      // X0 Enable Debug LED Off
      PORTD = 0xff;
      // LCD Display -> Door lock and Password
      // LCD Ŭ����
      COMMAND(0x01);
      // 1���� ������ ���
      COMMAND(0x02);  // Ŀ���� Ȩ���� ��
      for (k = 0; k < 16; k++) {
        CHAR_O(Door_lock1[k]);   // �����͸� LCD�� ������ ���
      }
      // 2���� ������ ���
      COMMAND(0xc0);  // Ŀ���� ���� 2�� ��
      for (k = 0; k < 16; k++) {
        CHAR_O(Door_lock2[k]);   // �����͸� LCD�� ������ ���
      }
      /*
      * Doorlock routine
      * modify_password ENABLE -> password modify mode
      */
      if(delay_count > 0) 
      {
        delay_count--;
      }
      
      if(delay_count == 0) {
        if(SCAN3() == 0x0a) {
          password_checker();
        } else if(SCAN3 () == 0x0b) {
          modify_password = ENABLE;
          insert_array = 0;
        }
        
        if(modify_password) {
          if(insert_array == 4) {
            modify_password = DISABLE;
            PORTD = 0xf3;
			delay(60000);
			PORTD = 0xf3;
			delay(60000);
			PORTD = 0xfb;
			delay(60000);
			PORTD = 0xfb;
			delay(60000);
          }
          
          SCAN();
          if (!(FLAG == 1)) {
            KEY2 = KCODE[KEY];
            // Insert password checking arrays.
            set_password[insert_array] = KEY2;
            PORTD = 0xf7;
            insert_array++;
            delay_count = SCAN_SPEED;
          }
          
          SCAN2();
          if (!(FLAG == 1)) {
            KEY2 = KCODE[KEY];
            set_password[insert_array] = KEY2;
            PORTD = 0xfb;
            insert_array++;
            delay_count = SCAN_SPEED;
          }
          
          SCAN3();
          if (!(FLAG == 1)) 
          {
            KEY2 = KCODE[KEY];
            set_password[insert_array] = KEY2;
            PORTD = 0xf3;
            insert_array++;
            delay_count = SCAN_SPEED;
          }
          
          SCAN4();
          if (!(FLAG == 1)) 
          {
            KEY2 = KCODE[KEY];
            set_password[insert_array] = KEY2;
            PORTD = 0xff;
            insert_array++;
            delay_count = SCAN_SPEED;
          }	
        } else {
          SCAN();
          if (!(FLAG == 1)) {
            KEY2 = KCODE[KEY];
            // Insert password checking arrays.
            check_password[insert_array] = KEY2;
            PORTD = 0xf7;
            insert_array++;
            delay_count = SCAN_SPEED;
          }
          
          SCAN2();
          if (!(FLAG == 1)) {
            KEY2 = KCODE[KEY];
            check_password[insert_array] = KEY2;
            PORTD = 0xfb;
            insert_array++;
            delay_count = SCAN_SPEED;
          }
          
          SCAN3();
          if (!(FLAG == 1)) 
          {
            KEY2 = KCODE[KEY];
            check_password[insert_array] = KEY2;
            PORTD = 0xf3;
            insert_array++;
            delay_count = SCAN_SPEED;
          }
          
          SCAN4();
          if (!(FLAG == 1)) 
          {
            KEY2 = KCODE[KEY];
            check_password[insert_array] = KEY2;
            PORTD = 0xff;
            insert_array++;
            delay_count = SCAN_SPEED;
          }	
        }
      }
    } 
    // 2. Door lock & Step Motor Close Process
    else if (X1) {
      spinLeft();
      // 3.Rotary Switch Boiler Process
    } 
    else if (X2) {
      if(LCD[r&0x0f]==0){ // 18�� = ROTARY B(0)
        COMMAND(0x01);
        boiler();
        delay(65000);
        // while(LCD[r&0x0f]==0);
        // while(PINB==0); or if �� �ۿ� �ۼ�.
      }
      else if(LCD[r&0x0f]==1){ // 19�� = 1
        COMMAND(0xce);
        CHAR_O(0x31);
        COMMAND(0xcf);
        CHAR_O(0x39);
        delay(65000);
      }
      else if(LCD[r&0x0f]==2){ // 20�� = 2
        COMMAND(0xce);
        CHAR_O(0x32);
        COMMAND(0xcf);
        CHAR_O(0x30);
        delay(65000);
      }
      else if(LCD[r&0x0f]==3){ // 21�� = 3
        COMMAND(0xce);
        CHAR_O(0x32);
        COMMAND(0xcf);
        CHAR_O(0x31);
        delay(65000);
      }
      else if(LCD[r&0x0f]==4){ // 22�� = 4
        COMMAND(0xce);
        CHAR_O(0x32);
        COMMAND(0xcf);
        CHAR_O(0x32);
        delay(65000);
      }
      else if(LCD[r&0x0f]==5){ // 23�� = 5
        COMMAND(0xce);
        CHAR_O(0x32);
        COMMAND(0xcf);
        CHAR_O(0x33);
        delay(65000);
      }
      else if(LCD[r&0x0f]==6){ // 24�� = 6
        COMMAND(0xce);
        CHAR_O(0x32);
        COMMAND(0xcf);
        CHAR_O(0x34);
        delay(65000);
      }
      else if(LCD[r&0x0f]==7){ // 25�� = 7
        COMMAND(0xce);
        CHAR_O(0x32);
        COMMAND(0xcf);
        CHAR_O(0x35);
        delay(65000);
      }
      else if(LCD[r&0x0f]==8){ // 26�� = 8
        COMMAND(0xce);
        CHAR_O(0x32);
        COMMAND(0xcf);
        CHAR_O(0x36);
        delay(65000);
      }
      else if(LCD[r&0x0f]==9){ // 27�� = 9
        COMMAND(0xce);
        CHAR_O(0x32);
        COMMAND(0xcf);
        CHAR_O(0x37);
        delay(65000);
      }
      else if(LCD[r&0x0f]==10){ // 28�� = A
        COMMAND(0xce);
        CHAR_O(0x32);
        COMMAND(0xcf);
        CHAR_O(0x38);
        delay(65000);
      }
      else if(LCD[r&0x0f]==11){ // 29�� = B
        COMMAND(0xce);
        CHAR_O(0x32);
        COMMAND(0xcf);
        CHAR_O(0x39);
        delay(65000);
      }
      else if(LCD[r&0x0f]==12){ // 30�� = C
        COMMAND(0xce);
        CHAR_O(0x33);
        COMMAND(0xcf);
        CHAR_O(0x30);
        delay(65000);
      }
      else if(LCD[r&0x0f]==13){ // 31�� = D
        COMMAND(0xce);
        CHAR_O(0x33);
        COMMAND(0xcf);
        CHAR_O(0x31);
        delay(65000);
      }
      else if(LCD[r&0x0f]==14){ // 32�� = E
        COMMAND(0xce);
        CHAR_O(0x33);
        COMMAND(0xcf);
        CHAR_O(0x32);
        delay(65000);
      }
      else if(LCD[r&0x0f]==15){ // 33�� = F
        COMMAND(0xce);
        CHAR_O(0x33);
        COMMAND(0xcf);
        CHAR_O(0x33);
        delay(65000);
      }
    }
    // 4.Heating Gas Valve On/Off Process 
    else if (X3) {
      
    }
    else {
      COMMAND(0x01);
      DISPLAY();
      delay(65000);
    }
  } while (1);
}

