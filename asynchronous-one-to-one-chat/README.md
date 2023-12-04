## 비동기형 1:1 채팅 프로그램 만들기
### 과제 소개
1. 개요
- 사용자 s(server)와 c(client)는 **비동기 방식**으로 1:1 채팅
    - 각 사용자는 언제든 메시지를 전송할 수 있음 **(= 즉, 한 명의 사용자가 연속적으로 메시지 전송 가능)**
- 각 사용자는 두 개의 프로그램 및 두 개의 터미널을 사용
    - 2번 프로그램의 터미널은 사용자 s 가 채팅 메시지를 입력하는 용도
    - 1번 프로그램의 터미널은 s가 입력한 메시지 및 상대방 c가 전송한 메시지를 출력하는 용도
        - 내가 2번 프로그램 터미널에서 입력한 메시지 "Hello" 는 1번 터미널에서 "[ME] Hello" 형식으로 표현됨
        - 상대방이 입력한 메시지 "Nice"는 1번 터미널에서 "[YOU] Nice" 형식으로 표현됨
    - 2명의 사용자, 2개의 프로그램, 2개의 터미널을 사용
        - 총 4개의 소스코드 개발
        - 총 4개의 터미널에서 실행


2. 프로그램별 통신 방식
   
![image](https://github.com/dlwltn0430/PNU_Computer-Networks/assets/101401447/f323f49f-81d0-422b-9ff9-f82f7d761be0)


3. 프로그램 실행 순서
   
![image](https://github.com/dlwltn0430/PNU_Computer-Networks/assets/101401447/a2b831a0-99fa-4d94-88ca-28462c759287)


### 구조
```
PNU_Computer-Networks
└─ asynchronous-one-to-one-chat
   ├─ 1-s-main.c
   ├─ 2-s-input.c
   ├─ 3-c-main.c
   └─ 4-c-input.c
```
- 사용자 s 
    - **1-s-main.c** : 사용자 s가 (같은 컴퓨터 내에서) 보낸 메시지 및 사용자 c가 네트워크를 통해 보낸 메시지를 터미널에 출력
    - **2-s-input.c** : 사용자(s)가 입력한 메시지를 1-s-main.c에 전달
- 사용자 c
    - **3-c-main.c** : 사용자 s가 네트워크를 통해 보낸 메시지 및 사용자 c가 (같은 컴퓨터 내에서) 보낸 메시지를 터미널에 출력
    - **4-c-input.c** : 사용자(c)가 입력한 메시지를 3-c-main.c에 전달

### 실행 환경
```Kali Linux```

### 실행 방법
컴파일 및 실행 파일 생성
```
gcc -o 1-s-main 1-s-main.c
gcc -o 2-s-input 2-s-input.c
gcc -o 3-c-main 3-c-main.c
gcc -o 4-c-input 4-c-input.c
```
총 4개의 터미널에서 실행
```
./1-s-main
./2-s-input
./3-c-main
./4-c-input
```

### 실행 화면
![image](https://github.com/dlwltn0430/PNU_Computer-Networks/assets/101401447/3ccc82ae-0f2e-48bd-9e3f-6d999e16ab5c)

###  버전 기록
|버전|설명|
|--|--|
|v1.0.0|- non-blocking 모드 설정 X <br /> - 테스트용으로 3-c-main.c와 4-c-input.c 동작만 확인|
|v1.1.0|non-blocking 모드 설정 O|
|v1.1.1|- 실행 순서 확인 위해 출력문 추가 <br /> - unlink 위치 수정|
