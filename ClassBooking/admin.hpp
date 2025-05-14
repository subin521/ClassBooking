#ifndef ADMIN_HPP
#define ADMIN_HPP

#include <string>

using namespace std;

// 관리자: 예약 목록 조회 및 삭제 (기능 선택 포함)
void reservationManagementMenu();

// 관리자: 강의실 예약 현황 확인 및 설정 (기능 선택 포함)
void classroomManagementMenu(const string &admin_id);

void reservation_check_admin1();


#endif
