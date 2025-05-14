# ClassBooking 시스템

강의실 예약 프로그램입니다. 일반 사용자와 관리자가 각각 기능을 수행할 수 있습니다.  
C++로 작성되었으며, 콘솔에서 실행됩니다.

---

## 📦 파일 구성

| 파일명             | 설명                                                  |
|--------------------|-------------------------------------------------------|
| `ClassBooking.cpp` | 메인 함수 (프로그램 시작점)                          |
| `models.hpp/cpp`   | 공통 데이터 모델 정의 (`User`, `Reservation`, `Classroom`) |
| `util.hpp/cpp`     | 공통 유틸리티 함수들 (`trim`, `checkIdx` 등)         |
| `user.hpp/cpp`     | 사용자 관련 기능 (`login`, `registerUser` 등)        |
| `admin.hpp/cpp`    | 관리자 관련 기능 (`허용/금지`, `예약 관리`)           |
| `reservation.hpp/cpp` | 예약 기능 (`reserveClassroom`, `cancelReservation`) |
| `classroom.hpp/cpp`   | 강의실 관련 기능 (`loadClassrooms`, `시간표 출력` 등) |

---

## ✅ 실행 방법

1. 터미널에서 `ClassBooking.cpp`가 있는 디렉토리로 이동  
2. 아래 명령어를 입력하여 컴파일 및 실행

```bash
g++ -std=c++17 -o ClassBooking ClassBooking.cpp models.cpp util.cpp admin.cpp classroom.cpp reservation.cpp user.cpp && ./ClassBooking
