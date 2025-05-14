#ifndef RESERVATION_HPP
#define RESERVATION_HPP

#include <string>
using std::string;

bool loadReservations();

void register_reservation(const std::string& user_id);   // 사용자 예약 메뉴
void delete_reservation(const std::string& user_id);  // 사용자 예약 취소 메뉴

bool reserveClassroom(const std::string user_id);   // 실제 예약 처리
bool cancelReservation(const std::string& user_id);      // 실제 취소 처리
void handleClassroomAccess(const string& admin_id, bool isAccept); // 관리자: 허용 또는 금지 설정

#endif
