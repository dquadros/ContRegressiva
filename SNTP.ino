/*
 * Classe que implementa o SNTP
 * 16/08/19 - Daniel Quadros
 * https://dqsoft.blogspot.com
 */

// Construtor
SNTP::SNTP (void) {
}

// Iniciação do acesso ao servidor SNTP
bool SNTP::init (WiFiUDP *pUDP, int fuso) {
  udp = pUDP;
  fusoHorario = fuso;
  memset (&pktTx, 0, sizeof(pktTx));
  pktTx.ctrl = (4 << 3) | 3;  // Versão 4, Modo 3 (client)
  return udp->begin (LOCAL_PORT);
}

// Trata atualização periódica do timestamp local
void SNTP::update() {
  if (millis() > proxTentativa) {
    uint32_t tempoDesdeAtualizacao = (millis() - ultAtualizacao) / 1000UL;
    // Envia a solicitação
    putUInt32 (pktTx.txmTimestamp, timestamp+tempoDesdeAtualizacao);
    udp->beginPacket(servidorNTP, NTP_PORT);
    udp->write((uint8_t *)&pktTx, sizeof(pktTx));
    udp->endPacket();
    // Espera a resposta
    int timeout = 0;
    int cb = 0;
    do {
      delay ( 10 );
      cb = udp->parsePacket();
      if (timeout > 100) {
        proxTentativa = millis() + intervTentativa;
        if (intervTentativa < MAX_TENTATIVA) {
          intervTentativa += intervTentativa;
        }
        return; // timeout de um segundo
      }
      timeout++;
    } while (cb == 0);
    intervTentativa = MIN_TENTATIVA;
    // Le a resposta
    udp->read((uint8_t *) &pktRx, sizeof(pktRx));
    // Consistência básica
    if (((pktRx.ctrl & 0x3F) != ( (4 << 3) | 4)) ||
        ((pktRx.ctrl & 0xC0) == (3 << 6)) ||
        (pktRx.stratum == 0) ||
        (memcmp(pktRx.orgTimestamp, pktTx.txmTimestamp, 4) != 0)) {
      proxTentativa = millis() + intervTentativa;
      if (intervTentativa < MAX_TENTATIVA) {
        intervTentativa += intervTentativa;
      }
      return;
    }
    // Pega o resultado
    ultAtualizacao = millis();
    proxTentativa = ultAtualizacao + intervAtualizacao;
    timestamp = getUInt32 (pktRx.txmTimestamp);
  }
}

// Informa a hora local no formato Unix
time_t SNTP::time(void) {
    uint32_t tempoDesdeAtualizacao = millis() - ultAtualizacao;
    uint32_t tempoUTC = timestamp + tempoDesdeAtualizacao/1000;
    return (time_t) (tempoUTC - epochUnix + fusoHorario);
}

// Informa se tem data e hora validos
bool SNTP::valid(void) {
  return timestamp != 0;
}

// Rotinas para mover uint32_t de/para os pacotes
void SNTP::putUInt32 (uint8_t *p, uint32_t val) {
  p[0] = (uint8_t) ((val >> 24) & 0xFF);
  p[1] = (uint8_t) ((val >> 16) & 0xFF);
  p[2] = (uint8_t) ((val >> 8) & 0xFF);
  p[3] = (uint8_t) (val & 0xFF);
}
uint32_t SNTP::getUInt32 (uint8_t *p) {
  return (((uint32_t) p[0]) << 24) |
         (((uint32_t) p[1]) << 16) |
         (((uint32_t) p[2]) << 8) |
         ((uint32_t) p[3]);
}
