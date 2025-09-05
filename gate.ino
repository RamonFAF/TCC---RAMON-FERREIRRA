/*
 * CONTROLE DE FASE — ESP8266
 * Ramon Ferreira de Andrade Feitosa · TCC Eng. Elétrica · IFPE Pesqueira
 * Zero-cross  → D7 (GPIO13)
 * Gate TRIAC  → D6 (GPIO12) 
 * PotenciômetroB10k entre 3V3-A0-GND
 */

#include <Ticker.h>

const uint8_t  ZC_PIN   = 13;     // D7 - Passagem por Zero 
const uint8_t  GATE_PIN = 12;    // D6 - GATE
const uint8_t  POT_PIN  = A0;    // 0–1023

const uint32_t GATE_PULSE_US = 1000;   // pulso de gate em µs
const uint16_t SAFETY_US     = 10;    // folga mínima garante que, mesmo no fim, o pulso ainda cai antes do próximo zero;

Ticker gateTicker, gateOff;

volatile unsigned long tRise = 0, period = 0;
volatile bool sampleReady = false;

ICACHE_RAM_ATTR void zcISR() {
  unsigned long now = micros();
  if (digitalRead(ZC_PIN)) {
    period = now - tRise;
    tRise = now;
    sampleReady = true;
  }
}
void fireGate() {
  digitalWrite(GATE_PIN, HIGH);
  gateOff.once(GATE_PULSE_US / 1e6f, [] {
    digitalWrite(GATE_PIN, LOW);
    ESP.wdtFeed();
  });}

void setup() {  
  pinMode(ZC_PIN, INPUT_PULLUP);
  pinMode(GATE_PIN, OUTPUT);
  digitalWrite(GATE_PIN, LOW);

  attachInterrupt(digitalPinToInterrupt(ZC_PIN), zcISR, CHANGE);

  Serial.begin(115200);
  Serial.println(F("\nControle de fase — inicializado"));}

void loop() {
  static bool gateScheduled = false;
  if (sampleReady) {
    noInterrupts();
    unsigned long p = period;
    sampleReady = false;
    interrupts();

    uint16_t pot     = analogRead(POT_PIN);
    uint32_t maxDelay = (p > SAFETY_US) ? (p - SAFETY_US) : (p / 2);
    uint32_t delayUs  = (uint32_t)pot * maxDelay / 1023;
    if (delayUs < 10) delayUs = 10;

    // PRINTS de debug/calculo de ângulo
    Serial.print(F("Per="));      Serial.print(p);
    Serial.print(F(" us  pot=")); Serial.print(pot);
    Serial.print(F("  atraso=")); Serial.print(delayUs);
    Serial.print(F(" us  Ângulo="));
    Serial.println(delayUs * 180.0f / maxDelay, 1);

    if (!gateScheduled) {
      gateScheduled = true;
      gateTicker.once(delayUs / 1e6f, [&gateScheduled]() {
        fireGate();
        gateScheduled = false;
      });
    }
  }
}
