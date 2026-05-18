/****************************************************************
 * [3] 쓰레드 / 뮤텍스 / 이벤트  -- 모범답안
 *
 *  1) std::thread 로 쓰레드 만들기
 *  2) std::condition_variable 로 "이벤트" 동작시점 관리
 *     (Win32 의 SetEvent/WaitForSingleObject 에 해당)
 *  3) std::mutex + std::lock_guard 로 atomic add 구현
 *
 *  컴파일: VS2019+ , C++11 이상 (Visual Studio 기본 OK)
 *
 *  주의 : Windows MSVC 에서는 그냥 컴파일/실행됨.
 *         g++ 는 -pthread 옵션 필요.
 ****************************************************************/
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
using namespace std;

/********** (1) 쓰레드 만들어보기 **********/
void Worker(int id) {
    cout << "Worker " << id << " 시작 (tid=" << this_thread::get_id() << ")" << endl;
    this_thread::sleep_for(chrono::milliseconds(300));
    cout << "Worker " << id << " 끝" << endl;
}

void Demo1_CreateThreads() {
    cout << "\n=== [1] 쓰레드 생성 ===" << endl;

    thread t1(Worker, 1);
    thread t2(Worker, 2);

    // join : 해당 쓰레드가 끝날 때까지 대기 (안 부르면 죽을 때 terminate)
    t1.join();
    t2.join();
}

/********** (2) 이벤트로 동작시점 관리 **********/
// "메인이 신호 줄 때까지 워커는 잠자고 있다가, 신호 받으면 깨서 일함"
mutex              g_evtMutex;
condition_variable g_evtCv;
bool               g_ready = false;   // <-- "이벤트 상태" 역할

void EventWorker() {
    unique_lock<mutex> lk(g_evtMutex);
    cout << "[EventWorker] 신호 대기중..." << endl;

    // ready 가 true가 될 때까지 잠. spurious wakeup 방지 위해 람다 사용
    g_evtCv.wait(lk, [] { return g_ready; });

    cout << "[EventWorker] 신호 받음! 일 시작" << endl;
}

void Demo2_Event() {
    cout << "\n=== [2] 이벤트(condition_variable)로 동작시점 제어 ===" << endl;

    thread t(EventWorker);

    // 메인이 일부러 0.5초 일하는 척
    this_thread::sleep_for(chrono::milliseconds(500));

    {
        lock_guard<mutex> lk(g_evtMutex);
        g_ready = true;                 // 이벤트 set
    }
    g_evtCv.notify_one();               // 워커 한 명 깨우기 (notify_all 도 있음)

    t.join();
}

/********** (3) 뮤텍스로 atomic add 구현 **********/
long long g_counter = 0;
mutex     g_counterMutex;

void AtomicAdd(int times) {
    for (int i = 0; i < times; ++i) {
        // lock_guard : 스코프 벗어나면 자동 unlock (RAII)
        lock_guard<mutex> lk(g_counterMutex);
        g_counter += 1;                 // <-- critical section
    }
}

void Demo3_Mutex() {
    cout << "\n=== [3] 뮤텍스로 atomic add ===" << endl;

    const int kTimes = 100000;
    g_counter = 0;

    thread a(AtomicAdd, kTimes);
    thread b(AtomicAdd, kTimes);
    thread c(AtomicAdd, kTimes);

    a.join(); b.join(); c.join();

    cout << "기대값: " << (kTimes * 3) << ", 실제값: " << g_counter << endl;
    // 뮤텍스 없으면 race condition으로 기대값보다 작게 나옴
}

int main() {
    Demo1_CreateThreads();
    Demo2_Event();
    Demo3_Mutex();
    return 0;
}
