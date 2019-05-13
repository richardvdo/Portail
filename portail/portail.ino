
// digital pin 2 has a pushbutton attached to it. Give it a name:
int finCourseFerme = 4;
int finCourseOuvert = 5;
int sens = 6;
int miseFctChargeur = 7;
int miseFctMoteur = 8;
int bttnOuvrir = 9;
int bttnFerme = 10;
int bttnArret = 2;
int bttnFctionement = 3;
int diode = 11;
int capteurCourant = A0;
int seuilCourant = 512;
enum { fermeture, ouverture };

void setup() {
  //start serial connection
  Serial.begin(9600);

  pinMode(finCourseFerme, INPUT_PULLUP);
  pinMode(finCourseOuvert, INPUT_PULLUP);
  pinMode(bttnOuvrir, INPUT_PULLUP);
  pinMode(bttnFerme, INPUT_PULLUP);
  pinMode(bttnArret, INPUT_PULLUP);
  pinMode(bttnFctionement, INPUT_PULLUP);
  pinMode(sens, OUTPUT);
  pinMode(miseFctChargeur, OUTPUT);
  pinMode(miseFctMoteur, OUTPUT);
  pinMode(diode, OUTPUT);
  digitalWrite(sens, LOW);
  digitalWrite(miseFctChargeur, LOW);
  digitalWrite(miseFctMoteur, LOW);
  digitalWrite(diode, LOW);
  
}


void mesureCourant(const byte sens) {
  int cptCourant = analogRead(capteurCourant);
  float voltage = cptCourant * (5.0 / 1023.0);
  if(voltage >= seuilCourant)
  {
    delay(0.25);
    cptCourant = analogRead(capteurCourant);
    voltage = cptCourant * (5.0 / 1023.0);
    if(voltage >= seuilCourant)
    {
      blocage(sens);
    }
  }
  
}


void ouvrir() {
  while(digitalRead(finCourseOuvert) != LOW)
  {
    digitalWrite(sens, LOW);
    digitalWrite(miseFctChargeur, HIGH);
    digitalWrite(miseFctMoteur, HIGH);
    mesureCourant(ouverture);
    digitalWrite(diode, LOW);  
    delay(0.25);
    digitalWrite(diode, HIGH);
    delay(0.25);  
  }
  digitalWrite(sens, LOW);
  digitalWrite(miseFctChargeur, LOW);
  digitalWrite(miseFctMoteur, LOW);
}


void fermer() {
  while(digitalRead(finCourseFerme) != LOW)
  {
    digitalWrite(sens, HIGH);
    digitalWrite(miseFctChargeur, HIGH);
    digitalWrite(miseFctMoteur, HIGH);
    mesureCourant(fermeture);
    digitalWrite(diode, LOW);  
    delay(0.25);
    digitalWrite(diode, HIGH);
    delay(0.25); 
  }
  digitalWrite(sens, LOW);
  digitalWrite(miseFctChargeur, LOW);
  digitalWrite(miseFctMoteur, LOW);    
}


void blocage(const byte sens) {
  switch (sens) {
    case ouverture:  
      digitalWrite(sens, HIGH);
      digitalWrite(miseFctChargeur, HIGH);
      digitalWrite(miseFctMoteur, HIGH);
      delay(2);
      digitalWrite(sens, LOW);
      digitalWrite(miseFctChargeur, LOW);
      digitalWrite(miseFctMoteur, LOW);    
      break;
    case fermeture: 
      digitalWrite(sens, LOW);
      digitalWrite(miseFctChargeur, HIGH);
      digitalWrite(miseFctMoteur, HIGH);
      delay(2);
      digitalWrite(sens, LOW);
      digitalWrite(miseFctChargeur, LOW);
      digitalWrite(miseFctMoteur, LOW);      
      break;
    default:
      digitalWrite(sens, LOW);
      digitalWrite(miseFctChargeur, LOW);
      digitalWrite(miseFctMoteur, LOW);
      break;
}

}

void loop() {
  Serial.println("debut"); 
  if (digitalRead(bttnFctionement) == LOW) {
    digitalWrite(diode, HIGH);
    if(digitalRead(bttnFerme) == LOW)
    {
      fermer();
    }
    if(digitalRead(bttnOuvrir) == LOW)
    {
      ouvrir();
    }    
  }
  delay(1);
  digitalWrite(diode, LOW);        
  } 

