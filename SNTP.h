// Pacote do NTP
typedef struct {
  uint8_t ctrl;       // LI/VN/Mode
  uint8_t stratum;
  uint8_t poll;
  uint8_t precision;
  uint8_t rootDelay[4];
  uint8_t rootDispersion[4];
  uint8_t refIdent[4];
  uint8_t refTimestamp[8];
  uint8_t orgTimestamp[8];
  uint8_t recTimestamp[8];
  uint8_t txmTimestamp[8];
} PKT_NTP;

// Classe que implementa o SNTP
class SNTP {
  private:
    PKT_NTP pktTx;  // pacote enviado ao servidor
    PKT_NTP pktRx;  // pacore recebido do servidor

    // Controle dos tempos da tentativa
    const uint32_t MIN_TENTATIVA = 30000;
    const uint32_t MAX_TENTATIVA = 180000;
    uint32_t intervTentativa = MIN_TENTATIVA;
    uint32_t proxTentativa = 0;

    // Controle dos tempos de atualização
    uint32_t ultAtualizacao = 0;
    const uint32_t intervAtualizacao = 60000; // 1 minuto em milisegundos
    
    // Timestamp local
    uint32_t timestamp = 0; 

    // Ajuste para o fuso horário (UTC-3)
    const uint32_t epochUnix = 2208988800UL;
    int fusoHorario;
    
    // Servidor NTP
    const char* servidorNTP = "a.ntp.br"; 
    const int NTP_PORT = 123;
    const int LOCAL_PORT = 1234;

    // Acesso ao UPD
    WiFiUDP *udp;

    // métodos auxiliares
    void putUInt32 (uint8_t *p, uint32_t val);
    uint32_t getUInt32 (uint8_t *p);
    
  public:
    // Construtor
    SNTP (void);
  
    // Iniciação
    bool init (WiFiUDP *pUDP, int fuso);
  
    // Atualiza o timestamp local
    void update (void);
  
    // Retorna data e hora locais no formato Unix
    time_t time(void);
  
    // Retorna true se conseguiu obter data e hora do servidor
    bool valid(void);
};
