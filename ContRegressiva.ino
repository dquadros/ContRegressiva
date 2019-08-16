/*
 * Contagem Regressiva
 * 16/08/19 - Daniel Quadros
 * https://dqsoft.blogspot.com
 */

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <time.h>
#include "MAX7219.h"
#include "SNTP.h"

// Conexões ao MAX7219  
#define PIN_LOAD 0
#define PIN_DIN  2  
#define PIN_CLK  4  

#define LED_SEMANAS MAX7219::MAX7219_DIG0
#define LED_DIAS    MAX7219::MAX7219_DIG1

// Configurações do WiFi
char *ssid   = "MinhaRede";
char *passwd = "(senha)";

MAX7219 max7219(PIN_LOAD, PIN_CLK, PIN_DIN);
WiFiUDP wifiudp;
SNTP sntp;

time_t evento;

// Iniciação  
void setup ()  
{
  Serial.begin (115200);
  Serial.println();
  max7219.init(1);
  WiFi.begin(ssid, passwd);
  sntp.init (&wifiudp, -10800);
  
  // Define a data do evento que estamos aguardando
  struct tm tmEvento;
  tmEvento.tm_year = 2019 - 1900;
  tmEvento.tm_mon = 9 - 1;
  tmEvento.tm_mday = 7;
  tmEvento.tm_hour = 23;
  tmEvento.tm_min = 0;
  tmEvento.tm_sec = 0;
  evento = mktime(&tmEvento);
}  
  
// Loop principal  
void loop ()  
{
  static int dias_ant = -1;
  static int semanas_ant = -1;
  
  if (WiFi.status() == WL_CONNECTED) {
    // Manter data e hora atualizados
    sntp.update();
  }
  if (sntp.valid()) {
    // Calcula quantas semanas e dias faltam
    int dias = 0;
    int semanas = 0;
    time_t agora = sntp.time();
    if (agora < evento) {
      uint32_t falta = evento - agora;
      falta = falta / (24UL * 60L * 60L);
      semanas = (int)(falta / 7UL);
      if (semanas > 8) {
        semanas = 8;
      }
      dias = (int) (falta % 7UL);
    }
    if ((semanas != semanas_ant) || (dias != dias_ant)) {
      // Atualiza o display
      semanas_ant = semanas;
      dias_ant = dias;
      uint8_t mask = 0;
      for (int i = 0; i < semanas; i++) {
        mask = (mask << 1) | 1;
      }
      max7219.write (LED_SEMANAS, mask);
      mask = 0;
      for (int i = 0; i < dias; i++) {
        mask = (mask << 1) | 1;
      }
      max7219.write (LED_DIAS, mask);
    }
  }
  delay (100);
}
 
