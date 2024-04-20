#include <Talkie.h>
#include <driver/i2s.h>
#include <Vocab_US_Large.h>

#define LRC_PIN 27
#define BCLK_PIN 26
#define DIN_PIN 25

Talkie voice;

void setup() {
  // Initialize I2S interface
  i2s_pin_config_t pin_config = {
    .bck_io_num = BCLK_PIN,
    .ws_io_num = LRC_PIN,
    .data_out_num = DIN_PIN,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 22050,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_set_sample_rates(I2S_NUM_0, 22050); // Set the sample rate to 22050 Hz
}

void loop() {
  // Speak the phrase "I am a robot"
  voice.say(sp5_ARRIVAL);
  while (voice.isTalking()) {
    // Wait for the speech to finish
  }

  // Introduce a delay between repetitions
  delay(1000);
}
