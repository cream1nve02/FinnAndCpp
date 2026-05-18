/****************************************************************
 * [1] 변수 / 포인터 / 클래스
 *  - 링크드 리스트 (1) 끝에 추가  (2) 인덱스 삽입  (3) 인덱스 삭제
 *
 * 핵심 개념
 *  - Node : 값(data) + 다음 노드 주소(next)를 가진 "한 칸"
 *  - LinkedList : head 포인터 하나만 들고 다니며, next를 따라가서 조작
 *  - new 로 만든 노드는 반드시 delete (소멸자에서 정리)
 ****************************************************************/
#include <iostream>
using namespace std;

class Node {
public:
    int     data;
    Node*   next;

    Node(int d) : data(d), next(nullptr) {}
};

class LinkedList {
public:
    LinkedList() : head(nullptr) {}
    ~LinkedList();                  // 소멸자: 전체 노드 delete

    void Append(int value);         // (1) 끝에 추가
    void InsertAt(int idx, int v);  // (2) idx 위치에 삽입
    void RemoveAt(int idx);         // (3) idx 위치 노드 삭제
    void Print() const;

private:
    Node* head;
};

/********************** main **********************/
int main() {
    LinkedList list;

    // 끝에 추가: [10] -> [10,20] -> [10,20,30]
    list.Append(10);
    list.Append(20);
    list.Append(30);
    list.Print();

    // 1번 인덱스에 99 삽입 -> [10,99,20,30]
    list.InsertAt(1, 99);
    list.Print();

    // 0번 인덱스 삭제 -> [99,20,30]
    list.RemoveAt(0);
    list.Print();

    return 0;
}

/********************** 함수 정의 **********************/

// 소멸자: head부터 차례로 delete
LinkedList::~LinkedList() {
    Node* cur = head;
    while (cur != nullptr) {
        Node* nxt = cur->next;   // 다음 주소 먼저 보관
        delete cur;              // 현재 노드 해제
        cur = nxt;
    }
}

// (1) 끝에 추가
void LinkedList::Append(int value) {
    Node* node = new Node(value);

    if (head == nullptr) {       // 리스트가 비어있으면 head로
        head = node;
        return;
    }

    Node* cur = head;            // 마지막 노드까지 이동
    while (cur->next != nullptr) cur = cur->next;
    cur->next = node;            // 끝에 연결
}

// (2) idx 위치에 삽입 (idx == 0 이면 head 교체)
void LinkedList::InsertAt(int idx, int v) {
    if (idx < 0) return;

    Node* node = new Node(v);

    if (idx == 0) {              // 맨 앞 삽입
        node->next = head;
        head = node;
        return;
    }

    // idx-1 번째 노드까지 이동
    Node* prev = head;
    for (int i = 0; i < idx - 1 && prev != nullptr; i++)
        prev = prev->next;

    if (prev == nullptr) {       // 범위 초과 시 누수 방지
        delete node;
        return;
    }

    node->next = prev->next;     // 끼워넣기
    prev->next = node;
}

// (3) idx 위치 노드 삭제
void LinkedList::RemoveAt(int idx) {
    if (head == nullptr || idx < 0) return;

    if (idx == 0) {              // 맨 앞 삭제
        Node* del = head;
        head = head->next;
        delete del;
        return;
    }

    // idx-1 번째 노드까지 이동
    Node* prev = head;
    for (int i = 0; i < idx - 1 && prev->next != nullptr; i++)
        prev = prev->next;

    if (prev->next == nullptr) return;  // 범위 초과

    Node* del = prev->next;      // 지울 노드
    prev->next = del->next;      // 이전 노드를 지울 노드의 다음과 연결
    delete del;
}

void LinkedList::Print() const {
    cout << "[ ";
    for (Node* cur = head; cur != nullptr; cur = cur->next)
        cout << cur->data << " ";
    cout << "]" << endl;
}
