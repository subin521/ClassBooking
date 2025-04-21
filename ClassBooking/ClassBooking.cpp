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
#include <limits> // numeric_limits 사용을 위해 추가
#include <algorithm> // max 함수 사용을 위해 추가
#include "util.hpp"
#include <regex>
#include <map>

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

// 존재하는 강의실인지 확인
bool isExistRoomNumber(const string& input) {
    for (const Classroom& cls : classrooms) {
        if (cls.room == input) {
            return true;
        }
    }
    return false;
}

// 존재하는 사용자인지 확인 
bool isExistUser(const string& input) {
    for (const User& usr : users) {
        if (usr.id == input) {
            return true;
        }
    }
    return false;
}

// 공백 제거 함수
string removeWhitespace(const string& input) {
    string result;
    for (char ch : input) {
        if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r') {
            result += ch;
        }
    }
    //cout << "[DEBUG] removeWhitespace result: \"" << result << "\"" << endl;
    return result;
}

// 인덱스 입력 유효성 검사
bool checkIdx(string callLocation, string& inputIdx) {
    string cleaned = removeWhitespace(inputIdx);
    cout << "[DEBUG] cleaned: " << cleaned << endl;
    if (cleaned.length() == 1 && isdigit(cleaned[0])) {
        return false;  // 정상 입력
    }
    string errPhrase;
    if (callLocation == "day") errPhrase = "of the day of the week.";
    else if (callLocation == "menu") errPhrase = "in the menu.";
    else errPhrase = ".";
    cout << ".!! Enter the index number " << errPhrase << endl;
    return true;  // 비정상 입력
}

string InputClassroom() {
    string input;

    while (true) {
        cout << "classroom number: ";
        cin >> input;

        // 포맷이 잘못됐거나 존재하지 않는 강의실이면 다시 입력
        if (!validateRoomNumber(input) || !isExistRoomNumber(input)) {
            cout << ".!! The classroom you entered doesn't exist. Please try again.\n";
        }
        else {
            return input;  // 유효하면 반환
        }
    }
}

string InputUser() {
    string input;

    while (true) {
        cout << "ID:";
        cin >> input;

        if (!isExistUser(input)) {
            cout << ".!! ID doesn't exist.\n";
        }
        else { return input; }
    }

}

void printClassroomList() {
    map<string, vector<Classroom>> floorMap;

    // 층별로 강의실 분류
    for (const Classroom& cls : classrooms) {
        if (cls.room.length() >= 1) {
            string floor = cls.room.substr(0, 1); // "3" -> 3층
            floorMap[floor].push_back(cls);
        }
    }

    // 출력
    for (const auto& pair : floorMap) {
        cout << pair.first << "F: ";
        for (size_t i = 0; i < pair.second.size(); ++i) {
            cout << pair.second[i].room;
            if (i != pair.second.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

void printTimeTable(const string& room) {
    // 요일 헤더
    cout << "                 Mon   Tue   Wed   Thu   Fri" << endl;

    for (int t = 0; t < 9; ++t) {
        string t1 = times[t];
        string t2 = times[t + 1];
        string timeLabel = t1 + "~" + t2;

        cout << timeLabel;

        // 정렬 맞추기용 공백 (문자열 길이가 항상 11자 → 13자 맞춤)
        if (timeLabel.length() < 13) cout << "  ";

        for (int d = 1; d <= 5; ++d) {
            bool reserved = false;
            for (const auto& r : reservations) {
                if (r.room == room && stoi(r.day) == d &&
                    isTimeOverlap(r.start_time, r.end_time, t1, t2)) {
                    reserved = true;
                    break;
                }
            }
            cout << "     " << (reserved ? "X" : "O");
        }
        cout << endl;
    }

    cout << "\npress any key to continue ...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
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

//로그아웃
bool logout() {
    string input;
	cout << "Would you like to log out? ";
    cin >> input;

    if (input == "Y") {
        return true;
    }
    else if (input == "N") {
        return false;
    }
    cout << ".!! You can only enter upper case 'Y' or 'N'." << endl;
    return logout();
}

void reserveClassroom(const string& user_id) {
    string day, start, end;
    string room = InputClassroom();

    // --- 요일 입력 유효성 검사 ---
    while (true) {
        cout << "day (1~5): ";
        cin.clear();while (cin.peek() == '\n') cin.ignore();  // 개행만 남은 버퍼 날리기
        getline(cin, day);
        if (checkIdx("day", day)) continue;
        else break;
    }

    // --- 사용자 유형 확인 (admin 여부) ---
    bool is_admin = false;
    for (const auto& u : users) {
        if (u.id == user_id) {
            is_admin = u.is_admin;
            break;
        }
    }

    // --- 시간 입력 유효성 검사 ---
    while (true) {
        cout << "start time (HH:MM): ";
        cin >> start;
        cout << "end time (HH:MM): ";
        cin >> end;

        regex pattern("^([01]?[0-9]|2[0-3]):00$");
        if (!regex_match(start, pattern) || !regex_match(end, pattern)) {
            cout << ".!! Incorrect form: an example of correct input is '14:00'.\n";
            continue;
        }

        int sh = stoi(start.substr(0, 2));
        int eh = stoi(end.substr(0, 2));

        if (sh < 9 || sh > 17) {
            cout << ".!! Incorrect form: start must be between 09:00 and 17:00.\n";
            continue;
        }
        if (eh < 10 || eh > 18) {
            cout << ".!! Incorrect form: end must be between 10:00 and 18:00.\n";
            continue;
        }
        if (sh >= eh) {
            cout << ".!! Start time must be earlier than end time.\n";
            continue;
        }

        if (!is_admin && eh - sh != 1) {
            cout << ".!! General users can only reserve exactly 1 hour.\n";
            continue;
        }

        break;
    }

    // --- 예약 충돌 확인 ---
    for (const auto& r : reservations) {
        if (r.user_id == user_id && r.day == day && isTimeOverlap(r.start_time, r.end_time, start, end)) {
            cout << ".!! Already reserved time\n";
            return;
        }
    }

    // --- 강의실 예약 가능 시간 확인 ---
    for (const auto& c : classrooms) {
        if (c.room == room && c.is_available && c.available_start <= start && c.available_end >= end) {
            reservations.push_back({ user_id, room, day, start, end });
            ofstream fout("reservation.txt", ios::app);
            fout << user_id << "\t" << room << "\t" << start << "\t" << end << "\t" << day << endl;
            fout.close();
            cout << "Reservation completed\n";
            return;
        }
    }

    cout << ".!! This is not a time available for reservation\n";
}

// 관리자 프롬프트에서 예약 내역 조회하는 함수 - 조현승
void reservation_check(){
    ifstream reservation_file("reservation.txt");
    map<string, string> day_map = {
        {"1", "Mon"},
        {"2", "Tue"},
        {"3", "Wed"},
        {"4", "Thu"},
        {"5", "Fri"},
    };
    string line;
    bool found = false;
    while (getline(reservation_file, line)) {
        stringstream ss(line);
        string user_id, class_num_str, start_time, end_time, day_str;

        getline(ss, user_id, '\t');
        getline(ss, class_num_str, '\t');
        getline(ss, start_time, '\t');
        getline(ss, end_time, '\t');
        getline(ss, day_str, '\t');

        cout << user_id << " "
             << class_num_str << " " 
             << day_map[day_str] << " "
             << start_time << " " << end_time << " " << endl;
        
        found = true; // 예약 정보를 찾았으므로 true로 설정
    }
    
    reservation_file.close();

    if (!found) {
        cout << "No reservation history found." << endl;
    }

    cout << "\npress any key to continue..." << endl;
    cin.get();

}

// 문자열의 앞이나 뒤에 공백이 있는지 확인 -조현승- 
bool has_leading_or_trailing_space(const string& str) {
    return (!str.empty() && isspace(str.front())) || (!str.empty() && isspace(str.back()));
}

// 사용자 ID가 유효한지 확인 (소문자, 숫자만 허용, 길이 제한) -조현승- 
bool is_valid_user_id(const string& id) {
    if (id.length() < 3 || id.length() > 20) {
        return false;
    }
    for (char c : id) {
        if (!islower(c) && !isdigit(c)) {
            return false;
        }
    }
    return true;
}

// user.txt 파일에서 해당 ID가 존재하는지 확인 -조현승- 
bool does_user_exist(const string& user_id) {
    ifstream user_file("user.txt");
    bool user_exists = false;
    if (user_file.is_open()) {
        string line;
        while (getline(user_file, line)) {
            stringstream ss(line);
            string file_user_id, password, is_admin;

            getline(ss, file_user_id, '\t');
            getline(ss, password, '\t');
            getline(ss, is_admin, '\t');

            if (file_user_id == user_id) {
                user_exists = true;
                break;
            }
        }
        user_file.close();
    } else {
        cout << "Error: Could not open user.txt file." << endl;
        return false;
    }
    return user_exists;
}
//관리자 프롬프트에서 예약 내역 삭제하는 함수 - 조현승
void reservation_delete(){
    string user_id_to_cancel;
    bool valid_id = false;

    // 유효한 사용자 ID를 입력받을 때까지 반복
    while (!valid_id) {
        cout << "Enter your ID: ";
        getline(cin, user_id_to_cancel);

        // 공백, 형식, 존재 여부 확인
        if (has_leading_or_trailing_space(user_id_to_cancel) || 
            !is_valid_user_id(user_id_to_cancel) || 
            !does_user_exist(user_id_to_cancel)) {
            cout << "!! Invalid or non-existent ID." << endl;
        } else {
            valid_id = true;
        }
    }

    ifstream in_file("reservation.txt");
    vector<string> user_reservations;

    if (in_file.is_open()) {
        string line;
        int reservation_number = 1;

        // 사용자 ID와 일치하는 예약 정보 출력
        while (getline(in_file, line)) {
            stringstream ss(line);
            string user_id, class_num_str, start_time, end_time, day_str;

            getline(ss, user_id, '\t');
            getline(ss, class_num_str, '\t');
            getline(ss, start_time, '\t');
            getline(ss, end_time, '\t');
            getline(ss, day_str, '\t');

            if (user_id == user_id_to_cancel) {
                cout << reservation_number++ << ". " << class_num_str << " ";
                if (day_str == "1") cout << "Mon";
                else if (day_str == "2") cout << "Tue";
                else if (day_str == "3") cout << "Wed";
                else if (day_str == "4") cout << "Thu";
                else if (day_str == "5") cout << "Fri";
                cout << " " << start_time << "-" << end_time << endl;

                user_reservations.push_back(line);
            }
        }
        in_file.close();

        // 해당 ID의 예약이 없을 경우
        if (user_reservations.empty()) {
            cout << "No reservations found for this ID." << endl;
        }

        int cancel_choice;
        bool valid_choice = false;

        // 유효한 번호를 입력받을 때까지 반복
        while (!valid_choice) {
            cout << "Enter the reservation number you want to cancel: ";
            cin >> cancel_choice;
            cin.ignore(); // 버퍼 비우기

            // 선택된 예약 삭제
            if (cancel_choice >= 1 && cancel_choice <= user_reservations.size()) {
                valid_choice = true;
                string reservation_to_cancel = user_reservations[cancel_choice - 1];

                ifstream read_file("reservation.txt");
                ofstream write_file("reservation_temp.txt");
                if (read_file.is_open() && write_file.is_open()) {
                    string current_line;
                    bool canceled = false;

                    while (getline(read_file, current_line)) {
                        if (current_line != reservation_to_cancel) {
                            write_file << current_line << endl;
                        } else {
                            canceled = true;
                        }
                    }

                    read_file.close();
                    write_file.close();

                    // 기존 파일 삭제 및 새 파일로 변경
                    remove("reservation.txt");
                    rename("reservation_temp.txt", "reservation.txt");

                    if (canceled) {
                        cout << "Reservation canceled successfully." << endl;
                    } else {
                        cout << "Error: Reservation not found." << endl;
                    }

                } else {
                    cout << "Error opening reservation file for update." << endl;
                }
            } else {
                cout << "!! Please enter a valid reservation number." << endl;
            }
        }
    } else {
        cout << "Error: Could not open reservation.txt file." << endl;
    }
}

//예약 목록 출력 및 수정 함수 호출 
void showListAndEditReservation() {
    while (true) {
        cout << "1. register reservation\n2. check reservation\n3. delete reservation\n>> ";
        int input; cin >> input;

        if(input == 1) { //예약자ID, 강의실 호수, 예약 시간을 입력 받고 등록

        }
        else if(input == 2) { //예약 내역 리스트 출력 6.2.1 reservation.txt
            reservation_check();
        }
        else if(input == 3) { //id를 입력받아 해당 사용자의 내약 내역 출력, 예약된 강의실 취소
            reservation_delete();
        }
        else{
            cout << ".!! Enter the index number in the menu.\n";
        }
    }
}


//강의실 상태 출력 및 수정 함수- 조수빈
void showAndEditClassroom(const string& admin_id) {
    while (true) {
        cout << "1. check reservation\n2. accept reservation\n3. ban reservation\n>> ";
        string input;
        cin.clear();while (cin.peek() == '\n') cin.ignore();  // 개행만 남은 버퍼 날리기
        getline(cin, input);

        //if (cin.fail()) {
        //    cin.clear();
        //    cin.ignore(1000, '\n');
        //    cout << ".!! Enter the index number in the menu.\n";
        //    continue;
        //}
        if (checkIdx("menu",input)) continue;

        if (stoi(input) == 1) { // 6.3.2.1 check reservation
            printClassroomList();
            string room = InputClassroom();
            printTimeTable(room);
        }
        else if (stoi(input) == 2) { // 6.3.2.2 accept reservation -> reservation.txt 예약 전체 허용 가능하게
            string room = InputClassroom();
            bool roomFound = false;
            for (auto& c : classrooms) {
                if (c.room == room) {
                    roomFound = true;
                    string day;
                    while (true) {
                        cout << "enter a number corresponding to the day of the week\n"
                            << "(1. Mon, 2. Tue, 3. Wed, 4. Thu, 5. Fri): ";
                        getline(cin, day);
                        if (checkIdx("day",day)) continue;
                        int dayInt = stoi(day);
                        if (dayInt >= 1 && dayInt <= 5) break;

                    }
                    
                    string start, end;
                    cout << "start accept time: "; cin >> start;
                    cout << "end accept time: "; cin >> end;

                    vector<Reservation> new_reservations;

                    // 시간 확인 검사 추가해야함.
                    // 기존 예약 리스트 순회하면서 금지(관리자) 예약 중 허용시간대와 겹치는 부분 수정
                    for (auto it = reservations.begin(); it != reservations.end(); ) {
                        if (it->user_id == admin_id &&
                            it->room == room &&
                            it->day == day &&
                            isTimeOverlap(it->start_time, it->end_time, start, end)) {
        
                            // 분할된 금지 시간으로 재생성
                            if (it->start_time < start) {
                                new_reservations.push_back({ admin_id, room, day, it->start_time, start });
                            }
                            if (it->end_time > end) {
                                new_reservations.push_back({ admin_id, room, day, end, it->end_time });
                            }
        
                            // 기존 금지 예약 삭제
                            it = reservations.erase(it);
                        }
                        else {
                            ++it;
                        }
                    }
        
                    // 새로운 금지 예약 추가
                    for (const auto& r : new_reservations) {
                        reservations.push_back(r);
                    }
        
                    // 파일 전체 갱신
                    ofstream fout("reservation.txt");
                    for (const auto& r : reservations) {
                        fout << r.user_id << "\t" << r.room << "\t" << r.start_time << "\t"
                             << r.end_time << "\t" << r.day << endl;
                    }
        
                    cout << "Accept completed.\n";
                    break;
                }
            }
                
            if (!roomFound) cout << ".!! Room not found\n";
        }
        else if (stoi(input) == 3) { // 6.3.2.3 ban reservation 예약 금지
            string room = InputClassroom();
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
                    reservations.push_back({admin_id, room, to_string(day), start, end});
                    ofstream fout("reservation.txt", ios::app);
                    fout << admin_id << "\t" << room << "\t" << start << "\t" << end << "\t" << day << endl;
                    cout << "Ban completed.\n";
                    break;
                }
            }
            if (!roomFound) cout << ".!! Room not found\n";
        }
        else {
            cout << ".!! Enter the index number in the menu.\n";
        }
        break;
    }
}

// 관리자가 강의실 예약하는 함수
void adminReserveClassroom() {
    string user_id = InputUser();
    reserveClassroom(user_id);
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
        cout << ".!! Enter the index number in the menu.\n";
        return;
    }
    reservations.erase(reservations.begin() + indices[choice - 1]);
    ofstream fout("reservation.txt");
    for (auto& r : reservations) {
        fout << r.user_id << "\t" << r.room << "\t" << r.start_time << "\t" << r.end_time << "\t" << r.day << endl;
    }
    cout << "Reservation canceled.\n";
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
        string sel;
        cin.clear();while (cin.peek() == '\n') cin.ignore();  // 개행만 남은 버퍼 날리기
        getline(cin, sel);
		if (checkIdx("menu", sel)) continue; // 인덱스 입력 유효성 검사
        int idx = stoi(sel);

        if (idx == 1) {
            User* user = nullptr;
            while (!user) user = login();
            if (user->is_admin) {
                cout << "-- Main for manager --\n";
                // 관리자 기능 - 조수빈
                while (true){
                    cout << "1. reservation list and change\n2. classroom situation and change\n3. logout\n>> ";
                    string choice;
                    cin.clear();while (cin.peek() == '\n') cin.ignore();  // 개행만 남은 버퍼 날리기
					getline(cin, choice);
					if (checkIdx("menu", choice)) continue; // 인덱스 입력 유효성 검사
					int idx_managerP = stoi(choice);
                    if(idx_managerP == 1){
                        //예약 목록 출력 및 수정 함수 호출
                        adminReserveClassroom();
                    }
                    else if (idx_managerP == 2){
                        //강의실 상태 출력 및 수정 함수 호출
                        showAndEditClassroom(user -> id);
                    }
                    else if (idx_managerP == 3){
                        break;
                    }
                    //else{
                    //    cout << ".!! Enter the index number in the mune.\n";

                    //}
                }
            }
            else {
                cout << "-- Main --\n";
                while (true) {
                    cout << "1. classroom list\n2. reserve classroom\n3. cancel reservation\n4. logout\n>> ";
                    cin.clear();while (cin.peek() == '\n') cin.ignore();  // 개행만 남은 버퍼 날리기
                    string c;
                    
					getline(cin, c);
					if (checkIdx("menu", c)) continue; // 인덱스 입력 유효성 검사
					int idx_userP = stoi(c);
                    if (idx_userP == 1) {
                        printClassroomList();
                        string room = InputClassroom();
                        printTimeTable(room);
                    }
                    else if (idx_userP == 2) reserveClassroom(user->id);
                    else if (idx_userP == 3) cancelReservation(user->id);
                    else if (idx_userP == 4) {
                        if (logout()) break;
                    }
                }
            }
        }
        //else if (sel == 2) {
        //    // 회원가입
        //    string id, pw;
        //    cout << "ID: "; cin >> id;
        //    cout << "PW: "; cin >> pw;
        //    users.push_back({ id, pw, false });
        //    ofstream fout("user.txt", ios::app);
        //    fout << id << "\t" << pw << "\t0\n";
        //    cout << "Registration complete\n";
        //}
        else if (idx == 2) {
            // 회원가입
            string id, pw;
            bool valid = false;

            while (!valid) {
                cout << "ID: ";
                cin >> id;

                // ID 유효성 검사 (길이, 문자 종류)
                if (id.length() < 3 || id.length() > 20) {
                    cout << ".!! Incorrect form: ID must be between 3 and 20 characters.\n";
                    continue;
                }

                bool isValidId = true;
                for (char c : id) {
                    if (!(islower(c) || isdigit(c))) {
                        isValidId = false;
                        break;
                    }
                }
                if (!isValidId) {
                    cout << ".!! Incorrect form: use only lowercase Engilish and number.\n";
                    continue;
                }

                // 중복 체크
                bool duplicated = false;
                for (const auto& u : users) {
                    if (u.id == id) {
                        duplicated = true;
                        break;
                    }
                }

                if (duplicated) {
                    cout << ".!! ID already exists.\n";
                    continue;
                }

                // 비밀번호 입력
                cout << "PW: ";
                cin >> pw;

                // 비밀번호 유효성 검사
                if (pw.length() < 4 || pw.length() > 20) {
                    cout << ".!! Incorrect form: password must be 4~20 characters.\n";
                    continue;
                }
                if (pw.find(' ') != string::npos) {
                    cout << ".!! Incorrect form: space not allowed in password.\n";
                    continue;
                }

                bool hasAlpha = false, hasDigit = false;
                for (char c : pw) {
                    if (isalpha(c)) hasAlpha = true;
                    if (isdigit(c)) hasDigit = true;
                }

                if (!hasAlpha || !hasDigit) {
                    cout << ".!! Incorrect form: password must include alphabet and number.\n";
                    continue;
                }

                // 통과 시 등록
                valid = true;
                users.push_back({ id, pw, false });
                ofstream fout("user.txt", ios::app);
                fout << id << "\t" << pw << "\t0\n";
                fout.close();
                cout << "Registration complete\n";
            }
        }
        else if (idx == 3) {
            // 종료 확인
            string confirm;
            cout << "If you want to quit this program, enter 'quit': ";
            cin >> confirm;
            if (confirm == "quit") break;
        }
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << ".!! Enter the index number in the menu.\n";
            continue;
        }
    }
    return 0;
    //
}