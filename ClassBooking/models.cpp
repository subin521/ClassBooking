#include "models.hpp"

// 실제 데이터 정의

std::vector<User> users;
std::vector<Reservation> reservations;
std::vector<Classroom> classrooms;

// 월~금 요일 문자열 (1~5)
const std::vector<std::string> weekdays = {"Mon", "Tue", "Wed", "Thu", "Fri"};

// 시간대 (09:00 ~ 18:00, 1시간 간격)
const std::vector<std::string> times = {
    "09:00", "10:00", "11:00", "12:00", "13:00",
    "14:00", "15:00", "16:00", "17:00", "18:00"
};

int lineCount_r = 0; // lineCount_r 정의 누락 해결

