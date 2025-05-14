// admin.cpp
#include "admin.hpp"
#include "models.hpp"
#include "util.hpp"
#include "classroom.hpp"
#include "reservation.hpp"
#include "user.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

// 관리자: 예약 목록 관리 메뉴
void reservationManagementMenu()
{
    cout << "1. register reservation\n";
    cout << "2. check reservation\n";
    cout << "3. delete reservation\n>> ";
    
    int idx = getValidatedMenuIndex("menu", 1, 3);


    if (idx == 1){
        string uid = InputUser();
        reserveClassroom(uid);
    }   
    else if (idx == 2)
        reservation_check_admin1();
    else if (idx == 3)
        cancelReservation(InputUser());
}

// 관리자: 강의실 예약 허용/금지 메뉴
void classroomManagementMenu(const string &admin_id)
{
    cout << "1. check reservation\n";
    cout << "2. accept reservation\n";
    cout << "3. ban reservation\n>> ";
    int idx = getValidatedMenuIndex("menu", 1, 3);

    if (idx == 1)
        viewClassroomTimetable();
    else if (idx == 2 || idx == 3)
        handleClassroomAccess(admin_id, idx == 2);
}

// 모든 예약 내역 출력 (관리자용)
void reservation_check_admin1()
{
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
    while (getline(reservation_file, line))
    {
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

    if (!found)
    {
        cout << "No reservation history found." << endl;
    }

    cout << "\npress any key to continue..." << endl;
    cin.get();
}

// 사용자/관리자 공통 예약 취소
void cancelReservationMenu(const string &current_user_id)
{
    string user_id = trimWhitespace(current_user_id); // 공백 제거
    bool is_admin = false;

    // 관리자 여부 확인
    for (const auto &u : users)
    {
        if (u.id == user_id && u.is_admin)
        {
            is_admin = true;
            break;
        }
    }

    // 관리자라면 ID 재입력 요구
    if (is_admin)
    {
        while (true)
        {
            cout << "Enter ID to cancel reservation: ";
            getline(cin, user_id);
            user_id = trimWhitespace(user_id);

            if (user_id == "admin01")
            {
                cout << ".!! Admin ID cannot cancel reservations.\n";
                return;
            }
            if (!isValidID(user_id) || !isExistUser(user_id))
            {
                cout << ".!! Invalid or non-existent ID.\n";
                continue;
            }
            break;
        }
    }

    bool success = false;
    while (!success)
        success = cancelReservation(user_id);
}

// 관리자: 강의실 예약 허용 또는 금지 설정
void handleClassroomAccess(const string &admin_id, bool is_accept_mode)
{
    string room = InputClassroom();

    string day;
    while (true)
    {
        cout << "enter a number corresponding to the day of the week\n"
             << "(1. Mon, 2. Tue, 3. Wed, 4. Thu, 5. Fri): ";
        getline(cin, day);
        if (checkIdx("day", day))
            continue;
        if (stoi(day) < 1 || stoi(day) > 5)
        {
            printIdxErrorMessage("day");
            continue;
        }
        break;
    }

    string start, end;
    while (true)
    {
        cout << (is_accept_mode ? "Start accept time: " : "Start ban time: ");
        getline(cin, start);
        start = trimWhitespace(start);
        cout << (is_accept_mode ? "End accept time: " : "End ban time: ");
        getline(cin, end);
        end = trimWhitespace(end);

        if (start.length() != 5 || end.length() != 5)
        {
            cout << ".!! Incorrect form: must be exactly HH:00 format (e.g., 09:00, 14:00).\n";
            continue;
        }

        if (!isValidTime(start) || !isValidTime(end))
        {
            cout << ".!! Incorrect form: an example of correct input is '14:00'. Please enter again.\n";
            continue;
        }
        if (timeToMinutes(start) >= timeToMinutes(end))
        {
            cout << ".!! Start time must be earlier than end time.\n";
            continue;
        }

        int sh = stoi(start.substr(0, 2));
        int eh = stoi(end.substr(0, 2));
        if (sh < 9 || eh > 18)
        {
            cout << ".!! Reservation time must be between 09:00 and 18:00.\n";
            continue;
        }
        break;
    }

    // 허용 시간 설정이면 기존 금지 시간에서 해당 시간 제거
    if (is_accept_mode)
    {
        vector<Reservation> updated;
        for (auto it = reservations.begin(); it != reservations.end();)
        {
            if (it->user_id == admin_id && it->room == room && it->day == day &&
                isTimeOverlap(it->start_time, it->end_time, start, end))
            {

                if (it->start_time < start)
                    updated.push_back({admin_id, room, day, it->start_time, start});
                if (it->end_time > end)
                    updated.push_back({admin_id, room, day, end, it->end_time});

                it = reservations.erase(it);
            }
            else
            {
                ++it;
            }
        }
        reservations.insert(reservations.end(), updated.begin(), updated.end());
        cout << "Accept time updated.\n";
    }
    // 금지 시간 추가
    else
    {
        bool duplicate = false;
        for (const auto &r : reservations)
        {
            if (r.user_id == admin_id && r.room == room && r.day == day &&
                r.start_time == start && r.end_time == end)
            {
                duplicate = true;
                break;
            }
        }
        if (!duplicate)
        {
            reservations.push_back({admin_id, room, day, start, end});
            cout << "Ban completed.\n";
        }
        else
        {
            cout << ".!! This ban reservation already exists.\n"; // 경고 띄우기
            return;
        }
    }

    vector<Reservation> adminBans;
    for (const auto &r : reservations)
    {
        if (r.user_id == admin_id)
            adminBans.push_back(r);
    }

    vector<Reservation> mergedBans;
    for (const auto &res : adminBans)
    {
        if (mergedBans.empty() || mergedBans.back().room != res.room || mergedBans.back().day != res.day || mergedBans.back().end_time < res.start_time)
        {
            mergedBans.push_back(res);
        }
        else
        {
            mergedBans.back().end_time = max(mergedBans.back().end_time, res.end_time);
        }
    }

    vector<Reservation> newReservations;
    for (const auto &r : reservations)
    {
        if (r.user_id != admin_id)
            newReservations.push_back(r);
    }
    for (const auto &r : mergedBans)
        newReservations.push_back(r);

    reservations = newReservations;
    // reservation.txt 저장
    ofstream fout("reservation.txt");
    for (const auto &r : reservations)
    {
        fout << r.user_id << "\t" << r.room << "\t"
             << r.start_time << "\t" << r.end_time << "\t" << r.day << endl;
    }
    fout.close();
}
