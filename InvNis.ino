float y = 0, e, u, Kp, Ki, Kd, P, I = 0, D, eprev=0, dt;
volatile bool A;
volatile bool B;
volatile float counter = 0;
void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(3),  ISRA, RISING);
  pinMode(3, INPUT);//enc A
  pinMode(4, INPUT);//enc B
  pinMode(6, OUTPUT);//motor PWM
  pinMode(7, OUTPUT);//motor rotA
  pinMode(8, OUTPUT);//motor rotB
}

void loop() {
  //input scaling 0.3515625
  e = 360.0/1024.0 * counter;
  
  //PID constants:
  Kp = 20;
  Ki = 15;
  Kd = 2;
  dt = 10.0;

  //PID values
  P = Kp*e;
  I += Ki*e*dt*0.001;
  D = Kd*(e - eprev)/(dt*0.001); 

  //I component saturation
  if (I > 80)
  {
    I = 80;
  }
  if (I < -80)
  {
    I = -80;
  }


  //control signal
  float u = P + I + D;

  //control signal saturation
  if (u > 255)
  {
    u = 255;
  }
  if (u<-255)
  {
    u = -255;
  }

  //output to motor:
  if (u<0)
  {
    analogWrite(6, -u);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    
  };
  if (u>=0)
  {
    analogWrite(6, u);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
   
  };

  eprev = e; //error time diff

  delay(dt);
  Serial.println(e); //print

}

void ISRA() {
  if (!digitalRead(4))
  {
    counter ++;
  }else
  {
    counter --;
  }
}

