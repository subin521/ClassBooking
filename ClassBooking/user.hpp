#ifndef USER_HPP
#define USER_HPP

#include <string>
#include "models.hpp"

bool loadUsers();

User* login();                  // 로그인 시도 → 성공 시 User* 반환
bool logout();
void registerUser();            // 회원가입

bool isExistUser(const std::string& id);
bool isValidID(const std::string& id);
bool isValidPassword(const std::string& pw);
std::string InputUser();  // 사용자 ID 입력 함수 선언

#endif
