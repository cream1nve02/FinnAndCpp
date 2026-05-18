/****************************************************************
 * [3] 쓰레드 / 뮤텍스 / 이벤트  -- 연습용
 *
 *  목표
 *   (1) std::thread 로 워커 2개 만들고 join
 *   (2) std::condition_variable 로 "이벤트" 처럼 신호 주고 깨우기
 *   (3) std::mutex + lock_guard 로 race condition 없는 add 구현
 *
 *  체크포인트
 *   - thread 객체 소멸 전에 join() (안 부르면 std::terminate)
 *   - condition_variable.wait(lock, predicate) 패턴 외우기
 *   - lock_guard 는 스코프 끝나면 자동 unlock (RAII)
 ****************************************************************/
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
using namespace std;

/********** (1) 쓰레드 만들어보기 **********/
void Worker(int id) {
    // TODO: "Worker {id} 시작" 출력
    // TODO: 300ms sleep  (this_thread::sleep_for + chrono::milliseconds)
    // TODO: "Worker {id} 끝" 출력
}

void Demo1_CreateThreads() {
    cout << "\n=== [1] 쓰레드 생성 ===" << endl;

    // TODO: thread t1(Worker, 1);  thread t2(Worker, 2);
    // TODO: t1.join(); t2.join();
}

/********** (2) 이벤트로 동작시점 관리 **********/
mutex              g_evtMutex;
condition_variable g_evtCv;
bool               g_ready = false;     // 이벤트 상태 플래그

void EventWorker() {
    // TODO:
    //  1. unique_lock<mutex> lk(g_evtMutex);
    //  2. "신호 대기중..." 출력
    //  3. g_evtCv.wait(lk, []{ return g_ready; });
    //     -> g_ready 가 true 될 때까지 잠
    //  4. "신호 받음!" 출력
}

void Demo2_Event() {
    cout << "\n=== [2] 이벤트(condition_variable) ===" << endl;

    thread t(EventWorker);

    // 메인이 일부러 0.5초 일하는 척
    this_thread::sleep_for(chrono::milliseconds(500));

    // TODO:
    //  1. { lock_guard<mutex> lk(g_evtMutex); g_ready = true; }
    //  2. g_evtCv.notify_one();    // 깨우기

    t.join();
}

/********** (3) 뮤텍스로 atomic add 구현 **********/
long long g_counter = 0;
mutex     g_counterMutex;

void AtomicAdd(int times) {
    for (int i = 0; i < times; ++i) {
        // TODO: lock_guard<mutex> 로 g_counterMutex 잡기
        // TODO: g_counter += 1;
        //  -> 락 안 잡으면 어떻게 되는지도 한 번 직접 빼고 돌려보기
    }
}

void Demo3_Mutex() {
    cout << "\n=== [3] 뮤텍스로 atomic add ===" << endl;

    const int kTimes = 100000;
    g_counter = 0;

    // TODO: 쓰레드 3개 (a, b, c) 만들어서 각각 AtomicAdd(kTimes) 호출
    // TODO: 셋 다 join
    // TODO: 기대값(kTimes*3) 과 g_counter 출력 비교
}

int main() {
    Demo1_CreateThreads();
    Demo2_Event();
    Demo3_Mutex();
    return 0;
}
