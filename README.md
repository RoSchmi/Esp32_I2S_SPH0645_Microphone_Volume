# Esp32_I2S_SPH0645_Microphone_Volume


  ESP32 I2S VU Meter Example: 'Esp32_I2S_SPH0645_Microphone_Volume'
  
  Used Board: Adafruit HUZZAH32 – ESP32 Feather Board (-https://www.adafruit.com/product/3405)
  
  Gets the volume of surrounding noise using Adafruit I2S Microphone (SPH0645)
 
  This example is based on the code of:
  https://www.esp32.com/viewtopic.php?t=4997
  
  the "VU Meter Demo" Example in the 
  Adafruit I2S MEMS Breakout board tutorial (https://learn.adafruit.com/adafruit-i2s-mems-microphone-breakout/arduino-wiring-and-test)
  
  and
  https://github.com/maspetsberger/esp32-i2s-mems
  
  Since I had some problems getting reasonable volume readings with the code of maspetsberger I included some tricks to make the SPH0645 work properly on the ESP32
  which are explained in the above link
  
  https://www.esp32.com/viewtopic.php?t=4997
  
  and this video
  
  https://www.youtube.com/watch?v=3g7l5bm7fZ8
  
