#include <Mouse.h> //per il mouse 
#include <Keyboard.h> //per la tastiera

//definzione costanti per O/I dispositivo
const int LedON = 10; //pin dell'indicatore led
const int BottoneStato = 9; //pin dello switch per l'accensione/spegnimento del dispositivo 

//definizione costanti per la simulazione del mouse (2 click+movimento)
const int TieniPremuto=8; //pin dello switch per tenere premuto il pulsante del mouse quando è attivo
const int ClickSN = 7; //pin del pulsante che simula il tasto sinistro del mouse
const int ClickDX = 6; //pin del pulsante che simula il tasto destro del mouse
const int JoyX = 0; //pin analogico del joystick per il movimento in verticale, su A0
const int JoyY = 1; //pin analogico del joystick per il movimento in orizzontale, su A1

//definizione costanti per i tasti direzionali
const int TastoSu = 5; //tasto freccia su
const int TastoGiu = 4; //tasto freccia giù
const int TastoSN = 3; //tasto freccia sinistra
const int TastoDX = 2; //tasto freccia destra

//setup
void setup() {
  //stato I/O del dispositivo
  pinMode(LedON, OUTPUT); //led
  pinMode(BottoneStato, INPUT); //switch on/off
  
  //mouse
  pinMode(ClickSN, INPUT); //click sinistro
  pinMode(ClickDX, INPUT); //click destro
  
  
  //joystick
  pinMode(JoyX, INPUT); // asse x per A0 (input analogico per joystick)
  pinMode(JoyY, INPUT); // asse y per A1 (input analogico per joystick)
  Mouse.begin(); //inizializzazione del mouse
  
  //tastiera
  pinMode(TastoSu, INPUT); 
  pinMode(TastoGiu, INPUT);
  pinMode(TastoSN, INPUT);
  pinMode(TastoDX, INPUT);
    
       
}

//loop
void loop() {
  while (digitalRead(BottoneStato) == HIGH) {  //quando lo switch è attivo, fai quello che segue
        
    digitalWrite(LedON, HIGH); //accendi led
    
    int x_val = readJoystick(JoyX); //leggi i valori di input del joystick
    int y_val = readJoystick(JoyY);
    
    Mouse.move(x_val, y_val, 0); //muovi il mouse seguendo i valori letti dall'input del joystick (A0, A1)

    //controllo dello stato dei tasti dx e sn del mouse (se sono cliccati o meno)
    GestioneClickMouse(ClickSN, MOUSE_LEFT);
    GestioneClickMouse(ClickDX, MOUSE_RIGHT);
    delay(10);  //ritardo dello spostamento del cursore del mouse, in millisecondi
    
  //Tasti freccia
  if (digitalRead(TastoSu) == LOW) {  //tasto freccia su
    Keyboard.write(KEY_UP_ARROW);
  }
  if (digitalRead(TastoGiu) == LOW) { //tasto freccia giu
    Keyboard.write(KEY_DOWN_ARROW);
  }
  if (digitalRead(TastoSN) == LOW) {  //tasto freccia sinistra
    Keyboard.write(KEY_LEFT_ARROW);
  }
  if (digitalRead(TastoDX) == LOW) {  //tasto freccia destra
    Keyboard.write(KEY_RIGHT_ARROW);
  }

  } 
  digitalWrite(LedON, LOW); //Spegni il led se il dispositivo non è attivo (switch su off)
}

//Joystick
//funzione che legge ogni asse del joystick e li configura con dei valori utilizzabili (calibrazione)
int readJoystick(int axis){
  float value = map(analogRead(axis), 0, 1023, -10, 10);  //leggi gli assi e mappali da -10 a 10
  if (value <= 1 && value >= -1){ //condizione per sistemare la reattività del joystick in caso di piccoli movimenti involontar 
    return 0;                                               
  }
  else {
    return value; //restituisci il valore delle coordinate del joystick
  }
}

//Funzione per vedere quando è cliccato un pulsante
void GestioneClickMouse(int pin, char comando){
  if (digitalRead(pin) == LOW){ //Se i pulsanti sono nello stato di LOW (non premuti)
    if (!Mouse.isPressed(comando)){ //se i pulsanti (simulazione mouse) non sono premuti
      Mouse.press(comando); //esegui il click
    }
  }
  else if(!digitalRead(TieniPremuto) == HIGH){ //se lo switch per tenere lo stato dei tasti attivo non è su HIGH (non premuto)
    if (Mouse.isPressed(comando)){  //se i pulsanti sono premuti
      Mouse.release(comando); //rilascia il click
    }
  }
}
