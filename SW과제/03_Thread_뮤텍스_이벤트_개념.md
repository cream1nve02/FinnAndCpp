# 쓰레드 / 뮤텍스 / 이벤트 - 개념부터

## 0. 한 줄 정리
- **쓰레드** : 한 프로그램 안에서 일하는 **직원**.
- **뮤텍스** : 공용 자료 만질 때 들고 가는 **화장실 열쇠**.
- **이벤트(condition_variable)** : "준비 됐어!" 알려주는 **호출벨**.

---

## 1. 쓰레드(Thread) 가 뭐냐

### 비유: 사무실 직원
- 프로그램 = 사무실
- 쓰레드 = 그 안에서 일하는 사람
- **싱글 쓰레드** : 직원 한 명이 위에서 아래로 일 순서대로 처리
- **멀티 쓰레드** : 직원 여러 명이 동시에 다른 일

### 머신비전 현장 그림 (감 잡으라고)
| 쓰레드 | 하는 일 |
| --- | --- |
| Capture 쓰레드 | 카메라에서 이미지 계속 받아오기 |
| Inspect 쓰레드 | 받은 이미지 검사 알고리즘 돌리기 |
| UI 쓰레드 | 화면에 결과 그리기 / 버튼 입력 받기 |
| Logger 쓰레드 | 결과 파일/DB 저장 |

이걸 한 명(싱글 쓰레드)이 다 하면? **UI가 멈춤.** 검사 도는 동안 버튼 안 먹음.

### 코드 한 줄로
```cpp
#include <thread>
std::thread t(함수이름, 인자1, 인자2);  // 새 직원 출근
t.join();                              // 이 직원 일 끝날 때까지 기다림
```

### 필수 약속
- `t.join()` 안 부르고 t가 죽으면 **프로그램 강제 종료** (`std::terminate`)
- 의도적으로 분리하고 싶으면 `t.detach()` (=떼어내기, 추적 안 함)
- **대부분 join이 정답**

---

## 2. 왜 뮤텍스(Mutex)가 필요한가 - "Race Condition"

### 사고 시나리오
```
공용 변수: count = 0
직원 A, 직원 B 동시에 "count += 1" 100,000번씩 수행

기대값: 200,000
실제값: ??? (199,873 같은 이상한 수)
```

### 왜 깨지는가? `count += 1` 은 사실 3단계
```
1) count 값을 CPU 레지스터로 읽기   (read)
2) 그 값에 1 더하기                (add)
3) 다시 메모리에 쓰기              (write)
```

A가 0 읽고 → 1로 만드는 사이 → B가 0 읽고 → 1로 만들면
**둘 다 1로 써버림.** 한 번의 +1 이 증발.

### 비유: 화장실 한 칸, 직원 여럿
- 동시에 들어가면 사고남
- 입구에 열쇠(mutex) 하나 두고: **열쇠 가진 사람만 들어감**
- 끝나면 열쇠 반납

### 코드
```cpp
#include <mutex>
std::mutex m;
long long count = 0;

void Add() {
    for (int i = 0; i < 100000; ++i) {
        std::lock_guard<std::mutex> lk(m);   // 열쇠 들고 들어감
        count += 1;                          // 화장실 (critical section)
    }                                        // lk 사라짐 = 자동 반납
}
```

### `lock_guard` 가 뭐냐
- 스코프(`{}`) 끝나면 **자동 unlock**. 까먹지 않게 도와주는 안전장치 (RAII).
- 직접 `m.lock(); ... m.unlock();` 도 가능하지만 중간에 return / throw 나면 락이 영영 안 풀려서 데드락.

### "atomic add" 라는 표현
- "쪼개지지 않는 1 더하기" 라는 뜻
- 위 코드처럼 mutex 로 감싸면 atomic add가 됨
- 사실 `std::atomic<long long>` 라는 전용 도구도 있는데, 과제 의도는 **뮤텍스로 직접 구현해 보기**

---

## 3. 이벤트(Event) - "기다리고 있다가 신호 받으면 일어남"

### 무슨 문제냐
쓰레드 B가 "쓰레드 A가 데이터 준비할 때까지 기다렸다" 가 시작하고 싶음.

**나쁜 방법** (busy wait, 폴링):
```cpp
while (!ready) { }   // CPU 100% 태우면서 무한 체크. 절대 안 됨.
```

**좋은 방법**: 잠들어 있다가 신호 받으면 일어나기.

### Windows API 로는
- `CreateEvent` / `SetEvent` / `WaitForSingleObject`
- (MFC 하다보면 곧 만남)

### 표준 C++ 로는 `condition_variable` + `mutex` + `플래그`
세 개가 한 세트.
```cpp
std::mutex              m;
std::condition_variable cv;
bool                    ready = false;
```

### 기다리는 쪽 (B)
```cpp
std::unique_lock<std::mutex> lk(m);
cv.wait(lk, []{ return ready; });   // ready 가 true 될 때까지 잠
// 깨어남! 여기서부터 일 시작
```

### 깨우는 쪽 (A)
```cpp
{
    std::lock_guard<std::mutex> lk(m);
    ready = true;          // 신호 set
}
cv.notify_one();           // 한 명 깨움 (notify_all 은 전부)
```

### 왜 굳이 mutex가 같이 끼는가
- ready 라는 변수를 두 쓰레드가 같이 보니까 → 보호 필요
- `wait(lk, predicate)` 는 락을 잠깐 풀고 잠 → 깨면 다시 락 잡고 predicate 체크

### 왜 람다(`[]{ return ready; }`) 가 필요한가
- **spurious wakeup** : 신호 없이도 운영체제가 가끔 깨움
- 깨났을 때 진짜 조건 맞는지 한 번 더 체크해야 안전

### 비유 정리
| 요소 | 비유 |
| --- | --- |
| condition_variable | 호출벨 |
| mutex | 호출벨 옆 인터폰 (한 명만 잡고 이야기 가능) |
| ready 플래그 | "음식 준비됨" 이라고 적힌 안내판 |
| notify_one | 벨 한 번 누름 |

---

## 4. 머신비전 현장 그림 - 종합

```
[Capture 쓰레드]                [Inspect 쓰레드]
     |                              |
  카메라 grab                     cv.wait(...)   <-- 잠
     |                              ^
  이미지 큐에 push                  |
  (queueMutex 잠금)                 |
     |                              |
  cv.notify_one() ------------------+
     v
   다시 grab                     이미지 큐에서 pop
                                   (queueMutex 잠금)
                                 검사 알고리즘 돌림
```

- **큐를 둘이 같이 만지니까** → mutex
- **새 이미지 들어왔는지 알려주려고** → condition_variable
- **각자 자기 일하니까** → 두 쓰레드

이 패턴이 **producer-consumer** 라고 부르는, 멀티쓰레드 단골 메뉴.

---

## 5. 한 줄 외움
- **쓰레드 만든다 = 직원 추가, 끝나면 join 잊지 말기**
- **공용 변수 만지면 = mutex (lock_guard로)**
- **다른 쓰레드 기다리면 = cv.wait(lock, predicate), notify_one 으로 깨움**
- **busy wait (while 빙빙 돌리기) 는 죄악**
