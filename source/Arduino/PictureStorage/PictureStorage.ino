#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"

// Configuration de la caméra pour ESP32-CAM (AI-Thinker)
#define PWDN_GPIO_NUM    32
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    0
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27
#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      21
#define Y4_GPIO_NUM      19
#define Y3_GPIO_NUM      18
#define Y2_GPIO_NUM      5
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

void setup() {
  // Initialisation du port série
  Serial.begin(115200);
  delay(10);
  
  // Configuration de la caméra
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // Configuration de la qualité d'image
  config.frame_size = FRAMESIZE_SVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;

  // Initialisation de la caméra
  if (!esp_camera_init(&config)) {
    Serial.println("Caméra initialisée avec succès");
  } else {
    Serial.println("Échec de l'initialisation de la caméra");
    return;
  }

  // Initialisation de la carte SD
  if (!SD_MMC.begin()) {
    Serial.println("Échec de l'initialisation de la carte SD");
    return;
  }
  Serial.println("Carte SD initialisée avec succès");
}

void loop() {
  // Capture de la photo
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Échec de la capture de la photo");
    return;
  }

  // Création d'un nom de fichier unique
  String path = "/photo" + String(millis()) + ".jpg";

  // Sauvegarde de l'image sur la carte SD
  File file = SD_MMC.open(path.c_str(), FILE_WRITE);
  if (!file) {
    Serial.println("Échec de l'ouvertu
