#include <MeMCore.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

float dir = 0;
float n = 0;
float move_step = 0;
float open_pos = 0;
float close_pos = 0;
float count1 = 0;
float count2 = 0;
float sen_car = 0;
float sen_start = 0;
int Start_pin = 11;
int finished_pin = 12;
float timer = 0;
int onoff_led = 0;
int running_stus = 0;
int pause_stop = 0;

MeRGBLed rgbled_7(7, 2);
MeUltrasonicSensor ultrasonic_2(2);
Servo servo_4_1;
MePort port_4(4);
void servo_control_N_N (double p1, double p2){
  if((p2 - p1) < 0){
      dir = -1 * move_step;
      n = p1;
      while(!(n < p2))
      {
        _loop();
        servo_4_1.write(n);
        n += dir;
        _delay(0.01);

      }

  }else{
      dir = move_step;
      n = p1;
      while(!(n > p2))
      {
        _loop();
        servo_4_1.write(n);
        n += dir;
        _delay(0.01);

      }

  }

}

void Stop_check()
{
    if((0^(analogRead(A7)>10?0:1))){
        rgbled_7.setColor(0,150,0,0);
        rgbled_7.show();
        pause_stop = 1;
        while(!((1^(analogRead(A7)>10?0:1))))
        {
            _loop();
        }
    }
}

MeUltrasonicSensor ultrasonic_3(3);
double currentTime = 0;
double lastTime = 0;
double led_timer = 0;
double current_time = 0;
double getLastTime(){
  return currentTime = millis() / 1000.0 - lastTime;
}

void car_come (){
  running_stus = 1;
  servo_control_N_N(open_pos, close_pos);
  count2 = 0;
  while(!(count2 > 10))
  {
    _loop();
    sen_start = ultrasonic_3.distanceCm();
    if(sen_start < 10){
        count2 += 1;

    }else{
        count2 = 0;

    }
    _delay(0.03);
  }
  digitalWrite(Start_pin, HIGH);
  _delay(1);
  digitalWrite(Start_pin, LOW);
  while(!(digitalRead(finished_pin) == HIGH))
  {
    _loop();
  }
  servo_control_N_N(close_pos, open_pos);
  timer = getLastTime();
  while(!((getLastTime() - timer) > 5))
  {
    Stop_check();
    _loop();
  }
  count1 = 0;
  running_stus = 0;
}

void _delay(float seconds) {
  long endTime = millis() + seconds * 1000;
  while(millis() < endTime) _loop();
}

void setup() {
  servo_4_1.attach(port_4.pin1());
  move_step = 2;
  open_pos = 25;
  close_pos = 115;
  pinMode(Start_pin, OUTPUT);
  pinMode(finished_pin, INPUT);
  pinMode(A7, INPUT);
  servo_control_N_N(60, open_pos);
  while(1) {
    running_stus = -1;
    led_timer = getLastTime();
    digitalWrite(Start_pin, LOW);
    while(!((0 ^ (analogRead(A7) > 10 ? 0 : 1))))
    {
      _loop();
    }
    running_stus = 0;
    while(!((1^(analogRead(A7)>10?0:1))))
    {
        _loop();
    }
    pause_stop = 0;
    while(pause_stop == 0) {
        sen_car = ultrasonic_2.distanceCm();
        if(sen_car < 10){
            count1 += 1;
            if(count1 > 10){
                car_come();
            }
        }else{
            count1 = 0;
        }
        _delay(0.03);
        Stop_check();
        _loop();
    }
  }
}

void _loop() {
  if (running_stus == 1){
    current_time = getLastTime();
    if (current_time-led_timer > 0.2){
        led_timer = getLastTime();
        onoff_led = (onoff_led+1)%2;
        if (onoff_led == 0){
            rgbled_7.setColor(2, 100, 0, 0);
            rgbled_7.show();
            rgbled_7.setColor(1, 0, 0, 0);
            rgbled_7.show();
        }else{
            rgbled_7.setColor(2, 0, 0, 0);
            rgbled_7.show();
            rgbled_7.setColor(1, 0, 0, 100);
            rgbled_7.show();
        }
    }
  }else if (running_stus == 0){
    rgbled_7.setColor(0, 0, 100, 0);
    rgbled_7.show();
  }else{
    rgbled_7.setColor(0, 100, 0, 0);
    rgbled_7.show();
  }
  if (pause_stop == 1){
    rgbled_7.setColor(0, 100, 0, 0);
    rgbled_7.show();
  }
}

void loop() {
  _loop();
}


