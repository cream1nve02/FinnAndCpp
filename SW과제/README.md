# SW 과제 - 한 폴더 안내

## 구성
| 번호 | 주제 | 개념 노트 | 모범답안 | 연습용 |
| --- | --- | --- | --- | --- |
| 1 | 변수/포인터/클래스 · 링크드 리스트 | `01_LinkedList_개념.md` | `01_LinkedList_solution.cpp` | `01_LinkedList_practice.cpp` |
| 2 | 클래스/포인터 · 상속 & 다형성 | `02_Inheritance_정리.md` | `02_Polymorphism_solution.cpp` | `02_Polymorphism_practice.cpp` |
| 3 | 쓰레드/뮤텍스/이벤트 | `03_Thread_뮤텍스_이벤트_개념.md` | `03_Thread_solution.cpp` | `03_Thread_practice.cpp` |

## 어디부터 봐야 함?
1. **`*_개념.md` 먼저** - 그림이랑 비유로 감 잡기
2. `*_practice.cpp` 손으로 채워보기
3. 막히면 `*_solution.cpp` 컨닝

## 빌드 (Visual Studio Developer Command Prompt)
```cmd
cl /EHsc /std:c++17 01_LinkedList_solution.cpp
cl /EHsc /std:c++17 02_Polymorphism_solution.cpp
cl /EHsc /std:c++17 03_Thread_solution.cpp
```
> `/std:c++17` 안 줘도 대부분 OK. 쓰레드 예제는 C++11 이상이면 됨.

## 막혔을 때 키워드
- 링크드 리스트 : "next 잃어버리지 않는 순서"
- 다형성 : `virtual` + `override` + 가상 소멸자
- 쓰레드 : `join()` 까먹지 말기 / `condition_variable::wait(lock, predicate)`
