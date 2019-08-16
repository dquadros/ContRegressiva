// Construtor
MAX7219::MAX7219(int pLoad, int pClk, int pDin) {
  pinLoad = pLoad;
  pinClk = pClk;
  pinDin = pDin;
}

// Inicia o MAX7219
void MAX7219::init (uint8_t lim) {
  pinMode (pinLoad, OUTPUT);  
  pinMode (pinDin, OUTPUT);  
  pinMode (pinClk, OUTPUT);  
  digitalWrite (pinLoad, HIGH);  
  digitalWrite (pinClk, LOW);  
  digitalWrite (pinDin, LOW);  
    
  write (MAX7219_SHUT, 0x00);  
  write (MAX7219_TEST, 0x00);  
  write (MAX7219_MODE, 0x00);  
  write (MAX7219_INT, 0x0F);  
  write (MAX7219_LIM, lim);  
  write (MAX7219_DIG0, 0);  
  write (MAX7219_DIG1, 0);  
  write (MAX7219_DIG2, 0);  
  write (MAX7219_DIG3, 0);  
  write (MAX7219_DIG4, 0);  
  write (MAX7219_DIG5, 0);  
  write (MAX7219_DIG6, 0);  
  write (MAX7219_DIG7, 0);  
  write (MAX7219_SHUT, 0x01);  
}

// Escreve em um registrador do MAX7219  
void MAX7219::write (uint8_t addr, uint8_t data)  
{  
  digitalWrite (pinLoad, LOW);  
  shiftOut (pinDin, pinClk, MSBFIRST, addr);  
  shiftOut (pinDin, pinClk, MSBFIRST, data);  
  digitalWrite (pinLoad, HIGH);  
}
