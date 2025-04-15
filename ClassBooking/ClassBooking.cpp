/*
* 2025 04 14
* 일반 사용자 예약 구현용 버전
*/


// 기본 입출력, 파일입출력, 문자열처리, 벡터 라이브러리 가져옴
#include <iostream>// 입출력 기능 사용
#include <fstream>// 파일 입출력 사용
#include <sstream> // 문자열 스트림 처리용
#include <string>  // 문자열 타입 사용
#include <vector> // 동적 배열 벡터 사용
#include <iomanip>// 출력 정렬용 (setw 쓸 때 필요)
using namespace std;

// 사용자 구조체
struct User {
    string id;
    string password;
    bool is_admin;
};

// 예약 구조체
struct Reservation {
    string user_id;
    string room;
    string day;
    string start_time;
    string end_time;
};

// 강의실 구조체
struct Classroom {
    string room;
    bool is_available;
    string available_start;
    string available_end;
};

// 벡터로 유저, 예약, 강의실 저장함, 이게 제일 간단함
vector<User> users;
vector<Reservation> reservations;
vector<Classroom> classrooms;

// 요일 리스트
vector<string> weekdays = { "Mon", "Tue", "Wed", "Thu", "Fri" };
// 시간대 리스트임, 1시간 단위
vector<string> times = {
    "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00", "17:00", "18:00"
};

// 시간 겹치는지 확인하는 함수
bool isTimeOverlap(const string& s1, const string& e1, const string& s2, const string& e2) {
    return !(e1 <= s2 || s1 >= e2);
}

// 강의실 불러오는 함수
bool loadClassrooms() {
    ifstream fin("classroom.txt");
    if (!fin) return false;
    string room, start, end;
    int avail;
    while (fin >> room >> avail >> start >> end) {
        bool available_flag = (avail != 0);  // int을 bool로 바꿈
        classrooms.push_back({ room, available_flag, start, end });
    }
    return true;
}

// 유저 불러오는 함수
bool loadUsers() {
    ifstream fin("user.txt");
    if (!fin) return false;
    string id, pw;
    int admin;
    while (fin >> id >> pw >> admin) {
        bool isAdmin = (admin != 0);
        users.push_back({ id, pw, isAdmin });
    }
    return true;
}

// 예약 불러오는 함수
bool loadReservations() {
    ifstream fin("reservation.txt");
    if (!fin) return false;
    string id, room, start, end, day;
    while (fin >> id >> room >> start >> end >> day) {
        reservations.push_back({ id, room, day, start, end });
    }
    return true;
}

// 강의실 층별로 출력해줌, 그대로 써도 될듯
void printClassroomList() {
    cout << "3F: "; for (auto& c : classrooms) if (c.room[0] == '3') cout << c.room << ", "; cout << endl;
    cout << "4F: "; for (auto& c : classrooms) if (c.room[0] == '4') cout << c.room << ", "; cout << endl;
    cout << "5F: "; for (auto& c : classrooms) if (c.room[0] == '5') cout << c.room << ", "; cout << endl;
    cout << "6F: "; for (auto& c : classrooms) if (c.room[0] == '6') cout << c.room << ", "; cout << endl;
}

// 강의실 시간표 출력함, 테스트 용으로 구현한거라 보완 필요
void printTimeTable(const string& room) {
    cout << "      ";
    for (auto& day : weekdays) cout << setw(6) << day;
    cout << endl;
    for (int t = 0; t < 9; ++t) {
        string t1 = times[t];
        string t2 = times[t + 1];
        cout << t1 << "~" << t2 << " ";
        for (int d = 1; d <= 5; ++d) {
            bool reserved = false;
            for (auto& r : reservations) {
                if (r.room == room && stoi(r.day) == d && isTimeOverlap(r.start_time, r.end_time, t1, t2)) {
                    reserved = true;
                    break;
                }
            }
            cout << setw(6) << (reserved ? "X" : "O");
        }
        cout << endl;
    }
    cout << "press any key to continue ...";
    cin.ignore(); cin.get();  // 아무 키 대기
}

// 로그인 기능, 테스트 용으로 구현한거라 보완 필요
User* login() {
    string id, pw;
    cout << "ID: "; cin >> id;
    cout << "PW: "; cin >> pw;
    for (auto& u : users) {
        if (u.id == id && u.password == pw) return &u;
    }
    cout << ".!! ID or PW is incorrect\n";
    return nullptr;
}

// 강의실 예약하는 함수
void reserveClassroom(const string& user_id) {
    string room, day, start, end;
    cout << "classroom number: "; cin >> room;
    cout << "day(1~5): "; cin >> day;
    cout << "start time(HH:MM): "; cin >> start;
    cout << "end time(HH:MM): "; cin >> end;

    // 겹치는 시간 예약했는지 체크
    for (const auto& r : reservations) {
        if (r.user_id == user_id && r.day == day && isTimeOverlap(r.start_time, r.end_time, start, end)) {
            cout << ".!! Already reserved time\n";
            return;
        }
    }

    // 강의실 예약 가능시간 안에 있는지 확인
    for (const auto& c : classrooms) {
        if (c.room == room && c.is_available && c.available_start <= start && c.available_end >= end) {
            reservations.push_back({ user_id, room, day, start, end });
            ofstream fout("reservation.txt", ios::app);
            fout << user_id << "\t" << room << "\t" << start << "\t" << end << "\t" << day << endl;
            cout << "Reservation completed\n";
            return;
        }
    }
    cout << ".!! This is not a time available for reservation\n";
}
//강의실 상태 출력 및 수정 함수- 조수빈
void showAndEditClassroom() {
    while (true) {
        cout << "1. check reservation\n2. accept reservation\n3. ban reservation\n>> ";
        int input; cin >> input;

        if (input == 1) { // 6.3.2.1 check reservation
            printClassroomList();
            cout << "classroom number: ";
            string room; cin >> room;
            printTimeTable(room);
        }
        else if (input == 2) { // 6.3.2.2 accept reservation
            cout << "classroom number: ";
            string room; cin >> room;
            bool roomFound = false;
            for (auto& c : classrooms) {
                if (c.room == room) {
                    roomFound = true;
                    cout << "enter a number corresponding to the day of the week\n"
                         << "(1. Mon, 2. Tue, 3. Wed, 4. Thu, 5. Fri): ";
                    int day; cin >> day;
                    if (day < 1 || day > 5) {
                        cout << ".!! Invalid weekday input\n";
                        break;
                    }
                    string start, end;
                    cout << "start accept time: "; cin >> start;
                    cout << "end accept time: "; cin >> end;
                    // 시간 포맷 검사는 생략했지만, 필요시 추가 가능
                    c.is_available = true;
                    c.available_start = start;
                    c.available_end = end;
                    cout << "Accept completed.\n";
                    break;
                }
            }
            if (!roomFound) cout << ".!! Room not found\n";
        }
        else if (input == 3) { // 6.3.2.3 ban reservation
            cout << "classroom number: ";
            string room; cin >> room;
            bool roomFound = false;
            for (auto& c : classrooms) {
                if (c.room == room) {
                    roomFound = true;
                    cout << "enter a number corresponding to the day of the week\n"
                         << "(1. Mon, 2. Tue, 3. Wed, 4. Thu, 5. Fri): ";
                    int day; cin >> day;
                    if (day < 1 || day > 5) {
                        cout << ".!! Invalid weekday input\n";
                        break;
                    }
                    string start, end;
                    cout << "start ban time: "; cin >> start;
                    cout << "end ban time: "; cin >> end;
                    // 실제로 요일별로 저장하는 구조는 없지만, 전체 시간으로 막는 방식으로 대체
                    c.is_available = false;
                    c.available_start = start;
                    c.available_end = end;
                    cout << "Ban completed.\n";
                    break;
                }
            }
            if (!roomFound) cout << ".!! Room not found\n";
        }
        else {
            cout << ".!! Enter the index number in the menu.\n";
        }

        // 관리자 메뉴로 복귀
        break;
    }
}



// 예약 취소 기능
void cancelReservation(const string& user_id) {
    vector<int> indices;
    for (int i = 0; i < reservations.size(); ++i) {
        if (reservations[i].user_id == user_id) {
            cout << indices.size() + 1 << ". " << reservations[i].room << " " << weekdays[stoi(reservations[i].day) - 1] << " " << reservations[i].start_time << "~" << reservations[i].end_time << endl;
            indices.push_back(i);
        }
    }
    if (indices.empty()) {
        cout << "No reservations to cancel\n";
        return;
    }
    cout << "Enter the number you want to cancel: ";
    int choice; cin >> choice;
    if (choice < 1 || choice > indices.size()) {
        cout << ".!! Invalid input\n";
        return;
    }
    reservations.erase(reservations.begin() + indices[choice - 1]);
    ofstream fout("reservation.txt");
    for (auto& r : reservations) {
        fout << r.user_id << "\t" << r.room << "\t" << r.start_time << "\t" << r.end_time << "\t" << r.day << endl;
    }
    cout << "Reservation canceled\n";
}

// 프로그램 시작
int main() {
    // 파일 로딩 안되면 종료함, 데이터 무결성 파트에서 추가할 예정
    if (!loadUsers() || !loadClassrooms() || !loadReservations()) {
        return 1;
    }

    while (true) {
        cout << "----Classroom Booking Program----\n";
        cout << "1. login\n2. accession\n3. exit\n>> ";
        int sel; cin >> sel;
        if (sel == 1) {
            User* user = nullptr;
            while (!user) user = login();
            if (user->is_admin) {
                cout << "-- Main for manager --\n";
                // 관리자 기능 - 조수빈
                while (true){
                    cout << "1. reservation list and change\n2. classroom situation and change\n3. logout\n>> ";
                    int choice; cin >> choice;
                    if(choice == 1){
                        //예약 목록 출력 및 수정 함수
                    }
                    else if (choice == 2){
                        //강의실 상태 출력 및 수정 함수 호출
                        showAndEditClassroom();
                    }
                    else if (choice == 3){
                        break;
                    }
                    else{
                        cout << ".!! Enter the index number in the mune.";

                    }
                }
            }
            else {
                cout << "-- Main --\n";
                while (true) {
                    cout << "1. classroom list\n2. reserve classroom\n3. cancel reservation\n4. logout\n>> ";
                    int c; cin >> c;
                    if (c == 1) {
                        printClassroomList();
                        cout << "classroom number: ";
                        string room; cin >> room;
                        printTimeTable(room);
                    }
                    else if (c == 2) reserveClassroom(user->id);
                    else if (c == 3) cancelReservation(user->id);
                    else if (c == 4) break;
                }
            }
        }
        else if (sel == 2) {
            // 회원가입
            string id, pw;
            cout << "ID: "; cin >> id;
            cout << "PW: "; cin >> pw;
            users.push_back({ id, pw, false });
            ofstream fout("user.txt", ios::app);
            fout << id << "\t" << pw << "\t0\n";
            cout << "Registration complete\n";
        }
        else if (sel == 3) {
            // 종료 확인
            string confirm;
            cout << "If you want to quit this program, enter 'quit': ";
            cin >> confirm;
            if (confirm == "quit") break;
        }
    }
    return 0;
}
