#include <WiFi.h>          // Inclusion de la bibliothèque Wi-Fi pour l'ESP32
//#include <FS.h>            // Système de fichiers
//#include <SPIFFS.h>        // Système SPIFFS pour le stockage

// Remplacez par votre SSID et mot de passe Wi-Fi
const char* ssid = "Redmi_Note_10_Pro";
const char* password = "programmation";

void setup() {

  //CONNEXION WIFI

  // Initialisation de la communication série pour le débogage
  Serial.begin(115200);

  // Démarrage de la connexion Wi-Fi
  Serial.print("Connexion au Wi-Fi...");
  WiFi.begin(ssid, password);

  // Attente de connexion
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Affichage de l'adresse IP une fois connecté
  Serial.println("\nConnexion établie !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  //STOCKAGE IMAGE

  // Initialisation de SPIFFS
  //if (!SPIFFS.begin(true)) {
  //  Serial.println("Erreur lors de l'initialisation de SPIFFS...");
  //  return;
  //}
  //Serial.println("SPIFFS initialisé avec succès.");

  // Vérifier la présence du fichier image
  //const char *imagePath = "C:\Users\Administrateur\Desktop\Dossier GitLAB\Timelapse_Box_Nano\img\Image_Test_Stockage.png";  // Le chemin doit correspondre au nom du fichier téléchargé dans SPIFFS
  //if (SPIFFS.exists(imagePath)) {
  //  Serial.println("Image PNG trouvée dans SPIFFS !");
  //} else {
  //  Serial.println("Image PNG non trouvée.");
  //}




}

void loop() {
  // Code à exécuter en boucle
}