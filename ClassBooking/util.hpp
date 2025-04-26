#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

// ���ǽ� ��ȣ�� ������ Ȯ���ϴ� �޼��� 
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