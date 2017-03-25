#include <Servo.h> 
#define tilt 18
#define side_tilt 5  //angle tilt of feet
#define mtilt 95  //angle rectification of feet so that it doesnot sweep ground
#define knee 10 
#define mknee 105 //angle tilt of knee
#define thigh 95  //angle thigh moves backward
#define mthigh 20 //angle thigh moves forward
#define relax 90  //initial or relax angle of all servo
#define sped 15 // the lesser the sped, bot will move that fast
#define distMin 10
#define trigPin_2 A2
#define echoPin_2 A3
#define trigPin_1 A1
#define echoPin_1 A0

#define interruptPin 16
#define rpi_r 14
#define rpi_l 15 

Servo lf,rf,lk,rk,lt,rt,lst,rst;
void left_feet(int a);
void left_knee(int a);
void left_side_thigh(int a);
void left_thigh(int a);
void right_feet(int a);
void right_knee(int a);
void right_side_thigh(int a);
void right_thigh(int a);
void right_knee_right_thigh(int a,int b);
void right_thigh_left_knee(int a,int b);
void left_thigh_right_knee(int a ,int b);
void left_knee_left_thigh(int a,int b);
void right_thigh_left_thigh(int a,int b);
boolean walkContinue=true;
int lfi=90,rfi=90,lki=90,rki=90,lti=90,rti=90,lsti=90,rsti=90;;//stores final position after movement
void straight()   //sets up the bot in straight position(initial position)
{

  lfi=lf.read();
rfi= rf.read();
 lki=lk.read();
 rki=rk.read();
lti= lt.read();
rti= rt.read();
 lsti=lst.read();
 rsti=rst.read();

// lf.write(90);
// rf.write(90);
// lk.write(90);
// rk.write(90);
// lt.write(90);
// rt.write(90);
// lst.write(90);
// rst.write(90);
right_feet(92);
 left_feet(85);
 right_knee(90);
 left_knee(83);
 right_side_thigh(95);
 left_side_thigh(98);
 right_thigh(97);
 left_thigh(97);
 //lfi=90,rfi=90,lki=90,rki=90,lti=90,rti=90;lsti=90;rsti=90;
 
 
}


void useRpi();
void useUltra();

void setup() 
{ 
 
 Serial.begin(9600);
 lf.attach(10);
 lk.attach(5);
 lst.attach(7);
 lt.attach(9);
 rf.attach(2);
 rk.attach(4);
 rst.attach(6);
 rt.attach(8);
  straight();
  delay(2000);
  pinMode(rpi_l,INPUT);
  pinMode(rpi_r,INPUT);
// attachInterrupt(digitalPinToInterrupt(interruptPin),useRpi,HIGH);
// attachInterrupt(digitalPinToInterrupt(interruptPin),useUltra,LOW);
} 

float get_left_dist(){
   float distance;
  long duration;
  pinMode(trigPin_1,OUTPUT);
  digitalWrite(trigPin_1,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_1, LOW);
  pinMode(echoPin_1,INPUT);
  duration=pulseIn(echoPin_1, HIGH);
  distance=(duration/2) / 29.1;
  Serial.print("left:" );
    Serial.println(distance);
    return distance;
}

boolean check_left_ultra(){//returns if obstacle is present->true
 
  if(get_left_dist()<=distMin){
    return true;
  }
  else
    return false;
}
float get_right_dist(){
  float distance;
  long duration;
  pinMode(trigPin_2,OUTPUT);
  digitalWrite(trigPin_2,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_2, LOW);
    pinMode(echoPin_2,INPUT);
  duration=pulseIn(echoPin_2, HIGH);
  distance=(duration/2) / 29.1;
    Serial.print("right:" );
    Serial.println(distance);
    return distance;
}
boolean check_right_ultra(){//returns if obstacle is present->true
  
  if(get_right_dist()<=distMin){
    return true;
  }
  else
    return false;
}


void bot_tilt_left(){
  right_feet(rfi-tilt);
  left_side_thigh(lsti-side_tilt);
  right_feet(rfi+2*side_tilt);
  left_feet(lfi+tilt);
}
void bot_tilt_right(){
//  Serial.print("Bot tilt right started");
  left_feet(lfi-tilt);
  right_side_thigh(rsti-side_tilt);
  left_feet(lfi+2*side_tilt);
  right_feet(rfi+tilt);
}
void motors_four(int slt,int srt,int fl,int fr){
  int i=slt>0?1:-1;
  int j=srt>0?1:-1;
  int k=fl>0?1:-1;
  int l=fr>0?1:-1;
  while(slt!=0&&fl!=0&&srt!=0&&fr!=0){
    lsti+=i;
    lst.write(lsti);
    rsti+=j;
    rst.write(rsti);
    lfi+=k;
    lf.write(lfi);
    rfi+=l;
    rf.write(rfi);
    slt-=i;
    srt-=j;
    fl-=k;
    fr-=l;
    delay(sped*2);
  }
  while(slt!=0){
    lsti+=i;
    lst.write(lsti);
    slt-=i;
    delay(sped*2);
  }
  while(srt!=0){
    rsti+=j;
    rst.write(rsti);
    srt-=j;
    delay(sped*2);
  }
  
  while(fl!=0){
    lfi+=k;
    lf.write(lfi);
    fl-=k;
    delay(sped*2);
  }
   while(fr!=0){
    rfi+=l;
    rf.write(rfi);
    fr-=l;
    delay(sped*2);
  }
  
}
void motors_four2(int slt,int srt,int fl,int fr){
  int i=slt>0?1:-1;
  int j=srt>0?1:-1;
  int k=fl>0?1:-1;
  int l=fr>0?1:-1;
  while(slt!=0&&fl!=0&&srt!=0&&fr!=0){
    lsti+=i;
    lst.write(lsti);
    rsti+=j;
    rst.write(rsti);
    lfi+=k;
    lf.write(lfi);
    rfi+=l;
    rf.write(rfi);
    slt-=i;
    srt-=j;
    fl-=k;
    fr-=l;
    delay(sped*2);
  }
  while(slt!=0){
    lsti+=i;
    lst.write(lsti);
    slt-=i;
    delay(sped*2);
  }
  while(srt!=0){
    rsti+=j;
    rst.write(rsti);
    srt-=j;
    delay(sped*2);
  }
   while(fr!=0){
    rfi+=l;
    rf.write(rfi);
    fr-=l;
    delay(sped*2);
  }
  while(fl!=0){
    lfi+=k;
    lf.write(lfi);
    fl-=k;
    delay(sped*2);
  }
  
  
}
void motors_thigh_knee(int tl,int tr, int kl, int kr){
  int i=tl>0?1:-1;
  int j=tr>0?1:-1;
  int k=kl>0?1:-1;
  int l=kr>0?1:-1;
  while(tl!=0&&tr!=0&&kl!=0&&kr!=0){
    lti+=i;
    lt.write(lti);
    rti+=j;
    rt.write(rti);
    lki+=k;
    lk.write(lki);
    rki+=l;
    rk.write(rki);
    tl-=i;
    tr-=j;
    kl-=k;
    kr-=l;
    delay(sped*3);
  }
  while(tl!=0){
    lti+=i;
    lt.write(lti);
    tl-=i;
    delay(sped);
  }
  while(tr!=0){
    rti+=j;
    rt.write(rti);
    tr-=j;
    delay(sped);
  }
  
  while(kl!=0){
    lki+=k;
    lk.write(lki);
    kl-=k;
    delay(sped);
  }
   while(kr!=0){
    rki+=l;
    rk.write(rki);
    kr-=l;
    delay(sped);
  }
}
void alignLeft(){    //BringLeft Leg Back
  lfi=lf.read();
  rfi= rf.read();
  lki=lk.read();
  rki=rk.read();
  lti= lt.read();
  rti= rt.read();
  lsti=lst.read();
  rsti=rst.read();
  
  
  left_knee(83);
  left_thigh(97);
  right_knee(90);
  right_thigh(97);
  left_feet(85);
  left_side_thigh(98);
  right_feet(92);
  right_side_thigh(95);
  
  
}
void alignRight(){   //bring rightLeg BAck
  lfi=lf.read();
  rfi= rf.read();
  lki=lk.read();
  rki=rk.read();
  lti= lt.read();
  rti= rt.read();
  lsti=lst.read();
  rsti=rst.read();
  
  right_knee(90);
  right_thigh(97);
  left_knee(83);
  left_thigh(97);
  right_feet(92);
  right_side_thigh(95);
  left_feet(85);
  left_side_thigh(98);
  
  
}
void walk_start()   //starts the walking process(code executed only once)
{
  bot_tilt_left();
  delay(200);
  right_side_thigh(rsti+side_tilt);
  delay(0);
  right_knee_right_thigh(rki-mthigh,rti+mthigh);
  delay(0);
  
   
  
}
int stepPos=0;
void walk2(){
  if(stepPos==0){
    walk_start();
    stepPos=1;
  }
  if(stepPos==1){
    delay(200);
    right_feet(rfi-2*side_tilt);
    motors_four(side_tilt,-1*side_tilt,-1*tilt,tilt);
    delay(200);
    motors_thigh_knee(-1*mthigh,-1*mthigh, mthigh,mthigh);
    delay(0);
    bot_tilt_right();  
    left_side_thigh(lsti+side_tilt);
    delay(0);
    left_knee_left_thigh(lki-mthigh*2,lti+mthigh*2);
    delay(0);
    stepPos=2;
  }
  else if(stepPos==2){
  
    left_feet(lfi-2*side_tilt);
    motors_four2(-1*side_tilt,side_tilt,tilt,-1*tilt);
    delay(100);
    motors_thigh_knee(-1*mthigh,-1*mthigh,mthigh,mthigh);
    delay(0);
    bot_tilt_left();
    right_side_thigh(rsti+side_tilt);
    delay(0);
    right_knee_right_thigh(rki-mthigh*2,rti+mthigh*2);
    stepPos=1;
  }
  
}

void right_kick(){
  bot_tilt_left();
  right_feet(rfi-side_tilt);
  right_knee(rki-mthigh*3);
  delay(500);
  right_knee_speed(rki+mthigh*3);
  right_knee_right_thigh_speed(rki+mthigh,rti+mthigh);
  //delay(500);
//   right_thigh_left_thigh(rti-5,lti-5);
  
  
}

void left_kick(){
  bot_tilt_right();
  left_feet(lfi-side_tilt);
  left_knee(lki-mthigh*3);
  delay(500);
  left_knee_speed(lki+mthigh*3);
  left_knee_left_thigh_speed(lki+mthigh,lti+mthigh);
  //delay(500);
//   right_thigh_left_thigh(rti-5,lti-5);
  
  
}


void sidewalkLeft(){
//  Serial.print("Walking Left start\n");
  bot_tilt_right();
  delay(200);
  left_side_thigh(lsti+tilt);
  left_feet(lfi+tilt);
  right_side_thigh(rsti+side_tilt);
  right_feet(rfi-tilt);
  left_feet(lfi-2*side_tilt-tilt);
  motors_four(side_tilt*(-4),side_tilt*4,side_tilt*4,side_tilt*(-4));
  bot_tilt_left();
  right_feet(rfi-2*side_tilt);
  delay(500);
  right_feet(rfi-side_tilt*4+tilt);
  right_side_thigh(rsti-side_tilt*2);
  //todo change this fucking straight function
  straight();
//  Serial.print("Walking Left finished\n");
//  while(1);
  
}

void sidewalkRight(){
//  Serial.print("Walking right start\n");
  bot_tilt_left();
  delay(200);
  right_side_thigh(rsti+tilt);
  right_feet(rfi+tilt);
  left_side_thigh(lsti+side_tilt);
  left_feet(lfi-tilt);
  right_feet(rfi-2*side_tilt-tilt);
  motors_four(side_tilt*(4),side_tilt*(-4),side_tilt*(-4),side_tilt*(4));
  bot_tilt_right();
  left_feet(lfi-2*side_tilt);
  delay(200);
  left_feet(lfi-side_tilt*4+tilt);
  left_side_thigh(lsti-side_tilt*2);
  //todo change this fucking straight function
  straight();
//  Serial.print("Walking right finished\n");
  
}

void alignStep(){
  if(stepPos==1){
    alignRight();
    stepPos=0;
  }
  else if(stepPos==2){
    alignLeft();
    stepPos=0;
  }
}

int walkLength=8;
void decision_rpi(){
 
    boolean right=digitalRead(rpi_r)==1;
    boolean left=digitalRead(rpi_l)==1;
    if(right && left){
      Serial.print("walk_r\n");
      walk2();
    }  
    else if(!right && left){
      alignStep();
      Serial.print("walk_right_r\n");
      sidewalkRight(); //walk right
    }
    else  if(right && !left){
      alignStep();
      Serial.print("walk_left_r\n");
      sidewalkLeft(); //walk left
        
    }
    else{
      Serial.print("kick_r\n");
      alignStep();
      delay(500);
      //find min dist
      int rightDist=get_right_dist();
      int leftDist=get_left_dist();
      int minDist=leftDist<rightDist? leftDist:rightDist;

      while(minDist>2){           //gets closer to the ball until dist is > 5 cm, then kicks in range of 0-5 cm
        walk2();
        rightDist=get_right_dist();
        leftDist=get_left_dist();
        minDist=leftDist<rightDist? leftDist:rightDist;
      }
      alignStep();
      if(get_right_dist()<=get_left_dist())
        right_kick();
      else {
        left_kick();
      }
      delay(500);
      straight();
      while(1);
    }
    

}


void decision_ultra(){
    boolean right;
    boolean left;
      right=check_right_ultra();
      left=check_left_ultra();
    if(right && left){
      ;//stop
      alignStep();
      Serial.print("stop\n");
    }  
    else if(!right && left){
      alignStep();
      Serial.print("walk_right\n");
      sidewalkRight(); //walk right
    }
    else  if(right && !left){
      alignStep();
      Serial.print("walk_left\n");
      sidewalkLeft(); //walk left
    }
    else{
      Serial.print("walk\n");
      walk2();
    }
   

}

void useRpi(){
//  if(firstStep){
//    straight();
//    delay(500);
// }
  decision_rpi();
}
void useUltra(){
  decision_ultra();

}

void loop() 
{ 
    if(digitalRead(interruptPin)){
      useRpi();
    }
    else
      useUltra();

} 

void sweep(Servo myServo,int* initial,int final){
  int i=*initial,toDecrease=1;
  if(final<*initial){ 
    toDecrease=-1;
  }
  
  while(final!=i){
    i+=toDecrease;
    myServo.write(i);
    delay(sped);
//    Serial.print("1\n");
  }
  *initial = i;
  
}
void sweep(Servo myServo,int* initial,int final,int abcd){
  int i=*initial,toDecrease=1;
  if(final<*initial){ 
    toDecrease=-1;
  }
  
  while(final!=i){
    i+=toDecrease;
    myServo.write(i);
    delay(sped/4);
    
  }
  *initial = i;
  
}

void right_thigh_left_thigh(int a,int b){
  int x,y;
  x=rti,y=lti;
  int i=(rti>a?-1:1),j=(lti>b?-1:1);
  while(x!=a && y!=b){
    x+=i;
    y+=j;
    right_thigh(x);
    left_thigh(y);
  }
  while(x!=a){
    x+=i;
   right_thigh(x);
  }
  while(y!=b){
     y+=j;
     left_thigh(y);
  }
}

//change
void right_knee_right_thigh(int a,int b)  //to move right knee and right thigh together to required pos
{
  int i=(rki>a?-1:1),j=(rti>b?-1:1);
  while(rki!=a && rti!=b){
    rki+=i;
    rti+=j;
    rk.write(rki);
    rt.write(rti);
    delay(sped);
  }
  while(rki!=a){
    rki+=i;
   rk.write(rki);
    delay(sped);
  }
  while(rti!=b){
     rti+=j;
      rt.write(rti);
    delay(sped);
  }
  
}
void right_knee_right_thigh_speed(int a,int b)  //to move right knee and right thigh together to required pos
{
  int sp=sped;
  int i=(rki>a?-1:1),j=(rti>b?-1:1);
  while(rki!=a && rti!=b){
    rki+=i;
    rti+=j;
    rk.write(rki);
    rt.write(rti);
    delay(sped/3);
  }
  while(rki!=a){
    rki+=i;
   rk.write(rki);
    delay(sped/3);
  }
  while(rti!=b){
     rti+=j;
      rt.write(rti);
    delay(sped/3);
  }
  
}
void left_knee_left_thigh_speed(int a,int b)  //to move right knee and right thigh together to required pos
{
  int sp=sped;
  int i=(lki>a?-1:1),j=(lti>b?-1:1);
  while(lki!=a && lti!=b){
    lki+=i;
    lti+=j;
    lk.write(lki);
    lt.write(lti);
    delay(sped/3);
  }
  while(lki!=a){
    lki+=i;
   lk.write(lki);
    delay(sped/3);
  }
  while(lti!=b){
     lti+=j;
      lt.write(lti);
    delay(sped/3);
  }
  
}

void left_knee_left_thigh(int a,int b)  //to move right knee and right thigh together to required pos
{
  int i=(lki>a?-1:1),j=(lti>b?-1:1);
  while(lki!=a && rki!=b){
    lki+=i;
    lk.write(lki);
    lti+=j;
    lt.write(lti);
    delay(sped);
  }
  while(lki!=a){
    lki+=i;
   lk.write(lki);
   delay(sped);
  }
  while(lti!=b){
     lti+=j;
    lt.write(lti);
    delay(sped);
  }
  
}
//change
void right_thigh_left_knee(int a,int b)   //to move right thigh and left knee together to required position
{
  int i,j;
  for(i=rti,j=lki; i>=a && j>=b; i-=1,j-=1)
  {
    rt.write(i);
    lk.write(j);
      delay(sped);
  }
  rti=i;
  lki=j;
}
//change
void left_thigh_right_knee(int a ,int b)  //to move left thigh and right knee together to required position
{
  int i,j;
  for(i=lti,j=rki; i<=a && j<=b; i+=2,j+=1)
  {
    lt.write(i);
    rk.write(j);
      delay(sped);
  }
  lti=i;
  rki=j;
}
void left_feet(int a)//to move left feet the required pos by argumnet
{
//  Serial.print("Left feet started\n");
  sweep(lf,&lfi,a);
}
void left_knee(int a)//to move left knee the required pos by argumnet
{
  sweep(lk,&lki,a);
}
void left_thigh(int a)//to move left thigh the required pos by argumnet
{
  sweep(lt,&lti,a);
}
void left_side_thigh(int a)//to move left side thigh the required pos by argumnet
{
  sweep(lst,&lsti,a);
}
void right_feet(int a)//to move right feet the required pos by argumnet
{
  sweep(rf,&rfi,a);
}
void right_knee(int a)//to move left knee the required pos by argumnet
{
 sweep(rk,&rki,a);
}
void right_knee_speed(int a)//to move left knee the required pos by argumnet
{
 sweep(rk,&rki,a,0);
}
void left_knee_speed(int a)//to move left knee the required pos by argumnet
{
 sweep(lk,&lki,a,0);
}
void right_thigh(int a)//to move left thigh the required pos by argumnet
{
  sweep(rt,&rti,a);
}
void right_side_thigh(int a)//to move left thigh the required pos by argumnet
{
  sweep(rst,&rsti,a);
}
