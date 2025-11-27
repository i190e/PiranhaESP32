/*
 * SPDX-FileCopyrightText: 2021-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

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
#include "init_all_main.h"
#include "MouseKeyboardCommands.h"

//#include <esp_random.h>

//#include "esp_system.h"
//#include "esp_heap_caps.h"
//#include <inttypes.h> 


//**********************  start hid_demo_task ************************************************************************************************************/
void hid_demo_task(void *pvParameters)
{	
uint16_t conn_id = (uint16_t)pvParameters;
//uint8_t buffer[6]={HID_KEY_RESERVED};
uint8_t buffer[6]={0};
char bufferChars[128]={' '};
char commandChars[128]={' '};
char keyboardCommand[128]={' '}; //structure:keyboard:keypress[]
char mouseCommand[128]={' '};  // structure: mouse:move:+-x[4]:+-y[4] // mouse click [command]//example//mouse:move:+0100:+0150//mouse:click:DOUBLECLICK//
char mouseX[5]={'0'}; //x координаты относительно положения курсора
int mouseXoffset;
char mouseY[5]={'0'}; //y координаты относительно положения курсора 
int mouseYoffset;
int mouseButtonCommand[1]={'0'};
commandFromSerialPort receivingCommand;
//fgets(bufferChars, sizeof(bufferChars), stdin);  // ввод с USB порта
//printf("%s",bufferChars); // вывод в USB (COM) порт
printf("Serial Port check communications");   
//dump_sram_stats();
 
while(1) {
//*******************************************************************************************************************************************************/
//MouseButtonsPatterns 0x00 — все кнопки отпущены; 0x01 — нажата левая кнопка; 0x02 — нажата правая кнопка; 0x04 — нажата средняя кнопка; 0x03 — нажаты левая и правая кнопки.
//esp_hidd_send_mouse_value( hid_conn_id, MouseButtonsPatterns, x, y); 

//esp_hidd_send_keyboard_value(hid_conn_id, special_key_mask,keyboard_cmd, 1);
//key_mask_t special_key_mask битовая маска модификаторов (Ctrl, Shift, Alt и т. п.).
//uint8_t *keyboard_cmd массив кодов клавиш (до 6 клавиш одновременно). скан‑коды HID (см. HID Usage Tables).
//uint8_t num_key количество реально нажатых клавиш в массиве keyboard_cmd (максимум 6).
vTaskDelay(400 / portTICK_PERIOD_MS);
fgets(bufferChars, sizeof(bufferChars), stdin);  // ввод с USB порта
//vTaskDelay(400 / portTICK_PERIOD_MS);
if(bufferChars[0]!=' ')
{
	printf("DataEcho:");
for(int i=0;i<128;i++)
{
	if(bufferChars[i]=='\0'||bufferChars[i]=='\n')
	{break;}
	
	commandChars[i]=bufferChars[i];
	//printf("%c",bufferChars[i]);
	printf("%c",commandChars[i]);
}
	printf("\n");
}
//*********************************** время подтверждения проверки правильности  считывания данных  ************************** */
if(bufferChars[0]!=' ')  
{vTaskDelay(2000 / portTICK_PERIOD_MS);

fgets(bufferChars, sizeof(bufferChars), stdin);  // ввод с USB порта  
if(bufferChars[0]=='O'&&bufferChars[1]=='K')
	{
printf("DataVerified:");

for(int i=0;i<128;i++)
		{				
printf("%c ",commandChars[i]);	
		}
printf("\n");

receivingCommand=decodingCommandCharArray(commandChars);

	if(receivingCommand.device==MOUSE)
	 {
			if(receivingCommand.command==MOVE)
       	     {
				moveMouseToXY( hid_conn_id,receivingCommand.x, receivingCommand.y,1);
			 }
   			 else 	if(receivingCommand.command==CLICK)
  	  		 { 
	      	MouseClick( hid_conn_id,  receivingCommand.mouseButton, 70);
			 }
	  }
	}
for(int i=0;i<128;i++)
{
	commandChars[i] = ' ';
	bufferChars[i]  = ' ';
}	
}
//*********************************** окончание подтверждения проверки правильности  считывания данных  ************************** */
vTaskDelay(2000 / portTICK_PERIOD_MS);
	  
		}//********while************* */
}
//**********************  end hid_demo_task ***********************************************************************************/


void app_main(void)
{
 uint16_t conn_id = app_init_main();
    xTaskCreate(&hid_demo_task, "hid_task", 10240, &conn_id, 5, NULL);
}
