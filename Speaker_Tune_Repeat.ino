#include <Arduino.h>
#include <driver/i2s.h>

// Define the I2S pin configuration
#define I2S_BCLK_PIN 0 // BCLK pin
#define I2S_LRC_PIN 33  // LRC pin
#define I2S_DOUT_PIN 32 // DIN (Data out) pin

// Define the parameters for the audio playback
#define SAMPLE_RATE 44100           // Sample rate in Hz
#define SAMPLE_SIZE_BITS 16         // Sample size in bits
#define SAMPLE_SIZE_BYTES (SAMPLE_SIZE_BITS / 8)

// Define musical notes with their frequencies (in Hz)
#define NOTE_C4 261.63
#define NOTE_D4 293.66
#define NOTE_E4 329.63
#define NOTE_F4 349.23
#define NOTE_G4 392.00
#define NOTE_A4 440.00
#define NOTE_B4 493.88
#define NOTE_C5 523.25

// Define durations for each note (in milliseconds)
#define NOTE_DURATION 500

// Volume control (adjust as needed)
#define VOLUME_FACTOR 0.5 // Adjust volume by multiplying the samples with this factor

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
    .dma_buf_len = 1024 // Increase buffer length for smooth playback
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

// Generate audio signal for a given frequency, duration, and volume
void generateToneWithVolume(int16_t* buffer, size_t numSamples, float frequency, float sampleRate, float volume) {
  size_t period = sampleRate / frequency;
  for (size_t i = 0; i < numSamples; i++) {
    int16_t sample = (i % period < period / 2) ? INT16_MAX : INT16_MIN;
    buffer[i] = (int16_t)(sample * volume);
  }
}

void playNote(float frequency, int duration) {
  size_t numSamples = (SAMPLE_RATE * duration) / 1000;
  int16_t *audioBuffer = (int16_t *)malloc(numSamples * SAMPLE_SIZE_BYTES);

  if (audioBuffer == NULL) {
    Serial.println("Error: Unable to allocate memory for audio buffer");
    return;
  }

  generateToneWithVolume(audioBuffer, numSamples, frequency, SAMPLE_RATE, VOLUME_FACTOR);

  size_t bytesWritten;
  i2s_write(I2S_NUM_0, audioBuffer, numSamples * SAMPLE_SIZE_BYTES, &bytesWritten, portMAX_DELAY);

  free(audioBuffer);
}

void setup() {
  Serial.begin(115200);
  initI2S();

  // Continuously play a simple tune (C major scale) with reduced volume
  while (1) {
    playNote(NOTE_C4, NOTE_DURATION);
    playNote(NOTE_D4, NOTE_DURATION);
    playNote(NOTE_E4, NOTE_DURATION);
    playNote(NOTE_F4, NOTE_DURATION);
    playNote(NOTE_G4, NOTE_DURATION);
    playNote(NOTE_A4, NOTE_DURATION);
    playNote(NOTE_B4, NOTE_DURATION);
    playNote(NOTE_C5, NOTE_DURATION);
  }
}

void loop() {
  // No operation in the loop
}
