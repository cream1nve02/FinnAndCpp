/****************************************************************
 * [1] 변수 / 포인터 / 클래스  -- 연습용
 *  주석 따라가면서 빈칸 채워보기
 *
 *  목표: 링크드 리스트의 Append / InsertAt / RemoveAt 구현
 *  체크포인트:
 *   - "다음 노드 주소"를 잃어버리지 않도록 순서 잘 잡기
 *   - new 한 노드는 항상 delete (소멸자에서!)
 ****************************************************************/
#include <iostream>
using namespace std;

class Node {
public:
    int     data;
    Node*   next;

    // TODO: 생성자에서 data는 d로, next는 nullptr 로 초기화
    Node(int d) {

    }
};

class LinkedList {
public:
    // TODO: head를 nullptr 로 초기화하는 생성자
    LinkedList() {

    }

    // 소멸자: 모든 노드 delete (메모리 누수 방지)
    ~LinkedList();

    void Append(int value);
    void InsertAt(int idx, int v);
    void RemoveAt(int idx);
    void Print() const;

private:
    Node* head;
};

/********************** main **********************/
int main() {
    LinkedList list;

    list.Append(10);
    list.Append(20);
    list.Append(30);
    list.Print();           // [ 10 20 30 ]

    list.InsertAt(1, 99);
    list.Print();           // [ 10 99 20 30 ]

    list.RemoveAt(0);
    list.Print();           // [ 99 20 30 ]

    return 0;
}

/********************** 함수 정의 **********************/

LinkedList::~LinkedList() {
    // TODO:
    // 1. cur = head 부터 시작
    // 2. while (cur != nullptr)
    //      next 주소 먼저 백업 -> delete cur -> cur = 백업
}

// (1) 리스트 끝에 새 노드 추가
void LinkedList::Append(int value) {
    // TODO:
    // - 새 노드 동적 할당
    // - head가 nullptr이면? -> head가 이 노드
    // - 아니면? -> 마지막 노드(next == nullptr)까지 이동 후 연결
}

// (2) idx 위치에 새 노드 삽입 (idx == 0 이면 맨 앞)
void LinkedList::InsertAt(int idx, int v) {
    // TODO:
    // - idx < 0 이면 무시
    // - idx == 0 이면 새 노드의 next를 현재 head로, head를 새 노드로
    // - 그 외에는 idx-1 까지 이동한 prev 를 찾고
    //   new->next = prev->next; prev->next = new;
    // - 범위를 벗어나면 new 한 노드 delete (메모리 누수 방지!)
}

// (3) idx 위치 노드 삭제
void LinkedList::RemoveAt(int idx) {
    // TODO:
    // - 비어있거나 idx < 0 이면 무시
    // - idx == 0 이면 head를 head->next로 바꾸고 기존 head delete
    // - 그 외에는 idx-1 까지 이동한 prev 를 찾고
    //   del = prev->next; prev->next = del->next; delete del;
}

void LinkedList::Print() const {
    cout << "[ ";
    for (Node* cur = head; cur != nullptr; cur = cur->next)
        cout << cur->data << " ";
    cout << "]" << endl;
}
