#include "util.hpp"
#include "models.hpp"

#include <iostream>
#include <regex>
#include <cctype>

using namespace std;

// 사용자 존재 여부 확인
bool isExistUser(const string &id)
{
    for (const auto &u : users)
    {
        if (u.id == id)
            return true;
    }
    return false;
}

// 강의실 존재 여부 확인
bool isExistRoomNumber(const string &input)
{
    for (const auto &cls : classrooms)
    {
        if (cls.room == input)
            return true;
    }
    return false;
}

// 강의실 번호가 유효한 형식인지 확인 (세 자리 숫자)
bool validateRoomNumber(const string &roomnumber)
{
    if (roomnumber.length() != 3)
        return false;
    for (char c : roomnumber)
    {
        if (!isdigit(c))
            return false;
    }
    return true;
}

// 문자열의 앞뒤 공백 제거
string trim(const string &str)
{
    return trimWhitespace(str);
}

// 문자열의 앞뒤 공백 제거
string trimWhitespace(const string &input)
{
    size_t start = input.find_first_not_of(" \t\n\r");
    size_t end = input.find_last_not_of(" \t\n\r");
    if (start == string::npos)
        return "";
    return input.substr(start, end - start + 1);
}

// 문자열 내 모든 공백 제거
string removeWhitespace(const string &input)
{
    string result;
    for (char ch : input)
    {
        if (!isspace(ch))
            result += ch;
    }
    return result;
}

// HH:00 형식인지 확인
bool isValidTime(const string &time)
{
    regex pattern("^([01][0-9]|2[0-3]):00$");
    return regex_match(time, pattern);
}

// 시간 문자열을 분 단위로 변환
int timeToMinutes(const string &time)
{
    int hour = stoi(time.substr(0, 2));
    int min = stoi(time.substr(3, 2));
    return hour * 60 + min;
}

// 시간 겹침 여부 확인
bool isTimeOverlap(const string &s1, const string &e1,
                   const string &s2, const string &e2)
{
    return !(e1 <= s2 || s1 >= e2);
}

int getValidatedMenuIndex(const string& context, int min, int max)
{
    string input;
    while (true)
    {
        cin.clear();
        while (cin.peek() == '\n')
            cin.ignore();
        getline(cin, input);

        if(checkIdx(context, input)) continue;

        int idx = stoi(input);
        if(idx < min || idx > max) {
            printIdxErrorMessage(context);
            continue;
        }
        return idx;
    }
}

// 인덱스 입력 유효성 검사: 한 자리 숫자인지 판단
bool checkIdx(const string &callLocation, const string &inputIdx)
{
    string cleaned = removeWhitespace(inputIdx);
    if (cleaned.length() == 1 && isdigit(cleaned[0]))
        return false;

    printIdxErrorMessage(callLocation);
    return true;
}

// 인덱스 입력 오류 메시지 출력
void printIdxErrorMessage(const string &context)
{
    string errPhrase = ".";
    if (context == "day")
        errPhrase = "of the day of the week.";
    else if (context == "menu")
        errPhrase = "in the menu.";
    cout << ".!! Enter the index number " << errPhrase << endl;
}

// 사용자 ID 유효성 검사
bool isValidID(const string &id)
{
    regex pattern("^[a-z0-9]{3,20}$");
    return regex_match(id, pattern);
}

// 비밀번호 유효성 검사
bool isValidPassword(const string &pw)
{
    if (pw.find(' ') != string::npos)
        return false;
    if (pw.length() < 4 || pw.length() > 20)
        return false;

    bool hasAlpha = false, hasDigit = false;
    for (char c : pw)
    {
        if (isalpha(c))
            hasAlpha = true;
        if (isdigit(c))
            hasDigit = true;
    }

    return hasAlpha && hasDigit;
}
