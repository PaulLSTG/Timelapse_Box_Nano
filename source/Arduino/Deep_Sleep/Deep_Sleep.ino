#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"
#include "esp_sleep.h"

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

// Intervalle entre les captures (en microsecondes, 100 secondes ici)
#define SLEEP_DURATION   100 * 1000000ULL
#define MAX_RETRIES      50  // Nombre maximum de tentatives avant une veille prolongée
#define LONG_SLEEP_DURATION 600 * 1000000ULL  // Veille prolongée en cas d'échec répété (10 minute)

RTC_DATA_ATTR static int photoCounter = 0;  // Conserve la valeur entre les cycles de sommeil
RTC_DATA_ATTR static int retryCounter = 0;  // Conserve le nombre de tentatives échouées

void setup() {
  // Initialisation du port série
  Serial.begin(115200);
  delay(10);

  // Vérification du compteur de tentatives
  if (retryCounter >= MAX_RETRIES) {
    Serial.println("Trop d'échecs consécutifs. Veille prolongée...");
    esp_deep_sleep(LONG_SLEEP_DURATION);
  }

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

  if (esp_camera_init(&config) == ESP_OK) {
    Serial.println("Caméra initialisée avec succès");
} else {
    Serial.println("Échec de l'initialisation de la caméra");
    ESP.restart();
}

  // Initialisation de la carte SD
  if (SD_MMC.begin()) {
    Serial.println("Carte SD initialisée avec succès");
  } else {
    Serial.println("Échec de l'initialisation de la carte SD");
    ESP.restart();
  }

  // Capture de la photo
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Échec de la capture de la photo");
    retryCounter++;
    ESP.restart();
  }

  // Sauvegarde de l'image sur la carte SD
  String path = "/photo" + String(photoCounter++) + ".jpg";
  File file = SD_MMC.open(path.c_str(), FILE_WRITE);
  if (!file) {
    Serial.println("Échec de l'ouverture du fichier pour l'écriture");
    esp_camera_fb_return(fb);
    retryCounter++;
    ESP.restart();
  }

  // Écriture des données de l'image dans le fichier
  file.write(fb->buf, fb->len);
  Serial.printf("Photo sauvegardée : %s\n", path.c_str());

  // Réinitialisation du compteur de tentatives après un succès
  retryCounter = 0;

  // Fermeture du fichier et libération de la mémoire
  file.close();
  esp_camera_fb_return(fb);

  // Mise en veille profonde
  Serial.println("Mise en veille profonde...");
  esp_deep_sleep(SLEEP_DURATION);
}

void loop() {
  // Rien ici, le programme ne devrait jamais atteindre cette section
}

