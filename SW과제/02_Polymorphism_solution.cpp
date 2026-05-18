/****************************************************************
 * [2] 클래스 / 포인터  -- 다형성 모범답안
 *
 * 1) public / private / protected 상속 효과는
 *    별도 노트(02_Inheritance_정리.md) 참고
 *
 * 2) override : 부모 함수에 virtual 붙이고, 자식은 override 키워드
 *
 * 3) 다형성 : 부모 타입 포인터로 자식 객체를 가리키면
 *           "실제 객체의 함수"가 호출됨 (런타임 디스패치)
 *
 * 예시 시나리오: 머신비전에서 Camera 인터페이스 하나 두고
 *  - DalsaCamera / BaslerCamera / VierowCamera 를 갈아끼우는 구조
 *
 * 여기서는 이해하기 쉽게 Weapon / Sword / Bow / Magic 로 작성
 ****************************************************************/
#include <iostream>
#include <string>
using namespace std;

/********** 부모(추상) 클래스 **********/
class Weapon {
public:
    // 가상 소멸자: 부모 포인터로 자식을 delete할 때 안전하게 정리
    virtual ~Weapon() {}

    // virtual = "자식이 갈아끼울 수 있음" 을 의미
    virtual void Attack() const { cout << "[Weapon] 공격" << endl; }
    virtual string Name() const { return "Weapon"; }
};

/********** 자식들 **********/
class Sword : public Weapon {
public:
    void Attack() const override { cout << "[Sword] 휘두름! 데미지 30" << endl; }
    string Name() const override { return "Sword"; }
};

class Bow : public Weapon {
public:
    void Attack() const override { cout << "[Bow] 화살 발사! 데미지 20" << endl; }
    string Name() const override { return "Bow"; }
};

class Magic : public Weapon {
public:
    void Attack() const override { cout << "[Magic] 파이어볼! 데미지 50" << endl; }
    string Name() const override { return "Magic"; }
};

/********** Player: 무기를 "갈아끼우는" 클래스 **********/
class Player {
public:
    Player() : weapon(nullptr) {}
    ~Player() { delete weapon; }

    // 새 무기로 교체 (기존 건 해제)
    void Equip(Weapon* w) {
        delete weapon;
        weapon = w;
        cout << ">> " << weapon->Name() << " 장착" << endl;
    }

    void Fight() const {
        if (weapon == nullptr) { cout << "맨손..." << endl; return; }
        weapon->Attack();      // <-- 다형성! Player는 Weapon만 알면 됨
    }

private:
    Weapon* weapon;
};

/********************** main **********************/
int main() {

    Player p;

    // 무기를 갈아끼워도 Player::Fight() 코드는 그대로
    p.Equip(new Sword());   p.Fight();
    p.Equip(new Bow());     p.Fight();
    p.Equip(new Magic());   p.Fight();

    /***********************************************************
     * "왜 다형성으로 갈아끼우는가?"  -- 한 줄 요약
     *
     *  Player 코드를 안 건드리고 무기 종류를 늘릴 수 있기 때문.
     *
     *  - 새 무기(Axe, Gun ...) 추가 시 Weapon 상속만 받으면 됨
     *  - Player::Fight() 안에 if(검) / else if(활) ... 분기 없음
     *  - 머신비전 예: Camera 인터페이스 두고 메이커별 SDK 래핑
     *    -> 상위 로직은 GetImage() 만 부르면 되니 카메라 바뀌어도
     *       그래버/검사 코드는 그대로
     ***********************************************************/
    return 0;
}
