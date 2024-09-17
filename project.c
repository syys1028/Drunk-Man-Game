// c언어 과제 2203098 박서연
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

// 전역 변수
int map[10][10] = { { 0,0,0,1,0,0,1,1,0,1 }, { 0,1,0,0,0,0,0,0,0,0 },   // 초기 설정, map 2차원 배열 생성, 0: 길, 1: 벽
	{ 0,1,0,1,0,0,1,1,1,0 }, { 0,1,0,1,0,0,0,0,0,0 }, { 0,0,0,1,0,0,1,1,0,0 }, { 0,0,0,0,0,0,0,0,0,0 },
	{ 1,0,1,1,1,0,1,0,1,0 }, { 0,0,0,0,0,0,0,0,1,0 }, { 0,1,1,1,0,0,1,0,1,0 }, { 0,0,0,0,0,0,0,0,0,0 } };
int drunk_man_x, drunk_man_y;         // 술취한 사람 좌표 5
int police1_x = -1, police1_y = -1;   // 경찰1 좌표      61
int police2_x = -1, police2_y = -1;   // 경찰2 좌표      62
int sniper_x = -1, sniper_y = -1;     // 저격수 좌표     7
int start_num, D_num, D_new_num, P1_num, P2_num, S_num;	// start : 처음 시작한 분면, D_num : 현재(혹은 이전) 분면, D_new_num : 새 분면, P_num : 경찰 분면, S_num : 저격수 분면
int n_t, r_t, speed;				  // 자동 이동 횟수, 수동 이동 횟수, 자동 이동 속도

// 함수 호출
void map_print();											// map 출력 함수
int check_quadrant(int x, int y);							// 사분면 체크 함수
int check_back_quadrant(int nx, int ny);					// 현재 분면 체크 함수
int check_P(int x, int y);									// D와 경찰 (x, y) 시야 체크
int check_S(int x, int y);									// D와 저격수 (x, y) 시야 체크
void random_D_step_auto();									// 초기 상태 자동이동모드
void random_P_step_auto(int *x, int *y, int num);			// 분면 이동 하지 않을 때 자동 이동
void auto_after_self();										// 수동 이후 자동 이동 모드
void auto_after_self_success();								// 경찰 1 등장 이후 자동이동모드 성공
void all_auto_after_self();									// 경찰 1, 2 등장 이후 자동이동모드 
void random_step_self();									// 키보드로 수동 이동 함수
int trap2_auto_S_mode(int trap, int *trap6, int *flag);		// 집 분면에서 자동 이동 모드, 저격수 등장하는 버전
int trap3_auto_mode(int trap, int *trap2, int *trap4);		// 경찰 등장 후 모드 전환
int trap4_self_mode(int trap, int *trap3);					// 수동 이동 모드 (경찰1, 2 버전)
int trap6_S_auto_mode(int trap);							// 저격수 등장 후 자동 이동 모드

// ------------------------------------------ 메인 함수 -----------------------------------------
void main() { 
	int count1 = 0, count2 = 0, result = -1, x1 = 0, y1 = 0, x2 = 0, y2 = 0;														// 카운트, 생성결과 변수
	int trap1 = -1, trap2 = 0, trap3 = 0, trap4 = 0, trap5 = 0, trap6 = 0, trap7 = 0, flag = 0;     // while문 제어용 변수
	int bar_x = -1, bar_y = -1;																		// 술집 좌표      8
	int home_x= -1, home_y = -1;																	// 집 좌표        9

	srand(time(NULL));               // random 선언

	// 객체 구성
	while (result != 0) {				 // 고정 객체인 술집과 집 생성하기 
		x1 = rand() % 10;			 // 술집 랜덤 x좌표 생성
		y1 = rand() % 10;		   	 // 술집 랜덤 y좌표 생성
		result = map[x1][y1];      // 길인지 벽인지 판별한 결과를 result에 저장
		if (result == 0) {
			D_num = check_quadrant(x1, y1);   // 좌표의 분면을 체크하는 함수 실행, 결과를 return값에 저장
			switch(D_num){			  // 술집이 1사분면이면 집은 4사분면
			case 1:
				x2 = rand() % 5 + 5;
				y2 = rand() % 5 + 5;
				break;
			case 2:					  // 술집이 2사분면이면 집은 3사분면
				x2 = rand() % 5 + 5;
				y2 = rand() % 5;
				break;
			case 3:                  // 술집이 3사분면이면 집은 2사분면
				x2 = rand() % 5;
				y2 = rand() % 5 + 5;
				break;
			case 4:                  // 술집이 4사분면이면 집은 1사분면
				x2 = rand() % 5;
				y2 = rand() % 5;
				break;
			}
			if (map[x1][y1] == 0 && map[x2][y2] == 0) {	   // 술집 좌표와 집 좌표가 모두 0(길)이면
				bar_x = x1;				// 술집과 집 x, y좌표에 값 저장
				bar_y = y1;
				home_x = x2;
				home_y = y2;
				map[bar_x][bar_y] = 8;	 // map에 술집과 집 숫자 지정
				map[home_x][home_y] = 9;
			}
		}
	}
	drunk_man_x = bar_x;						// 술 취한 사람의 시작 좌표 = 술집의 좌표
	drunk_man_y = bar_y;
	start_num = D_num;							// 초기 상태(start_num = D_num = D_new_num)로 세팅
	D_new_num = D_num;

	// 게임 초기 설정
	printf("설정값을 입력해주세요.\n");         // 초기 설정 값 외부 입력, n_t, r_t, speed
	printf("자동 이동 모드 이동 횟수 : ");
	scanf_s("%d", &n_t);
	printf("수동 이동 모드 이동 횟수 : ");
	scanf_s("%d", &r_t);
	printf("자동 이동 모드에서 술 취한 사람의 이동속도(1000 -> 1초): ");
	scanf_s("%d", &speed);

	// 게임 시작
	printf("게임 시작! \n");

	// trap1 : 첫 화면. 시작분면에서 자동이동
	count1 = n_t;										// 카운트 세팅 (자동 모드)
	while (trap1 != 0) {								// 시작분면에서 자동이동, 카운트 체크
		system("cls");									// while문 돌 때마다 지우고 다시 쓰기
		random_D_step_auto();							// 초기 상태 자동이동모드 (이전분면 : start_num, 현재 분면 : D_num)
		map_print();									// 맵 출력
		Sleep(speed);									// 출력 속도 설정
		if (count1 >= 0) {								// 자동이동모드 카운트 다 쓰기 전이나 다 썼을 때
			printf("남은 자동 이동 횟수 : %d\n", count1);	// 카운트 표시
			if (start_num != D_num) {					// 다른 분면으로 이동해버리면
				printf("횟수 이내 이동 성공! \n");
				printf("전체 자동이동 모드 돌입 \n");
				trap2 = 1;								// trap2(전체 자동이동 모드) 조건 발동시키고
				trap1 = 0;								// trap1(현재 while문) 종료
			}
		}
		if (count1 < 0) {								// 자동이동모드 카운트 다 썼을 때,
			if (start_num != D_num) {					// 다음 분면으로 이동하면 (카운트 지나도 다른 분면으로 이동할 때까지 계속 실행)
				printf("자동이동모드 횟수 초과!\n");
				printf("경찰 등장 \n");
				trap3 = 1;								// trap3(경찰1 생성 이후 자동이동) 조건 발동시키고
				flag = 1;								// 경찰 생성 조건 발동시키고
				trap1 = 0;								// trap1(현재 while문) 종료
			}
		}        
		count1--;										// 카운트 -1
	}

	// trap2 : trap1(시작분면에서 자동이동) 성공 후 집까지 전체 자동이동모드
	while (trap2 != 0) {								// 전체 자동이동모드 (경찰, 저격수 등장 x)
		system("cls");									// while문 돌 때마다 지우고 다시 쓰기
		random_D_step_auto();							// 초기 상태 자동이동모드 (이전분면 : D_num, 현재 분면 : D_new_num)
		map_print();									// 맵 출력
		Sleep(speed);
		printf("자동이동 모드 진행 중\n");
		if (map[drunk_man_x][drunk_man_y] == 9) {		// 술 취한 사람의 좌표값이 9(집)일경우
			map[drunk_man_x][drunk_man_y] = 5;			// 5(술 취한 사람)로 변경하고
			printf("집 도착!\n");
			trap2 = 0;									// trap2(현재 while문) 종료, 게임 마침
		}
	}

	// trap3 -> flag : trap1(시작분면에서 자동이동) 실패 후 경찰1 생성
	while (flag != 0) {												// 경찰1 생성하기
		police1_x = rand() % 10;									// 경찰1 랜덤 x좌표 생성
		police1_y = rand() % 10;									// 경찰1 랜덤 y좌표 생성
		P1_num = check_quadrant(police1_x, police1_y);				// 경찰1 좌표로 현재 분면 계산
		if (P1_num == check_quadrant(drunk_man_x, drunk_man_y)) {	// 경찰1의 분면이 술 취한 사람이 넘어온 분면과 같을 경우 
			result = map[police1_x][police1_y];						// 길인지 벽인지 판별한 결과를 result에 저장
			if (result == 0) {										// 길이면 생성 종료
				flag = 0;											// flag(현재 while문) 종료, 게임 마침
			}
		}
	}

	// trap3 : 경찰1 생성 이후, 만나기 전까지 자동이동
	while (trap3 != 0) {									// 경찰1 생성 이후 자동이동, 경찰과 술 취한 사람 분면 같음
		system("cls");			 
		random_D_step_auto();								// 술 취한 사람 자동이동 (이전분면 : start_num, 현재 분면 : D_num)
		random_P_step_auto(&police1_x, &police1_y, P1_num);	// 경찰은 현재 분면에서 자동이동
		map_print();
		Sleep(speed);
		D_num = check_quadrant(drunk_man_x, drunk_man_y);	// 술 취한 사람 분면 값 체크
		if (check_P(police1_x, police1_y) == 1) {			// 경찰에 발각되면 수동이동 모드로 전환
			printf("경찰에 발각!\n");
			printf("수동 이동 모드 전환 \n");
			trap4 = 1;										// trap4(경찰 발각 후 수동모드) 조건 발동시키고
			trap3 = 0;										// trap3(현재 while문) 종료
		}
		if (check_P(police1_x, police1_y) != 1){ 			 // 경찰에 발각되지 않고
			if (D_num == check_quadrant(home_x, home_y)) {    // 집 분면에 이동하면 전체 자동이동모드
				printf("경찰에 발각되지 않고 집 분면 이동!\n");
				printf("전체 자동이동 모드 돌입 \n");
				trap2 = 1;									// trap2(전체 자동이동 모드) 조건 발동시키고
				trap3 = 0;									// trap3(현재 while문) 종료
			}
		}
	}
	
	// trap2 : 집 분면 자동이동 (저격수 버전)
	if (trap2 == 1){												// trap2 조건에 맞으면
		trap2 = trap2_auto_S_mode(trap2, &trap6, &flag);			// 집 분면에서 자동이동하는 함수 호출, 카운트 넘으면 저격수 등장
		if (flag == 1) {											// flag(저격수 생성 함수) 조건 만족하면
			while (flag != 0) {										// 저격수 생성 실행
				sniper_x = rand() % 10;								// 저격수 랜덤 x좌표 생성
				sniper_y = rand() % 10;								// 저격수 랜덤 y좌표 생성
				S_num = check_quadrant(sniper_x, sniper_y);			// 저격수 좌표 계산
				if (check_quadrant(drunk_man_x, drunk_man_y) == S_num) {	// 술 취한 사람과 저격수 분면이 같으면
					result = map[sniper_x][sniper_y];				// 길인지 벽인지 판별한 결과를 result에 저장
					if (result == 1) {								// 벽이면 벽 대신 저격수 값 넣음
						map[sniper_x][sniper_y] = 7;				// 저격수 위치 고정
						flag = 0;									// flag(현재 while문) 종료
					}
				}
			}
		}
		if (trap6 == 1) {											// trap6 조건에 맞으면
			trap6 = trap6_S_auto_mode(trap6);						// 저격수 이동 함수 발동
		}
	}

	// trap4 : 경찰 발각 후 수동모드 실행
	count2 = r_t;										// 카운트 세팅 (수동 모드)
	while (trap4 != 0) {								// 수동모드 시작, 카운트 체크
		while (count2 > 0) {							// 수동이동 카운트 다 쓰기 전까지 반복
			printf("목적지 : %d 사분면, 현재 위치 : %d 사분면", start_num, D_num);	// 목적지와 현재 위치 출력
			random_step_self();							// 수동이동 모드 실행
			system("cls");
			map_print();
			count2--;									// 카운트 -1
			printf("남은 수동 이동 횟수 : %d\n", count2);
			Sleep(speed);
			if (count2 >= 0) {							// 이동 횟수가 남아있을 때,
				if (start_num == D_num) {				// 초기 분면으로 돌아가면 성공, 다시 자동 이동 모드로
					printf("수동이동 성공!\n");
					printf("자동 이동 모드 전환\n");
					trap5 = 1;							// trap5(수동이동 성공 후 다시 자동이동) 조건 발동시키고
					count2 = 0;							// count2(현재 while문) 종료
					trap4 = 0;							// trap4(현재 while문) 종료
				}
			}
			if (count2 == 0) {							// 이동 횟수가 끝났을 때,
				if (start_num != D_num) {				// 돌아가지 못하면 바로 die
					printf("수동이동 실패\n");
					printf("----- DIE -----\n");
					trap4 = 0;							// trap4(현재 while문) 종료
				}
			}
		}
	}

	// trap5: 수동이동 성공 후 다시 자동이동 (시작 분면에서 다시 시작)
	count1 = n_t;											// 카운트 세팅 (자동 모드)
	while (trap5 != 0) {                           // 자동이동모드 시작, 카운트 체크
		system("cls");
		auto_after_self();                           // 수동 이후 자동 이동 모드 실행
		random_P_step_auto(&police1_x, &police1_y, P1_num);   // 경찰은 현재 분면에서 자동이동
		map_print();
		Sleep(speed);
		if (count1 >= 0) {                           // 자동이동모드 카운트 다 쓰기 전일 때
			printf("남은 자동 이동 횟수 : %d\n", count1);
			if (start_num != D_num && P1_num != D_num) {   // start도 아니고 경찰1 분면도 아닌 다른 분면으로 이동해버리면
				printf("횟수 이내 이동 성공! \n");
				printf("자동이동 모드 돌입 \n");
				trap7 = 1;                           // trap7(다시 start로 돌아오지 않음) 조건 발동시키고
				trap5 = 0;                           // trap5(현재 while문) 종료
			}
			else count1--;                           // 카운트 -1
		}
		if (count1 < 0) {                           // 자동이동모드 카운트 다 썼을 때,
			if (start_num != D_num && P1_num != D_num) {   // start도 아니고 경찰1 분면도 아닌 다른 분면으로 이동하면
				printf("자동이동모드 횟수 초과!\n");
				printf("경찰2 등장 \n");
				trap3 = 1;                           // trap3(경찰 생성 이후 자동이동) 조건 발동시키고
				flag = 1;                           // 경찰 생성 조건 발동시키고
				trap5 = 0;                           // trap5(현재 while문) 종료
			}
			else count1--;                           // 카운트 -1
		}
	}
	while (trap7 != 0) {
		system("cls");
		auto_after_self_success();								// 경찰1 등장 이후 자동이동모드 성공
		random_P_step_auto(&police1_x, &police1_y, P1_num);     // 경찰은 현재 분면에서 자동이동
		map_print();
		Sleep(speed);
		if (D_num == check_quadrant(home_x, home_y)) {    // 집 분면에 이동하면 전체 자동이동모드
			printf("경찰에 발각되지 않고 집 분면 이동!\n");
			printf("전체 자동이동 모드 돌입 \n");
			trap2 = 1;                           // trap2(전체 자동이동 모드) 조건 발동시키고
			trap7 = 0;                           // trap7(현재 while문) 종료
		}
	}

	// trap2 : 집 분면 자동이동 (저격수 버전)
	if (trap2 == 1) {												// trap2 조건에 맞으면
		trap2 = trap2_auto_S_mode(trap2, &trap6, &flag);			// 집 분면에서 자동이동하는 함수 호출, 카운트 넘으면 저격수 등장
		if (flag == 1) {											// flag(저격수 생성 함수) 조건 만족하면
			while (flag != 0) {										// 저격수 생성
				sniper_x = rand() % 10;								// 저격수 랜덤 x좌표 생성
				sniper_y = rand() % 10;								// 저격수 랜덤 y좌표 생성
				S_num = check_quadrant(sniper_x, sniper_y);			// 저격수 좌표 계산
				if (check_quadrant(drunk_man_x, drunk_man_y) == S_num) {	// 술 취한 사람과 저격수 분면이 같으면
					result = map[sniper_x][sniper_y];				// 길인지 벽인지 판별한 결과를 result에 저장
					if (result == 1) {								// 벽이면 벽 대신 저격수 값 넣음
						map[sniper_x][sniper_y] = 7;				// 저격수 위치 고정
						flag = 0;									// flag(현재 while문) 종료
					}
				}
			}
		}
		if (trap6 == 1) {											// trap6 조건에 맞으면
			trap6 = trap6_S_auto_mode(trap6);						// 저격수 이동 함수 발동
		}
	}
	
	// trap3 -> flag: 경찰2 생성 이후, 만나기 전까지 자동이동
	while (flag != 0) {												// 경찰생성 조건 만족하면 경찰2 생성하기
		police2_x = rand() % 10;									// 경찰2 랜덤 x좌표 생성
		police2_y = rand() % 10;									// 경찰2 랜덤 y좌표 생성
		P2_num = check_quadrant(police2_x, police2_y);				// 경찰2 좌표 계산
		if (check_quadrant(drunk_man_x, drunk_man_y) == P2_num) {	// 경찰1의 분면이 술 취한 사람이 넘어온 분면과 같을 경우 
			result = map[police2_x][police2_y];						// 길인지 벽인지 판별한 결과를 result에 저장
			if (result == 0) {										// 길이면 생성 종료
				flag = 0;											// flag(현재 while문) 종료, 게임 마침
			}
		}
	}
	
	// trap3 : 경찰1,2 생성 이후, 만나기 전까지 자동이동
	while (trap2 != 1) {											// 분면 이동 후 trap2(자동이동모드 진입) 조건에 맞으면 빠져나옴, 아닐 시 무한루프
		if (trap3 == 1) {											// trap3(경찰 만나기 전까지 자동이동) 실행
			trap3 = trap3_auto_mode(trap3, &trap2, &trap4);			// 경찰1 생성 이후 자동이동, 경찰과 술 취한 사람 분면 같음
			if (trap4 == 1) trap4 = trap4_self_mode(trap4, &trap3);	// 수동이동 횟수 내에 집 분면에 들어가면 다시 trap3, 못들어가면 주금
		}
	}
	// trap3 : 경찰 1,2 자동-수동루프 빠져나온 이후
	if (trap2 == 1) {												// trap2 조건에 맞으면
		trap2 = trap2_auto_S_mode(trap2, &trap6, &flag);			// 자동이동 횟수 내에 집 분면에 들어가면 trap2
		if (flag == 1) {											// flag(저격수 생성 함수) 조건 만족하면
			while (flag != 0) {										// 저격수 생성
				sniper_x = rand() % 10;								// 저격수 랜덤 x좌표 생성
				sniper_y = rand() % 10;								// 저격수 랜덤 y좌표 생성
				S_num = check_quadrant(sniper_x, sniper_y);			// 저격수 좌표 계산
				if (check_quadrant(drunk_man_x, drunk_man_y) == S_num) {	// 술 취한 사람과 저격수 분면이 같으면
					result = map[sniper_x][sniper_y];				// 길인지 벽인지 판별한 결과를 result에 저장
					if (result == 1) {								// 벽이면 벽 대신 저격수 값 넣음
						map[sniper_x][sniper_y] = 7;				// 저격수 위치 고정
						flag = 0;									// flag(현재 while문) 종료
					}
				}
			}
		}
		if (trap6 == 1) {											// trap6(저격수 이동 함수) 조건에 맞으면
			trap6 = trap6_S_auto_mode(trap6);						// 저격수 이동 함수 발동
		}
	}

}

// ------------------------------------------ 함수 -----------------------------------------

void map_print() {								// map 출력 함수
	int copy[10][10] = { 0, };					// 임시로 map의 값을 저장할 copy 2차원 배열 선언 후 초기화

	for (int i = 0; i < 10; i++) {				// for문으로 현재 map 값을 copy에 넣어줌
		for (int j = 0; j < 10; j++) {
			copy[i][j] = map[i][j];
		}
	}
	copy[drunk_man_x][drunk_man_y] = 5;			// 술 취한 사람의 x,y좌표에 5표시
	if (police1_x != -1 && police1_y != -1) copy[police1_x][police1_y] = 61;	// 경찰1의 좌표가 존재할 경우 경찰1의 x,y좌표에 61표시
	if (police2_x != -1 && police2_y != -1) copy[police2_x][police2_y] = 62;	// 경찰2의 좌표가 존재할 경우 경찰2의 x,y좌표에 63표시
	if (sniper_x != -1 && sniper_y != -1) copy[sniper_x][sniper_y] = 7; 		// 저격수의 좌표가 존재할 경우 저격수의 x,y좌표에 7표시

	for (int i = 0; i < 10; i++) {				// for문 돌려가며 실행
		for (int j = 0; j < 10; j++) {
			if (copy[i][j] == 0) {				// 0이면 길, 빈 사각형 표시
				printf_s("□ ");
			}
			if (copy[i][j] == 1) {				// 1이면 벽, 꽉 찬 사각형 표시
				printf_s("■ ");
			}
			if (copy[i][j] == 7) {				// 7이면 저격수, S 표시
				printf_s("S  ");
			}
			if (copy[i][j] == 8) {				// 8이면 술집, B 표시
				printf_s("B  ");
			}
			if (copy[i][j] == 9) {				// 9이면 집, H 표시
				printf_s("H  ");
			}
			if (copy[i][j] == 61) {				// 61이면 경찰1, P1 표시
				printf_s("P1 ");
			}
			if (copy[i][j] == 62) {				// 62이면 경찰2, P2 표시
				printf_s("P2 ");
			}
			if (copy[i][j] == 5) {				// 5이면 술 취한 사람, D 표시
				printf_s("D  ");
			}
		}
		printf_s("\n");
	}
}

// check 함수
int check_quadrant(int x, int y) {										// 사분면 체크 함수
	int result = 0;					
	if (x == 0 || x == 1 || x == 2 || x == 3 || x == 4) {               // x값이 0 ~ 4이고
		if (y == 0 || y == 1 || y == 2 || y == 3 || y == 4)   result = 1;      // y값이 0 ~ 4이면 1사분면
		if (y == 5 || y == 6 || y == 7 || y == 8 || y == 9)   result = 2;      // y값이 5 ~ 9이면 2사분면
	}
	if (x == 5 || x == 6 || x == 7 || x == 8 || x == 9) {               // x값이 5 ~ 9이고
		if (y == 0 || y == 1 || y == 2 || y == 3 || y == 4)   result = 3;      // y값이 0 ~ 4이면 3사분면
		if (y == 5 || y == 6 || y == 7 || y == 8 || y == 9)   result = 4;      // y값이 5 ~ 9이면 4사분면
	}
	return result;
}
int check_back_quadrant(int nx, int ny) {								// 현재 분면 체크 함수(start_num - D_num - D_new_num) : 초기 자동이동에 쓰임
	int result = 0;
	if ((nx > -1 && nx < 10) && (ny > -1 && ny < 10)) {					// 좌표가 맵의 크기를 넘어가지 않을 때,
		if (map[nx][ny] == 0 || map[nx][ny] == 8 || map[nx][ny] == 9) {	// 길이거나, 술집이거나, 집일 때
			if (D_num == start_num && D_num == D_new_num) {				// start, now, new가 같은 값이면 초기 상태
				if (check_quadrant(nx, ny) == D_num) {					// 초기 값과 같으면 그냥 이동
					result = 1;
				}
				if (check_quadrant(nx, ny) != D_num) {					// 초기 값과 다르면 분면이동 한다는 뜻
					D_num = check_quadrant(nx, ny);						// now값과 new 값을 새로 이동할 분면으로 변경해줌
					D_new_num = check_quadrant(nx, ny);
					result = 1;
				}
			}
			if (D_num != start_num && D_num == D_new_num) {				// start는 다르고 now, new는 같은 값이면 (첫번째 이동)
				if (check_quadrant(nx, ny) != start_num) {				// 이전분면인 start가 아닐 때
					if (check_quadrant(nx, ny) == D_num) {				// 현재 분면과 같으면 그냥 이동
						result = 1;
					}
					if (check_quadrant(nx, ny) != D_new_num) {			// 현재 분면이 아니고 새로운 분면이면 분면이동 한다는 뜻
						D_new_num = check_quadrant(nx, ny);				// new 값을 새로운 분면으로 변경해줌
						result = 1;
					}
				}
			}
			if (D_num != start_num && D_num != D_new_num) {				// start, now, new 전부 다른 값이면 (두번째 이동)
				if (check_quadrant(nx, ny) == D_new_num) {				// 새로운 분면인 D_new_num에서만 이동 가능
					result = 1;
				}
			}
		}
	}
	return result;														// 나머지 경우는 0 반환
}
int check_P(int x, int y) {												// D와 경찰 (x, y) 시야 체크, 경찰의 시야에 D가 존재한다면 x와 y중 하나라도 같아야함
	int i = 0, result = 0;								
	D_num = check_quadrant(drunk_man_x, drunk_man_y);					// 술취한 사람의 현재 좌표 받아옴
	if (drunk_man_x == x) {												// 술취한 사람의 x좌표와 경찰의 x좌표가 같은 경우
		if (drunk_man_y < y) {											// 술취한 사람의 y좌표가 경찰의 y좌표보다 작을 때(술취한 사람이 경찰 왼쪽)
			for (i = 0; i < 5; i++) {									
				if (y - i == drunk_man_y) result = 1;					// 경찰의 y좌표에서 0에서 4까지 1씩 감소시키며 y1과 맞으면 1 반환(긍정)
				if (map[x][y - i] == 1 || check_quadrant(x, y - 1) != D_num)  break;	// 벽에 다다르거나 분면 벗어나면 for문 종료
			}
		}
		if (drunk_man_y > y) {											// 술취한 사람의 y좌표가 경찰의 y좌표보다 작을 때(술취한 사람이 경찰 오른쪽)
			for (i = 0; i < 5; i++) {							
				if (y + i == drunk_man_y) result = 1;					// 경찰의 y좌표에서 0에서 4까지 1씩 증가시키며 y1과 맞으면 1 반환(긍정)
				if (map[x][y + i] == 1 || check_quadrant(x, y + 1) != D_num)  break;	// 벽에 다다르거나 분면 벗어나면 for문 종료
			}
		}
	}
	if (drunk_man_y == y) {												// 술취한 사람의 y좌표와 경찰의 y좌표가 같은 경우
		if (drunk_man_x < x) {											// 술취한 사람의 x좌표가 경찰의 x좌표보다 작을 때(술취한 사람이 경찰 위쪽)
			for (i = 0; i < 5; i++) {							
				if (x - i == drunk_man_x) result = 1;					// 경찰의 x좌표에서 0에서 4까지 1씩 감소시키며 y1과 맞으면 1 반환(긍정)
				if (map[x - i][y] == 1 || check_quadrant(x - 1, y) != D_num)  break;	// 벽에 다다르거나 분면 벗어나면 for문 종료
			}
		}
		if (drunk_man_x > x) {											// 술취한 사람의 y좌표가 경찰의 y좌표보다 작을 때(술취한 사람이 경찰 아래쪽)
			for (i = 0; i < 5; i++) {									
				if (x + i == drunk_man_x) result = 1;					// 경찰의 x좌표에서 0에서 4까지 1씩 증가시키며 y1과 맞으면 1 반환(긍정)
				if (map[x + i][y] == 1 || check_quadrant(x + 1, y) != D_num)  break;	// 벽에 다다르거나 분면 벗어나면 for문 종료
			}
		}
	}
	if (drunk_man_x == x && drunk_man_y == y) result = 1;				// 좌표가 같아도 잡힘
	return result;														// 나머지 경우는 0 반환		
}
int check_S(int x, int y) {												// D와 저격수 (x, y) 시야 체크, 저격수 시야에 D가 존재한다면 x와 y중 하나라도 같아야함
	int result = 0;														
	D_num = check_quadrant(drunk_man_x, drunk_man_y);					// 술취한 사람의 현재 좌표 받아옴
	if (drunk_man_x == x) {												// 술취한 사람의 x좌표와 저격수 x좌표가 같은 경우								
		if (y - 1 == drunk_man_y) {										// 저격수의 y좌표에서 1 뺀 값이 술취한 사람의 y좌표와 같고
			if (map[x][y - 1] == 0)										// 좌표가 길에 존재하면 1(존재함)
				result = 1;
		}
		if (y + 1 == drunk_man_y) {										// 저격수의 y좌표에서 1 더한 값이 술취한 사람의 y좌표와 같고
			if (map[x][y + 1] == 0)										// 좌표가 길에 존재하면 1(존재함)
				result = 1;
		}
	}
	if (drunk_man_y == y) {												// 술취한 사람의 y좌표와 저격수 y좌표가 같은 경우								
		if (x - 1 == drunk_man_x) {										// 저격수의 x좌표에서 1 뺀 값이 술취한 사람의 x좌표와 같고
			if (map[x - 1][y] == 0)										// 좌표가 길에 존재하면 1(존재함)
				result = 1;
		}
		if (x + 1 == drunk_man_x) {										// 저격수의 x좌표에서 1 더한 값이 술취한 사람의 x좌표와 같고
			if (map[x + 1][y] == 0)										// 좌표가 길에 존재하면 1(존재함)
				result = 1;
		}
	}
	if (drunk_man_x == x && drunk_man_y == y) result = 1;				// 좌표가 같아도 잡힘
	return result;														// 나머지 경우는 0 반환	
}

// 이동 함수(자동)
void random_D_step_auto() {							// 초기 상태 자동이동모드 (상: 0, 하 : 1, 좌 : 2, 우 : 3)
	int random_number, trap = -1, nx, ny;			
	while (trap != 0) {								// trap이 조건을 만족할 때 (제어)
		nx = drunk_man_x;							// nx는 술 취한 사람의 x좌표 저장
		ny = drunk_man_y;							// ny는 술 취한 사람의 y좌표 저장
		random_number = rand() % 4;				
		switch (random_number) {					// 0~3까지의 숫자 랜덤하게 받아온 후
		case 0:										// 0이면 위로 이동, x값 -1
			nx -= 1;
			break;
		case 1:										// 1이면 아래로 이동, x값 +1
			nx += 1;
			break;
		case 2:										// 2이면 왼쪽로 이동, y값 -1
			ny -= 1;
			break;
		case 3:										// 3이면 오른쪽로 이동, y값 +1
			ny += 1;
			break;
		}
		if (check_back_quadrant(nx, ny) == 1) {		// 현재 분면 체크 함수 실행. 조건에 만족하면
			drunk_man_x = nx;						// 술 취한 사람의 x, y 좌표에 반영
			drunk_man_y = ny;
			trap = 0;								// trap(현재 while문) 종료
		}
	}
}
void random_P_step_auto(int *x, int *y, int num) {	// 분면 이동 하지 않을 때 자동 이동 (상: 0, 하 : 1, 좌 : 2, 우 : 3)
	int random_number, trap = -1, nx, ny;      
	while (trap != 0) {								// trap이 조건을 만족할 때 (제어)
		nx = *x;									// nx는 x좌표 저장
		ny = *y;									// ny는 y좌표 저장
		random_number = rand() % 4;
		switch (random_number) {					// 0~3까지의 숫자 랜덤하게 받아온 후
		case 0:										// 0이면 위로 이동, x값 -1
			nx -= 1;
			break;
		case 1:										// 1이면 아래로 이동, x값 +1
			nx += 1;
			break;
		case 2:										// 2이면 왼쪽로 이동, y값 -1
			ny -= 1;
			break;
		case 3:										// 3이면 오른쪽로 이동, y값 +1
			ny += 1;
			break;
		}
		if ((nx > -1 && nx < 10) && (ny > -1 && ny < 10)) {		// 좌표가 맵의 크기를 넘어가지 않을 때,
			if ((map[nx][ny] == 0) || (map[nx][ny] == 8) || (map[nx][ny] == 9)) {	// 길이거나, 술집이거나, 집일 때
				if (check_quadrant(nx, ny) == num) {		// 분면이 현재 분면과 일치하면
					*x = nx;								// 포인터 사용해서 현재 x, y 좌표에 반영
					*y = ny;
					trap = 0;								// trap(현재 while문) 종료
				}
			}
		}
	}
}
void auto_after_self() {							// 수동 이후 자동 이동 모드 (상: 0, 하 : 1, 좌 : 2, 우 : 3)
	int random_number, trap = -1, nx, ny;
	while (trap != 0) {								// trap이 조건을 만족할 때 (제어)
		nx = drunk_man_x;							// nx는 술 취한 사람의 x좌표 저장
		ny = drunk_man_y;							// ny는 술 취한 사람의 y좌표 저장
		random_number = rand() % 4;
		switch (random_number) {					// 0~3까지의 숫자 랜덤하게 받아온 후
		case 0:										// 0이면 위로 이동, x값 -1
			nx -= 1;
			break;
		case 1:										// 1이면 아래로 이동, x값 +1
			nx += 1;
			break;
		case 2:										// 2이면 왼쪽로 이동, y값 -1
			ny -= 1;
			break;
		case 3:										// 3이면 오른쪽로 이동, y값 +1
			ny += 1;
			break;
		}
		if ((nx > -1 && nx < 10) && (ny > -1 && ny < 10)) { 	// 좌표가 맵의 크기를 넘어가지 않을 때,
			if ((map[nx][ny] == 0) || (map[nx][ny] == 8) || (map[nx][ny] == 9)) {	// 길이거나, 술집이거나, 집일 때
				if (check_quadrant(nx, ny) != P1_num) {		// 분면이 경찰의 분면으로 다시 돌아가지 않으면
					drunk_man_x = nx;						// 술 취한 사람의 x, y 좌표에 반영
					drunk_man_y = ny;
					D_num = check_quadrant(drunk_man_x, drunk_man_y);	// 이동 후 D_num에 현재 분면값 저장
					trap = 0;								// trap(현재 while문) 종료
				}
			}
		}
	}
}
void auto_after_self_success() {				// 경찰1 등장 이후 자동이동모드 성공
	int random_number, trap = -1, nx, ny;
	while (trap != 0) {                        // trap이 조건을 만족할 때 (제어)
		nx = drunk_man_x;                     // nx는 술 취한 사람의 x좌표 저장
		ny = drunk_man_y;                     // ny는 술 취한 사람의 y좌표 저장
		random_number = rand() % 4;
		switch (random_number) {               // 0~3까지의 숫자 랜덤하게 받아온 후
		case 0:                              // 0이면 위로 이동, x값 -1
			nx -= 1;
			break;
		case 1:                              // 1이면 아래로 이동, x값 +1
			nx += 1;
			break;
		case 2:                              // 2이면 왼쪽로 이동, y값 -1
			ny -= 1;
			break;
		case 3:                              // 3이면 오른쪽로 이동, y값 +1
			ny += 1;
			break;
		}
		if ((nx > -1 && nx < 10) && (ny > -1 && ny < 10)) {    // 좌표가 맵의 크기를 넘어가지 않을 때,
			if ((map[nx][ny] == 0) || (map[nx][ny] == 8) || (map[nx][ny] == 9)) {   // 길이거나, 술집이거나, 집일 때
				if (check_quadrant(nx, ny) != P1_num && check_quadrant(nx, ny) != start_num) {      // 분면이 경찰의 분면과 시작분면으로 다시 돌아가지 않으면
					drunk_man_x = nx;                  // 술 취한 사람의 x, y 좌표에 반영
					drunk_man_y = ny;
					D_num = check_quadrant(drunk_man_x, drunk_man_y);   // 이동 후 D_num에 현재 분면값 저장
					trap = 0;                        // trap(현재 while문) 종료
				}
			}
		}
	}
}
void all_auto_after_self() {						// 경찰 1, 2 등장 이후 자동이동모드 (분면 제한 없음)
	int random_number, trap = -1, nx, ny;
	while (trap != 0) {								// trap이 조건을 만족할 때 (제어)
		nx = drunk_man_x;							// nx는 술 취한 사람의 x좌표 저장
		ny = drunk_man_y;							// ny는 술 취한 사람의 y좌표 저장
		random_number = rand() % 4;
		switch (random_number) {					// 0~3까지의 숫자 랜덤하게 받아온 후
		case 0:										// 0이면 위로 이동, x값 -1
			nx -= 1;
			break;
		case 1:										// 1이면 아래로 이동, x값 +1
			nx += 1;
			break;
		case 2:										// 2이면 왼쪽로 이동, y값 -1
			ny -= 1;
			break;
		case 3:										// 3이면 오른쪽로 이동, y값 +1
			ny += 1;
			break;
		}
		if ((nx > -1 && nx < 10) && (ny > -1 && ny < 10)) { 	// 좌표가 맵의 크기를 넘어가지 않을 때,
			if ((map[nx][ny] == 0) || (map[nx][ny] == 8) || (map[nx][ny] == 9)) {	// 길이거나, 술집이거나, 집일 때
				drunk_man_x = nx;							// 술 취한 사람의 x, y 좌표에 반영
				drunk_man_y = ny;
				D_num = check_quadrant(drunk_man_x, drunk_man_y);	// 이동 후 D_num에 현재 분면값 저장
				trap = 0;									// trap(현재 while문) 종료
			}
		}
	}
}

// 이동 함수 (수동)
void random_step_self() {			// 키보드로 수동 이동 함수
	int trap = 1, key, nx, ny;      
	while (trap != 0) {
		key = _getch();				// 첫 번째 key값 받아와서 버림
		key = _getch();				// 두 번째 key값 받아와서 저장
		nx = drunk_man_x;			// nx는 술 취한 사람의 x좌표 저장
		ny = drunk_man_y;			// ny는 술 취한 사람의 y좌표 저장
		switch (key) {
		case 72:					// 키의 값이 72면 화살표 위로
			nx -= 1;
			break;
		case 80:					// 키의 값이 80면 화살표 아래로
			nx += 1;
			break;
		case 75:					// 키의 값이 75면 화살표 왼쪽으로
			ny -= 1;
			break;
		case 77:					// 키의 값이 77면 화살표 오른쪽으로
			ny += 1;
			break;
		}
		if ((nx > -1 && nx < 10) && (ny > -1 && ny < 10)) { 	// 좌표가 맵의 크기를 넘어가지 않을 때,
			if ((map[nx][ny] == 0) || (map[nx][ny] == 8) || (map[nx][ny] == 9)) {	// 길이거나, 술집이거나, 집일 때
				if (D_num == check_quadrant(nx, ny) || start_num == check_quadrant(nx, ny)) {	// 현재 분면에서 이동하거나 시작분면에 돌아가면
					drunk_man_x = nx;							// 술 취한 사람의 x, y 좌표에 반영
					drunk_man_y = ny;
					D_num = check_quadrant(drunk_man_x, drunk_man_y);	// 이동 후 D_num에 현재 분면값 저장
					trap = 0;											// trap(현재 while문) 종료
				}
			}
		}
	}
}

// trap 함수 (제어, 반복, 호출)
int trap2_auto_S_mode(int trap, int *trap6, int *flag) {		// 집 분면에서 자동 이동 모드, 저격수 등장하는 버전
	int count1 = n_t;											// 자동 이동 횟수 저장
	while (trap != 0) {
		system("cls");
		random_P_step_auto(&drunk_man_x, &drunk_man_y, D_num);	// 술 취한 사람은 현재 분면에서 자동이동
		map_print();
		Sleep(speed); 
		printf("자동이동 모드 진행 중\n");
		if (count1 >= 0) {										// 자동이동모드 카운트 다 쓰기 전일 때
			printf("남은 자동 이동 횟수 : %d\n", count1);
			if (map[drunk_man_x][drunk_man_y] == 9) {			// 집 좌표와 같으면 도착 성공
				map[drunk_man_x][drunk_man_y] = 5;				// 좌표에 술 취한 사람 집어넣고
				printf("집 도착!\n");
				*flag = 0;										// 받아온 flag값 바꿈(저격수 생성 x)
				trap = 0;										// trap(현재 while문) 종료, 게임 마침
			}
		}
		if (count1 < 0) {								// 집에 들어가지 못하고 자동이동모드 카운트 다 썼을 때,
			printf("자동이동모드 횟수 초과!\n");
			printf("저격수 등장 \n");
			*trap6 = 1;									// trap6(저격수 이동모드) 조건 발동시키고
			*flag = 1;									// 저격수 생성 조건 발동시키고
			trap = 0;									// trap(현재 while문) 종료, 게임 마침
		}
		count1--;										// 종료되지 않으면 count -1
	}
	return trap;										// 함수 종료후 값 반환
}
int trap3_auto_mode(int trap, int *trap2, int *trap4){	   // 경찰 등장 후 모드 전환
	D_num = check_quadrant(drunk_man_x, drunk_man_y);
	D_new_num = start_num;
	while (trap != 0) {
		system("cls");					
		all_auto_after_self();								// 수동이동 후 자동이동 함수 호출(분면 제한 없음)
		D_num = check_quadrant(drunk_man_x, drunk_man_y);	// 현재 분면값 D_num과 D_new_num에 저장
		D_new_num = D_num;
		random_P_step_auto(&police1_x, &police1_y, P1_num);	// 경찰1 현재 분면에서 자동이동
		if (police2_x != -1 && police2_y != -1)				// 경찰2가 좌표에 존재할 경우 현재 분면에서 자동이동
			random_P_step_auto(&police2_x, &police2_y, P2_num);
		map_print();
		Sleep(speed);
		if (D_num == P1_num){								// 술취한 사람과 경찰1의 분면이 같을 때 
			if (check_P(police1_x, police1_y) == 1) {		// 경찰1에 발각되면 수동이동 모드로 전환
				printf("경찰에 발각!\n");
				printf("수동 이동 모드 전환 \n");
				*trap4 = 1;									// trap4(수동 이동 모드) 조건 발동시키고
				trap = 0;									// trap(현재 while문) 종료
			}
		}
		if (D_num == P2_num) {								// 술취한 사람과 경찰2의 분면이 같을 때 
			if (check_P(police2_x, police2_y) == 1) {       // 경찰2에 발각되면 수동이동 모드로 전환
				printf("경찰에 발각!\n");
				printf("수동 이동 모드 전환 \n");
				*trap4 = 1;									// trap4(수동 이동 모드) 조건 발동시키고
				trap = 0;									// trap(현재 while문) 종료
			}
		}
		if (check_P(police1_x, police1_y) != 1 && check_P(police2_x, police2_y) != 1){
			if (D_num != P1_num && D_num != P2_num && D_num != start_num) {
				printf("이동 성공!");
				printf("자동 이동 모드 전환 \n");
				*trap2 = 1;									// trap2(자동 이동 모드) 조건 발동시키고
				trap = 0;									// trap(현재 while문) 종료
			}
		}
	}
	return trap;											// 함수 종료후 값 반환
}
int trap4_self_mode(int trap, int *trap3) {					// 수동 이동 모드 (경찰1, 2 버전)
	int count2 = r_t;										// 수동 이동 횟수 저장
	while (trap != 0) {
		while (count2 > 0) {								// 수동 이동 횟수가 끝나지 않으면 반복
			printf("목적지 : %d 사분면, 현재 위치 : %d 사분면", start_num, D_num);	// 목적지(이전 분면) 프린트
			random_step_self();								// 키보드로 수동 이동 함수 호출
			system("cls");
			map_print();
			count2--;										// count -1
			printf("남은 수동 이동 횟수 : %d\n", count2);
			Sleep(speed);
			if (count2 >= 0) {								// 이동 횟수가 남아있을 때,
				if (start_num == D_num) {					// 초기 분면으로 돌아가면 성공, 다시 자동 이동 모드로
					printf("수동이동 성공!\n");
					printf("자동 이동 모드 전환\n");
					*trap3 = 1;								// trap3(모드 전환 함수) 조건 발동시키고
					count2 = 0;								// count2(현재 while문) 종료
					trap = 0;								// trap(현재 while문) 종료
				}
			}
			if (count2 == 0) {								// 이동 횟수가 끝났을 때,
				if (start_num != D_num) {					// 이전 분면에 돌아가지 못하면 바로 die
					printf("수동이동 실패\n");
					printf("----- DIE -----\n");
					trap = 0;								// trap(현재 while문) 종료
				}
			}
			
		}
	}
	return trap;											// 함수 종료후 값 반환
}
int trap6_S_auto_mode(int trap) {							// 저격수 등장 후 자동 이동 모드
	while (trap != 0) {
		system("cls");
		random_P_step_auto(&drunk_man_x, &drunk_man_y, D_num);	// 술 취한 사람은 현재 분면에서 자동이동
		map_print();
		Sleep(speed);
		if (check_S(sniper_x, sniper_y) == 1) {				// 저격수에 발각되면 die
			printf("저격수에 발각!\n");
			printf("----- DIE -----\n");
			trap = 0;										// trap(현재 while문) 종료
		}
		if (map[drunk_man_x][drunk_man_y] == 9) {			// 술 취한 사람의 좌표값이 9(집)일경우
			map[drunk_man_x][drunk_man_y] = 5;				// 5(술 취한 사람)로 변경하고
			printf("집 도착!\n");
			trap = 0;										// trap(현재 while문) 종료
		}
	}
	return trap;											// 함수 종료후 값 반환
}

