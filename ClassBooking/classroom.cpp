#include "classroom.hpp"
#include "models.hpp"
#include "util.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <map>
#include <limits>

using namespace std;

// classroom.txt에서 강의실 정보 불러오기
bool loadClassrooms()
{
    ifstream fin("classroom.txt");
    // ifstream fin("../../ClassBooking/ClassBooking/classroom.txt"); // release .exe용

    // 파일이 없는 경우
    if (!fin)
    {
        cerr << "[Warning] classroom.txt file not found. Creating default classrooms...\n";
        ofstream fout("classroom.txt");
        if (!fout)
        {
            cerr << "[Error] Failed to create classroom.txt.\n";
            exit(1);
        }
        fout << "301 1 09:00 18:00\n"
             << "302 1 09:00 18:00\n"
             << "401 1 09:00 18:00\n"
             << "402 1 09:00 18:00\n"
             << "501 1 09:00 18:00\n"
             << "502 1 09:00 18:00\n"
             << "601 1 09:00 18:00\n"
             << "602 1 09:00 18:00\n";
        fout.close();
        fin.open("classroom.txt");
        if (!fin)
        {
            cerr << "[Error] Failed to open classroom.txt after creation.\n";
            exit(1);
        }
    }

    // 파일이 존재하지만 비어있는 경우
    fin.seekg(0, ios::end);
    if (fin.tellg() == 0)
    {
        fin.close();
        cerr << "[Warning] classroom.txt is empty. Inserting default classrooms...\n";
        ofstream fout("classroom.txt");
        if (!fout)
        {
            cerr << "[Error] Failed to recreate classroom.txt.\n";
            exit(1);
        }
        fout << "301 1 09:00 18:00\n"
             << "302 1 09:00 18:00\n"
             << "401 1 09:00 18:00\n"
             << "402 1 09:00 18:00\n"
             << "501 1 09:00 18:00\n"
             << "502 1 09:00 18:00\n"
             << "601 1 09:00 18:00\n"
             << "602 1 09:00 18:00\n";
        fout.close();
        fin.open("classroom.txt");
        if (!fin)
        {
            cerr << "[Error] Failed to open classroom.txt after recreating.\n";
            exit(1);
        }
    }
    fin.seekg(0);

    string room, start, end;
    int avail;
    int lineNum = 0;

    while (fin >> room >> avail >> start >> end)
    {
        ++lineNum;

        if (room.empty())
        {
            cerr << "[Error] Line " << lineNum << ": Missing classroom name.\n";
            exit(1);
        }

        try
        {
            int roomNum = stoi(room);
            if (roomNum < 101 || roomNum > 1909)
            {
                cerr << "[Error] Line " << lineNum << ": Classroom number must be between 101 and 1909. (got " << roomNum << ")\n";
                exit(1);
            }
        }
        catch (const exception &)
        {
            cerr << "[Error] Line " << lineNum << ": Classroom name must be a valid number.\n";
            exit(1);
        }

        if (avail != 0 && avail != 1)
        {
            cerr << "[Error] Line " << lineNum << ": Availability must be 0 or 1. (got " << avail << ")\n";
            exit(1);
        }

        if (!isValidTime(start) || !isValidTime(end))
        {
            cerr << "[Error] Line " << lineNum << ": Invalid time format (must be HH:00).\n";
            exit(1);
        }

        if (start >= end)
        {
            cerr << "[Error] Line " << lineNum << ": Start time must be earlier than end time.\n";
            exit(1);
        }

        classrooms.push_back({room, avail != 0, start, end});
    }

    fin.close();
    return true;
}

// 층별로 강의실 목록 출력
void printClassroomList() {
    map<string, vector<string>> floorMap;

    for (const auto& c : classrooms) {
        if (c.room.length() >= 1) {
            string floor = c.room.substr(0, 1); // "3" → 3층
            floorMap[floor].push_back(c.room);
        }
    }

    for (const auto& pair : floorMap) {
        cout << pair.first << "F: ";
        for (size_t i = 0; i < pair.second.size(); ++i) {
            cout << pair.second[i];
            if (i != pair.second.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}

// 특정 강의실의 시간표 출력
void printTimeTable(const string& room) {
    cout << "                 Mon   Tue   Wed   Thu   Fri" << endl;

    for (int t = 0; t < 9; ++t) {
        string t1 = times[t];
        string t2 = times[t + 1];
        string timeLabel = t1 + "~" + t2;

        cout << timeLabel;
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

    cout << "\npress any key to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// 강의실 번호를 입력받고 존재하는지 확인
string InputClassroom()
{
    string input;

    while (true)
    {
        cout << "classroom number: ";

        cin.clear();
        while (cin.peek() == '\n')
            cin.ignore();

        getline(cin, input);

        input = trim(input);

        if (!validateRoomNumber(input) || !isExistRoomNumber(input))
        {
            cout << ".!! The classroom you entered doesn't exist. Please try again.\n";
        }
        else
        {
            return input;
        }
    }
}


// 강의실 목록 + 시간표 보기 (사용자/관리자 공통)
void viewClassroomTimetable() {
    printClassroomList();
    string room = InputClassroom();
    printTimeTable(room);
}
