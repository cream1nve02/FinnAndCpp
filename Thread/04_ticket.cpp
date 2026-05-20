// 티켓 판매 예제
// 시나리오: 콘서트 티켓 100장을 4명의 판매원이 동시에 판매
//          - mutex 없으면: 100장보다 더 많이 팔리는 사고 발생 (초과 판매)
//          - mutex 있으면: 정확히 100장만 팔림

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <windows.h>     // 한글 출력 깨짐 방지
using namespace std;

int tickets = 100;       // 남은 티켓 (공유 자원!)
int sold = 0;            // 판매된 티켓 수 (공유 자원!)

mutex mtx;               // 자물쇠

void seller(int id) {
    while (true) {
        // ---- 임계 구역 시작 (한 번에 한 명만 실행) ----
        lock_guard<mutex> lock(mtx);

        if (tickets <= 0) {
            // 매진이면 종료
            break;
        }

        // 티켓 판매 처리
        tickets--;
        sold++;
        cout << "판매원 " << id << ": 티켓 판매 완료. 남은 티켓 " << tickets << "장\n";
        // ---- 임계 구역 끝 (lock 자동 해제) ----
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    vector<thread> sellers;

    // 판매원 4명 동시 가동
    for (int i = 1; i <= 4; i++) {
        sellers.push_back(thread(seller, i));
    }

    // 모든 판매원 일 끝날 때까지 대기
    for (int i = 0; i < 4; i++) {
        sellers[i].join();
    }

    cout << "\n=== 최종 결과 ===\n";
    cout << "총 판매량: " << sold << "장\n";
    cout << "남은 티켓: " << tickets << "장\n";

    if (sold == 100 && tickets == 0) {
        cout << "✅ 정상! 정확히 100장 판매됨\n";
    } else {
        cout << "❌ 오류! 초과/미달 판매 발생\n";
    }
}
