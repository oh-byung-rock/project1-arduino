  #include <SoftwareSerial.h>
  #include <Servo.h>

  int frontLeftPin = 22;
  int frontRightPin = 23;
  int rearLeftPin = 24;
  int rearRightPin = 25;

  int frontLeftPin2 = 26;
  int frontRightPin2 = 27;
  int rearLeftPin2 = 28;
  int rearRightPin2 = 29;

  int frontLeftPin3 = 30;
  int frontRightPin3 = 31;
  int rearLeftPin3 = 32;
  int rearRightPin3 = 33;

  int minAngle = 45;                                    // 제 1 관절의 이동 범위 중 최소 각도
  int maxAngle = 135;                                   // 제 1 관절의 이동 범위 중 최대 각도
  int centerAngle = (minAngle + maxAngle) / 2;          // 제 1 관절의 이동 범위 중 중간 각도
  int changedVal = 5;                                   // 제 1 관절의 증감 단위
  int term = (maxAngle - centerAngle) / changedVal;     // 제 1 관절의 중간 각도와 최소 각도 또는 최대 각도 간의 차이 

  int secondStartAngle = 90;                            // 제 2 관절의 초기 각도
  int secondChangeVal = 3;                              // 제 2 관절의 증감 단위
  int secondHighAngle = secondStartAngle + (secondChangeVal * changedVal * (term / 2)); // 제 2 관절의 리턴 포인트 (최대) (이 각도 이후에는 초기 각도로 되돌아옴
  int secondLowAngle = secondStartAngle - (secondChangeVal * changedVal * (term / 2));  // 제 2 관절의 리턴 포인트 (최소) (이 각도 이후에는 초기 각도로 되돌아옴

  Servo frontLeft;
  Servo frontRight;
  Servo rearLeft;
  Servo rearRight;

  Servo frontLeft2;
  Servo frontRight2;
  Servo rearLeft2;
  Servo rearRight2;

  Servo frontLeft3;
  Servo frontRight3;
  Servo rearLeft3;
  Servo rearRight3;

  bool stepPhase = true; 
  bool start = true;

  char type[4];
  char target[1];
  char action[1];
  char _direction[1];;

  void setup()

  {
    Serial.begin(9600);

    frontLeft.attach(frontLeftPin);
    frontRight.attach(frontRightPin);
    rearLeft.attach(rearLeftPin);
    rearRight.attach(rearRightPin);

    frontLeft2.attach(frontLeftPin2);
    frontRight2.attach(frontRightPin2);
    rearLeft2.attach(rearLeftPin2);
    rearRight2.attach(rearRightPin2);

    frontLeft3.attach(frontLeftPin3);
    frontRight3.attach(frontRightPin3);
    rearLeft3.attach(rearLeftPin3);
    rearRight3.attach(rearRightPin3);

    frontLeft.write(centerAngle);
    delay(15);
    frontRight.write(centerAngle); 
    delay(15);
    rearLeft.write(centerAngle); 
    delay(150);
    rearRight.write(centerAngle); 

    frontLeft2.write(secondStartAngle);
    delay(15);
    frontRight2.write(secondStartAngle); 
    delay(15);
    rearLeft2.write(secondStartAngle); 
    delay(15);
    rearRight2.write(secondStartAngle); 

    frontLeft3.write(secondStartAngle);
    delay(15);
    frontRight3.write(secondStartAngle); 
    delay(15);
    rearLeft3.write(secondStartAngle); 
    delay(15);
    rearRight3.write(secondStartAngle); 

    Serial1.begin(9600);
  }
 
  char temp[4];;
  char _temp=0x00;
  
  // 앱인버터로 앱 생성후 각 버튼에 따른 기능할당
  void loop(){
    while (Serial1.available()){ // Serial 모니터창에 어떤 값이 들어오면 실행을 합니다.
      Serial1.readBytes(temp, 4);
    if(temp[3]>96)
      _temp=(((int)(temp[2])-48)*16+((int)(temp[3])-87));
    else
      _temp=(((int)(temp[2])-48)*16+((int)(temp[3])-48));
    }
      if (_temp == 0x66) {
         goForward();
      } else if (_temp == 0x62) {
         goBackward();
      } else if (_temp == 0x6C) {
         goLeft();
      } else if (_temp == 0x72) {
         goRight();
      } else if (_temp == 0x69) {
        reset();
      }
      else if(_temp == 0x6a||_temp == 0x7a||_temp == 0x8a||_temp == 0x9a||_temp == 0x5a){
        //이동키 땠을때 상태값 바꾸고 정지
        _temp=0x00;
      }else{
        //미동작 공란(멈춤)
      }

    while (Serial.available()){ 
      byte data = Serial.read();
      Serial1.write(data);
    }
  }

// 처음 자세로 리셋
void reset() {
    start = true;
    frontLeft.write(centerAngle);
    delay(15);
    frontRight.write(centerAngle); 
    delay(15);
    rearLeft.write(centerAngle); 
    delay(15);
    rearRight.write(centerAngle); 

    frontLeft2.write(secondStartAngle);
    delay(15);
    frontRight2.write(secondStartAngle); 
    delay(15);
    rearLeft2.write(secondStartAngle); 
    delay(15);
    rearRight2.write(secondStartAngle); 

    frontLeft3.write(secondStartAngle);
    delay(15);
    frontRight3.write(secondStartAngle); 
    delay(15);
    rearLeft3.write(secondStartAngle); 
    delay(15);
    rearRight3.write(secondStartAngle); 

    Serial1.write("<- RESET");
  }

  // 로봇 전진 ( 각 phase에 따른 각도정리는 사진첨부 )
  void goForward() {
    Serial.println("go forward");
    if (start) {
      start = false;

      int rearRightAngle2 = rearRight2.read();
      int rearRightAngle3 = rearRight3.read();                              
      int tempCenter = centerAngle, tempCenter2 = centerAngle;
      int level = changedVal;

      for (int i = 0; i < term; i++) {
        rearRight.write(tempCenter -= changedVal);
        delay(15);
        frontRight.write(tempCenter2 += changedVal);
        delay(15);
      }
      Serial1.write("<- GO FORWARD FIRST STEP");
      Serial.println("<- GO FORWARD FIRST STEP");
    } else {
        int frontLeftAngle = frontLeft.read();
        int frontRightAngle = frontRight.read();
        int rearLeftAngle = rearLeft.read();
        int rearRightAngle = rearRight.read();

        int frontLeftAngle2 = secondStartAngle;
        int frontRightAngle2 = secondStartAngle;
        int rearLeftAngle2 = secondStartAngle;
        int rearRightAngle2 = secondStartAngle;

        int frontLeftAngle3 = secondStartAngle;
        int frontRightAngle3 = secondStartAngle;
        int rearLeftAngle3 = secondStartAngle;
        int rearRightAngle3 = secondStartAngle;

        int tempCenterFL = centerAngle;
        int tempCenterFR = centerAngle;
        int tempCenterRL = centerAngle;
        int tempCenterRR = centerAngle;

        int tempMin = minAngle;
        int tempMax = maxAngle;
        if (stepPhase) {
            stepPhase = false;
            int level = changedVal * -1;

            for (int i = 0; i < term; i++) {
                frontLeft.write(tempCenterFL -= changedVal);
                delay(15);
                rearRight.write(tempMin += changedVal);
                delay(15);
                if (frontRightAngle2 == secondLowAngle) {
                    level = changedVal;
                }
                frontRight3.write(frontRightAngle3 -= (secondChangeVal * level)); 
                frontRight2.write(frontRightAngle2 += (secondChangeVal * level));
                frontRight.write(tempMax -= changedVal);
                delay(15);           

                rearLeft3.write(rearLeftAngle3 -= (secondChangeVal * level));
                rearLeft2.write(rearLeftAngle2 += (secondChangeVal * level));
                rearLeft.write(tempCenterRL += changedVal);
                delay(15);
            }
            Serial1.write("<- GO FORWARD PHASE 1");
            Serial.println("<- GO FORWARD PHASE 1");
        } else {
            stepPhase = true;
            int level = changedVal;

            for (int i = 0; i < term; i++) {
                frontRight.write(tempCenterFR += changedVal);
                delay(15);
                rearLeft.write(tempMax -= changedVal);
                delay(15);
                if (frontLeftAngle2 == secondHighAngle) {
                    level = -1 * changedVal;
                }
                frontLeft3.write(frontLeftAngle3 -= (secondChangeVal * level));
                frontLeft2.write(frontLeftAngle2 += (secondChangeVal * level));
                frontLeft.write(tempMin += changedVal);
                delay(15);
                rearRight3.write(rearRightAngle3 -= (secondChangeVal * level));
                rearRight2.write(rearRightAngle2 += (secondChangeVal * level));
                rearRight.write(tempCenterRR -= changedVal);
                delay(15); 
            }
            Serial1.write("<- GO FORWARD PHASE 2");
            Serial.println("<- GO FORWARD PHASE 2");
        }
    }
  }

  // 로봇 후진 ( 각 phase에 따른 각도정리는 사진첨부 )
void goBackward() {
    Serial.println("go backward");
    if (start) {
      start = false;

      int frontLeftAngle2 = frontLeft2.read();
      int frontLeftAngle3 = frontLeft2.read();
      int tempCenter = centerAngle, tempCenter2 = centerAngle;
      int level = changedVal;

      for (int i = 0; i < term; i++) {
        rearLeft.write(tempCenter += changedVal);
        delay(15);
        frontLeft.write(tempCenter2 -= changedVal);
        delay(15);
      }
      Serial1.write("<- GO GO BACKWARD FIRST STEP");
      Serial.println("<- GO GO BACKWARD FIRST STEP");

    } else {
      int frontLeftAngle = frontLeft.read();
      int frontRightAngle = frontRight.read();
      int rearLeftAngle = rearLeft.read();
      int rearRightAngle = rearRight.read();

      int frontLeftAngle2 = secondStartAngle;
      int frontRightAngle2 = secondStartAngle;
      int rearLeftAngle2 = secondStartAngle;
      int rearRightAngle2 = secondStartAngle;

      int frontLeftAngle3 = secondStartAngle;
      int frontRightAngle3 = secondStartAngle;
      int rearLeftAngle3 = secondStartAngle;
      int rearRightAngle3 = secondStartAngle;

      int tempCenterFL = centerAngle;
      int tempCenterFR = centerAngle;
      int tempCenterRL = centerAngle;
      int tempCenterRR = centerAngle;

      int tempMin = minAngle;
      int tempMax = maxAngle;
  
      if (stepPhase) {
        stepPhase = false;
        int level = changedVal * -1;

        for (int i = 0; i < term; i++) {
          frontRight.write(tempCenterFL -= changedVal);
          delay(15);
          rearLeft.write(tempMin += changedVal);
          delay(15);

          if (frontLeftAngle2 == secondLowAngle) {
            level = changedVal;
          }
          frontLeft3.write(frontLeftAngle3 -= (secondChangeVal * level)); 
          frontLeft2.write(frontLeftAngle2 += (secondChangeVal * level));   
          frontLeft.write(tempCenterFL -= changedVal);
          delay(15);

          rearRight3.write(rearLeftAngle3 -= (secondChangeVal * level));
          rearRight2.write(rearRightAngle2 += (secondChangeVal * level));
          rearRight.write(tempMin += changedVal);
          delay(15);
        }
        Serial.write("<- GO BACKWARD PHASE 1");
        Serial1.write("<- GO BACKWARD PHASE 1");
      } else {
        stepPhase = true;
        int level = changedVal;

        frontLeft.write(tempCenterFR += changedVal);
        delay(15);
        rearRight.write(tempMax -= changedVal);
        elay(15);

        for (int i = 0; i < term; i++) {
          if (frontRightAngle2 == secondHighAngle) {
             level = -1 * changedVal;
          }
          frontRight3.write(frontLeftAngle3 -= (secondChangeVal * level));
          frontRight2.write(frontRightAngle2 += (secondChangeVal * level));  
          frontRight.write(tempCenterFR += changedVal);
          delay(15);

          rearLeft3.write(rearRightAngle3 -= (secondChangeVal * level));
          rearLeft2.write(rearLeftAngle2 += (secondChangeVal * level)); 
          rearLeft.write(tempMax -= changedVal);
          delay(15);  
        } 
        Serial.write("<- GO BACKWARD PHASE 2");
        Serial1.write("<- GO BACKWARD PHASE 2");
      }
    }
  }

  // 로봇 좌로 이동
  void goLeft() {
    // 로봇 앞,뒤 이동에서 시점만 바꾸면 좌,우 이동 입니다.
  }

  // 로봇 우로 이동
  void goRight() {
  // 로봇 앞,뒤 이동에서 시점만 바꾸면 좌,우 이동 입니다.
  }