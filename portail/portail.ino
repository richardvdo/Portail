// digital pin 2 has a pushbutton attached to it. Give it a name:
int finCourseFerme = 4;
int finCourseOuvert = 5;
int sensFct = 6;
int miseFctChargeur = 7;
int miseFctMoteur = 8;
int bttnOuvrir = 9;
int bttnFerme = 10;
int bttnArret = 2;
int bttnFctionement = 2;
int diode = 11;
int chargement = 12;
int resetProtection = 13;
int capteurCourant = A0;
int reglageCourantMax = A1;
enum { fermeture, ouverture };
byte on = LOW;
byte off = HIGH;
byte open = LOW;
byte close = HIGH;
byte protection = LOW;

void setup() {
  //start serial connection
  Serial.begin(9600);

  pinMode(finCourseFerme, INPUT_PULLUP);
  pinMode(finCourseOuvert, INPUT_PULLUP);
  pinMode(bttnOuvrir, INPUT_PULLUP);
  pinMode(bttnFerme, INPUT_PULLUP);
  pinMode(bttnArret, INPUT_PULLUP);
  pinMode(bttnFctionement, INPUT_PULLUP);
  pinMode(chargement, INPUT_PULLUP);
  pinMode(resetProtection, INPUT_PULLUP);
  pinMode(sensFct, OUTPUT);
  pinMode(miseFctChargeur, OUTPUT);
  pinMode(miseFctMoteur, OUTPUT);
  pinMode(diode, OUTPUT);
  digitalWrite(sensFct, close);
  digitalWrite(miseFctChargeur, off);
  digitalWrite(miseFctMoteur, off);
  digitalWrite(diode, off);

}


void mesureCourant(const byte sens) {
  int cptCourant = analogRead(capteurCourant);
  float voltage = cptCourant * (5.0 / 1023.0);
  int courantMax = analogRead(reglageCourantMax);
  float voltageMax = courantMax * (5.0 / 1023.0);
  Serial.println(voltage);
  Serial.println(voltageMax);
  if (voltage >= voltageMax)
  {
    delay(500);
    cptCourant = analogRead(capteurCourant);
    voltage = cptCourant * (5.0 / 1023.0);
    Serial.println("depassement crt");
    if (voltage >= voltageMax)
    {
      Serial.println("depassementcrt blocage");
      blocage(sens);
    }
  }

}


void ouvrir() {
  //  Serial.println("debut ouverture");
  //  Serial.println(finCourseOuvert);
  while (digitalRead(finCourseOuvert) != HIGH && digitalRead(bttnOuvrir) == LOW && digitalRead(bttnFctionement) == HIGH && protection == LOW)
  {
    //    Serial.println("ouverture");
    digitalWrite(sensFct, open);
    digitalWrite(miseFctChargeur, on);
    delay(250);
    digitalWrite(miseFctMoteur, on);
    mesureCourant(ouverture);
    digitalWrite(diode, off);
    delay(250);
    digitalWrite(diode, on);
    delay(250);
  }
  //  Serial.println("arret ouverture");
  digitalWrite(sensFct, close);
  digitalWrite(miseFctMoteur, off);
  delay(2500);
  digitalWrite(miseFctChargeur, off);
}


void fermer() {
  //  Serial.println("debut fermeture");
  //  Serial.println(finCourseFerme);
  while (digitalRead(finCourseFerme) != HIGH && digitalRead(bttnFerme) == LOW && digitalRead(bttnFctionement) == HIGH && protection == LOW)
  {
    //    Serial.println("fermeture");
    digitalWrite(sensFct, close);
    digitalWrite(miseFctChargeur, on);
    digitalWrite(miseFctMoteur, on);
    mesureCourant(fermeture);
    digitalWrite(diode, off);
    delay(250);
    digitalWrite(diode, on);
    delay(250);
  }
  //  Serial.println("arret fermeture");
  digitalWrite(sensFct, close);
  digitalWrite(miseFctMoteur, off);
  delay(2500);
  digitalWrite(miseFctChargeur, off);
}


void blocage(const byte sens) {
  //  Serial.println("blocage");
  switch (sens) {
    case ouverture:
      //      Serial.println("ouverture");
      digitalWrite(sensFct, close);
      digitalWrite(miseFctChargeur, on);
      digitalWrite(miseFctMoteur, on);
      delay(2000);
      //      Serial.println("apres inversion");
      digitalWrite(sensFct, close);
      digitalWrite(miseFctChargeur, off);
      digitalWrite(miseFctMoteur, off);
      break;
    case fermeture:
      //      Serial.println("fermeture");
      digitalWrite(sensFct, open);
      digitalWrite(miseFctChargeur, on);
      digitalWrite(miseFctMoteur, on);
      delay(2000);
      //      Serial.println("apres inversion");
      digitalWrite(sensFct, close);
      digitalWrite(miseFctChargeur, off);
      digitalWrite(miseFctMoteur, off);
      break;
    default:
      //      Serial.println("defaut");
      digitalWrite(sensFct, close);
      digitalWrite(miseFctChargeur, off);
      digitalWrite(miseFctMoteur, off);
      break;
  }
  protection = HIGH;
}

void loop() {
  //  Serial.println("debut");
  if (digitalRead(bttnFctionement) == HIGH && protection == LOW) {
    digitalWrite(diode, HIGH);
    //    Serial.println("bouclebttnfct");
    if (digitalRead(bttnFerme) == LOW)
    {
      //      Serial.println("fermer");
      fermer();
    }
    if (digitalRead(bttnOuvrir) == LOW)
    {
      //      Serial.println("ouvrir");
      ouvrir();
    }
    if (digitalRead(chargement) == LOW)
    {
      //      Serial.println("fermer");
      digitalWrite(miseFctChargeur, on);
    }
  }
  if (digitalRead(resetProtection) == LOW)
  {
    protection = LOW;
    digitalWrite(miseFctChargeur, off);
  }
  delay(250);
  digitalWrite(diode, LOW);
  //  Serial.println("fin");
}

