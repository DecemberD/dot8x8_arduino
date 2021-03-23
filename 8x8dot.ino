
class A_5880E {  
  uint8_t shift_h = 0;
  uint8_t shift_v = 0;    
  uint8_t matrix[8][2] ={0xFF, 0, 0, 0xFF, 0xFF, 0, 0, 0xFF, 0xFF, 0, 0, 0xFF, 0xFF, 0, 0, 0xFF};
  

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
    void A_5880E();           
    void column(uint8_t **data_ptr, uint8_t shift_h);
    void row(byte number, byte state);
    void dispProcess();
    void loadDispMem();
    void loadShadMem();
    void shiftL();
    void shiftR();
    void shiftU();
    void shiftD();
} dot8x8;





 
void A_5880E::A_5880E()
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
void A_5880E::column(uint8_t **data_ptr, uint8_t shift_h)
{
  uint16_t data = *(uint16_t *)data_ptr;
  Serial.print("data = ") ;
  Serial.print(data, HEX) ;
  Serial.print("\n") ;
  data >>= shift_h;
  PORTC &= 0b11110000;
  PORTC |= (uint8_t)(((data & 0b10000000)>>7) | ((data & 0b00010000)>>1) | ((data & 0b00001000)>>2) | (data & 0b00000100));
  PORTB &= 0b11000011;
  PORTB |= (uint8_t)(((data & 0b01000000)>>3) | ((data & 0b00100000)>>3) | ((data & 0b00000010)<<3) | ((data & 0b00000001)<<5));
 
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
  static unsigned long shift_period = millis();
  
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
    column((uint8_t **)matrix + (i + shift_v), shift_h);
    row(i, 1);
    j= potValue;
    while(j--)
    {
      delayMicroseconds(2);
    }
  }
  if(millis() - shift_period > 1000)
  {
    shift_h++;
    if(shift_h >8) shift_h = 0;
    shift_period = millis();
  }
  Serial.print("shift_h = ") ;
  Serial.print(shift_h) ;
  Serial.print("\n") ;
//  if(millis() - shift_period > 1000)
//  {
//    shift_v++;
//    if(shift_v >7) shift_v = 0;
//    shift_period = millis();
//  }
//  Serial.print("shift_v = ") ;
//  Serial.print(shift_v) ;
//  Serial.print("\n") ;
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  dot8x8.dispProcess();
}

