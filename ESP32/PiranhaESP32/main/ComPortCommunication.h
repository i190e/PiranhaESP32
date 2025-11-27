#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"

#include "esp_hidd_prf_api.h"
#include "esp_bt_defs.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "driver/gpio.h"
#include "hid_dev.h"

//Mouse buttons for switch case
#define LEFT              0
#define RIGHT             1
#define DOUBLECLICK       2
#define LEFTDOWN          3
#define RIGHTDOWN         4
#define LEFTUP            5
#define RIGHTUP           6
#define MIDDLE            7
#define MIDDLEDOWN        8
#define MIDDLEUP          9
#define LEFTRIGHTDOWN     10
#define LEFTRIGHTUP       11

//define device
#define MOUSE              0
#define KEYBOARD           1
//define commands MOUSE
#define MOVE               2
#define CLICK              3

typedef struct 
{
	int device;
	int command;
	int x;
	int y;
	int mouseButton;
	
} commandFromSerialPort;

// перевод координат мыши в int, формат (+-)0000, первый char +-, следующие 4 число
int charArrayToNumber(const char arr[5]) 
         {   
         if(arr[0]=='+')
           {
			return (arr[1] - '0') * 1000 +
           (arr[2] - '0') * 100 +
           (arr[3] - '0') * 10 +
           (arr[4] - '0') * 1;
           }
         else 
         if(arr[0]=='-')
           {
		 return(arr[1] - '0') * (-1000) +
           (arr[2] - '0') * (-100) +
           (arr[3] - '0') * (-10) +
           (arr[4] - '0') * (-1);
           }         
           else return 0;          
}

 commandFromSerialPort decodingCommandCharArray(char commadCharArray[128]) 
{
// структура команд
// (mouse/keyboard) command
commandFromSerialPort resultDecoding;
resultDecoding.device      = 0;
resultDecoding.command     = 0;
resultDecoding.x           = 0;
resultDecoding.y           = 0;
resultDecoding.mouseButton = LEFTUP;
char tempX[5];
char tempY[5];

//пример команды:  mouse:move:+0100:+0150
if(commadCharArray[0]=='m' && commadCharArray[1]=='o' && commadCharArray[2]=='u' && commadCharArray[3]=='s' && commadCharArray[4]=='e' && commadCharArray[5]==':' )
{	
   resultDecoding.device= MOUSE;
	if(commadCharArray[6]=='m' && commadCharArray[7]=='o' && commadCharArray[8]=='v' && commadCharArray[9]=='e' && commadCharArray[10]==':' )	
		{			
   resultDecoding.command=MOVE ;
			for(int i=0;i<5;i++)
			{
				tempX[i]=commadCharArray[i+11];
				tempY[i]=commadCharArray[i+17];
			}
			resultDecoding.x = charArrayToNumber(tempX);
			resultDecoding.y = charArrayToNumber(tempY);
		}
	else if(commadCharArray[6]=='c' && commadCharArray[7]=='l' && commadCharArray[8]=='i' && commadCharArray[9]=='c' && commadCharArray[10]=='k' && commadCharArray[10]==':' )
		{
	   resultDecoding.command=CLICK ;
	          if(commadCharArray[11]=='L' && commadCharArray[12]=='E' && commadCharArray[13]=='F' && commadCharArray[14]=='T')
	  {resultDecoding.mouseButton=LEFT;}
	  else 	  if(commadCharArray[11]=='R' && commadCharArray[12]=='I' && commadCharArray[13]=='G' && commadCharArray[14]=='H' && commadCharArray[15]=='T' )
	  {resultDecoding.mouseButton=RIGHT;}
	  else 	  if(commadCharArray[11]=='D' && commadCharArray[12]=='O' && commadCharArray[13]=='U' && commadCharArray[14]=='B' && commadCharArray[15]=='L' && commadCharArray[16]=='E' && commadCharArray[17]=='C' && commadCharArray[18]=='L' && commadCharArray[19]=='I'   && commadCharArray[20]=='C' && commadCharArray[21]=='K')
	  {resultDecoding.mouseButton=DOUBLECLICK;}
	  else 	  if(commadCharArray[11]=='L' && commadCharArray[12]=='E' && commadCharArray[13]=='F' && commadCharArray[14]=='T' && commadCharArray[15]=='D' && commadCharArray[16]=='O' && commadCharArray[17]=='W' && commadCharArray[18]=='N')
	  {resultDecoding.mouseButton=LEFTDOWN;}
	  else 	  if(commadCharArray[11]=='R' && commadCharArray[12]=='I' && commadCharArray[13]=='G' && commadCharArray[14]=='H' && commadCharArray[15]=='T' && commadCharArray[16]=='D' && commadCharArray[17]=='O' && commadCharArray[18]=='W' && commadCharArray[19]=='N')
	  {resultDecoding.mouseButton=RIGHTDOWN;}
	  else 	  if(commadCharArray[11]=='L' && commadCharArray[12]=='E' && commadCharArray[13]=='F' && commadCharArray[14]=='T' && commadCharArray[15]=='U' && commadCharArray[16]=='P')
	  {resultDecoding.mouseButton=LEFTUP;}
	  else 	  if(commadCharArray[11]=='R' && commadCharArray[12]=='I' && commadCharArray[13]=='G' && commadCharArray[14]=='H' && commadCharArray[15]=='T' && commadCharArray[16]=='U' && commadCharArray[17]=='P')
	  {resultDecoding.mouseButton=RIGHTUP;}
	  else 	  if(commadCharArray[11]=='M' && commadCharArray[12]=='I' && commadCharArray[13]=='D' && commadCharArray[14]=='D' && commadCharArray[15]=='L' && commadCharArray[16]=='E' )
	  {resultDecoding.mouseButton=MIDDLE;}
	  else 	  if(commadCharArray[11]=='M' && commadCharArray[12]=='I' && commadCharArray[13]=='D' && commadCharArray[14]=='D' && commadCharArray[15]=='L' && commadCharArray[16]=='E' && commadCharArray[17]=='D' && commadCharArray[18]=='O' && commadCharArray[19]=='W' && commadCharArray[20]=='N' )
	  {resultDecoding.mouseButton=MIDDLEDOWN;}
	  else 	  if(commadCharArray[11]=='M' && commadCharArray[12]=='I' && commadCharArray[13]=='D' && commadCharArray[14]=='D' && commadCharArray[15]=='L' && commadCharArray[16]=='E' && commadCharArray[17]=='U' && commadCharArray[18]=='P' )
	  {resultDecoding.mouseButton=MIDDLEUP;}
	  else 	  if(commadCharArray[11]=='L' && commadCharArray[12]=='E' && commadCharArray[13]=='F' && commadCharArray[14]=='T' && commadCharArray[15]=='R' && commadCharArray[16]=='I' && commadCharArray[17]=='G' && commadCharArray[18]=='H' && commadCharArray[19]=='T' && commadCharArray[20]=='D'  && commadCharArray[20]=='O'  && commadCharArray[20]=='W'  && commadCharArray[20]=='N' )
	  {resultDecoding.mouseButton=LEFTRIGHTDOWN;}
	  else 	  if(commadCharArray[11]=='L' && commadCharArray[12]=='E' && commadCharArray[13]=='F' && commadCharArray[14]=='T' && commadCharArray[15]=='R' && commadCharArray[16]=='I' && commadCharArray[17]=='G' && commadCharArray[18]=='H' && commadCharArray[18]=='T' && commadCharArray[20]=='U'  && commadCharArray[20]=='P'  )
	  {resultDecoding.mouseButton=LEFTRIGHTUP;}			
		}		
}
//пример команды:  keyboard:
else if(commadCharArray[0]=='k' && commadCharArray[1]=='e' && commadCharArray[2]=='y' && commadCharArray[3]=='b' && commadCharArray[4]=='o' && commadCharArray[5]=='a' 
&& commadCharArray[6]=='r' && commadCharArray[7]=='d' && commadCharArray[8]==':')
		{
	   resultDecoding.device= KEYBOARD;
		}	
return resultDecoding;  
}
