# 02. 상속 접근 지정자 & 다형성 정리 (노션 복붙용)

## 1) public / protected / private 상속, 한 줄로

> "부모 멤버의 접근 권한이 자식 입장에서 **최대 어디까지 보일 수 있는가**"를 정하는 거.
> 멤버를 가리는 게 아니라 **권한의 천장(ceiling)** 을 씌우는 개념.

| 부모 멤버 | `public` 상속 | `protected` 상속 | `private` 상속 |
| --- | --- | --- | --- |
| public    | public    | protected | private |
| protected | protected | protected | private |
| private   | 접근 불가 | 접근 불가 | 접근 불가 |

- **private 멤버는** 어떤 상속이든 자식에서 직접 접근 불가 (부모만의 영역)
- **public 상속** : "is-a" 관계. 외부에서도 부모 인터페이스 그대로 사용 가능 → **가장 흔함**
- **protected 상속** : 외부에서는 부모로 못 봄. 자식의 자식까지만 부모로 접근
- **private 상속** : "has-a (구현 재사용)" 관계. 외부에서 부모로 못 봄

### 빠른 감 잡기
```cpp
class Base {
public:    int a;
protected: int b;
private:   int c;
};

class D1 : public    Base {};  // a=public,    b=protected, c=불가
class D2 : protected Base {};  // a=protected, b=protected, c=불가
class D3 : private   Base {};  // a=private,   b=private,   c=불가
```

> **현장 팁** : 99% `public` 상속만 씀. protected/private 상속은 의도가 명확할 때만.

---

## 2) override

- 부모 함수에 **virtual** 을 붙여야 자식이 갈아끼울 수 있음
- 자식에선 **override** 키워드로 "나는 부모 함수 덮어쓰는 중" 을 컴파일러에게 명시
- override 키워드는 오타 방지용 — 부모에 같은 시그니처 없으면 컴파일 에러로 잡아줌

```cpp
class Weapon { public: virtual void Attack() const; };
class Sword : public Weapon { public: void Attack() const override; };
```

부모 소멸자도 **virtual** 안 붙이면, 부모 포인터로 delete 할 때 자식 소멸자가 안 불려서 누수남.

---

## 3) 다형성으로 객체 "갈아끼우기" — 왜 필요한가

### 안 좋은 예 (분기 떡칠)
```cpp
void Fight(int type) {
    if      (type == 0) // 칼 공격
    else if (type == 1) // 활 공격
    else if (type == 2) // 마법 공격
    // 무기 추가될 때마다 여기 if 가 늘어남 -> 수정/버그 폭탄
}
```

### 다형성 사용
```cpp
void Fight(Weapon* w) { w->Attack(); }   // 끝.
```

- 새 무기 추가 → Weapon 상속만 받으면 됨, 호출부 변경 0줄
- **OCP (개방-폐쇄 원칙)** 자연스레 따라옴 : 확장엔 열려있고 수정엔 닫혀있음
- 머신비전 현장 예시
  - `ICamera` 인터페이스만 정의 → Dalsa / Basler / Hik 각 SDK는 자식에서 래핑
  - 그래버/검사 로직은 `ICamera*` 만 들고 다님 → 카메라 교체 시 상위 로직 무수정
- ROS1로 치면 : 메시지 타입(Topic) 추상화 후 콜백 안에서 처리하는 것과 같은 결.

---

## 한 줄 외움 노트
- 상속 지정자 = **권한의 천장**
- virtual + override = **갈아끼우기 허락 + 덮어쓰기 명시**
- 다형성 = **상위 로직 안 건드리고 종류만 늘리는 기술**
