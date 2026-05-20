#include <iostream>
#include <thread>
#include <vector>
#include <mutex>      // mutex, lock_guard 사용
using namespace std;

// 전역 자물쇠 (모든 스레드가 같은 mtx를 공유해야 의미가 있음)
mutex mtx;

void worker(int& counter){
    for (int i = 0; i < 10000; i++) {
        // 생성 시 mtx.lock(), 스코프 종료 시 자동으로 mtx.unlock()
        // → counter += 1 이 한 번에 한 스레드만 실행됨 (race condition 방지)
        lock_guard<mutex> lock(mtx);
        counter += 1;
    }   // 여기서 lock 객체 소멸 → 자물쇠 자동 해제
}

int main(){
    int counter = 0;            // 4개 스레드가 공유할 변수
    vector<thread> workers;     // 생성한 스레드들을 보관할 명단

    // 스레드 4개 생성 → 각자 worker(counter) 실행
    // std::ref(counter) : 참조로 전달하려면 ref로 감싸야 함
    for (int i = 0; i < 4; i++){
        workers.push_back(thread(worker, std::ref(counter)));
    }

    // 4개 스레드가 모두 끝날 때까지 main 대기
    for (int i = 0; i < 4; i++) {
        workers[i].join();
    }

    // mutex 덕분에 항상 40000 (4 × 10000) 출력
    cout << "Counter 최종 값 : " << counter << endl;
}