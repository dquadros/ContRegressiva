// Classe para controlar o MAX7219
class MAX7219 {
  private:
    // Conexões
    int pinLoad;
    int pinClk;
    int pinDin;

  public:

    // Registradores do MAX7219
    static const uint8_t MAX7219_NOP  = 0x00;
    static const uint8_t MAX7219_DIG0 = 0x01;
    static const uint8_t MAX7219_DIG1 = 0x02;
    static const uint8_t MAX7219_DIG2 = 0x03;
    static const uint8_t MAX7219_DIG3 = 0x04;
    static const uint8_t MAX7219_DIG4 = 0x05;
    static const uint8_t MAX7219_DIG5 = 0x06;
    static const uint8_t MAX7219_DIG6 = 0x07;
    static const uint8_t MAX7219_DIG7 = 0x08;
    static const uint8_t MAX7219_MODE = 0x09;
    static const uint8_t MAX7219_INT  = 0x0A;
    static const uint8_t MAX7219_LIM  = 0x0B;
    static const uint8_t MAX7219_SHUT = 0x0C;
    static const uint8_t MAX7219_TEST = 0x0F;

    // Construtor - recebe as conexões
    MAX7219 (int pLoad, int pClk, int pDin);
  
    // Iniciação
    // lim é o número de "dígitos" - 1
    void init (uint8_t lim);
    
    // Escreve num registrador
    void write (uint8_t addr, uint8_t data);
};
