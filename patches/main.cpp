#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "Arduino.h"

#ifndef CONFIG_ARDUINO_LOOP_STACK_SIZE
  #define CONFIG_ARDUINO_LOOP_STACK_SIZE 8192
#endif



#define USING_DEFAULT_ARDUINO_LOOP_STACK_SIZE      false


#if !(USING_DEFAULT_ARDUINO_LOOP_STACK_SIZE)

//KH
// Have to use in your code as follows
// uint16_t USER_CONFIG_ARDUINO_LOOP_STACK_SIZE = 16384;

#warning Using USER_CONFIG_ARDUINO_LOOP_STACK_SIZE, You have to declare USER_CONFIG_ARDUINO_LOOP_STACK_SIZE in your code

extern uint16_t USER_CONFIG_ARDUINO_LOOP_STACK_SIZE;
/////
#endif

TaskHandle_t loopTaskHandle = NULL;

#if CONFIG_AUTOSTART_ARDUINO
bool loopTaskWDTEnabled;
void loopTask(void *pvParameters)
{
    setup();
    for(;;) {
        if(loopTaskWDTEnabled){
            esp_task_wdt_reset();
        }
        loop();
        if (serialEventRun) serialEventRun();
    }
}

extern "C" void app_main()
{
    loopTaskWDTEnabled = false;
    initArduino();  

    #if USING_DEFAULT_ARDUINO_LOOP_STACK_SIZE
        // Original
        xTaskCreateUniversal(loopTask, "loopTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &loopTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);

    #else
        if (USER_CONFIG_ARDUINO_LOOP_STACK_SIZE < 8192)
        USER_CONFIG_ARDUINO_LOOP_STACK_SIZE = 8192;
        else if (USER_CONFIG_ARDUINO_LOOP_STACK_SIZE > 32768)
        USER_CONFIG_ARDUINO_LOOP_STACK_SIZE = 32768;   
        xTaskCreateUniversal(loopTask, "loopTask", USER_CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &loopTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
    #endif    
}
#endif
