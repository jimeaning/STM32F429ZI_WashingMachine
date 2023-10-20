# 👚 STM32F429ZI_WashingMachine
**STM32 MCU를 활용한 세탁기 기능 설계 및 제작**

![](https://velog.velcdn.com/images/jimeaning/post/7b38245a-72af-4828-9d9c-e5e231fb2998/image.jpg)


## 🧦 프로젝트 소개

Intel AI SW 아카데미에서 한 달 동안 배운 내용을 바탕으로 세탁기를 만드는 프로젝트

- **프로젝트 기간** : 23.10.17 ~ 23.10.19
- **프로젝트 인원** : 개인 프로젝트
- **프로젝트 기능**
	- 세탁, 행굼, 탈수 3가지 모드 선택 가능
	- Normal, Speedy 2가지 옵션 선택 가능
	- 초음파 센서로 세제 양 판단
	- FND로 남은 시간 출력

<br>

## ✔️ 설계 프로세스

![](https://velog.velcdn.com/images/jimeaning/post/eec49ca9-cfe7-472c-9fa5-ae6b7a5441a1/image.png)

<br>

## 💡 기능 구상 및 기획
-   Washing, Rinsing, Dehydrating 총 3가지 모드가 있다
-   각 모드는 Normal, Speedy 중 한 옵션을 선택할 수 있다
-   모드에 따라 다르게 모터를 작동시킨다  
    - Normal : 80세기, 10초  
    - Speedy : 160세기, 5초
-   Ultrasonic으로 세제 양을 판단한다  
    - 남은 공간이 50cm 미만이면 Too much Detergent! 출력
-   FND에 남은 시간을 출력한다

<br>

## 🌰 FSM
![](https://velog.velcdn.com/images/jimeaning/post/c343c948-1d60-4603-b274-6449fb1b085f/image.png)

<br>

## 💻 기술

STM32 MCU(Cortex-M4), C언어, STM32CubeIDE

<br>

## 🔧 하드웨어 구성 장치 및 역할
![](https://velog.velcdn.com/images/jimeaning/post/15df7fac-349e-4e3c-8a7c-a9a17e21d983/image.png)

<br>

## 🔩 하드웨어 구현
![](https://velog.velcdn.com/images/jimeaning/post/a686b6f7-78a6-4f96-a2c8-62f27518b1f6/image.png)

<br>



## 🛠 하드웨어와 제어 시스템 결합

![](https://velog.velcdn.com/images/jimeaning/post/5bac4bb1-4d98-4a52-9932-4b0f1ada984f/image.png)

<br>

## 🧺 초음파 센서로 세제 양 판단하는 화면
![](https://velog.velcdn.com/images/jimeaning/post/dfbc1f4e-4e4f-476e-b8e5-1fee3d7cd6d1/image.png)

<br>

## 😆 프로젝트 후기
1. 문제점 및 해결 방안 <br>
	a. 타이머가 끝날 때까지 함수가 실행되어야 하는데 한 번만 실행되고 나옴<br>
-> flag 변수를 선언해 해당 변수가 1일 때는 계속 실행될 수 있도록 수정<br>
	b. FND가 0이 될 때까지 감소해야 하는데 한 번만 감소함<br>
-> 무수히 많이 반복되어야 하기 때문에 조건문 바깥으로 빼줌<br>

2. 아쉬운 점<br>
  a. 시간의 한계로 정방향/역방향 모두 돌아가게 하지 못한 점<br>
	b. 버저 소리 기능을 추가하지 못한 것<br>
  c. 세탁기가 세탁 > 헹굼 > 탈수의 과정을 거친다면 연속적으로 처리하는 것도 좋을 것 같음<br>

