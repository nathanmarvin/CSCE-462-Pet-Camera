#include <Arduino.h>
#include <driver/i2s.h>

// Define the I2S pin configuration
#define I2S_BCLK_PIN 26 // BCLK pin
#define I2S_LRC_PIN 27  // LRC pin
#define I2S_DOUT_PIN 25 // DIN (Data out) pin

// Define the parameters for the square wave
#define SQUARE_WAVE_FREQUENCY 1000 // Frequency of the square wave in Hz
#define SAMPLE_RATE 44100           // Sample rate in Hz
#define SAMPLE_SIZE_BITS 16         // Sample size in bits
#define SAMPLE_SIZE_BYTES (SAMPLE_SIZE_BITS / 8)
#define NUM_SAMPLES 128             // Number of samples in the buffer

// Initialize the I2S configuration
void initI2S() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags = 0,
    .dma_buf_count = 2,
    .dma_buf_len = NUM_SAMPLES
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_BCLK_PIN,
    .ws_io_num = I2S_LRC_PIN,
    .data_out_num = I2S_DOUT_PIN,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}

// Generate a square wave audio signal
void generateSquareWave(int16_t* buffer, size_t numSamples, float frequency, float sampleRate) {
  float period = sampleRate / frequency;
  for (size_t i = 0; i < numSamples; i++) {
    if (i % (int)period < period / 2) {
      buffer[i] = INT16_MAX; // Set the value to the maximum (positive) amplitude
    } else {
      buffer[i] = INT16_MIN; // Set the value to the minimum (negative) amplitude
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize the I2S configuration
  initI2S();

  // Allocate memory for the audio buffer
  int16_t *audioBuffer = (int16_t *)malloc(NUM_SAMPLES * SAMPLE_SIZE_BYTES);
  if (audioBuffer == NULL) {
    Serial.println("Error: Unable to allocate memory for audio buffer");
    while (1);
  }

  // Generate the square wave audio signal
  generateSquareWave(audioBuffer, NUM_SAMPLES, SQUARE_WAVE_FREQUENCY, SAMPLE_RATE);

  // Output the square wave audio signal through I2S
  size_t bytesWritten;
  i2s_write(I2S_NUM_0, audioBuffer, NUM_SAMPLES * SAMPLE_SIZE_BYTES, &bytesWritten, portMAX_DELAY);

  // Free the allocated memory for the audio buffer
  free(audioBuffer);
}

void loop() {
  // No operation in the loop
}
