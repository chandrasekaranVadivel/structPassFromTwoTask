#include<Arduino_FreeRTOS.h>
#include<task.h>
#include<queue.h>
TaskHandle_t *adc1_h,*adc2_h,*print_h,*led_h;
QueueHandle_t xQueueToPrint;

typedef struct dataToPrint_h
{
  int pin;
  int val;
}dataToPrint_t;

void adc1(void *parm)
{
  pinMode(A0,INPUT);
  dataToPrint_t data;
  while(1)
  {
    
    data.pin=0;
    data.val=analogRead(A0);
    xQueueSend(xQueueToPrint, &data, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(500));
    
  }
}
void adc2(void *parm)
{
  pinMode(A1,INPUT);
  dataToPrint_t data;
  while(1)
  {
    
    data.pin=1;
    data.val=analogRead(A1);
    xQueueSend(xQueueToPrint, &data, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(500));
    
  }
}
void led(void *parm)
{
  pinMode(LED_BUILTIN,OUTPUT);
  while(1)
  {
    digitalWrite(LED_BUILTIN,HIGH);
    vTaskDelay(pdMS_TO_TICKS(100));
    digitalWrite(LED_BUILTIN,LOW);
    vTaskDelay(pdMS_TO_TICKS(100));    
    
  }
}
void print(void *parm)
{
  Serial.begin(9600);
  dataToPrint_t data;
  char datStr[10];
  while(1)
  {
    
   if(xQueueReceive(xQueueToPrint, &data, portMAX_DELAY)==pdTRUE)
   {
    sprintf(datStr,"pin%d=%d",data.pin,data.val);
    Serial.println(datStr);
   }

  }
  
}
void setup() 
{
  xTaskCreate(adc1,"adc1",125,NULL,1,adc1_h);
  xTaskCreate(adc2,"adc2",125,NULL,1,adc2_h);
  xTaskCreate(led,"led",200,NULL,1,led_h);
  xTaskCreate(print,"print",125,NULL,1,print_h);
  xQueueToPrint=xQueueCreate(5, sizeof(dataToPrint_t));
}
void loop() 
{}
