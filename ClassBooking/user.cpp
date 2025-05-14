#include "user.hpp"
#include "models.hpp"
#include "util.hpp"

#include <iostream>
#include <fstream>
#include <regex>
#include <map>

using namespace std;

// 사용자 ID 입력 (존재하는 사용자만 입력 허용)
string InputUser() {
    string input;
    while (true) {
        cout << "ID: ";
        getline(cin, input);

        if (!isExistUser(input)) {
            cout << ".!! ID doesn't exist.\n";
        } else {
            return input;
        }
    }
}

// user.txt에서 사용자 목록을 불러옴
bool loadUsers()
{
    ifstream fin("user.txt"); // .cpp용

    if (!fin)
    {
        cerr << "[Warning] user.txt file not found. Creating empty user.txt...\n";
        ofstream fout("user.txt");
        if (!fout)
        {
            cerr << "[Error] Failed to create user.txt\n";
            exit(1);
        }
        fout.close();
        // 파일 만들어주고 진행
    }

    string id, pw;
    int admin;
    map<string, bool> id_check;

    while (fin >> id >> pw >> admin)
    {
        if (!isValidID(id))
        {
            cerr << "[Error] Invalid ID format detected in user.txt -> " << id << endl;
            exit(1);
        }

        if (!isValidPassword(pw))
        {
            cerr << "[Error] Invalid Password format detected in user.txt -> " << pw << endl;
            exit(1);
        }

        if (id_check[id])
        {
            cerr << "[Error] Duplicate ID detected in user.txt -> " << id << endl;
            exit(1);
        }
        id_check[id] = true;

        bool isAdmin = (admin != 0);
        users.push_back({id, pw, isAdmin});
    }
    fin.close();

    users.push_back({"admin1", "admin123", true});

    return true;
}

// 로그인 함수
User* login() {
    string id, pw;
    cout << "ID: ";
    getline(cin, id);
    cout << "PW: ";
    getline(cin, pw);

    for (auto& u : users) {
        if (u.id == id && u.password == pw) {
            return &u;
        }
    }
    cout << ".!! ID or PW is incorrect.\n";
    return nullptr;
}
// 로그아웃
bool logout()
{
    string input;
    cout << "Would you like to log out? ";
    cin >> input;

    if (input == "Y")
    {
        return true;
    }
    else if (input == "N")
    {
        return false;
    }
    cout << ".!! You can only enter upper case 'Y' or 'N'." << endl;
    return logout();
}


// 회원가입 기능
void registerUser() {
    string id, pw;
    bool valid = false;

    while (!valid) {
        cout << "ID: ";
        getline(cin, id);

        if (!isValidID(id)) {
            cout << ".!! Incorrect ID format.\n";
            continue;
        }

        if (isExistUser(id)) {
            cout << ".!! ID already exists.\n";
            continue;
        }

        cout << "PW: ";
        getline(cin, pw);

        if (!isValidPassword(pw)) {
            cout << ".!! Incorrect password format.\n";
            continue;
        }

        valid = true;
        users.push_back({id, pw, false});
        ofstream fout("user.txt", ios::app);
        fout << id << "\t" << pw << "\t0\n";
        fout.close();
        cout << "Registration complete.\n";
    }
}

