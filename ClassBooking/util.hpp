#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

// 강의실 번호의 형식을 확인하는 메서드 
bool validateRoomNumber(const string& roomnumber) {
    if (roomnumber.length() != 3) {
        return false;
    }
    for (char c : roomnumber) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

