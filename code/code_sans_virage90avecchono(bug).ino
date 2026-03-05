#include <SoftwareSerial.h>

SoftwareSerial bluetooth(10, 9);

// Configuration des broches pour les moteurs
int EN1 = 11; // PWM moteur Droit
int EN2 = 3;  // PWM moteur Gauche
int IN1 = A0; // Direction 1 moteur Droit
int IN2 = A1; // Direction 2 moteur Droit
int IN3 = A2; // Direction 1 moteur Gauche
int IN4 = A3; // Direction 2 moteur Gauche

// Configuration des capteurs de ligne
#define D1 2
#define D2 4
#define D3 5
#define D4 7
#define D5 8

// Configuration du buzzer
#define BUZZER_PIN 6 // Utiliser une broche PWM

// Configuration du capteur HC-SR04
#define trigPin 12
#define echoPin 13

int Conteur = 0;
int porteObjectif = 0;
bool robotEnMarche = false;
bool pauseDejaFaite = false;
bool antiBugEnCours = false;
bool obstacleDetecte = false;
unsigned long dernierBip = 0;
unsigned long dernierCompte = 0; // Délai pour éviter de compter deux fois la même porte

bool surBlanc = false;
unsigned long debutBlanc = 0;
unsigned long dernierMouvement = 0;
bool enVirageDroite = false;
bool enVirageGauche = false;

unsigned long bipStartTime = 0;
bool bipEnCours = false;

void MotorD(int pwm, String sens) {
  analogWrite(EN1, pwm);
  if (sens == "avant") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (sens == "arriere") {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
}

void MotorG(int pwm, String sens) {
  analogWrite(EN2, pwm);
  if (sens == "avant") {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else if (sens == "arriere") {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}

void Arret() {
  MotorD(0, "stop");
  MotorG(0, "stop");
}


void Avancer() {
  if (enVirageDroite || enVirageGauche) {
    return; // Ne pas avancer si un virage est en cours
  }

  int CD1 = digitalRead(D1);
  int CD2 = digitalRead(D2);
  int CD3 = digitalRead(D3);
  int CD4 = digitalRead(D4);
  int CD5 = digitalRead(D5);


  if (CD1 == 0 && CD2 == 0 && CD3 == 1 && CD4 == 0 && CD5 == 0) {
    MotorD(86, "avant"); // c'est pas les même car moteur droit avance moins vite
    MotorG(80, "avant");
  } else if (CD1 == 0 && CD2 == 1 && CD3 == 1 && CD4 == 1 && CD5 == 0) {
    MotorD(86, "avant"); // c'est pas les même car moteur droit avance moins vite
    MotorG(80, "avant");
  } else if (CD1 == 0 && CD2 == 0 && CD3 == 1 && CD4 == 1 && CD5 == 0) {
    MotorD(80, "avant");
    MotorG(83, "avant");
  } else if (CD1 == 0 && CD2 == 1 && CD3 == 1 && CD4 == 0 && CD5 == 0) {
    MotorD(86, "avant");
    MotorG(80, "avant");
  } else if (CD1 == 0 && CD2 == 1 && CD3 == 0 && CD4 == 0 && CD5 == 0) {
    MotorD(83, "avant");
    MotorG(70, "avant");
  } else if (CD1 == 0 && CD2 == 0 && CD3 == 0 && CD4 == 1 && CD5 == 0) {
    MotorD(70, "avant");
    MotorG(80, "avant");
  } else if (CD1 == 0 && CD2 == 0 && CD3 == 1 && CD4 == 1 && CD5 == 1) {
    MotorD(80, "arriere");
    MotorG(90, "avant");
  } else if (CD1 == 0 && CD2 == 0 && CD3 == 0 && CD4 == 1 && CD5 == 1) {
    MotorD(80, "arriere");
    MotorG(90, "avant");
  } else if (CD1 == 0 && CD2 == 0 && CD3 == 0 && CD4 == 0 && CD5 == 1) {
    MotorD(80, "arriere");
    MotorG(90, "avant");
  } else if (CD1 == 0 && CD2 == 0 && CD3 == 1 && CD4 == 0 && CD5 == 1) {
    MotorD(80, "arriere");
    MotorG(90, "avant");
  } else if (CD1 == 0 && CD2 == 1 && CD3 == 1 && CD4 == 1 && CD5 == 1) {
    MotorD(80, "arriere");
    MotorG(90, "avant");
  } else if (CD1 == 1 && CD2 == 1 && CD3 == 1 && CD4 == 0 && CD5 == 0) {
    MotorD(90, "avant");
    MotorG(80, "arriere");
  } else if (CD1 == 1 && CD2 == 1 && CD3 == 0 && CD4 == 0 && CD5 == 0) {
    MotorD(90, "avant");
    MotorG(80, "arriere");
  } else if (CD1 == 1 && CD2 == 0 && CD3 == 0 && CD4 == 0 && CD5 == 0) {
    MotorD(90, "avant");
    MotorG(80, "arriere");
  } else if (CD1 == 1 && CD2 == 0 && CD3 == 1 && CD4 == 0 && CD5 == 0) {
    MotorD(90, "avant");
    MotorG(80, "arriere");
  } else if (CD1 == 1 && CD2 == 1 && CD3 == 1 && CD4 == 1 && CD5 == 0) {
    MotorD(90, "avant");
    MotorG(80, "arriere");

  }

  dernierMouvement = millis();
}

bool enVirage(int CD1, int CD2, int CD3, int CD4, int CD5) {
  return (
    (CD1 == 0 && CD2 == 1 && CD3 == 1 && CD4 == 1 && CD5 == 1) || 
    (CD1 == 1 && CD2 == 1 && CD3 == 1 && CD4 == 0 && CD5 == 0) ||
    (CD1 == 1 && CD2 == 1 && CD3 == 1 && CD4 == 1 && CD5 == 0) ||
    (CD1 == 1 && CD2 == 0 && CD3 == 1 && CD4 == 0 && CD5 == 0) ||
    (CD1 == 0 && CD2 == 0 && CD3 == 1 && CD4 == 0 && CD5 == 1) ||
    (CD1 == 0 && CD2 == 0 && CD3 == 1 && CD4 == 1 && CD5 == 1) ||
    (CD1 == 1 && CD2 == 0 && CD3 == 0 && CD4 == 0 && CD5 == 0) ||
    (CD1 == 1 && CD2 == 1 && CD3 == 0 && CD4 == 0 && CD5 == 0) ||
    (CD1 == 0 && CD2 == 0 && CD3 == 0 && CD4 == 1 && CD5 == 1) ||
    (CD1 == 0 && CD2 == 0 && CD3 == 0 && CD4 == 0 && CD5 == 1)
  );
}

void setup() {
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  pinMode(D5, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  bluetooth.begin(9600);
  Serial.println("En attente de commande Bluetooth...");

  // Configurer le buzzer pour générer une onde carrée à 2000 Hz
  analogWrite(BUZZER_PIN, 127); // 50% duty cycle
}

void loop() {
  // Vérifier les commandes Bluetooth à chaque itération
  if (bluetooth.available()) {
    char c = bluetooth.read();
    Serial.print(">> ");
    Serial.println(c);

    if (c >= '1' && c <= '6') {
      porteObjectif = c - '0';
      robotEnMarche = true;
      Conteur = 0;
      pauseDejaFaite = false;
      antiBugEnCours = false;
      obstacleDetecte = false;
      Serial.print("Le robot s'arrêtera à la porte : ");
      Serial.println(porteObjectif);
    } else if (c == '7') {
      porteObjectif = 0;
      robotEnMarche = false;
      Conteur = 0;
      pauseDejaFaite = false;
      antiBugEnCours = false;
      obstacleDetecte = false;
      Serial.println("Programme réinitialisé.");
      Arret(); // Arrêter le robot immédiatement
    }
  }

  // Mesure de la distance avec le capteur HC-SR04
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  if (distance <= 7 && distance >= 0) {
    if (!obstacleDetecte) {
      Serial.println("Obstacle détecté ! Arrêt du robot.");
      obstacleDetecte = true;
      dernierBip = millis();
    }
    Arret();
    if (millis() - dernierBip >= 1000) {
      analogWrite(BUZZER_PIN, 127); // Générer une onde carrée à 1000 Hz
      delay(200); // Durée du bip
      analogWrite(BUZZER_PIN, 0); // Arrêter le buzzer
      dernierBip = millis();
    }
  } else {
    if (obstacleDetecte) {
      Serial.println("Obstacle retiré. Reprise du programme.");
      obstacleDetecte = false;
      analogWrite(BUZZER_PIN, 0); // Arrêter le buzzer
      antiBugEnCours = true; // Activer la fonction anti-bug
    }
  }

  if (robotEnMarche && !obstacleDetecte) {
    int CD1 = digitalRead(D1);
    int CD2 = digitalRead(D2);
    int CD3 = digitalRead(D3);
    int CD4 = digitalRead(D4);
    int CD5 = digitalRead(D5);
    unsigned long maintenant = millis();

    bool toutBlanc = (CD1 == 0 && CD2 == 0 && CD3 == 0 && CD4 == 0 && CD5 == 0);
    bool virage = enVirage(CD1, CD2, CD3, CD4, CD5);

    if (toutBlanc && !surBlanc && !virage) {
      surBlanc = true;
      debutBlanc = maintenant;
    } else if (!toutBlanc || virage) {
      surBlanc = false;
    } else if (toutBlanc && surBlanc && (maintenant - debutBlanc >= 120) && (maintenant - dernierMouvement <= 700)) {
      if (maintenant - dernierCompte >= 1000) { // Délai pour éviter de compter deux fois la même porte
        Conteur++;
        Serial.print("Porte détectée : ");
        Serial.println(Conteur);
        surBlanc = false;
        bipStartTime = millis(); // Démarre le bip
        bipEnCours = true;
        dernierCompte = maintenant; // Mettre à jour le dernier compte
      }
    }

    if (Conteur == porteObjectif && !pauseDejaFaite) {
      Serial.println("Pause à la porte choisie !");
      bluetooth.println("STOP"); // Envoyer le message "stop" à l'application MIT      
      delay(50); // Micro-délai avant l'arrêt total des moteurs
      Arret();
      unsigned long startPauseTime = millis();
      unsigned long bipDuration = 100; // Durée de chaque bip en ms
      unsigned long bipInterval = 500; // Intervalle entre chaque bip en ms

      // Générer les bips pendant la pause
      for (int i = 0; i < porteObjectif; i++) {
        analogWrite(BUZZER_PIN, 127); // Générer une onde carrée à 2000 Hz
        delay(bipDuration); // Durée du bip
        analogWrite(BUZZER_PIN, 0); // Arrêter le buzzer
        delay(bipInterval - bipDuration); // Attendre avant le prochain bip
      }

      pauseDejaFaite = true;
      delay(5000 - (porteObjectif * (bipDuration + bipInterval - bipDuration))); // Attendre le reste des 5 secondes

      // Vérifier si tous les capteurs détectent du blanc après la pause
      CD1 = digitalRead(D1);
      CD2 = digitalRead(D2);
      CD3 = digitalRead(D3);
      CD4 = digitalRead(D4);
      CD5 = digitalRead(D5);
      if (CD1 == 0 && CD2 == 0 && CD3 == 0 && CD4 == 0 && CD5 == 0) {
        antiBugEnCours = true;
        unsigned long antiBugStartTime = millis();
        while (millis() - antiBugStartTime < 1000) {
          MotorD(80, "avant");
          MotorG(80, "avant");
        }
        Arret();
      }
    }

    if (!antiBugEnCours) {
      Avancer();
    } else {
      antiBugEnCours = false;
      robotEnMarche = true;
    }
  }

  // Gérer le bip
  if (bipEnCours && (millis() - bipStartTime < 100)) {
    analogWrite(BUZZER_PIN, 127); // Générer une onde carrée à 2000 Hz
  } else {
    bipEnCours = false;
    analogWrite(BUZZER_PIN, 0); // Arrêter le buzzer
  }

  // Vérifier si le robot doit s'arrêter et réinitialiser à la porte 7 ou plus
  if (Conteur >= 7) {
    Serial.println("Porte 7 ou plus atteinte ! Arrêt et réinitialisation du programme.");
    Arret();
    porteObjectif = 0;
    robotEnMarche = false;
    Conteur = 0;
    pauseDejaFaite = false;
    antiBugEnCours = false;
    obstacleDetecte = false;
    Serial.println("Programme réinitialisé.");
  }
}
