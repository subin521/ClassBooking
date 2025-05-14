#ifndef CLASSROOM_HPP
#define CLASSROOM_HPP

#include <string>

bool loadClassrooms();  // classroom.txt 불러오기

void printClassroomList();      // 층별 강의실 목록 출력
void printTimeTable(const std::string& room); // 특정 강의실 시간표 출력
std::string InputClassroom();   // 유효한 강의실 번호 입력받기

void viewClassroomTimetable();  // 강의실 목록 → 선택 → 시간표 보기

#endif
