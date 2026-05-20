#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <windows.h>
using namespace std;

int tickets = 100;
int sold = 0;

mutex mtx;

void seller(int id){
    while(true){
        lock_guard<mutex> lock(mtx);
        if (tickets <= 0) {
            break;
        }

        tickets --;
        sold ++;
        cout << "판매원" << id << ": 티켓 판매 완료. 남은 티켓" << tickets << "장\n";
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    vector<thread> sellers;

    for (int i = 0; i <= 4; i++){
        sellers.push_back(thread(seller,i));
    }

    for (int i = 0; i < 4; i++) {
        sellers[i].join();
    }

    cout << "\n=== 최종 결과 ===\n";
    cout << "총 판매량: " << sold << "장\n";
    cout << "남은 티켓: " << tickets << "장\n";

    if (sold == 100 && tickets == 0) {
        cout << "정상! 정확히 100장 판매됨\n";
    } else{
        cout << "오류! 초과/미달 판매 발생\n";
    }

}