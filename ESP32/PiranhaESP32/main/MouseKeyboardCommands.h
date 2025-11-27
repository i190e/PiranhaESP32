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
#include "esp_random.h"
#include "ComPortCommunication.h"
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
//define commands
#define MOVE               2
#define CLICK              3

/*
typedef struct 
{
	int device;
	int command;
	int x;
	int y;
	int mouseButton;
} commandFromSerialPort;
*/

//#include <EspHal.h>
 
 uint32_t crypto_random_in_range(uint32_t min, uint32_t max) {
    uint32_t range = max - min + 1;
    uint32_t val;

    do {
        esp_fill_random(&val, sizeof(val));
        val %= range;
    } while (val >= range);  // Избегаем смещения из‑за %
    return min + val;
}



//esp_hidd_send_keyboard_value(hid_conn_id, special_key_mask,keyboard_cmd, 1);
//key_mask_t special_key_mask битовая маска модификаторов (Ctrl, Shift, Alt и т. п.).
//uint8_t *keyboard_cmd массив кодов клавиш (до 6 клавиш одновременно). скан‑коды HID (см. HID Usage Tables).
//uint8_t num_key количество реально нажатых клавиш в массиве keyboard_cmd (максимум 6). 
 void typeKeyboard(uint16_t conn_id, key_mask_t special_key_mask, uint8_t *keyboard_cmd, uint8_t num_key)
 {
	 int keyPressTime = crypto_random_in_range(79, 130);
	 esp_hidd_send_keyboard_value( conn_id,  special_key_mask,  keyboard_cmd,  num_key);
	 vTaskDelay(keyPressTime / portTICK_PERIOD_MS);
	 esp_hidd_send_keyboard_value( conn_id,  special_key_mask,  keyboard_cmd,  0);	
 }
//MouseButtonsPatterns 0x00 — все кнопки отпущены; 0x01 — нажата левая кнопка; 0x02 — нажата правая кнопка; 0x04 — нажата средняя кнопка; 0x03 — нажаты левая и правая кнопки.
 void MouseMoveTeleport(uint16_t conn_id, uint8_t mouse_button, uint8_t mickeys_x, uint8_t mickeys_y)
 {
	uint8_t X=0;
	uint8_t Y=0;
	
   esp_hidd_send_mouse_value( conn_id,  mouse_button,  mickeys_x,  mickeys_y);// Buttons 0x00 — все кнопки отпущены; 0x01 — нажата левая кнопка; 0x02 — нажата правая кнопка; 0x04 — нажата средняя кнопка; 0x03 — нажаты левая и правая кнопки.		
 
 }

void moveMouseToXY(uint16_t conn_id, int x2, int y2, int delay_ms ) 
{
	int x1=0;
	int deltaX=0;
	int y1=0;	
	int deltaY=0;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (true) {
        // Place a point at (x1, y1)
                 if(deltaX>x1 && deltaY==y1)
    {
	esp_hidd_send_mouse_value( conn_id,  0x00,  -1,  0);// Buttons 0x00 — все кнопки отпущены; 0x01 — нажата левая кнопка; 0x02 — нажата правая кнопка; 0x04 — нажата средняя кнопка; 0x03 — нажаты левая и правая кнопки.		
	}
        else     if(deltaX<x1 && deltaY==y1)
    {
	esp_hidd_send_mouse_value( conn_id,  0x00,  1,  0);// Buttons 0x00 — все кнопки отпущены; 0x01 — нажата левая кнопка; 0x02 — нажата правая кнопка; 0x04 — нажата средняя кнопка; 0x03 — нажаты левая и правая кнопки.		
	}
        else     if(deltaX==x1 && deltaY>y1)
    {
	esp_hidd_send_mouse_value( conn_id,  0x00,  0,  -1);// Buttons 0x00 — все кнопки отпущены; 0x01 — нажата левая кнопка; 0x02 — нажата правая кнопка; 0x04 — нажата средняя кнопка; 0x03 — нажаты левая и правая кнопки.		
	}
        else     if(deltaX==x1 && deltaY<y1)
    {
	esp_hidd_send_mouse_value( conn_id,  0x00,  0,  1);// Buttons 0x00 — все кнопки отпущены; 0x01 — нажата левая кнопка; 0x02 — нажата правая кнопка; 0x04 — нажата средняя кнопка; 0x03 — нажаты левая и правая кнопки.		
	}
        else     if(deltaX>x1 && deltaY>y1)
    {
	esp_hidd_send_mouse_value( conn_id,  0x00,  -1,  -1);// Buttons 0x00 — все кнопки отпущены; 0x01 — нажата левая кнопка; 0x02 — нажата правая кнопка; 0x04 — нажата средняя кнопка; 0x03 — нажаты левая и правая кнопки.		
	}
        else     if(deltaX<x1 && deltaY<y1)
    {
	esp_hidd_send_mouse_value( conn_id,  0x00,  1,  1);// Buttons 0x00 — все кнопки отпущены; 0x01 — нажата левая кнопка; 0x02 — нажата правая кнопка; 0x04 — нажата средняя кнопка; 0x03 — нажаты левая и правая кнопки.		
	}
        else     if(deltaX>x1 && deltaY<y1)
    {
	esp_hidd_send_mouse_value( conn_id,  0x00,  -1,  1);// Buttons 0x00 — все кнопки отпущены; 0x01 — нажата левая кнопка; 0x02 — нажата правая кнопка; 0x04 — нажата средняя кнопка; 0x03 — нажаты левая и правая кнопки.		
	}
        else     if(deltaX<x1 && deltaY>y1)
    {
	esp_hidd_send_mouse_value( conn_id,  0x00,  1,  -1);// Buttons 0x00 — все кнопки отпущены; 0x01 — нажата левая кнопка; 0x02 — нажата правая кнопка; 0x04 — нажата средняя кнопка; 0x03 — нажаты левая и правая кнопки.		
	}
    //esp_hidd_send_mouse_value( hid_conn_id,  0x00,  x1-deltaX,  y1-deltaY);// Buttons 0x00 — все кнопки отпущены; 0x01 — нажата левая кнопка; 0x02 — нажата правая кнопка; 0x04 — нажата средняя кнопка; 0x03 — нажаты левая и правая кнопки.		
    deltaX=x1;
    deltaY=y1;
   // printf("x=%d:",x1);
   // printf("deltaX=%d:",deltaX);
   // printf("y=%d:",y1);
   // printf("deltaY=%d\n",deltaY);
    vTaskDelay(delay_ms / portTICK_PERIOD_MS);
        if (x1 == x2 && y1 == y2) break;
        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
     printf("Move Complete");
}

 void MouseClick(uint16_t conn_id, uint8_t mouse_button,uint8_t clickTimeDelay)
 {
// Buttons 0x00 — все кнопки отпущены; 0x01 — нажата левая кнопка; 0x02 — нажата правая кнопка; 0x04 — нажата средняя кнопка; 0x03 — нажаты левая и правая кнопки.		
	
	switch (mouse_button) {
    case LEFT:            //0x01 — нажата левая кнопка
                           esp_hidd_send_mouse_value( conn_id,  0x01,0,  0);
                           vTaskDelay(clickTimeDelay / portTICK_PERIOD_MS);
                           esp_hidd_send_mouse_value( conn_id,  0x00,0,  0);
                          break;
    case RIGHT:           //0x02 — нажата правая кнопка
                           esp_hidd_send_mouse_value( conn_id,  0x02,0,  0);
                           vTaskDelay(clickTimeDelay / portTICK_PERIOD_MS);
                           esp_hidd_send_mouse_value( conn_id,  0x00,0,  0);
                          break;
    case DOUBLECLICK:     //0x01 — нажата левая кнопка
                           esp_hidd_send_mouse_value( conn_id,  0x01,0,  0);
                           vTaskDelay(clickTimeDelay / portTICK_PERIOD_MS);
                           esp_hidd_send_mouse_value( conn_id,  0x00,0,  0);
                           vTaskDelay(clickTimeDelay / portTICK_PERIOD_MS);
                           esp_hidd_send_mouse_value( conn_id,  0x01,0,  0);
                           vTaskDelay(clickTimeDelay / portTICK_PERIOD_MS);
                           esp_hidd_send_mouse_value( conn_id,  0x00,0,  0);
                          break;
    case LEFTDOWN:        //0x01 — нажата левая кнопка
                           esp_hidd_send_mouse_value( conn_id,  0x01,0,  0);
                           vTaskDelay(clickTimeDelay / portTICK_PERIOD_MS);
                          break;
    case LEFTUP:          // 0x00 — все кнопки отпущены
                           esp_hidd_send_mouse_value( conn_id,  0x00,0,  0);
                           vTaskDelay(clickTimeDelay / portTICK_PERIOD_MS);
                          break;
    case RIGHTDOWN:       //0x02 — нажата правая кнопка
                           esp_hidd_send_mouse_value( conn_id,  0x02,0,  0);
                           vTaskDelay(clickTimeDelay / portTICK_PERIOD_MS);
                          break;
    case RIGHTUP:         // 0x00 — все кнопки отпущены
                           esp_hidd_send_mouse_value( conn_id,  0x00,0,  0);
                           vTaskDelay(clickTimeDelay / portTICK_PERIOD_MS);
                          break;
    case MIDDLE:          //0x04 — нажата средняя кнопка
                           esp_hidd_send_mouse_value( conn_id,  0x04,0,  0);
                           vTaskDelay(clickTimeDelay / portTICK_PERIOD_MS);
                           esp_hidd_send_mouse_value( conn_id,  0x00,0,  0);
                          break;
    case MIDDLEDOWN:      // 0x04 — нажата средняя кнопка
                           esp_hidd_send_mouse_value( conn_id,  0x04,0,  0);
                           vTaskDelay(clickTimeDelay / portTICK_PERIOD_MS);
                          break;
    case MIDDLEUP:        // 0x00 — все кнопки отпущены
                           esp_hidd_send_mouse_value( conn_id,  0x00,0,  0);
                           vTaskDelay(clickTimeDelay / portTICK_PERIOD_MS);
                          break;
    case LEFTRIGHTDOWN:   //0x03 — нажаты левая и правая кнопки.	
                           esp_hidd_send_mouse_value( conn_id,  0x03,0,  0);
                           vTaskDelay(clickTimeDelay / portTICK_PERIOD_MS);
                          break;
    case LEFTRIGHTUP:     // 0x00 — все кнопки отпущены 
                           esp_hidd_send_mouse_value( conn_id,  0x00,0,  0);
                           vTaskDelay(clickTimeDelay / portTICK_PERIOD_MS);
                          break;                          
    default:
        break;
 }
}