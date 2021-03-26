

class A_5880E {
  private:
  uint8_t shift_v = 0;
  public:
  //uint8_t matrix[2][8] ={0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0,  0, 0xFF, 0, 0xFF, 0, 0xFF, 0, 0xFF};
  uint8_t matrix[2][8] ={0};


/*
 *                                    DISPLAY                                              SHADOW
 *
 *  PD4[4](9)      X     X     X     X     X     X     X     X          X     X     X     X     X     X     X     X
 *
 *  PD7[7](14)     X     X     X     X     X     X     X     X          X     X     X     X     X     X     X     X
 *
 *  PD2[2](8)      X     X     X     X     X     X     X     X          X     X     X     X     X     X     X     X
 *
 *  PD6[6](12)     X     X     X     X     X     X     X     X          X     X     X     X     X     X     X     X
 *
 *  PB1[9](1)      X     X     X     X     X     X     X     X          X     X     X     X     X     X     X     X
 *
 *  PD3[3](7)      X     X     X     X     X     X     X     X          X     X     X     X     X     X     X     X
 *
 *  PB0[8](2)      X     X     X     X     X     X     X     X          X     X     X     X     X     X     X     X
 *
 *  PD5[5](5)      X     X     X     X     X     X     X     X          X     X     X     X     X     X     X     X
 *
 *                PC0   PB3   PB2   PC3   PC1   PC2   PB4   PB5
 *               [A0]   [11]  [10]  [A3]  [A1]  [A2]  [12]  [13]
 *               (13)   (3)   (4)   (10)  (6)   (11)  (15)  (16)
 *
 *
 */

  public:
    A_5880E();
    void column(uint8_t *data_ptr);
    void row(byte number, byte state);
    void dispProcess();
    void loadDispMem(uint8_t *data);
    void loadShadMem(uint8_t *data);
    void shiftL();
    void shiftR();
    void shiftU();
    void shiftD();
} dot8x8;








void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
  //A_5880E dot8x8;
  dot8x8.loadDispMem((const uint8_t[]){
    0b10000000,
    0b01000000,
    0b00100000,
    0b00010000,
    0b00001000,
    0b00000100,
    0b00000010,
    0b00000001
    });

    dot8x8.loadShadMem((const uint8_t[]){
    0b00000001,
    0b00000010,
    0b00000100,
    0b00001000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b10000000
    });

    
// uint8_t i;
// 
// for(i=0;i<8;i++)
// {
//      Serial.print("matrix= ") ;
//      Serial.print(dot8x8.matrix[0][i]) ;
//      Serial.print("\n") ;
// }
// for(i=0;i<8;i++)
// {
//      Serial.print("matrix= ") ;
//      Serial.print(dot8x8.matrix[1][i]) ;
//      Serial.print("\n") ;
// }

}



void loop() {
  // put your main code here, to run repeatedly:
  uint8_t inByte;
  static unsigned long shift_period = millis();
  static bool left=0, right=0, up=0, down=0;
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    
switch (inByte)
  {
    case 'l':
      dot8x8.shiftL();
      break;
    case 'r':
      dot8x8.shiftR();
      break;
    case 'u':
      dot8x8.shiftU();
      break;
    case 'd':
      dot8x8.shiftD();
      break;
    case 'L':
      left ^= 1UL;
      break;
    case 'R':
      right ^= 1UL;
      break;
    case 'U':
      up ^= 1UL;
      break;
    case 'D':
      down ^= 1UL;
      break;
    case 'S':
      left = 0;
      right = 0;
      up = 0;
      down = 0;
      break;
    default:
      break;
  }
 }

  

  if(millis() - shift_period > 1000)
  {
    if(left) dot8x8.shiftL();
    if(right) dot8x8.shiftR();
    if(up) dot8x8.shiftU();
    if(down) dot8x8.shiftD();
    
    shift_period = millis();
  }
  dot8x8.dispProcess();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





A_5880E::A_5880E()
{
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  //pinMode(A4, OUTPUT);
  pinMode(A5, INPUT_PULLUP);
}
void A_5880E::column(uint8_t *data_ptr)
{
  PORTC &= 0b11110000;
  PORTC |= (((*data_ptr & 0b10000000)>>7) | ((*data_ptr & 0b00010000)>>1) | ((*data_ptr & 0b00001000)>>2) | (*data_ptr & 0b00000100));
  PORTB &= 0b11000011;
  PORTB |= (((*data_ptr & 0b01000000)>>3) | ((*data_ptr & 0b00100000)>>3) | ((*data_ptr & 0b00000010)<<3) | ((*data_ptr & 0b00000001)<<5));

}
void A_5880E::row(byte number, byte state)
{
  switch (((state > 0)? 0:1)<<3 |number)
  {
    case 0:
      PIND = 0b00010000;
      break;
    case 1:
      PIND = 0b10000000;
      break;
    case 2:
      PIND = 0b00000100;
      break;
    case 3:
      PIND = 0b01000000;
      break;
    case 4:
      PINB = 0b00000010;
      break;
    case 5:
      PIND = 0b00001000;
      break;
    case 6:
      PINB = 0b00000001;
      break;
    case 7:
      PIND = 0b00100000;
      break;
    default:
      PORTD |= 0b11111100;
      PORTB |= 0b00000011;
  }
}
void A_5880E::dispProcess()
{
  uint8_t i;
  uint16_t j;
  uint16_t potValue;

  for(i=0;i<8;i++)
  {
    row(i, 0);
    potValue = analogRead(A5);
    //potValue >>= 5;
    //potValue *= potValue;

    j= 1023 - potValue;
    while(j--)
    {
      delayMicroseconds(2);
    }
    column((uint8_t *)matrix + (((i + this->shift_v) > 15)?(i + this->shift_v - 16):(i + this->shift_v)) );
    //column((uint8_t *)(this->matrix) + i );
    row(i, 1);
    j= potValue;
    while(j--)
    {
      delayMicroseconds(2);
    }
  }
}
void A_5880E::loadDispMem(uint8_t *data)
{
  uint8_t i;
  for(i=0; i<8; i++)
  {
    *((uint8_t *)matrix + i) =  *(data + i);
//
//    Serial.print("*(data + i) ") ;
//    Serial.print(*(data + i)) ;
//    Serial.print("\n") ;
  }
}
void A_5880E::loadShadMem(uint8_t *data)
{
  uint8_t i;
  for(i=0; i<8; i++)
  {
    *((uint8_t *)matrix + 8 + i) =  *(data + i);

    //Serial.begin(2000000);
//    Serial.print("*(data + i) ") ;
//    Serial.print(*(data + i)) ;
//    Serial.print("\n") ;
  }
}
void A_5880E::shiftL()
{
  uint8_t i;
  uint8_t bit0;
  uint8_t bit1;
  for(i=0; i<8; i++)
  {
    bit1 = (((this->matrix[1][i]) & 0x80) ? 1:0);
    bit0 = (((this->matrix[0][i]) & 0x80) ? 1:0);
    (this->matrix[1][i]) <<= 1;
    (this->matrix[0][i]) <<= 1;
    (this->matrix[1][i]) |= bit0;
    (this->matrix[0][i]) |= bit1;
  }

}
void A_5880E::shiftR()
{
  uint8_t i;
  uint8_t bit0;
  uint8_t bit1;
  for(i=0; i<8; i++)
  {
    bit1 = (((this->matrix[1][i]) & 0x01) ? 0x80:0x00);
    bit0 = (((this->matrix[0][i]) & 0x01) ? 0x80:0x00);
    (this->matrix[1][i]) >>= 1;
    (this->matrix[0][i]) >>= 1;
    (this->matrix[1][i]) |= bit0;
    (this->matrix[0][i]) |= bit1;
  }
}
void A_5880E::shiftU()
{
  this->shift_v++;
  if(this->shift_v > 15) this->shift_v = 0;
}
void A_5880E::shiftD()
{
  this->shift_v--;
  if(this->shift_v > 15) this->shift_v = 15;
}
