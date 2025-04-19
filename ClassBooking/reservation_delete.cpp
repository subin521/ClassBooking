//reseration_delete
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype> // islower, isdigit
#include <iomanip>

using namespace std;

// 문자열의 처음과 끝에 공백이 있는지 확인하는 함수
bool has_leading_or_trailing_space(const string& str) {
    return (!str.empty() && isspace(str.front())) || (!str.empty() && isspace(str.back()));
}

// 사용자 ID의 유효성을 검사하는 함수
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

int main() {
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
    return 0;
}