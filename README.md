# 퀴즈 프로그램 - [과제 내용](./assignment.pdf)

퀴즈 목록이 들어있는 암호화된 JSON 파일을 복호화하여 사용자에게 무작위로 퀴즈를 선택하여 내는 C 언어 퀴즈 프로그램입니다. 사용자는 질문에 대한 답변을 번호로 입력하고, 퀴즈가 끝난 후 점수가 표시됩니다.

## 기능

- 퀴즈 질문이 포함된 GPG 암호화된 JSON 파일을 복호화합니다.
- 무작위로 퀴즈 질문을 선택하여 사용자에게 제시합니다.
- N 개의 선택지를 가진 객관식 질문을 지원합니다.
- 퀴즈가 끝난 후 사용자의 점수를 표시합니다.

## 요구 사항

- C 컴파일러 (예: `gcc`)
- `cmake` (버전 3.29 이상)
- `json-c` 라이브러리
- `gpg` (GNU Privacy Guard)
- `pkg-config`

## 설치

1. 레포지토리를 clone 합니다.

2. 필요한 패키지 `json-c`와 `gpg`를 설치합니다.
    ```sh
    brew install json-c gpg
    ```

3. `cmake`를 사용하여 프로젝트를 빌드합니다:
    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

## 사용법

1. 프로젝트 디렉토리에 `quizzes.json.gpg`라는 이름의 GPG 암호화된 JSON 파일이 있는지 확인합니다. JSON 파일은 다음과 같은 형식을 따릅니다:
    ```json
      {
      "quizzes": [
         {
           "question": "What is the capital of France?",
           "options": ["Paris", "London", "Berlin", "Madrid"],
           "answer": "Paris"
         },
         {
           "question": "What is the capital of Germany?",
           "options": ["Paris", "London", "Berlin", "Madrid"],
           "answer": "Berlin"
         }
      ]
   }
    ```
2. 퀴즈 프로그램을 실행합니다.
    ```sh
    ./quiz
    ```
3. 화면에 표시되는 지시에 따라 퀴즈 질문에 답변합니다.
   ```
   총 1개의 문제를 출제합니다.
   What is the capital of Germany?
   1. Paris
   2. London
   3. Berlin
   4. Madrid
   정답을 입력하세요: 3
   정답입니다!
   
   총 1문제 중 1문제를 맞추셨습니다.
   ```
