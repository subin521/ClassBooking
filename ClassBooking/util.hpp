#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <vector>
#include <regex>

// ======= 사용자 및 강의실 유효성 =======
bool isExistUser(const std::string& id);
bool isExistRoomNumber(const std::string& input);  
bool validateRoomNumber(const std::string& roomnumber);

// ======= 문자열 처리 =======
int getValidatedMenuIndex(const std::string& context, int min, int max);

std::string trim(const std::string& str);                // 문자열 앞뒤 공백 제거
std::string removeWhitespace(const std::string& input);  // 전체 공백 제거
std::string trimWhitespace(const std::string& input);    // 앞뒤 공백 제거

// ======= 시간 관련 =======
bool isValidTime(const std::string& time);               // HH:00 형식인지 검사
int timeToMinutes(const std::string& time);              // "09:00" -> 540 변환
bool isTimeOverlap(const std::string& s1, const std::string& e1,
                   const std::string& s2, const std::string& e2); // 시간 겹침 여부

// ======= 인덱스 검사 =======
bool checkIdx(const std::string& callLocation, const std::string& inputIdx);  // 위치 기반
void printIdxErrorMessage(const std::string& context);                         // 에러 메시지

// ======= ID / PW 유효성 =======
bool isValidID(const std::string& id);
bool isValidPassword(const std::string& pw);

#endif
