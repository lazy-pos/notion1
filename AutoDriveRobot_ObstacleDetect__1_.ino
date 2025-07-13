#include <AutoDriveRobot_Testdata.h>
#include <BP_MachineLearning.h>
/*
 * mode 1: Obstacle Detection Mode
 * echopin 8
 * trigpin 7
 * leftIR A0
 * rightIR A1
 * neck servo 11
 * left servo 9
 * right servo 10
 */
#include <Servo.h>

// 초음파 모듈 핀 설정
int echoPin = 8;
int trigPin = 7;

// 서보모터 핀 설정
int leftServo = 9;
int rightServo = 10;
int neckServo = 11;

// 서보모터 라이브러리 객체 생성
Servo neck;
Servo leftWheel;
Servo rightWheel;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // 초음파 모듈 핀 연결
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  // 서보모터 핀 연결
  neck.attach(neckServo);
  leftWheel.attach(leftServo);
  rightWheel.attach(rightServo);

  // 서보모터 초기 각도 설정
  neck.write(90);
  leftWheel.write(90);
  rightWheel.write(88);
}

void loop() {
  // mode 1 : obstacle detection mode
  float obstaclesD = getDistance();
  Serial.println(obstaclesD);

  // 초음파 모듈로 측정한 거리가 10cm보다 작다면
  // 정지 후 경로 탐색 함수 실행
  if(obstaclesD < 15){
    stopMove();
    searchNewPath();
  }
  // 초음파 모듈로 측정한 거리가 10cm 이상이면
  // 전진 함수 실행
  else{
    goForward();
  }
  delay(200);
}

// 초음파 모듈로 측정한 거리를 출력하는 함수
float getDistance(){
  digitalWrite(trigPin, LOW);
  digitalWrite(echoPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH);
  float distance = ((float)(340 * duration) / 10000) / 2;

  return distance;
}

// 전진 함수 : 왼쪽바퀴는 반시계방향, 오른쪽바퀴는 시계방향 회전
void goForward(){
  neck.write(90);
  leftWheel.write(115);
  rightWheel.write(60);
  Serial.println(F("go Forward"));
}

// 좌회전 함수 : 왼쪽바퀴는 시계방향, 오른쪽바퀴는 시계방향 회전
void leftTurn(){
  leftWheel.write(80);
  rightWheel.write(40);
  Serial.println(F("Turn Left"));  
}

// 우회전 함수 : 왼쪽바퀴는 반시계방향, 오른쪽바퀴는 반시계방향 회전
void rightTurn(){
  leftWheel.write(140);
  rightWheel.write(110);
  Serial.println(F("Turn Right"));
}

// 정지 함수 : 왼쪽 바퀴, 오른쪽 바퀴 정지
void stopMove(){
  // stop moving
  leftWheel.write(90);
  rightWheel.write(88);
  Serial.println(F("stop"));
}

// 경로 탐색 함수
void searchNewPath(){
  // 목 부분 서보모터를 작동시켜 오른쪽을 바라본 후 거리 측정, 왼쪽을 바라본 후 거리 측정
  Serial.println(F("Searching..."));
  neck.write(2);
  delay(250);
  float rightside = getDistance();
  Serial.println(rightside);
  delay(250);
  neck.write(178);
  delay(250);
  float leftside = getDistance();  
  Serial.println(leftside);
  delay(250);  
  // Serial.println(String(leftside) + " / " + String(rightside));

  // 측정한 두 거리값을 비교해서 회전
  // 왼쪽거리 < 오른쪽거리 => 우회전
  // 왼쪽거리 > 오른쪽거리 => 좌회전
  // 왼쪽거리 = 오른쪽거리 -> 정지
  if(leftside < rightside){
    rightTurn();
    delay(500);
  }
  else if(leftside > rightside){
    leftTurn();
    delay(500);
  }
  else {
    stopMove();
  }
  // 목 서보모터를 정면을 보도록 회전  
  neck.write(90);
}
