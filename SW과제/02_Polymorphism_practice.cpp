/****************************************************************
 * [2] 클래스 / 포인터  -- 연습용
 *
 *  목표
 *   1) Weapon (부모) - virtual Attack 정의
 *   2) Sword, Bow, Magic (자식) - override 로 Attack 재정의
 *   3) Player가 Weapon* 만 가지고 갈아끼우면서 사용 (다형성)
 *
 *  체크포인트
 *   - 부모의 함수에 virtual 안 붙이면 override 안 됨 (정적 바인딩됨)
 *   - 부모 포인터로 자식 delete 하려면 부모 소멸자도 virtual 이어야 안전
 ****************************************************************/
#include <iostream>
#include <string>
using namespace std;

/********** 부모 클래스 **********/
class Weapon {
public:
    // TODO: 가상 소멸자 선언 (왜? 아래 Player::Equip 에서 delete 함)
    // ~Weapon() ...

    // TODO: virtual void Attack() const; 으로 선언
    // TODO: virtual string Name() const;
};

/********** 자식 클래스들 **********/
class Sword /* TODO: : public Weapon */ {
public:
    // TODO: Attack 오버라이드 -> "[Sword] 휘두름! 데미지 30" 출력
    // TODO: Name 오버라이드 -> "Sword" 반환
};

class Bow /* TODO: : public Weapon */ {
public:
    // TODO: Attack / Name 오버라이드
};

class Magic /* TODO: : public Weapon */ {
public:
    // TODO: Attack / Name 오버라이드
};

/********** Player : 무기 갈아끼우는 클래스 **********/
class Player {
public:
    Player() : weapon(nullptr) {}

    // TODO: 소멸자 -> weapon 해제
    ~Player() {

    }

    // TODO: 기존 weapon 해제 후 새 무기로 교체
    void Equip(Weapon* w) {

    }

    // TODO: weapon이 nullptr이 아니면 weapon->Attack() 호출
    //       (Player는 무기 종류를 몰라도 됨 - 이게 다형성의 장점)
    void Fight() const {

    }

private:
    Weapon* weapon;
};

/********************** main **********************/
int main() {

    Player p;

    // 아래 코드가 그대로 동작해야 함
    p.Equip(new Sword());   p.Fight();
    p.Equip(new Bow());     p.Fight();
    p.Equip(new Magic());   p.Fight();

    /***********************************************************
     * 자기 답안 작성:
     *  Q. 왜 if/else 로 무기 종류 분기하지 않고 다형성을 쓰는가?
     *  A. (여기에 본인 말로 적어보기)
     ***********************************************************/
    return 0;
}
