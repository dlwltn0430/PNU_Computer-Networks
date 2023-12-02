## 비동기형 1:1 채팅 프로그램 만들기
### 구조
```
PNU_Computer-Networks
├─ asynchronous-one-to-one-chat
│  ├─ 1-s-main.c
│  ├─ 2-s-input.c
│  ├─ 3-c-main.c
│  └─ 4-c-input.c
```

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
4개의 터미널에서 실행
```
./1-s-main
./2-s-input
./3-c-main
./4-c-input
```

###  버전 기록
|버전|설명|
|--|--|
|v1.0.0|- non-blocking 모드 설정 X <br /> - 테스트용으로 3-c-main.c와 4-c-input.c 동작만 확인|
|||
