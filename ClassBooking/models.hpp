#ifndef MODELS_HPP
#define MODELS_HPP

#include <string>
#include <vector>

// 사용자 정보
struct User {
    std::string id;
    std::string password;
    bool is_admin;
};

// 예약 정보
struct Reservation {
    std::string user_id;
    std::string room;
    std::string day;
    std::string start_time;
    std::string end_time;
};

// 강의실 정보
struct Classroom {
    std::string room;
    bool is_available;
    std::string available_start;
    std::string available_end;
};

// 전역 데이터 컨테이너
extern std::vector<User> users;
extern std::vector<Reservation> reservations;
extern std::vector<Classroom> classrooms;

// 요일 및 시간 리스트
extern const std::vector<std::string> weekdays;
extern const std::vector<std::string> times;

extern int lineCount_r;

#endif
