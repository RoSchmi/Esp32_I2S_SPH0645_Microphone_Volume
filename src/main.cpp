


/** 
 * ESP32 I2S VU Meter Example: 'Esp32_I2S_SPH0645_Microphone_Volume'
 * 
 * Used Board: Adafruit HUZZAH32 – ESP32 Feather Board
 * 
 * Gets the volume of surrounding noise using Adafruit I2S Microphone (SPH0645)
 * 
 * This example is based on the code of:
 * https://www.esp32.com/viewtopic.php?t=4997
 * and the "VU Meter Demo" Example in the 
 * Adafruit I2S MEMS Breakout board.
 * 
 * Some tricks to make the SPH0645 work properly on the ESP32
 * are explained in the above link and in this video:
 * https://www.youtube.com/watch?v=3g7l5bm7fZ8
 * 
 * 
 * @author RoSchmi
 */

#include <Arduino.h>
#include <driver/i2s.h>
#include "soc/i2s_reg.h"

// If default stack size of 8192 byte is not enough for your application.
// --> configure stack size dynamically from code to 16384
// https://community.platformio.org/t/esp32-stack-configuration-reloaded/20994/4
// Patch: Replace C:\Users\thisUser\.platformio\packages\framework-arduinoespressif32\cores\esp32\main.cpp
// with the file 'main.cpp' from folder 'patches' of this repository, then use the following code to configure stack size
// comment the following 4 lines if you do not want to use the patch
#if !(USING_DEFAULT_ARDUINO_LOOP_STACK_SIZE)
  //uint16_t USER_CONFIG_ARDUINO_LOOP_STACK_SIZE = 16384;
  uint16_t USER_CONFIG_ARDUINO_LOOP_STACK_SIZE = 8192;
#endif

#define BUFLEN 256

static const i2s_port_t i2s_num = I2S_NUM_0; // i2s port number

static const i2s_config_t i2s_config = {
     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
     .sample_rate = 22050,
     .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
     .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
     .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
     .intr_alloc_flags = 0, // default interrupt priority
     .dma_buf_count = 8,
     .dma_buf_len = 64,
     .use_apll = false
};

// For Adafruit Huzzah Esp32

static const i2s_pin_config_t pin_config = {
    .bck_io_num = 14,                   // BCKL
    .ws_io_num = 15,                    // LRCL
    .data_out_num = I2S_PIN_NO_CHANGE,  // not used (only for speakers)
    .data_in_num = 32                   // DOUT
};

// For some other Esp32 board
/* 
static const i2s_pin_config_t pin_config = {
    .bck_io_num = 26,                   // BCKL
    .ws_io_num = 25,                    // LRCL
    .data_out_num = I2S_PIN_NO_CHANGE,  // not used (only for speakers)
    .data_in_num = 22                   // DOUT
};
*/

void setup() 
{ 
  Serial.begin(115200);
  Serial.println("Configuring I2S...");

  pinMode(22, INPUT);
  i2s_driver_install(i2s_num, &i2s_config, 0, NULL);   //install and start i2s driver
  REG_SET_BIT(  I2S_TIMING_REG(i2s_num),BIT(9));   /*  #include "soc/i2s_reg.h"   I2S_NUM -> 0 or 1*/
  REG_SET_BIT( I2S_CONF_REG(i2s_num), I2S_RX_MSB_SHIFT);
  i2s_set_pin(i2s_num, &pin_config);
}

int32_t audio_buf[BUFLEN];

void loop() {
    size_t bytes_read;
    i2s_read(i2s_num, audio_buf, sizeof(audio_buf), &bytes_read, portMAX_DELAY);
    int32_t cleanBuf[BUFLEN / 2] {0};
    int cleanBufIdx = 0;
    for (int i = 0; i < BUFLEN; i++)
    {
      if (audio_buf[i] != 0)    // Exclude values from other channel
      {
          cleanBuf[cleanBufIdx] = audio_buf[i] >> 14;
          cleanBufIdx++;
      }
    }
    float meanval = 0;
    int volCount = 0;
    for (int i=0; i < BUFLEN / 2; i++) 
    {
         if (cleanBuf[i] != 0)
         {
          meanval += cleanBuf[i];
          volCount++;
         }
    }
    meanval /= volCount;

    // subtract it from all sapmles to get a 'normalized' output
    for (int i=0; i< volCount; i++) 
    {
        cleanBuf[i] -= meanval;
    }

    // find the 'peak to peak' max
    float maxsample, minsample;
    minsample = 100000;
    maxsample = -100000;
    for (int i=0; i<volCount; i++) {
      minsample = _min(minsample, cleanBuf[i]);
      maxsample = _max(maxsample, cleanBuf[i]);
    }
    Serial.print("Volume: ");
    Serial.println(maxsample - minsample);
    delay(500);
}

