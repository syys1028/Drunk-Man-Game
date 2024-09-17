// c��� ���� 2203098 �ڼ���
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

// ���� ����
int map[10][10] = { { 0,0,0,1,0,0,1,1,0,1 }, { 0,1,0,0,0,0,0,0,0,0 },   // �ʱ� ����, map 2���� �迭 ����, 0: ��, 1: ��
	{ 0,1,0,1,0,0,1,1,1,0 }, { 0,1,0,1,0,0,0,0,0,0 }, { 0,0,0,1,0,0,1,1,0,0 }, { 0,0,0,0,0,0,0,0,0,0 },
	{ 1,0,1,1,1,0,1,0,1,0 }, { 0,0,0,0,0,0,0,0,1,0 }, { 0,1,1,1,0,0,1,0,1,0 }, { 0,0,0,0,0,0,0,0,0,0 } };
int drunk_man_x, drunk_man_y;         // ������ ��� ��ǥ 5
int police1_x = -1, police1_y = -1;   // ����1 ��ǥ      61
int police2_x = -1, police2_y = -1;   // ����2 ��ǥ      62
int sniper_x = -1, sniper_y = -1;     // ���ݼ� ��ǥ     7
int start_num, D_num, D_new_num, P1_num, P2_num, S_num;	// start : ó�� ������ �и�, D_num : ����(Ȥ�� ����) �и�, D_new_num : �� �и�, P_num : ���� �и�, S_num : ���ݼ� �и�
int n_t, r_t, speed;				  // �ڵ� �̵� Ƚ��, ���� �̵� Ƚ��, �ڵ� �̵� �ӵ�

// �Լ� ȣ��
void map_print();											// map ��� �Լ�
int check_quadrant(int x, int y);							// ��и� üũ �Լ�
int check_back_quadrant(int nx, int ny);					// ���� �и� üũ �Լ�
int check_P(int x, int y);									// D�� ���� (x, y) �þ� üũ
int check_S(int x, int y);									// D�� ���ݼ� (x, y) �þ� üũ
void random_D_step_auto();									// �ʱ� ���� �ڵ��̵����
void random_P_step_auto(int *x, int *y, int num);			// �и� �̵� ���� ���� �� �ڵ� �̵�
void auto_after_self();										// ���� ���� �ڵ� �̵� ���
void auto_after_self_success();								// ���� 1 ���� ���� �ڵ��̵���� ����
void all_auto_after_self();									// ���� 1, 2 ���� ���� �ڵ��̵���� 
void random_step_self();									// Ű����� ���� �̵� �Լ�
int trap2_auto_S_mode(int trap, int *trap6, int *flag);		// �� �и鿡�� �ڵ� �̵� ���, ���ݼ� �����ϴ� ����
int trap3_auto_mode(int trap, int *trap2, int *trap4);		// ���� ���� �� ��� ��ȯ
int trap4_self_mode(int trap, int *trap3);					// ���� �̵� ��� (����1, 2 ����)
int trap6_S_auto_mode(int trap);							// ���ݼ� ���� �� �ڵ� �̵� ���

// ------------------------------------------ ���� �Լ� -----------------------------------------
void main() { 
	int count1 = 0, count2 = 0, result = -1, x1 = 0, y1 = 0, x2 = 0, y2 = 0;														// ī��Ʈ, ������� ����
	int trap1 = -1, trap2 = 0, trap3 = 0, trap4 = 0, trap5 = 0, trap6 = 0, trap7 = 0, flag = 0;     // while�� ����� ����
	int bar_x = -1, bar_y = -1;																		// ���� ��ǥ      8
	int home_x= -1, home_y = -1;																	// �� ��ǥ        9

	srand(time(NULL));               // random ����

	// ��ü ����
	while (result != 0) {				 // ���� ��ü�� ������ �� �����ϱ� 
		x1 = rand() % 10;			 // ���� ���� x��ǥ ����
		y1 = rand() % 10;		   	 // ���� ���� y��ǥ ����
		result = map[x1][y1];      // ������ ������ �Ǻ��� ����� result�� ����
		if (result == 0) {
			D_num = check_quadrant(x1, y1);   // ��ǥ�� �и��� üũ�ϴ� �Լ� ����, ����� return���� ����
			switch(D_num){			  // ������ 1��и��̸� ���� 4��и�
			case 1:
				x2 = rand() % 5 + 5;
				y2 = rand() % 5 + 5;
				break;
			case 2:					  // ������ 2��и��̸� ���� 3��и�
				x2 = rand() % 5 + 5;
				y2 = rand() % 5;
				break;
			case 3:                  // ������ 3��и��̸� ���� 2��и�
				x2 = rand() % 5;
				y2 = rand() % 5 + 5;
				break;
			case 4:                  // ������ 4��и��̸� ���� 1��и�
				x2 = rand() % 5;
				y2 = rand() % 5;
				break;
			}
			if (map[x1][y1] == 0 && map[x2][y2] == 0) {	   // ���� ��ǥ�� �� ��ǥ�� ��� 0(��)�̸�
				bar_x = x1;				// ������ �� x, y��ǥ�� �� ����
				bar_y = y1;
				home_x = x2;
				home_y = y2;
				map[bar_x][bar_y] = 8;	 // map�� ������ �� ���� ����
				map[home_x][home_y] = 9;
			}
		}
	}
	drunk_man_x = bar_x;						// �� ���� ����� ���� ��ǥ = ������ ��ǥ
	drunk_man_y = bar_y;
	start_num = D_num;							// �ʱ� ����(start_num = D_num = D_new_num)�� ����
	D_new_num = D_num;

	// ���� �ʱ� ����
	printf("�������� �Է����ּ���.\n");         // �ʱ� ���� �� �ܺ� �Է�, n_t, r_t, speed
	printf("�ڵ� �̵� ��� �̵� Ƚ�� : ");
	scanf_s("%d", &n_t);
	printf("���� �̵� ��� �̵� Ƚ�� : ");
	scanf_s("%d", &r_t);
	printf("�ڵ� �̵� ��忡�� �� ���� ����� �̵��ӵ�(1000 -> 1��): ");
	scanf_s("%d", &speed);

	// ���� ����
	printf("���� ����! \n");

	// trap1 : ù ȭ��. ���ۺи鿡�� �ڵ��̵�
	count1 = n_t;										// ī��Ʈ ���� (�ڵ� ���)
	while (trap1 != 0) {								// ���ۺи鿡�� �ڵ��̵�, ī��Ʈ üũ
		system("cls");									// while�� �� ������ ����� �ٽ� ����
		random_D_step_auto();							// �ʱ� ���� �ڵ��̵���� (�����и� : start_num, ���� �и� : D_num)
		map_print();									// �� ���
		Sleep(speed);									// ��� �ӵ� ����
		if (count1 >= 0) {								// �ڵ��̵���� ī��Ʈ �� ���� ���̳� �� ���� ��
			printf("���� �ڵ� �̵� Ƚ�� : %d\n", count1);	// ī��Ʈ ǥ��
			if (start_num != D_num) {					// �ٸ� �и����� �̵��ع�����
				printf("Ƚ�� �̳� �̵� ����! \n");
				printf("��ü �ڵ��̵� ��� ���� \n");
				trap2 = 1;								// trap2(��ü �ڵ��̵� ���) ���� �ߵ���Ű��
				trap1 = 0;								// trap1(���� while��) ����
			}
		}
		if (count1 < 0) {								// �ڵ��̵���� ī��Ʈ �� ���� ��,
			if (start_num != D_num) {					// ���� �и����� �̵��ϸ� (ī��Ʈ ������ �ٸ� �и����� �̵��� ������ ��� ����)
				printf("�ڵ��̵���� Ƚ�� �ʰ�!\n");
				printf("���� ���� \n");
				trap3 = 1;								// trap3(����1 ���� ���� �ڵ��̵�) ���� �ߵ���Ű��
				flag = 1;								// ���� ���� ���� �ߵ���Ű��
				trap1 = 0;								// trap1(���� while��) ����
			}
		}        
		count1--;										// ī��Ʈ -1
	}

	// trap2 : trap1(���ۺи鿡�� �ڵ��̵�) ���� �� ������ ��ü �ڵ��̵����
	while (trap2 != 0) {								// ��ü �ڵ��̵���� (����, ���ݼ� ���� x)
		system("cls");									// while�� �� ������ ����� �ٽ� ����
		random_D_step_auto();							// �ʱ� ���� �ڵ��̵���� (�����и� : D_num, ���� �и� : D_new_num)
		map_print();									// �� ���
		Sleep(speed);
		printf("�ڵ��̵� ��� ���� ��\n");
		if (map[drunk_man_x][drunk_man_y] == 9) {		// �� ���� ����� ��ǥ���� 9(��)�ϰ��
			map[drunk_man_x][drunk_man_y] = 5;			// 5(�� ���� ���)�� �����ϰ�
			printf("�� ����!\n");
			trap2 = 0;									// trap2(���� while��) ����, ���� ��ħ
		}
	}

	// trap3 -> flag : trap1(���ۺи鿡�� �ڵ��̵�) ���� �� ����1 ����
	while (flag != 0) {												// ����1 �����ϱ�
		police1_x = rand() % 10;									// ����1 ���� x��ǥ ����
		police1_y = rand() % 10;									// ����1 ���� y��ǥ ����
		P1_num = check_quadrant(police1_x, police1_y);				// ����1 ��ǥ�� ���� �и� ���
		if (P1_num == check_quadrant(drunk_man_x, drunk_man_y)) {	// ����1�� �и��� �� ���� ����� �Ѿ�� �и�� ���� ��� 
			result = map[police1_x][police1_y];						// ������ ������ �Ǻ��� ����� result�� ����
			if (result == 0) {										// ���̸� ���� ����
				flag = 0;											// flag(���� while��) ����, ���� ��ħ
			}
		}
	}

	// trap3 : ����1 ���� ����, ������ ������ �ڵ��̵�
	while (trap3 != 0) {									// ����1 ���� ���� �ڵ��̵�, ������ �� ���� ��� �и� ����
		system("cls");			 
		random_D_step_auto();								// �� ���� ��� �ڵ��̵� (�����и� : start_num, ���� �и� : D_num)
		random_P_step_auto(&police1_x, &police1_y, P1_num);	// ������ ���� �и鿡�� �ڵ��̵�
		map_print();
		Sleep(speed);
		D_num = check_quadrant(drunk_man_x, drunk_man_y);	// �� ���� ��� �и� �� üũ
		if (check_P(police1_x, police1_y) == 1) {			// ������ �߰��Ǹ� �����̵� ���� ��ȯ
			printf("������ �߰�!\n");
			printf("���� �̵� ��� ��ȯ \n");
			trap4 = 1;										// trap4(���� �߰� �� �������) ���� �ߵ���Ű��
			trap3 = 0;										// trap3(���� while��) ����
		}
		if (check_P(police1_x, police1_y) != 1){ 			 // ������ �߰����� �ʰ�
			if (D_num == check_quadrant(home_x, home_y)) {    // �� �и鿡 �̵��ϸ� ��ü �ڵ��̵����
				printf("������ �߰����� �ʰ� �� �и� �̵�!\n");
				printf("��ü �ڵ��̵� ��� ���� \n");
				trap2 = 1;									// trap2(��ü �ڵ��̵� ���) ���� �ߵ���Ű��
				trap3 = 0;									// trap3(���� while��) ����
			}
		}
	}
	
	// trap2 : �� �и� �ڵ��̵� (���ݼ� ����)
	if (trap2 == 1){												// trap2 ���ǿ� ������
		trap2 = trap2_auto_S_mode(trap2, &trap6, &flag);			// �� �и鿡�� �ڵ��̵��ϴ� �Լ� ȣ��, ī��Ʈ ������ ���ݼ� ����
		if (flag == 1) {											// flag(���ݼ� ���� �Լ�) ���� �����ϸ�
			while (flag != 0) {										// ���ݼ� ���� ����
				sniper_x = rand() % 10;								// ���ݼ� ���� x��ǥ ����
				sniper_y = rand() % 10;								// ���ݼ� ���� y��ǥ ����
				S_num = check_quadrant(sniper_x, sniper_y);			// ���ݼ� ��ǥ ���
				if (check_quadrant(drunk_man_x, drunk_man_y) == S_num) {	// �� ���� ����� ���ݼ� �и��� ������
					result = map[sniper_x][sniper_y];				// ������ ������ �Ǻ��� ����� result�� ����
					if (result == 1) {								// ���̸� �� ��� ���ݼ� �� ����
						map[sniper_x][sniper_y] = 7;				// ���ݼ� ��ġ ����
						flag = 0;									// flag(���� while��) ����
					}
				}
			}
		}
		if (trap6 == 1) {											// trap6 ���ǿ� ������
			trap6 = trap6_S_auto_mode(trap6);						// ���ݼ� �̵� �Լ� �ߵ�
		}
	}

	// trap4 : ���� �߰� �� ������� ����
	count2 = r_t;										// ī��Ʈ ���� (���� ���)
	while (trap4 != 0) {								// ������� ����, ī��Ʈ üũ
		while (count2 > 0) {							// �����̵� ī��Ʈ �� ���� ������ �ݺ�
			printf("������ : %d ��и�, ���� ��ġ : %d ��и�", start_num, D_num);	// �������� ���� ��ġ ���
			random_step_self();							// �����̵� ��� ����
			system("cls");
			map_print();
			count2--;									// ī��Ʈ -1
			printf("���� ���� �̵� Ƚ�� : %d\n", count2);
			Sleep(speed);
			if (count2 >= 0) {							// �̵� Ƚ���� �������� ��,
				if (start_num == D_num) {				// �ʱ� �и����� ���ư��� ����, �ٽ� �ڵ� �̵� ����
					printf("�����̵� ����!\n");
					printf("�ڵ� �̵� ��� ��ȯ\n");
					trap5 = 1;							// trap5(�����̵� ���� �� �ٽ� �ڵ��̵�) ���� �ߵ���Ű��
					count2 = 0;							// count2(���� while��) ����
					trap4 = 0;							// trap4(���� while��) ����
				}
			}
			if (count2 == 0) {							// �̵� Ƚ���� ������ ��,
				if (start_num != D_num) {				// ���ư��� ���ϸ� �ٷ� die
					printf("�����̵� ����\n");
					printf("----- DIE -----\n");
					trap4 = 0;							// trap4(���� while��) ����
				}
			}
		}
	}

	// trap5: �����̵� ���� �� �ٽ� �ڵ��̵� (���� �и鿡�� �ٽ� ����)
	count1 = n_t;											// ī��Ʈ ���� (�ڵ� ���)
	while (trap5 != 0) {                           // �ڵ��̵���� ����, ī��Ʈ üũ
		system("cls");
		auto_after_self();                           // ���� ���� �ڵ� �̵� ��� ����
		random_P_step_auto(&police1_x, &police1_y, P1_num);   // ������ ���� �и鿡�� �ڵ��̵�
		map_print();
		Sleep(speed);
		if (count1 >= 0) {                           // �ڵ��̵���� ī��Ʈ �� ���� ���� ��
			printf("���� �ڵ� �̵� Ƚ�� : %d\n", count1);
			if (start_num != D_num && P1_num != D_num) {   // start�� �ƴϰ� ����1 �и鵵 �ƴ� �ٸ� �и����� �̵��ع�����
				printf("Ƚ�� �̳� �̵� ����! \n");
				printf("�ڵ��̵� ��� ���� \n");
				trap7 = 1;                           // trap7(�ٽ� start�� ���ƿ��� ����) ���� �ߵ���Ű��
				trap5 = 0;                           // trap5(���� while��) ����
			}
			else count1--;                           // ī��Ʈ -1
		}
		if (count1 < 0) {                           // �ڵ��̵���� ī��Ʈ �� ���� ��,
			if (start_num != D_num && P1_num != D_num) {   // start�� �ƴϰ� ����1 �и鵵 �ƴ� �ٸ� �и����� �̵��ϸ�
				printf("�ڵ��̵���� Ƚ�� �ʰ�!\n");
				printf("����2 ���� \n");
				trap3 = 1;                           // trap3(���� ���� ���� �ڵ��̵�) ���� �ߵ���Ű��
				flag = 1;                           // ���� ���� ���� �ߵ���Ű��
				trap5 = 0;                           // trap5(���� while��) ����
			}
			else count1--;                           // ī��Ʈ -1
		}
	}
	while (trap7 != 0) {
		system("cls");
		auto_after_self_success();								// ����1 ���� ���� �ڵ��̵���� ����
		random_P_step_auto(&police1_x, &police1_y, P1_num);     // ������ ���� �и鿡�� �ڵ��̵�
		map_print();
		Sleep(speed);
		if (D_num == check_quadrant(home_x, home_y)) {    // �� �и鿡 �̵��ϸ� ��ü �ڵ��̵����
			printf("������ �߰����� �ʰ� �� �и� �̵�!\n");
			printf("��ü �ڵ��̵� ��� ���� \n");
			trap2 = 1;                           // trap2(��ü �ڵ��̵� ���) ���� �ߵ���Ű��
			trap7 = 0;                           // trap7(���� while��) ����
		}
	}

	// trap2 : �� �и� �ڵ��̵� (���ݼ� ����)
	if (trap2 == 1) {												// trap2 ���ǿ� ������
		trap2 = trap2_auto_S_mode(trap2, &trap6, &flag);			// �� �и鿡�� �ڵ��̵��ϴ� �Լ� ȣ��, ī��Ʈ ������ ���ݼ� ����
		if (flag == 1) {											// flag(���ݼ� ���� �Լ�) ���� �����ϸ�
			while (flag != 0) {										// ���ݼ� ����
				sniper_x = rand() % 10;								// ���ݼ� ���� x��ǥ ����
				sniper_y = rand() % 10;								// ���ݼ� ���� y��ǥ ����
				S_num = check_quadrant(sniper_x, sniper_y);			// ���ݼ� ��ǥ ���
				if (check_quadrant(drunk_man_x, drunk_man_y) == S_num) {	// �� ���� ����� ���ݼ� �и��� ������
					result = map[sniper_x][sniper_y];				// ������ ������ �Ǻ��� ����� result�� ����
					if (result == 1) {								// ���̸� �� ��� ���ݼ� �� ����
						map[sniper_x][sniper_y] = 7;				// ���ݼ� ��ġ ����
						flag = 0;									// flag(���� while��) ����
					}
				}
			}
		}
		if (trap6 == 1) {											// trap6 ���ǿ� ������
			trap6 = trap6_S_auto_mode(trap6);						// ���ݼ� �̵� �Լ� �ߵ�
		}
	}
	
	// trap3 -> flag: ����2 ���� ����, ������ ������ �ڵ��̵�
	while (flag != 0) {												// �������� ���� �����ϸ� ����2 �����ϱ�
		police2_x = rand() % 10;									// ����2 ���� x��ǥ ����
		police2_y = rand() % 10;									// ����2 ���� y��ǥ ����
		P2_num = check_quadrant(police2_x, police2_y);				// ����2 ��ǥ ���
		if (check_quadrant(drunk_man_x, drunk_man_y) == P2_num) {	// ����1�� �и��� �� ���� ����� �Ѿ�� �и�� ���� ��� 
			result = map[police2_x][police2_y];						// ������ ������ �Ǻ��� ����� result�� ����
			if (result == 0) {										// ���̸� ���� ����
				flag = 0;											// flag(���� while��) ����, ���� ��ħ
			}
		}
	}
	
	// trap3 : ����1,2 ���� ����, ������ ������ �ڵ��̵�
	while (trap2 != 1) {											// �и� �̵� �� trap2(�ڵ��̵���� ����) ���ǿ� ������ ��������, �ƴ� �� ���ѷ���
		if (trap3 == 1) {											// trap3(���� ������ ������ �ڵ��̵�) ����
			trap3 = trap3_auto_mode(trap3, &trap2, &trap4);			// ����1 ���� ���� �ڵ��̵�, ������ �� ���� ��� �и� ����
			if (trap4 == 1) trap4 = trap4_self_mode(trap4, &trap3);	// �����̵� Ƚ�� ���� �� �и鿡 ���� �ٽ� trap3, ������ �ֱ�
		}
	}
	// trap3 : ���� 1,2 �ڵ�-�������� �������� ����
	if (trap2 == 1) {												// trap2 ���ǿ� ������
		trap2 = trap2_auto_S_mode(trap2, &trap6, &flag);			// �ڵ��̵� Ƚ�� ���� �� �и鿡 ���� trap2
		if (flag == 1) {											// flag(���ݼ� ���� �Լ�) ���� �����ϸ�
			while (flag != 0) {										// ���ݼ� ����
				sniper_x = rand() % 10;								// ���ݼ� ���� x��ǥ ����
				sniper_y = rand() % 10;								// ���ݼ� ���� y��ǥ ����
				S_num = check_quadrant(sniper_x, sniper_y);			// ���ݼ� ��ǥ ���
				if (check_quadrant(drunk_man_x, drunk_man_y) == S_num) {	// �� ���� ����� ���ݼ� �и��� ������
					result = map[sniper_x][sniper_y];				// ������ ������ �Ǻ��� ����� result�� ����
					if (result == 1) {								// ���̸� �� ��� ���ݼ� �� ����
						map[sniper_x][sniper_y] = 7;				// ���ݼ� ��ġ ����
						flag = 0;									// flag(���� while��) ����
					}
				}
			}
		}
		if (trap6 == 1) {											// trap6(���ݼ� �̵� �Լ�) ���ǿ� ������
			trap6 = trap6_S_auto_mode(trap6);						// ���ݼ� �̵� �Լ� �ߵ�
		}
	}

}

// ------------------------------------------ �Լ� -----------------------------------------

void map_print() {								// map ��� �Լ�
	int copy[10][10] = { 0, };					// �ӽ÷� map�� ���� ������ copy 2���� �迭 ���� �� �ʱ�ȭ

	for (int i = 0; i < 10; i++) {				// for������ ���� map ���� copy�� �־���
		for (int j = 0; j < 10; j++) {
			copy[i][j] = map[i][j];
		}
	}
	copy[drunk_man_x][drunk_man_y] = 5;			// �� ���� ����� x,y��ǥ�� 5ǥ��
	if (police1_x != -1 && police1_y != -1) copy[police1_x][police1_y] = 61;	// ����1�� ��ǥ�� ������ ��� ����1�� x,y��ǥ�� 61ǥ��
	if (police2_x != -1 && police2_y != -1) copy[police2_x][police2_y] = 62;	// ����2�� ��ǥ�� ������ ��� ����2�� x,y��ǥ�� 63ǥ��
	if (sniper_x != -1 && sniper_y != -1) copy[sniper_x][sniper_y] = 7; 		// ���ݼ��� ��ǥ�� ������ ��� ���ݼ��� x,y��ǥ�� 7ǥ��

	for (int i = 0; i < 10; i++) {				// for�� �������� ����
		for (int j = 0; j < 10; j++) {
			if (copy[i][j] == 0) {				// 0�̸� ��, �� �簢�� ǥ��
				printf_s("�� ");
			}
			if (copy[i][j] == 1) {				// 1�̸� ��, �� �� �簢�� ǥ��
				printf_s("�� ");
			}
			if (copy[i][j] == 7) {				// 7�̸� ���ݼ�, S ǥ��
				printf_s("S  ");
			}
			if (copy[i][j] == 8) {				// 8�̸� ����, B ǥ��
				printf_s("B  ");
			}
			if (copy[i][j] == 9) {				// 9�̸� ��, H ǥ��
				printf_s("H  ");
			}
			if (copy[i][j] == 61) {				// 61�̸� ����1, P1 ǥ��
				printf_s("P1 ");
			}
			if (copy[i][j] == 62) {				// 62�̸� ����2, P2 ǥ��
				printf_s("P2 ");
			}
			if (copy[i][j] == 5) {				// 5�̸� �� ���� ���, D ǥ��
				printf_s("D  ");
			}
		}
		printf_s("\n");
	}
}

// check �Լ�
int check_quadrant(int x, int y) {										// ��и� üũ �Լ�
	int result = 0;					
	if (x == 0 || x == 1 || x == 2 || x == 3 || x == 4) {               // x���� 0 ~ 4�̰�
		if (y == 0 || y == 1 || y == 2 || y == 3 || y == 4)   result = 1;      // y���� 0 ~ 4�̸� 1��и�
		if (y == 5 || y == 6 || y == 7 || y == 8 || y == 9)   result = 2;      // y���� 5 ~ 9�̸� 2��и�
	}
	if (x == 5 || x == 6 || x == 7 || x == 8 || x == 9) {               // x���� 5 ~ 9�̰�
		if (y == 0 || y == 1 || y == 2 || y == 3 || y == 4)   result = 3;      // y���� 0 ~ 4�̸� 3��и�
		if (y == 5 || y == 6 || y == 7 || y == 8 || y == 9)   result = 4;      // y���� 5 ~ 9�̸� 4��и�
	}
	return result;
}
int check_back_quadrant(int nx, int ny) {								// ���� �и� üũ �Լ�(start_num - D_num - D_new_num) : �ʱ� �ڵ��̵��� ����
	int result = 0;
	if ((nx > -1 && nx < 10) && (ny > -1 && ny < 10)) {					// ��ǥ�� ���� ũ�⸦ �Ѿ�� ���� ��,
		if (map[nx][ny] == 0 || map[nx][ny] == 8 || map[nx][ny] == 9) {	// ���̰ų�, �����̰ų�, ���� ��
			if (D_num == start_num && D_num == D_new_num) {				// start, now, new�� ���� ���̸� �ʱ� ����
				if (check_quadrant(nx, ny) == D_num) {					// �ʱ� ���� ������ �׳� �̵�
					result = 1;
				}
				if (check_quadrant(nx, ny) != D_num) {					// �ʱ� ���� �ٸ��� �и��̵� �Ѵٴ� ��
					D_num = check_quadrant(nx, ny);						// now���� new ���� ���� �̵��� �и����� ��������
					D_new_num = check_quadrant(nx, ny);
					result = 1;
				}
			}
			if (D_num != start_num && D_num == D_new_num) {				// start�� �ٸ��� now, new�� ���� ���̸� (ù��° �̵�)
				if (check_quadrant(nx, ny) != start_num) {				// �����и��� start�� �ƴ� ��
					if (check_quadrant(nx, ny) == D_num) {				// ���� �и�� ������ �׳� �̵�
						result = 1;
					}
					if (check_quadrant(nx, ny) != D_new_num) {			// ���� �и��� �ƴϰ� ���ο� �и��̸� �и��̵� �Ѵٴ� ��
						D_new_num = check_quadrant(nx, ny);				// new ���� ���ο� �и����� ��������
						result = 1;
					}
				}
			}
			if (D_num != start_num && D_num != D_new_num) {				// start, now, new ���� �ٸ� ���̸� (�ι�° �̵�)
				if (check_quadrant(nx, ny) == D_new_num) {				// ���ο� �и��� D_new_num������ �̵� ����
					result = 1;
				}
			}
		}
	}
	return result;														// ������ ���� 0 ��ȯ
}
int check_P(int x, int y) {												// D�� ���� (x, y) �þ� üũ, ������ �þ߿� D�� �����Ѵٸ� x�� y�� �ϳ��� ���ƾ���
	int i = 0, result = 0;								
	D_num = check_quadrant(drunk_man_x, drunk_man_y);					// ������ ����� ���� ��ǥ �޾ƿ�
	if (drunk_man_x == x) {												// ������ ����� x��ǥ�� ������ x��ǥ�� ���� ���
		if (drunk_man_y < y) {											// ������ ����� y��ǥ�� ������ y��ǥ���� ���� ��(������ ����� ���� ����)
			for (i = 0; i < 5; i++) {									
				if (y - i == drunk_man_y) result = 1;					// ������ y��ǥ���� 0���� 4���� 1�� ���ҽ�Ű�� y1�� ������ 1 ��ȯ(����)
				if (map[x][y - i] == 1 || check_quadrant(x, y - 1) != D_num)  break;	// ���� �ٴٸ��ų� �и� ����� for�� ����
			}
		}
		if (drunk_man_y > y) {											// ������ ����� y��ǥ�� ������ y��ǥ���� ���� ��(������ ����� ���� ������)
			for (i = 0; i < 5; i++) {							
				if (y + i == drunk_man_y) result = 1;					// ������ y��ǥ���� 0���� 4���� 1�� ������Ű�� y1�� ������ 1 ��ȯ(����)
				if (map[x][y + i] == 1 || check_quadrant(x, y + 1) != D_num)  break;	// ���� �ٴٸ��ų� �и� ����� for�� ����
			}
		}
	}
	if (drunk_man_y == y) {												// ������ ����� y��ǥ�� ������ y��ǥ�� ���� ���
		if (drunk_man_x < x) {											// ������ ����� x��ǥ�� ������ x��ǥ���� ���� ��(������ ����� ���� ����)
			for (i = 0; i < 5; i++) {							
				if (x - i == drunk_man_x) result = 1;					// ������ x��ǥ���� 0���� 4���� 1�� ���ҽ�Ű�� y1�� ������ 1 ��ȯ(����)
				if (map[x - i][y] == 1 || check_quadrant(x - 1, y) != D_num)  break;	// ���� �ٴٸ��ų� �и� ����� for�� ����
			}
		}
		if (drunk_man_x > x) {											// ������ ����� y��ǥ�� ������ y��ǥ���� ���� ��(������ ����� ���� �Ʒ���)
			for (i = 0; i < 5; i++) {									
				if (x + i == drunk_man_x) result = 1;					// ������ x��ǥ���� 0���� 4���� 1�� ������Ű�� y1�� ������ 1 ��ȯ(����)
				if (map[x + i][y] == 1 || check_quadrant(x + 1, y) != D_num)  break;	// ���� �ٴٸ��ų� �и� ����� for�� ����
			}
		}
	}
	if (drunk_man_x == x && drunk_man_y == y) result = 1;				// ��ǥ�� ���Ƶ� ����
	return result;														// ������ ���� 0 ��ȯ		
}
int check_S(int x, int y) {												// D�� ���ݼ� (x, y) �þ� üũ, ���ݼ� �þ߿� D�� �����Ѵٸ� x�� y�� �ϳ��� ���ƾ���
	int result = 0;														
	D_num = check_quadrant(drunk_man_x, drunk_man_y);					// ������ ����� ���� ��ǥ �޾ƿ�
	if (drunk_man_x == x) {												// ������ ����� x��ǥ�� ���ݼ� x��ǥ�� ���� ���								
		if (y - 1 == drunk_man_y) {										// ���ݼ��� y��ǥ���� 1 �� ���� ������ ����� y��ǥ�� ����
			if (map[x][y - 1] == 0)										// ��ǥ�� �濡 �����ϸ� 1(������)
				result = 1;
		}
		if (y + 1 == drunk_man_y) {										// ���ݼ��� y��ǥ���� 1 ���� ���� ������ ����� y��ǥ�� ����
			if (map[x][y + 1] == 0)										// ��ǥ�� �濡 �����ϸ� 1(������)
				result = 1;
		}
	}
	if (drunk_man_y == y) {												// ������ ����� y��ǥ�� ���ݼ� y��ǥ�� ���� ���								
		if (x - 1 == drunk_man_x) {										// ���ݼ��� x��ǥ���� 1 �� ���� ������ ����� x��ǥ�� ����
			if (map[x - 1][y] == 0)										// ��ǥ�� �濡 �����ϸ� 1(������)
				result = 1;
		}
		if (x + 1 == drunk_man_x) {										// ���ݼ��� x��ǥ���� 1 ���� ���� ������ ����� x��ǥ�� ����
			if (map[x + 1][y] == 0)										// ��ǥ�� �濡 �����ϸ� 1(������)
				result = 1;
		}
	}
	if (drunk_man_x == x && drunk_man_y == y) result = 1;				// ��ǥ�� ���Ƶ� ����
	return result;														// ������ ���� 0 ��ȯ	
}

// �̵� �Լ�(�ڵ�)
void random_D_step_auto() {							// �ʱ� ���� �ڵ��̵���� (��: 0, �� : 1, �� : 2, �� : 3)
	int random_number, trap = -1, nx, ny;			
	while (trap != 0) {								// trap�� ������ ������ �� (����)
		nx = drunk_man_x;							// nx�� �� ���� ����� x��ǥ ����
		ny = drunk_man_y;							// ny�� �� ���� ����� y��ǥ ����
		random_number = rand() % 4;				
		switch (random_number) {					// 0~3������ ���� �����ϰ� �޾ƿ� ��
		case 0:										// 0�̸� ���� �̵�, x�� -1
			nx -= 1;
			break;
		case 1:										// 1�̸� �Ʒ��� �̵�, x�� +1
			nx += 1;
			break;
		case 2:										// 2�̸� ���ʷ� �̵�, y�� -1
			ny -= 1;
			break;
		case 3:										// 3�̸� �����ʷ� �̵�, y�� +1
			ny += 1;
			break;
		}
		if (check_back_quadrant(nx, ny) == 1) {		// ���� �и� üũ �Լ� ����. ���ǿ� �����ϸ�
			drunk_man_x = nx;						// �� ���� ����� x, y ��ǥ�� �ݿ�
			drunk_man_y = ny;
			trap = 0;								// trap(���� while��) ����
		}
	}
}
void random_P_step_auto(int *x, int *y, int num) {	// �и� �̵� ���� ���� �� �ڵ� �̵� (��: 0, �� : 1, �� : 2, �� : 3)
	int random_number, trap = -1, nx, ny;      
	while (trap != 0) {								// trap�� ������ ������ �� (����)
		nx = *x;									// nx�� x��ǥ ����
		ny = *y;									// ny�� y��ǥ ����
		random_number = rand() % 4;
		switch (random_number) {					// 0~3������ ���� �����ϰ� �޾ƿ� ��
		case 0:										// 0�̸� ���� �̵�, x�� -1
			nx -= 1;
			break;
		case 1:										// 1�̸� �Ʒ��� �̵�, x�� +1
			nx += 1;
			break;
		case 2:										// 2�̸� ���ʷ� �̵�, y�� -1
			ny -= 1;
			break;
		case 3:										// 3�̸� �����ʷ� �̵�, y�� +1
			ny += 1;
			break;
		}
		if ((nx > -1 && nx < 10) && (ny > -1 && ny < 10)) {		// ��ǥ�� ���� ũ�⸦ �Ѿ�� ���� ��,
			if ((map[nx][ny] == 0) || (map[nx][ny] == 8) || (map[nx][ny] == 9)) {	// ���̰ų�, �����̰ų�, ���� ��
				if (check_quadrant(nx, ny) == num) {		// �и��� ���� �и�� ��ġ�ϸ�
					*x = nx;								// ������ ����ؼ� ���� x, y ��ǥ�� �ݿ�
					*y = ny;
					trap = 0;								// trap(���� while��) ����
				}
			}
		}
	}
}
void auto_after_self() {							// ���� ���� �ڵ� �̵� ��� (��: 0, �� : 1, �� : 2, �� : 3)
	int random_number, trap = -1, nx, ny;
	while (trap != 0) {								// trap�� ������ ������ �� (����)
		nx = drunk_man_x;							// nx�� �� ���� ����� x��ǥ ����
		ny = drunk_man_y;							// ny�� �� ���� ����� y��ǥ ����
		random_number = rand() % 4;
		switch (random_number) {					// 0~3������ ���� �����ϰ� �޾ƿ� ��
		case 0:										// 0�̸� ���� �̵�, x�� -1
			nx -= 1;
			break;
		case 1:										// 1�̸� �Ʒ��� �̵�, x�� +1
			nx += 1;
			break;
		case 2:										// 2�̸� ���ʷ� �̵�, y�� -1
			ny -= 1;
			break;
		case 3:										// 3�̸� �����ʷ� �̵�, y�� +1
			ny += 1;
			break;
		}
		if ((nx > -1 && nx < 10) && (ny > -1 && ny < 10)) { 	// ��ǥ�� ���� ũ�⸦ �Ѿ�� ���� ��,
			if ((map[nx][ny] == 0) || (map[nx][ny] == 8) || (map[nx][ny] == 9)) {	// ���̰ų�, �����̰ų�, ���� ��
				if (check_quadrant(nx, ny) != P1_num) {		// �и��� ������ �и����� �ٽ� ���ư��� ������
					drunk_man_x = nx;						// �� ���� ����� x, y ��ǥ�� �ݿ�
					drunk_man_y = ny;
					D_num = check_quadrant(drunk_man_x, drunk_man_y);	// �̵� �� D_num�� ���� �и鰪 ����
					trap = 0;								// trap(���� while��) ����
				}
			}
		}
	}
}
void auto_after_self_success() {				// ����1 ���� ���� �ڵ��̵���� ����
	int random_number, trap = -1, nx, ny;
	while (trap != 0) {                        // trap�� ������ ������ �� (����)
		nx = drunk_man_x;                     // nx�� �� ���� ����� x��ǥ ����
		ny = drunk_man_y;                     // ny�� �� ���� ����� y��ǥ ����
		random_number = rand() % 4;
		switch (random_number) {               // 0~3������ ���� �����ϰ� �޾ƿ� ��
		case 0:                              // 0�̸� ���� �̵�, x�� -1
			nx -= 1;
			break;
		case 1:                              // 1�̸� �Ʒ��� �̵�, x�� +1
			nx += 1;
			break;
		case 2:                              // 2�̸� ���ʷ� �̵�, y�� -1
			ny -= 1;
			break;
		case 3:                              // 3�̸� �����ʷ� �̵�, y�� +1
			ny += 1;
			break;
		}
		if ((nx > -1 && nx < 10) && (ny > -1 && ny < 10)) {    // ��ǥ�� ���� ũ�⸦ �Ѿ�� ���� ��,
			if ((map[nx][ny] == 0) || (map[nx][ny] == 8) || (map[nx][ny] == 9)) {   // ���̰ų�, �����̰ų�, ���� ��
				if (check_quadrant(nx, ny) != P1_num && check_quadrant(nx, ny) != start_num) {      // �и��� ������ �и�� ���ۺи����� �ٽ� ���ư��� ������
					drunk_man_x = nx;                  // �� ���� ����� x, y ��ǥ�� �ݿ�
					drunk_man_y = ny;
					D_num = check_quadrant(drunk_man_x, drunk_man_y);   // �̵� �� D_num�� ���� �и鰪 ����
					trap = 0;                        // trap(���� while��) ����
				}
			}
		}
	}
}
void all_auto_after_self() {						// ���� 1, 2 ���� ���� �ڵ��̵���� (�и� ���� ����)
	int random_number, trap = -1, nx, ny;
	while (trap != 0) {								// trap�� ������ ������ �� (����)
		nx = drunk_man_x;							// nx�� �� ���� ����� x��ǥ ����
		ny = drunk_man_y;							// ny�� �� ���� ����� y��ǥ ����
		random_number = rand() % 4;
		switch (random_number) {					// 0~3������ ���� �����ϰ� �޾ƿ� ��
		case 0:										// 0�̸� ���� �̵�, x�� -1
			nx -= 1;
			break;
		case 1:										// 1�̸� �Ʒ��� �̵�, x�� +1
			nx += 1;
			break;
		case 2:										// 2�̸� ���ʷ� �̵�, y�� -1
			ny -= 1;
			break;
		case 3:										// 3�̸� �����ʷ� �̵�, y�� +1
			ny += 1;
			break;
		}
		if ((nx > -1 && nx < 10) && (ny > -1 && ny < 10)) { 	// ��ǥ�� ���� ũ�⸦ �Ѿ�� ���� ��,
			if ((map[nx][ny] == 0) || (map[nx][ny] == 8) || (map[nx][ny] == 9)) {	// ���̰ų�, �����̰ų�, ���� ��
				drunk_man_x = nx;							// �� ���� ����� x, y ��ǥ�� �ݿ�
				drunk_man_y = ny;
				D_num = check_quadrant(drunk_man_x, drunk_man_y);	// �̵� �� D_num�� ���� �и鰪 ����
				trap = 0;									// trap(���� while��) ����
			}
		}
	}
}

// �̵� �Լ� (����)
void random_step_self() {			// Ű����� ���� �̵� �Լ�
	int trap = 1, key, nx, ny;      
	while (trap != 0) {
		key = _getch();				// ù ��° key�� �޾ƿͼ� ����
		key = _getch();				// �� ��° key�� �޾ƿͼ� ����
		nx = drunk_man_x;			// nx�� �� ���� ����� x��ǥ ����
		ny = drunk_man_y;			// ny�� �� ���� ����� y��ǥ ����
		switch (key) {
		case 72:					// Ű�� ���� 72�� ȭ��ǥ ����
			nx -= 1;
			break;
		case 80:					// Ű�� ���� 80�� ȭ��ǥ �Ʒ���
			nx += 1;
			break;
		case 75:					// Ű�� ���� 75�� ȭ��ǥ ��������
			ny -= 1;
			break;
		case 77:					// Ű�� ���� 77�� ȭ��ǥ ����������
			ny += 1;
			break;
		}
		if ((nx > -1 && nx < 10) && (ny > -1 && ny < 10)) { 	// ��ǥ�� ���� ũ�⸦ �Ѿ�� ���� ��,
			if ((map[nx][ny] == 0) || (map[nx][ny] == 8) || (map[nx][ny] == 9)) {	// ���̰ų�, �����̰ų�, ���� ��
				if (D_num == check_quadrant(nx, ny) || start_num == check_quadrant(nx, ny)) {	// ���� �и鿡�� �̵��ϰų� ���ۺи鿡 ���ư���
					drunk_man_x = nx;							// �� ���� ����� x, y ��ǥ�� �ݿ�
					drunk_man_y = ny;
					D_num = check_quadrant(drunk_man_x, drunk_man_y);	// �̵� �� D_num�� ���� �и鰪 ����
					trap = 0;											// trap(���� while��) ����
				}
			}
		}
	}
}

// trap �Լ� (����, �ݺ�, ȣ��)
int trap2_auto_S_mode(int trap, int *trap6, int *flag) {		// �� �и鿡�� �ڵ� �̵� ���, ���ݼ� �����ϴ� ����
	int count1 = n_t;											// �ڵ� �̵� Ƚ�� ����
	while (trap != 0) {
		system("cls");
		random_P_step_auto(&drunk_man_x, &drunk_man_y, D_num);	// �� ���� ����� ���� �и鿡�� �ڵ��̵�
		map_print();
		Sleep(speed); 
		printf("�ڵ��̵� ��� ���� ��\n");
		if (count1 >= 0) {										// �ڵ��̵���� ī��Ʈ �� ���� ���� ��
			printf("���� �ڵ� �̵� Ƚ�� : %d\n", count1);
			if (map[drunk_man_x][drunk_man_y] == 9) {			// �� ��ǥ�� ������ ���� ����
				map[drunk_man_x][drunk_man_y] = 5;				// ��ǥ�� �� ���� ��� ����ְ�
				printf("�� ����!\n");
				*flag = 0;										// �޾ƿ� flag�� �ٲ�(���ݼ� ���� x)
				trap = 0;										// trap(���� while��) ����, ���� ��ħ
			}
		}
		if (count1 < 0) {								// ���� ���� ���ϰ� �ڵ��̵���� ī��Ʈ �� ���� ��,
			printf("�ڵ��̵���� Ƚ�� �ʰ�!\n");
			printf("���ݼ� ���� \n");
			*trap6 = 1;									// trap6(���ݼ� �̵����) ���� �ߵ���Ű��
			*flag = 1;									// ���ݼ� ���� ���� �ߵ���Ű��
			trap = 0;									// trap(���� while��) ����, ���� ��ħ
		}
		count1--;										// ������� ������ count -1
	}
	return trap;										// �Լ� ������ �� ��ȯ
}
int trap3_auto_mode(int trap, int *trap2, int *trap4){	   // ���� ���� �� ��� ��ȯ
	D_num = check_quadrant(drunk_man_x, drunk_man_y);
	D_new_num = start_num;
	while (trap != 0) {
		system("cls");					
		all_auto_after_self();								// �����̵� �� �ڵ��̵� �Լ� ȣ��(�и� ���� ����)
		D_num = check_quadrant(drunk_man_x, drunk_man_y);	// ���� �и鰪 D_num�� D_new_num�� ����
		D_new_num = D_num;
		random_P_step_auto(&police1_x, &police1_y, P1_num);	// ����1 ���� �и鿡�� �ڵ��̵�
		if (police2_x != -1 && police2_y != -1)				// ����2�� ��ǥ�� ������ ��� ���� �и鿡�� �ڵ��̵�
			random_P_step_auto(&police2_x, &police2_y, P2_num);
		map_print();
		Sleep(speed);
		if (D_num == P1_num){								// ������ ����� ����1�� �и��� ���� �� 
			if (check_P(police1_x, police1_y) == 1) {		// ����1�� �߰��Ǹ� �����̵� ���� ��ȯ
				printf("������ �߰�!\n");
				printf("���� �̵� ��� ��ȯ \n");
				*trap4 = 1;									// trap4(���� �̵� ���) ���� �ߵ���Ű��
				trap = 0;									// trap(���� while��) ����
			}
		}
		if (D_num == P2_num) {								// ������ ����� ����2�� �и��� ���� �� 
			if (check_P(police2_x, police2_y) == 1) {       // ����2�� �߰��Ǹ� �����̵� ���� ��ȯ
				printf("������ �߰�!\n");
				printf("���� �̵� ��� ��ȯ \n");
				*trap4 = 1;									// trap4(���� �̵� ���) ���� �ߵ���Ű��
				trap = 0;									// trap(���� while��) ����
			}
		}
		if (check_P(police1_x, police1_y) != 1 && check_P(police2_x, police2_y) != 1){
			if (D_num != P1_num && D_num != P2_num && D_num != start_num) {
				printf("�̵� ����!");
				printf("�ڵ� �̵� ��� ��ȯ \n");
				*trap2 = 1;									// trap2(�ڵ� �̵� ���) ���� �ߵ���Ű��
				trap = 0;									// trap(���� while��) ����
			}
		}
	}
	return trap;											// �Լ� ������ �� ��ȯ
}
int trap4_self_mode(int trap, int *trap3) {					// ���� �̵� ��� (����1, 2 ����)
	int count2 = r_t;										// ���� �̵� Ƚ�� ����
	while (trap != 0) {
		while (count2 > 0) {								// ���� �̵� Ƚ���� ������ ������ �ݺ�
			printf("������ : %d ��и�, ���� ��ġ : %d ��и�", start_num, D_num);	// ������(���� �и�) ����Ʈ
			random_step_self();								// Ű����� ���� �̵� �Լ� ȣ��
			system("cls");
			map_print();
			count2--;										// count -1
			printf("���� ���� �̵� Ƚ�� : %d\n", count2);
			Sleep(speed);
			if (count2 >= 0) {								// �̵� Ƚ���� �������� ��,
				if (start_num == D_num) {					// �ʱ� �и����� ���ư��� ����, �ٽ� �ڵ� �̵� ����
					printf("�����̵� ����!\n");
					printf("�ڵ� �̵� ��� ��ȯ\n");
					*trap3 = 1;								// trap3(��� ��ȯ �Լ�) ���� �ߵ���Ű��
					count2 = 0;								// count2(���� while��) ����
					trap = 0;								// trap(���� while��) ����
				}
			}
			if (count2 == 0) {								// �̵� Ƚ���� ������ ��,
				if (start_num != D_num) {					// ���� �и鿡 ���ư��� ���ϸ� �ٷ� die
					printf("�����̵� ����\n");
					printf("----- DIE -----\n");
					trap = 0;								// trap(���� while��) ����
				}
			}
			
		}
	}
	return trap;											// �Լ� ������ �� ��ȯ
}
int trap6_S_auto_mode(int trap) {							// ���ݼ� ���� �� �ڵ� �̵� ���
	while (trap != 0) {
		system("cls");
		random_P_step_auto(&drunk_man_x, &drunk_man_y, D_num);	// �� ���� ����� ���� �и鿡�� �ڵ��̵�
		map_print();
		Sleep(speed);
		if (check_S(sniper_x, sniper_y) == 1) {				// ���ݼ��� �߰��Ǹ� die
			printf("���ݼ��� �߰�!\n");
			printf("----- DIE -----\n");
			trap = 0;										// trap(���� while��) ����
		}
		if (map[drunk_man_x][drunk_man_y] == 9) {			// �� ���� ����� ��ǥ���� 9(��)�ϰ��
			map[drunk_man_x][drunk_man_y] = 5;				// 5(�� ���� ���)�� �����ϰ�
			printf("�� ����!\n");
			trap = 0;										// trap(���� while��) ����
		}
	}
	return trap;											// �Լ� ������ �� ��ȯ
}

