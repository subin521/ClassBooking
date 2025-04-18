/*
* 2025 04 14
* �Ϲ� ����� ���� ������ ����
*/


// �⺻ �����, ���������, ���ڿ�ó��, ���� ���̺귯�� ������
#include <iostream>// ����� ��� ���
#include <fstream>// ���� ����� ���
#include <sstream> // ���ڿ� ��Ʈ�� ó����
#include <string>  // ���ڿ� Ÿ�� ���
#include <vector> // ���� �迭 ���� ���
#include <iomanip>// ��� ���Ŀ� (setw �� �� �ʿ�)
#include <map>

using namespace std;

// ����� ����ü
struct User {
    string id;
    string password;
    bool is_admin;
};

// ���� ����ü
struct Reservation {
    string user_id;
    string room;
    string day;
    string start_time;
    string end_time;
};

// ���ǽ� ����ü
struct Classroom {
    string room;
    bool is_available;
    string available_start;
    string available_end;
};

// ���ͷ� ����, ����, ���ǽ� ������, �̰� ���� ������
vector<User> users;
vector<Reservation> reservations;
vector<Classroom> classrooms;

// ���� ����Ʈ
vector<string> weekdays = { "Mon", "Tue", "Wed", "Thu", "Fri" };
// �ð��� ����Ʈ��, 1�ð� ����
vector<string> times = {
    "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00", "17:00", "18:00"
};

// �ð� ��ġ���� Ȯ���ϴ� �Լ�
bool isTimeOverlap(const string& s1, const string& e1, const string& s2, const string& e2) {
    return !(e1 <= s2 || s1 >= e2);
}

// ���ǽ� �ҷ����� �Լ�
bool loadClassrooms() {
    ifstream fin("classroom.txt");
    if (!fin) return false;
    string room, start, end;
    int avail;
    while (fin >> room >> avail >> start >> end) {
        bool available_flag = (avail != 0);  // int�� bool�� �ٲ�
        classrooms.push_back({ room, available_flag, start, end });
    }
    return true;
}

// ���� �ҷ����� �Լ�
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

// ���� �ҷ����� �Լ�
bool loadReservations() {
    ifstream fin("reservation.txt");
    if (!fin) return false;
    string id, room, start, end, day;
    while (fin >> id >> room >> start >> end >> day) {
        reservations.push_back({ id, room, day, start, end });
    }
    return true;
}

// ���ǽ� ������ �������, �״�� �ᵵ �ɵ�
void printClassroomList() {
    cout << "3F: "; for (auto& c : classrooms) if (c.room[0] == '3') cout << c.room << ", "; cout << endl;
    cout << "4F: "; for (auto& c : classrooms) if (c.room[0] == '4') cout << c.room << ", "; cout << endl;
    cout << "5F: "; for (auto& c : classrooms) if (c.room[0] == '5') cout << c.room << ", "; cout << endl;
    cout << "6F: "; for (auto& c : classrooms) if (c.room[0] == '6') cout << c.room << ", "; cout << endl;
}

// ���ǽ� �ð�ǥ �����, �׽�Ʈ ������ �����ѰŶ� ���� �ʿ�
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
    cin.ignore(); cin.get();  // �ƹ� Ű ���
}

// �α��� ���, �׽�Ʈ ������ �����ѰŶ� ���� �ʿ�
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

// ���ǽ� �����ϴ� �Լ� ----------++ �����ڰ� ���� ���� �� �ð��̶� �Ȱ�ġ���� Ȯ�� �ϴ� ���� �߰� �ʿ�.
void reserveClassroom(const string& user_id) {
    string room, day, start, end;
    cout << "classroom number: "; cin >> room;
    cout << "day(1~5): "; cin >> day;
    cout << "start time(HH:MM): "; cin >> start;
    cout << "end time(HH:MM): "; cin >> end;

    // ��ġ�� �ð� �����ߴ��� üũ
    for (const auto& r : reservations) {
        if (r.user_id == user_id && r.day == day && isTimeOverlap(r.start_time, r.end_time, start, end)) {
            cout << ".!! Already reserved time\n";
            return;
        }
    }

    // ���ǽ� ���� ���ɽð� �ȿ� �ִ��� Ȯ��
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

//관리자 프롬프트에서 예약 조회하는 함수 - 조현승
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
        
    }
    
    reservation_file.close();

    if (!found) {
        cout << "해당 ID로 예약된 정보가 없습니다." << endl;
    }

    cout << "\npress any key to continue..." << endl;
    cin.get();

}
// 문자열의 처음과 끝에 공백이 있는지 확인하는 함수 - 조현승
bool has_leading_or_trailing_space(const string& str) {
    return (!str.empty() && isspace(str.front())) || (!str.empty() && isspace(str.back()));
}

// 사용자 ID의 유효성을 검사하는 함수 - 조현승
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

// user.txt를 읽어서 있는 사용자인지 확인하는 함수 - 조현승
bool does_user_exist(const string& user_id) {
    ifstream user_file("user.txt");
    bool user_exists = false;
    if (user_file.is_open()) {
        string line;
        while (getline(user_file, line)) {
            stringstream ss(line);
            string file_user_id;
            string password;
            string is_admin;

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
        cout << "오류: user.txt 파일을 열 수 없습니다." << endl;
        return false; // 파일 열기 실패 시 사용자 없음으로 처리
    }
    return user_exists;
}

//관리자 프롬프트에서 예약 취소하는 함수
void reservation_delete(){
    string user_id_to_cancel; //취소할 user_id
    bool valid_id = false; //유효성 검사용
    while (!valid_id) {
        cout << "ID:";
        getline(cin, user_id_to_cancel);
        //공백 확인, 문법 규칙 확인, reservation.txt 에 있는지
        if (has_leading_or_trailing_space(user_id_to_cancel) || !is_valid_user_id(user_id_to_cancel) || !does_user_exist(user_id_to_cancel)) {
            cout << ".!! ID doesn’t exist." << endl;
        } else {
            valid_id = true;
        }
    }

    ifstream in_file("reservation.txt");
    vector<string> user_reservations;
    if (in_file.is_open()) {
        string line;
        int reservation_number = 1; //예약 조회하면서 순서대로 1번,2번
        while (getline(in_file, line)) {
            stringstream ss(line);
            string user_id, class_num_str, start_time, end_time, day_str;

            getline(ss, user_id, '\t');
            getline(ss, class_num_str, '\t');
            getline(ss, start_time, '\t');
            getline(ss, end_time, '\t');
            getline(ss, day_str, '\t');

            if (user_id == user_id_to_cancel) { //입력한 ID의 예약 내역 출력
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

        int cancel_choice;
        bool valid_choice = false;
        while (!valid_choice) {
            cout << "Enter the classroom number you want to cancel: ";
            cin >> cancel_choice;
            cin.ignore(); // 버퍼 비우기

            //삭제하고자 하는 예약번호가 유효해야함
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
                } 
            } else {
                cout << "!! Enter the index number in the menu." << endl;
            }
        }
    }
}
//���� ��� ��� �� ���� �Լ� ȣ��
void showListAndEditReservation() {
    while (true) {
        cout << "1. register reservation\n2. checkreservation\n3. delete reservation\n>> ";
        int input; cin >> input;

        if(input == 1) { //������ID, ���ǽ� ȣ��, ���� �ð��� �Է� �ް� ���
        
        }
        else if(input == 2) { //���� ���� ����Ʈ ��� 6.2.1 reservation.txt
            
        }
        else if(input == 3) { //id�� �Է¹޾� �ش� ������� ���� ���� ���, ����� ���ǽ� ���
        
        }
        else{
            cout << ".!! Enter the index number in the menu.\n";
        }
    }
}


//���ǽ� ���� ��� �� ���� �Լ�- ������
void showAndEditClassroom(const string& admin_id) {
    while (true) {
        cout << "1. check reservation\n2. accept reservation\n3. ban reservation\n>> ";
        int input; cin >> input;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << ".!! Enter the index number in the menu.\n";
            continue;
        }

        if (input == 1) { // 6.3.2.1 check reservation
            printClassroomList();
            cout << "classroom number: ";
            string room; cin >> room;
            printTimeTable(room);
        }
        else if (input == 2) { // 6.3.2.2 accept reservation -> reservation.txt ���� ��ü ��� �����ϰ�
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

                    vector<Reservation> new_reservations;

                    // �ð� Ȯ�� �˻� �߰��ؾ���.
                    // ���� ���� ����Ʈ ��ȸ�ϸ鼭 ����(������) ���� �� ���ð���� ��ġ�� �κ� ����
                    for (auto it = reservations.begin(); it != reservations.end(); ) {
                        if (it->user_id == admin_id &&
                            it->room == room &&
                            it->day == to_string(day) &&
                            isTimeOverlap(it->start_time, it->end_time, start, end)) {
        
                            // ���ҵ� ���� �ð����� �����
                            if (it->start_time < start) {
                                new_reservations.push_back({ admin_id, room, to_string(day), it->start_time, start });
                            }
                            if (it->end_time > end) {
                                new_reservations.push_back({ admin_id, room, to_string(day), end, it->end_time });
                            }
        
                            // ���� ���� ���� ����
                            it = reservations.erase(it);
                        }
                        else {
                            ++it;
                        }
                    }
        
                    // ���ο� ���� ���� �߰�
                    for (const auto& r : new_reservations) {
                        reservations.push_back(r);
                    }
        
                    // ���� ��ü ����
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
        else if (input == 3) { // 6.3.2.3 ban reservation ���� ����
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
                    // ������ ���Ϻ��� �����ϴ� ������ ������, ��ü �ð����� ���� ������� ��ü
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



// ���� ��� ���
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

// ���α׷� ����
int main() {
    // ���� �ε� �ȵǸ� ������, ������ ���Ἲ ��Ʈ���� �߰��� ����
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
                // ������ ��� - ������
                while (true){
                    cout << "1. reservation list and change\n2. classroom situation and change\n3. logout\n>> ";
                    int choice; cin >> choice;
                    if(choice == 1){
                        //���� ��� ��� �� ���� �Լ� ȣ��
                        
                    }
                    else if (choice == 2){
                        //���ǽ� ���� ��� �� ���� �Լ� ȣ��
                        showAndEditClassroom(user -> id);
                    }
                    else if (choice == 3){
                        break;
                    }
                    else{
                        cout << ".!! Enter the index number in the mune.\n";

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
        //else if (sel == 2) {
        //    // ȸ������
        //    string id, pw;
        //    cout << "ID: "; cin >> id;
        //    cout << "PW: "; cin >> pw;
        //    users.push_back({ id, pw, false });
        //    ofstream fout("user.txt", ios::app);
        //    fout << id << "\t" << pw << "\t0\n";
        //    cout << "Registration complete\n";
        //}
        else if (sel == 2) {
            // ȸ������
            string id, pw;
            bool valid = false;

            while (!valid) {
                cout << "ID: ";
                cin >> id;

                // ID ��ȿ�� �˻� (����, ���� ����)
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

                // �ߺ� üũ
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

                // ��й�ȣ �Է�
                cout << "PW: ";
                cin >> pw;

                // ��й�ȣ ��ȿ�� �˻�
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

                // ��� �� ���
                valid = true;
                users.push_back({ id, pw, false });
                ofstream fout("user.txt", ios::app);
                fout << id << "\t" << pw << "\t0\n";
                fout.close();
                cout << "Registration complete\n";
            }
        }
        else if (sel == 3) {
            // ���� Ȯ��
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
}
