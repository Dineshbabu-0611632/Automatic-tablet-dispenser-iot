
Arduino code
#include <SPI.h>
#include <MFRC522.h>
#include<LiquidCrystal_I2C.h>
#include<Wire.h>
#include <Servo.h>
int led = 13;
int ir = 3;
int ir2=4;
int ir3=5;
int ir4=6;
int voice=22;
int spk = 8;
#include<Servo.h>
int servopin1=11;
Servo myservo;
int servopin2=23;
Servo myservo2;
int pulsePin = A0; // Pulse Sensor purple wire connected to analog pin A0
int blinkPin = 12; // pin to blink led at each beat
// Volatile Variables, used in the interrupt service routine!
volatile int BPM; // int that holds raw Analog in 0. updated every 2mS
volatile int Signal; // holds the incoming raw data
volatile int IBI = 600; // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse = false; // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS = false; // becomes true when Arduoino finds a beat.
static boolean serialVisual = true; // Set to 'false' by Default. Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse
volatile int rate[10]; // array to hold last ten IBI values
volatile unsigned long sampleCounter = 0; // used to determine pulse timing
volatile unsigned long lastBeatTime = 0; // used to find IBI
volatile int P = 512; // used to find peak in pulse wave, seeded
volatile int T = 512; // used to find trough in pulse wave, seeded
volatile int thresh = 525; // used to find instant moment of heart beat, seeded
volatile int amp = 100; // used to hold amplitude of pulse waveform, seeded
volatile boolean firstBeat = true; // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat = false; // used to seed rate array so we startup with reasonable BPM
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define SS_PIN 9
#define RST_PIN 8
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
//Servo myServo; //define servo name
void setup()
{
//Serial.begin(9600); // Initiate a serial communication
SPI.begin(); // Initiate SPI bus
mfrc522.PCD_Init(); // Initiate MFRC522
lcd.init();
lcd.backlight();
myservo.attach(servopin1);
myservo2.attach(servopin2);
myservo.write(85);
myservo2.write(0); //servo start position
pinMode(blinkPin,OUTPUT); // pin that will blink to your heartbeat!
Serial.begin(115200); // we agree to talk fast!
interruptSetup();
pinMode(spk, OUTPUT);
Serial.println("Put your card to the reader...");
Serial.println();
lcd.clear();
lcd.setCursor(0,0);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("1-Fever 2-cold");
lcd.setCursor(0,1);
lcd.print("3-cough 4-pain");
delay(1000);
pinMode(led,OUTPUT);
pinMode(ir,INPUT);
pinMode(ir2,INPUT);
pinMode(ir3,INPUT);
pinMode(ir4,INPUT);
pinMode(voice,OUTPUT);
}
void loop()
{
digitalWrite(spk,LOW);
//myservo.write(50);
serialOutput();
if(digitalRead(ir))
{
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" (1- FEVER)");
lcd.setCursor(0,0);
delay(3000);
lcd.clear();
lcd.print("hold-1 & 2-HIGH");
lcd.setCursor(0,1);
lcd.print("3-med 4-low");
delay(1000);
if(digitalRead(ir2))
{
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" HIGH");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("............");
delay(3000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("please cheak the");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("Heart rate");
delay(5000);
serialOutputWhenBeatHappens();
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(voice,LOW);
delay(5000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" Take the ");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("tablet");
delay(1000);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
myservo.write(50);
delay(3000);
myservo.write(85);
}
if(digitalRead(ir3))
{
lcd.clear();
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
lcd.setCursor(0,0);
lcd.print("med");
//delay(3000);
lcd.setCursor(0,1);
lcd.print(".....");
delay(3000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("please cheak the");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("Heart rate");
delay(5000);
serialOutputWhenBeatHappens();
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(voice,LOW);
delay(5000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" Take the ");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("tablet");
delay(1000);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
myservo.write(50);
delay(3000);
myservo.write(85);
}
if(digitalRead(ir4))
{
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("low");
//delay(3000);
lcd.setCursor(0,1);
lcd.print(".....");
delay(3000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("please cheak the");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("Heart rate");
delay(5000);
serialOutputWhenBeatHappens();
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(voice,LOW);
delay(5000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" Take the ");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("tablet");
delay(1000);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
myservo.write(50);
delay(3000);
myservo.write(85);
}
}
if(digitalRead(ir2))
{ digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" (2-COLD)");
lcd.setCursor(0,0);
delay(3000);
lcd.clear();
lcd.print("hold-2 & 1-HIGH");
lcd.setCursor(0,1);
lcd.print("3-med 4-low");
delay(1000);
if(digitalRead(ir))
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" HIGH");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("............");
delay(3000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("please cheak the");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("Heart rate");
delay(5000);
serialOutputWhenBeatHappens();
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(voice,LOW);
delay(5000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" Take the ");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("tablet");
delay(1000);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
myservo.write(115);
delay(3000);
myservo.write(85);
}
if(digitalRead(ir3))
{ digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("med");
//delay(3000);
lcd.setCursor(0,1);
lcd.print(".....");
delay(3000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("please cheak the");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("Heart rate");
delay(5000);
serialOutputWhenBeatHappens();
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(voice,LOW);
delay(5000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" Take the ");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("tablet");
delay(1000);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
myservo.write(115);
delay(3000);
myservo.write(85);
}
if(digitalRead(ir4))
{ digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("low");
//delay(3000);
lcd.setCursor(0,1);
lcd.print(".....");
delay(3000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("please cheak the");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("Heart rate");
delay(5000);
serialOutputWhenBeatHappens();
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(voice,LOW);
delay(5000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" Take the ");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("tablet");
delay(1000);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
myservo.write(115);
delay(3000);
myservo.write(85);
}
}
if(digitalRead(ir3))
{ digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" (3-COUGH)");
lcd.setCursor(0,0);
delay(3000);
lcd.clear();
lcd.print("hold-3 & 1-HIGH");
lcd.setCursor(0,1);
lcd.print("2-med 4-low");
delay(1000);
if(digitalRead(ir))
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" HIGH");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("............");
delay(3000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("please cheak the");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("Heart rate");
delay(5000);
serialOutputWhenBeatHappens();
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(voice,LOW);
delay(5000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" Take the ");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("tablet");
delay(1000);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
myservo.write(50);
delay(3000);
myservo.write(85);
}
if(digitalRead(ir2))
{ digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("med");
//delay(3000);
lcd.setCursor(0,1);
lcd.print(".....");
delay(3000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("please cheak the");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("Heart reat");
delay(5000);
serialOutputWhenBeatHappens();
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(voice,LOW);
delay(5000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" Take the ");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("tablet");
delay(1000);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
myservo.write(50);
delay(3000);
myservo.write(85);
}
if(digitalRead(ir4))
{ digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("low");
//delay(3000);
lcd.setCursor(0,1);
lcd.print(".....");
delay(3000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("please cheak the");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("Heart rate");
delay(5000);
serialOutputWhenBeatHappens();
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(voice,LOW);
delay(5000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" Take the ");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("tablet");
delay(1000);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
myservo.write(50);
delay(3000);
myservo.write(85);
}
}
if(digitalRead(ir4))
{ digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" (4-PAIN)");
lcd.setCursor(0,0);
delay(3000);
lcd.clear();
lcd.print("hold-4 & 1-HIGH");
lcd.setCursor(0,1);
lcd.print("2-med 3-low");
delay(1000);
if(digitalRead(ir))
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" HIGH");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("............");
delay(3000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("please cheak the");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("Heart rate");
delay(5000);
serialOutputWhenBeatHappens();
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(voice,LOW);
delay(5000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" Take the ");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("tablet");
delay(1000);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
myservo.write(180);
delay(3000);
myservo.write(85);
}
if(digitalRead(ir2))
{ digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("med");
//delay(3000);
lcd.setCursor(0,1);
lcd.print(".....");
delay(3000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("please cheak the");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("Heart reat");
delay(5000);
serialOutputWhenBeatHappens();
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(voice,LOW);
delay(5000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" Take the ");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("tablet");
delay(1000);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
myservo.write(180);
delay(3000);
myservo.write(85);
}
if(digitalRead(ir3))
{ digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("low");
//delay(3000);
lcd.setCursor(0,1);
lcd.print(".....");
delay(3000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("please cheak the");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("Heart rate");
delay(5000);
serialOutputWhenBeatHappens();
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(led,HIGH);
delay(100);
digitalWrite(led,LOW);
delay(100);
digitalWrite(voice,LOW);
delay(5000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(" Take the ");
//delay(3000);
lcd.setCursor(0,1);
lcd.print("tablet");
delay(1000);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
digitalWrite(spk,HIGH);
delay(100);
digitalWrite(spk,LOW);
delay(100);
myservo.write(180);
delay(3000);
myservo.write(85);
}
}
// Look for new cards
if ( ! mfrc522.PICC_IsNewCardPresent())
{
return;
}
// Select one of the cards
if ( ! mfrc522.PICC_ReadCardSerial())
{
return;
}
//Show UID on serial monitor
Serial.print("UID tag :");
String content= "";
byte letter;
for (byte i = 0; i < mfrc522.uid.size; i++)
{
Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
Serial.print(mfrc522.uid.uidByte[i], HEX);
content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
content.concat(String(mfrc522.uid.uidByte[i], HEX));
}
Serial.println();
Serial.print("Message : ");
content.toUpperCase();
if (content.substring(1) == "B6 91 3A 57") //change here the UID of the card/cards that you want to give access
{
Serial.println("Authorized access");
Serial.println();
//delay(500);
myservo2.write(90);
delay(2000);
myservo2.write(0);
delay(1000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Hi Mathan");
lcd.setCursor(0,1);
lcd.print("Raj");
delay(3000);
// delay(30*1000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("1-Fever 2-cold");
lcd.setCursor(0,1);
lcd.print("3-cough 4-pain");
delay(1000);
// digitalWrite(LED_G, LOW);
}
if (content.substring(1) == "83 55 44 1C") //change here the UID of the card/cards that you want to give access
{
myservo2.write(90);
delay(2000);
myservo2.write(0);
delay(1000);
Serial.println("Authorized access");
Serial.println();
delay(500);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Hi sethu");
lcd.setCursor(0,1);
lcd.print("Raman");
delay(3000);
// delay(30*1000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("1-Fever 2-cold");
lcd.setCursor(0,1);
lcd.print("3-cough 4-pain");
delay(3000);
}
else {
Serial.println(" Access denied");
}
}
void interruptSetup()
{
// Initializes Timer2 to throw an interrupt every 2mS.
TCCR2A = 0x02; // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
TCCR2B = 0x06; // DON'T FORCE COMPARE, 256 PRESCALER
OCR2A = 0X7C; // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
TIMSK2 = 0x02; // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
sei(); // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED
}
void serialOutput()
{ // Decide How To Output Serial.
if (serialVisual == true)
{
arduinoSerialMonitorVisual('-', Signal); // goes to function that makes Serial Monitor Visualizer
}
else
{
sendDataToSerial('S', Signal); // goes to sendDataToSerial function
}
}
void serialOutputWhenBeatHappens()
{
if (serialVisual == true) // Code to Make the Serial Monitor Visualizer Work
{
Serial.print(" Heart Beat Rate "); //ASCII Art Madness
Serial.print("BPM: ");
Serial.println(BPM);
lcd.clear();
delay(3000);
lcd.print("Heart Beat Rate ");
lcd.setCursor(1,1);
lcd.print("BPM: ");
lcd.setCursor(5,1);
lcd.print(BPM);
delay(3000);
lcd.clear();
}
else
{
sendDataToSerial('B',BPM); // send heart rate with a 'B' prefix
sendDataToSerial('Q',IBI); // send time between beats with a 'Q' prefix
}
}
void arduinoSerialMonitorVisual(char symbol, int data )
{
const int sensorMin = 0; // sensor minimum, discovered through experiment
const int sensorMax = 1024; // sensor maximum, discovered through experiment
int sensorReading = data; // map the sensor range to a range of 12 options:
int range = map(sensorReading, sensorMin, sensorMax, 0, 11);
// do something different depending on the
// range value:
}
void sendDataToSerial(char symbol, int data )
{
Serial.print(symbol);
Serial.println(data);
}
ISR(TIMER2_COMPA_vect) //triggered when Timer2 counts to 124
{
cli(); // disable interrupts while we do this
Signal = analogRead(pulsePin); // read the Pulse Sensor
sampleCounter += 2; // keep track of the time in mS with this variable
int N = sampleCounter - lastBeatTime; // monitor the time since the last beat to avoid noise
// find the peak and trough of the pulse wave
if(Signal < thresh && N > (IBI/5)*3) // avoid dichrotic noise by waiting 3/5 of last IBI
{
if (Signal < T) // T is the trough
{
T = Signal; // keep track of lowest point in pulse wave
}
}
if(Signal > thresh && Signal > P)
{ // thresh condition helps avoid noise
P = Signal; // P is the peak
} // keep track of highest point in pulse wave
// NOW IT'S TIME TO LOOK FOR THE HEART BEAT
// signal surges up in value every time there is a pulse
if (N > 250)
{ // avoid high frequency noise
if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) )
{
Pulse = true; // set the Pulse flag when we think there is a pulse
digitalWrite(blinkPin,HIGH); // turn on pin 13 LED
IBI = sampleCounter - lastBeatTime; // measure time between beats in mS
lastBeatTime = sampleCounter; // keep track of time for next pulse
if(secondBeat)
{ // if this is the second beat, if secondBeat == TRUE
secondBeat = false; // clear secondBeat flag
for(int i=0; i<=9; i++) // seed the running total to get a realisitic BPM at startup
{
rate[i] = IBI;
}
}
if(firstBeat) // if it's the first time we found a beat, if firstBeat == TRUE
{
firstBeat = false; // clear firstBeat flag
secondBeat = true; // set the second beat flag
sei(); // enable interrupts again
return; // IBI value is unreliable so discard it
}
// keep a running total of the last 10 IBI values
word runningTotal = 0; // clear the runningTotal variable
for(int i=0; i<=8; i++)
{ // shift data in the rate array
rate[i] = rate[i+1]; // and drop the oldest IBI value
runningTotal += rate[i]; // add up the 9 oldest IBI values
}
rate[9] = IBI; // add the latest IBI to the rate array
runningTotal += rate[9]; // add the latest IBI to runningTotal
runningTotal /= 10; // average the last 10 IBI values
BPM = 60000/runningTotal; // how many beats can fit into a minute? that's BPM!
QS = true; // set Quantified Self flag
// QS FLAG IS NOT CLEARED INSIDE THIS ISR
}
}
if (Signal < thresh && Pulse == true)
{ // when the values are going down, the beat is over
digitalWrite(blinkPin,LOW); // turn off pin 13 LED
Pulse = false; // reset the Pulse flag so we can do it again
amp = P - T; // get amplitude of the pulse wave
thresh = amp/2 + T; // set thresh at 50% of the amplitude
P = thresh; // reset these for next time
T = thresh;
}
if (N > 2500)
{ // if 2.5 seconds go by without a beat
thresh = 512; // set thresh default
P = 512; // set P default
T = 512; // set T default
lastBeatTime = sampleCounter; // bring the lastBeatTime up to date
firstBeat = true; // set these to avoid noise
secondBeat = false; // when we get the heartbeat back
}
sei(); // enable interrupts when youre done!
}// end isr