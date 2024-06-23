#include <stdio.h>
#include <wiringPi.h>

#define RED_LIGHT 0 // 红灯 GPIO 引脚
#define YELLOW_LIGHT 1 // 黄灯 GPIO 引脚
#define GREEN_LIGHT 2 // 绿灯 GPIO 引脚
#define PEDESTRIAN_RED_LIGHT 3 // 行人红灯 GPIO 引脚
#define PEDESTRIAN_GREEN_LIGHT 4 // 行人绿灯 GPIO 引脚
#define BUTTON 5 // 按钮 GPIO 引脚

// 初始化 GPIO 引脚
void setupPins() {
    pinMode(RED_LIGHT, OUTPUT);
    pinMode(YELLOW_LIGHT, OUTPUT);
    pinMode(GREEN_LIGHT, OUTPUT);
    pinMode(PEDESTRIAN_RED_LIGHT, OUTPUT);
    pinMode(PEDESTRIAN_GREEN_LIGHT, OUTPUT);
    pinMode(BUTTON, INPUT);
    digitalWrite(PEDESTRIAN_RED_LIGHT, HIGH); // 初始设置行人信号灯为红灯
}

// 交通灯控制循环
void trafficLightLoop() {
    while (1) {
        // 红 -> 黄 -> 绿 -> 红
        digitalWrite(RED_LIGHT, HIGH);
        digitalWrite(GREEN_LIGHT, LOW);
        digitalWrite(YELLOW_LIGHT, LOW);
        digitalWrite(PEDESTRIAN_GREEN_LIGHT, HIGH); // 三色交通灯为红时, 行人灯必须为绿
        digitalWrite(PEDESTRIAN_RED_LIGHT, LOW);
        delay(30000);

        digitalWrite(RED_LIGHT, LOW);
        digitalWrite(GREEN_LIGHT, LOW);
        digitalWrite(YELLOW_LIGHT, HIGH);
        digitalWrite(PEDESTRIAN_RED_LIGHT, HIGH); // 三色交通灯为黄时, 行人灯必须为红
        digitalWrite(PEDESTRIAN_GREEN_LIGHT, LOW);
        delay(5000);

        digitalWrite(RED_LIGHT, LOW);
        digitalWrite(GREEN_LIGHT, HIGH);
        digitalWrite(YELLOW_LIGHT, LOW);
        digitalWrite(PEDESTRIAN_GREEN_LIGHT, LOW); // 三色交通灯为绿时, 行人灯必须为红
        digitalWrite(PEDESTRIAN_RED_LIGHT, HIGH);
        delay(30000);
    }
}

// 行人信号灯控制
void pedestrianLightControl(int isButtonPressed) {
    if (isButtonPressed) { // 按钮被按下
        digitalWrite(PEDESTRIAN_GREEN_LIGHT, HIGH); // 触发行人绿灯
        digitalWrite(PEDESTRIAN_RED_LIGHT, LOW); // 行人红灯熄灭
        digitalWrite(RED_LIGHT, HIGH); // 三色交通灯变为红灯
        digitalWrite(GREEN_LIGHT, LOW);
        digitalWrite(YELLOW_LIGHT, LOW);
        delay(10000); // 持续10s
    }
    digitalWrite(PEDESTRIAN_RED_LIGHT, HIGH); // 行人信号灯恢复到红灯
    digitalWrite(PEDESTRIAN_GREEN_LIGHT, LOW);
}

int main() {
    wiringPiSetup(); // 初始化 WiringPi 库

    setupPins();

    while(1) {
        int isButtonPressed = digitalRead(BUTTON);
        if (isButtonPressed) {
            pedestrianLightControl(isButtonPressed);
        } else {
            trafficLightLoop();
        }
    }

    return 0;
}
