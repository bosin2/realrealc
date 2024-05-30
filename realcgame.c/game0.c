#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define ITEM_COUNT 14      // 총 아이템의 수
#define ITEMS_TO_DISPLAY 5 // 한 번에 표시할 아이템의 수

//글씨한글자씩 출력해주는 함수
void print_slow(const char* str) {
    for (int i = 0; i < strlen(str); i++) {
        printf("%c", str[i]);
        fflush(stdout);
        Sleep(30);
    }
}

//출력 속도를 조절
void print_slow2(const char* str, int speed) {
    for (int j = 0; j < strlen(str); j++) {
        printf("%c", str[j]);
        fflush(stdout);
        Sleep(speed);
    }
}
//정수 출력할수있게해주는거
void print_slow3(const char* str, int speed, int* replace_int, int replace_count) {
    int replace_index = 0;
    for (int j = 0; j < strlen(str); j++) {
        if (str[j] == '%') {
            printf("%d", replace_int[replace_index]);
            replace_index++;
        }
        else {
            printf("%c", str[j]);
        }
        fflush(stdout);
        Sleep(speed);
    }
}

//터미널창 크기 변환해주는 함수
void set_console_size(int width, int height) {
    char command[100];
    sprintf(command, "mode con: cols=%d lines=%d", width, height);
    system(command);
}
//원하는 좌표에 글자 출력하는 함수
void print_at(int x, int y, const char* str) {
    COORD pos = { x, y };
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, pos);
    printf("%s", str);
}

// 좌표로 커서변경해주는 함수
void move_cursor(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, pos);
}

//좌표에 천천히 출력
void print_slow_at(int x, int y, const char* str) {
    move_cursor(x, y);
    print_slow(str);
}

//텍스트 지워주는 함수
void clear_text() {
    system("cls");
}
//목숨0일때 죽는거
void Death() {
    print_slow("뒤짐");
    Sleep(1000);
    exit(0);
}

//함수값 초기화
int totalGold = 0;
int totalStrength = 0;
int totalHeart = 3;
int totalHealth = 100;
//일반 적 체력
int total_youHealth = 100;
//보스 체력
int total_bossHealth = 200;
//골드
void Gold(int amount) {
    totalGold += amount;
    if (totalGold <= 0) {
        totalGold = 0;
    }
}
//공격력
void Strength(int amount) {
    totalStrength += amount;
    if (totalStrength <= 0) {
        totalStrength = 0;
    }
}
//목숨
void Heart(int amount) {
    totalHeart += amount;
    if (totalHeart == 0) {
        Death();
    }
}
//체력
void Health(int amount) {
    totalHealth += amount;
    if (totalHealth <= 0) {
        Heart(-1);
        Health(totalHealth);
    }
    if (totalHealth > 100) {
        totalHealth = 100;
    }
}
//일반적체력
void youHealth(int amount) {
    total_youHealth += amount;
}
//보스체력
void bossHealth(int amount) {
    total_bossHealth += amount;
}
//아이템용 무작위로 섞어주는 함수
void shuffle(int* array, int n) {
    for (int i = 0; i < n; ++i) {
        int j = rand() % n;
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}
// 네모의 위치를 업데이트하고 화면을 새로 그리는 함수
void drawSquare(int position) {
    clear_text();
    // 네모 출력
    for (int i = 0; i < position; i++) {
        printf(" ");
    }
    printf("■\n\n타이밍에 맞춰 'p'키를 눌러 공격하세요!\n");

    // 화살표 출력
    printf("        ↓\n");
}

// 게임 로직을 처리하는 함수
int updateGame(int* position, int* direction) {
    // 네모의 위치와 방향 조정
    *position += *direction;
    if (*position == 0 || *position == 20) {
        *direction *= -1;
    }

    // 키 입력 처리
    if (_kbhit()) {
        char ch = _getch();
        if (ch == 'p') {
            if (*position >= 8 && *position <= 12) {
                printf("Great!\n");
                return 1; // 성공
            }
            else {
                printf("Miss...\n");
                return 0; // 실패
            }
        }
    }
    return -1;
}

// 메인 게임 루프 함수
void gameLoop() {
    int position = 0; // 네모의 위치
    int direction = 1; // 네모의 이동 방향
    int result;

    while (1) {
        drawSquare(position);
        result = updateGame(&position, &direction);
        if (result != -1) {
            break; // 게임 종료
        }
        Sleep(100); // 0.1초 대기
    }
}

//이경민특수적공격함수
void exgameLoop() {
    int position = 0; // 네모의 위치
    int direction = 1; // 네모의 이동 방향
    int result;
    int successCount = 0; // 성공 횟수

    while (1) {
        drawSquare(position);
        result = updateGame(&position, &direction);
        if (result == 1) {
            successCount++;
            if (successCount == 3) {
                printf("3번 성공했습니다! 적을 쓰러트렸습니다!\n");
                break;
            }
        }
        else if (result == 0) {
            successCount = 0; // 실패하면 성공 횟수 초기화
        }
        Sleep(100); // 0.1초 대기
    }
}
//이벤트 구조체
typedef struct {
    char* event_name;
    void (*event_Function)(void);
} Event;
//전투 구조체
typedef struct {
    char* fight_name;
    void (*fight_Function)(void);
} Fight;
//장비 구조체
typedef struct {
    char* weapon_name;
    void (*weapon_Function)(int*, int*);
    int price;
} Weapon;
//특수적 구조체
typedef struct {
    char* special_name;
    void (*special_Function)(void);
} Special;
//보스 구조체
typedef struct {
    char* boss_name;
    void (*boss_Function)(void);
} Boss;
//스탯
void status() {
    move_cursor(40, 5);
    print_at(40, 3, " _______상 태 창_______\n");
    print_at(40, 4, "|                     |\n");
    move_cursor(40, 5);
    printf("| > 소지금: %3d       |\n", totalGold);
    print_at(40, 6, "|                     |\n");
    move_cursor(40, 7);
    printf("| > 공격력: %3d       |\n", totalStrength);
    print_at(40, 8, "|                     |\n");
    move_cursor(40, 9);
    printf("| > 체력: %3d         |\n", totalHealth);
    print_at(40, 10, "|                     |\n");
    move_cursor(40, 11);
    printf("| > 목숨: %3d         |\n", totalHeart);
    print_at(40, 12, "|_____________________|\n");
}
//전투 ui
void Fight_UI() {
    status();
    print_at(4, 3, "_______공 격 창________\n");
    print_at(4, 4, "|                     |\n");
    print_at(4, 5, "|   > 1. 공격하기     |\n");
    print_at(4, 6, "|                     |\n");
    print_at(4, 7, "|                     |\n");
    print_at(4, 8, "|   > 2. 방어하기     |\n");
    print_at(4, 9, "|                     |\n");
    print_at(4, 10, "|                     |\n");
    print_at(4, 11, "|   > 3. 도망가기     |\n");
    print_at(4, 12, "|_____________________|\n");
    print_at(0, 40, "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");

}
//특수 적 및 보스 ui
void Fight_UI2() {
    status();
    print_at(4, 3, "_______공 격 창________\n");
    print_at(4, 4, "|                     |\n");
    print_at(4, 5, "|   > 1. 공격하기     |\n");
    print_at(4, 6, "|                     |\n");
    print_at(4, 7, "|                     |\n");
    print_at(4, 8, "|   > 2. 방어하기     |\n");
    print_at(4, 9, "|                     |\n");
    print_at(4, 10, "|                     |\n");
    print_at(4, 11, "|   > 3. 대화하기     |\n");
    print_at(4, 12, "|_____________________|\n");
    print_at(0, 40, "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");

};

//적들
//야생멧돼지
void fight1() {
    clear_text();
    Fight_UI();
    print_at(49, 24, "   ¯ ╭━━━━╮___         ");
    print_at(49, 25, "   . | :::::::::: / ___ /  ");
    print_at(49, 26, " * < | :::::::::(｡ ●ω●｡)");
    print_at(49, 27, "    >し------し-- - Ｊ     ");
    move_cursor(15, 35);
    print_slow("야생멧돼지가 나타났다 ! 귀여워 !");
    Sleep(1000);
    print_at(15, 45, "                                                           ");
    while (totalHealth > 0 && total_youHealth > 0) {
        print_at(15, 45, "돼지는 꿀꿀거리기를 준비한다... 멧돼지 아니야 ?");
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        int choice;
        move_cursor(15, 46);
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            move_cursor(15, 45);
            print_slow("공격하기를 선택했다 ! 타이밍에 맞춰 키를 눌러야해 !");
            Sleep(1000);
            print_at(15, 45, "                                                           ");

            break;
        case 2:
            move_cursor(15, 45);
            print_slow("방어하기를 선택했다 ! 어디로 공격이 들어올까 ?? ");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            break;

        case 3:
            move_cursor(15, 45);
            print_slow("도망가기를 선택했다 ! 도망칠수 있을까 ...?");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            int run = rand() % 100;
            Sleep(1000);

            if (run < 30) {
                move_cursor(15, 45);
                print_slow("도망가기에 성공했다 ! 나이스 !!!");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                return;
            }

            else {
                move_cursor(15, 45);
                print_slow("도망치기에 실패했다... 다시 맞서 싸워야해");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                break;
            }
        default:
            print_slow_at(15, 45, "지금 뭐하는거야 !!! 소중한 한 턴을 낭비 했다...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            break;

        }
        print_slow_at(15, 45, "멧돼지는 꿀꿀거린다 !\n");
        int youDamage = rand() % 10 + 20;
        printf("으악! %d의 피해를 입었다!\n", youDamage);
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        totalHealth -= youDamage;
        status();
        Sleep(1000);

        if (totalHealth <= 0) {
            print_slow_at(15, 45, "모든 체력이 소모 되었다...\n");
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            Heart(-1);
            status();
            Sleep(1000);
            return 0;
        }
        else if (total_youHealth <= 0) {
            print_slow_at(15, 45, "멧돼지를 처치했다 ! !");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "멧돼지를 처치하여 20G를 얻었다 !");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            Gold(20);
            status();
            Sleep(1000);
            print_at(15, 45, "                                                       ");

            return 0;
        }
    }
}

//과제에미친수뭉이
void fight2() {
    clear_text();
    Fight_UI();
    print_at(49, 17, "     @     @      @@@@@        ");
    print_at(49, 18, "      @   @       @   @        ");
    print_at(49, 19, "        @         @@@@@        ");
    print_at(49, 20, "      @@@@@         @          ");
    print_at(49, 21, "        @         @@@@@        ");
    print_at(49, 22, "        @           @          ");
    print_at(49, 23, "       @@@@@@@@@@@@@@          ");
    print_at(49, 24, "    @@@@             @@@@      ");
    print_at(49, 25, "   @                      @    ");
    print_at(49, 26, "    @                    @     ");
    print_at(49, 27, "   @      \        /      @    ");
    print_at(49, 28, "   @     ●       ●      @    ");
    print_at(49, 29, "    @         ㅅ         @     ");
    print_at(49, 29, "      @     _______    @      ");
    print_at(49, 30, "    /@#@#@ |       | @#@#@\    ");
    print_at(49, 31, "    @      @ 과 제 @       @   ");
    print_at(49, 32, "     \@#@#@|       |,@#@#@/    ");
    print_at(49, 33, "       @    ---------  @       ");
    print_at(49, 34, "        @     ____    @        ");
    print_at(49, 35, "        @    @   @    @        ");
    print_at(49, 36, "         @@@@@   @@@@@         ");
    move_cursor(15, 45);
    print_slow("과제에 미친 수뭉이가 나타났다 ! 무서워 !");
    Sleep(1000);
    print_at(15, 45, "                                                           ");

    print_at(15, 45, "수뭉이는 과제던지기를 준비한다 !!! 어떻게 하지 ???");
    Sleep(1000);
    print_at(15, 45, "                                                           ");
    while (totalHealth > 0 && youHealth > 0) {
        print_at(15, 45, "수뭉이는 과제던지기를 준비한다 !");
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        int choice;
        move_cursor(15, 46);
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            move_cursor(15, 45);
            print_slow("공격하기를 선택했다 ! 타이밍에 맞춰 키를 눌러야해 !");
            Sleep(1000);
            print_at(15, 45, "                                                           ");

            break;
        case 2:
            move_cursor(15, 45);
            print_slow("방어하기를 선택했다 ! 어디로 공격이 들어올까 ?? ");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            break;

        case 3:
            move_cursor(15, 45);
            print_slow("도망가기를 선택했다 ! 도망칠수 있을까 ...?");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            int run = rand() % 100;
            Sleep(1000);

            if (run < 30) {
                move_cursor(15, 45);
                print_slow("도망가기에 성공했다 ! 나이스 !!!");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                return;
            }

            else {
                move_cursor(15, 45);
                print_slow("도망치기에 실패했다... 다시 맞써 싸워야해");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                break;
            }
        default:
            print_at(15, 45, "지금 뭐하는거야 !!! 소중한 한 턴을 낭비 했다...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            break;

        }
        print_slow_at(15, 45, "수뭉이는 과제를 던진다 !\n");
        int youDamage = rand() % 10 + 20;
        move_cursor(15, 45);
        printf("으악! %d의 피해를 입었다!\n", youDamage);
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        totalHealth -= youDamage;
        status();
        Sleep(1000);

        if (totalHealth <= 0) {
            print_slow_at(15, 45, "모든 체력이 소모 되었다...\n");
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            Heart(-1);
            status();
            Sleep(1000);
            return 0;
        }
        else if (total_youHealth <= 0) {
            print_slow_at(15, 45, "과제에 미친 수뭉이를 처치했다 ! !");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "수뭉아... 과제 힘내");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "수뭉이를 편하게 해주어 20G를 얻었다");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            Gold(20);
            status();
            Sleep(1000);
            print_at(15, 45, "                                                       ");

            return 0;
        }
    }
}

//조별과제응원빌런
void fight3() {
    clear_text();
    Fight_UI();

    print_at(49, 20, "    ___________________________________           ");
    print_at(49, 21, "   |       간      전                  |          ");
    print_at(49, 22, "   |         바레    빠집니다          |          ");
    print_at(49, 23, "    ￣￣￣￣￣ヽ___ノ￣￣￣￣￣￣￣￣￣           ");
    print_at(49, 24, "        Ｏ                                ");
    print_at(49, 25, "         o                                ");
    print_at(49, 26, "        ,. ─冖'⌒'─､                    ");
    print_at(49, 27, "       ノ           ＼              ");
    print_at(49, 28, "       / ,r‐へへく⌒'￢､   ヽ             ");
    print_at(49, 29, "      {ノ へ._、 ,,／~`  〉 ｝            ");
    print_at(49, 30, "     ／プ￣￣`y'¨Y´￣￣ヽ─}j=く           ");
    print_at(49, 31, "    ノ /レ'>ー{___ｭ`ーー'  ﾘ,ｲ}             ");
    print_at(49, 32, "   / _勺 ｲ;；∵r===､､∴'∵;  シ             ");
    print_at(49, 33, "  ,/ └'ノ ＼  ご`    ノ{ー—､__          ");
    print_at(49, 34, "  人＿_/ー┬ー个-､＿＿,,.. ‐´ 〃`ァーｧー＼   ");
    print_at(49, 35, ". /  |／ |::::|､      〃 /:::/   ヽ  ");
    print_at(49, 36, "/    |  |::::|＼､_________／ /:::/〃   |  ");
    move_cursor(15, 45);
    print_slow("조별과제응원빌런이 나타났다 ! 저리가 !");
    Sleep(1000);
    print_at(15, 45, "                                                           ");
    print_at(15, 45, "갑자기 짜증이 막 난다... 하아 나도자고싶은데");
    Sleep(1000);
    print_at(15, 45, "                                                           ");
    while (totalHealth > 0 && youHealth > 0) {
        print_at(15, 45, "빌런은 아무것도 안하기를 준비한다... 짜증나");
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        int choice;
        move_cursor(15, 46);
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            move_cursor(15, 45);
            print_slow("공격하기를 선택했다 ! 타이밍에 맞춰 키를 눌러야해 !");
            Sleep(1000);
            print_at(15, 45, "                                                           ");

            break;
        case 2:
            move_cursor(15, 45);
            print_slow("방어하기를 선택했다 ! 어디로 공격이 들어올까 ?? ");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            break;

        case 3:
            move_cursor(15, 45);
            print_slow("도망가기를 선택했다 ! 도망칠수 있을까 ...?");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            int run = rand() % 100;
            Sleep(1000);

            if (run < 30) {
                move_cursor(15, 45);
                print_slow("도망가기에 성공했다 ! 나이스 !!!");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                return;
            }

            else {
                move_cursor(15, 45);
                print_slow("도망치기에 실패했다... 다시 맞써 싸워야해");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                break;
            }
        default:
            print_at(15, 45, "지금 뭐하는거야 !!! 소중한 한 턴을 낭비 했다...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            break;

        }
    }
    print_slow_at(15, 45, "조별과제응원빌런은 아무것도 하지 않는다... !\n");
    int youDamage = rand() % 10 + 20;
    move_cursor(15, 45);
    printf("... %d의 피해를 입었다. 너무 고통스럽다. 살려주세요...\n", youDamage);
    Sleep(1000);
    print_at(15, 45, "                                                           ");
    totalHealth -= youDamage;
    status();
    Sleep(1000);

    if (totalHealth <= 0) {
        print_slow_at(15, 45, "모든 체력이 소모 되었다...\n");
        Sleep(1000);
        print_at(15, 45, "                                                       ");
        Heart(-1);
        status();
        Sleep(1000);
        return 0;
    }
    else if (total_youHealth <= 0) {
        print_slow_at(15, 45, "조별과제빌런을 처치했다 ! ! ! ! ! ! !");
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        print_slow_at(15, 45, "야 차준영 너 나와");
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        print_slow_at(15, 45, "조화를 편하게 해주고 20G를 얻었다");
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        Gold(20);
        status();
        Sleep(1000);
        print_at(15, 45, "                                                       ");

        return 0;
    }
}
//다크데빌수뭉
void fight4() {
    clear_text();
    Fight_UI();
    print_at(49, 17, "       @     @      @@@@@        ");
    print_at(49, 18, "        @   @       @   @        ");
    print_at(49, 19, "          @         @@@@@        ");
    print_at(49, 20, "        @@@@@         @          ");
    print_at(49, 21, "          @         @@@@@        ");
    print_at(49, 22, "          @           @          ");
    print_at(49, 23, "          @@@@@@@@@@@@@          ");
    print_at(49, 24, "    @ @@@@             @@@@ @    ");
    print_at(49, 25, "     @                      @    ");
    print_at(49, 26, "      @   888       888    @     ");
    print_at(49, 27, " ####@     ●_______●       @####    ");
    print_at(49, 28, "@    @       |     |         @    @");
    print_at(49, 29, "@     @      |_____|        @    @ ");
    print_at(49, 29, " @     @                  @    @ ");
    print_at(49, 30, "   @    @                @    @    ");
    print_at(49, 31, "     @                     @   ");
    print_at(49, 32, "       @                 @        ");
    print_at(49, 33, "         @             @       ");
    print_at(49, 34, "         @    ____     @        ");
    print_at(49, 35, "         @    @   @    @        ");
    print_at(49, 36, "          @@@@@   @@@@@         ");
    move_cursor(15, 45);
    print_slow("다크데빌수뭉이가 나타났다 ! 무서워 !");
    Sleep(1000);
    print_at(15, 45, "                                                           ");
    print_slow_at(15, 45, "사실 전혀 무섭지 않지만 무서워 해야할거같다...");
    Sleep(1000);
    print_at(15, 45, "                                                           ");
    while (totalHealth > 0 && youHealth > 0) {
        print_at(15, 45, "수뭉이는 흑염룡 봉인해제를 준비한다...");
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        int choice;
        move_cursor(15, 46);
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            move_cursor(15, 45);
            print_slow("공격하기를 선택했다 ! 타이밍에 맞춰 키를 눌러야해 !");
            Sleep(1000);
            print_at(15, 45, "                                                           ");

            break;
        case 2:
            move_cursor(15, 45);
            print_slow("방어하기를 선택했다 ! 어디로 공격이 들어올까 ?? ");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            break;

        case 3:
            move_cursor(15, 45);
            print_slow("도망가기를 선택했다 ! 도망칠수 있을까 ...?");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            int run = rand() % 100;
            Sleep(1000);

            if (run < 30) {
                move_cursor(15, 45);
                print_slow("도망가기에 성공했다 ! 나이스 !!!");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                return;
            }

            else {
                move_cursor(15, 45);
                print_slow("도망치기에 실패했다... 다시 맞써 싸워야해");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                break;
            }
        default:
            print_at(15, 45, "지금 뭐하는거야 !!! 소중한 한 턴을 낭비 했다...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            break;

        }
        print_slow_at(15, 45, "수뭉이는 흑염룡을 풀어놓는다...!\n");
        int youDamage = rand() % 10 + 20;
        move_cursor(15, 45);
        printf("크아악! %d의 피해를 입었다!\n", youDamage);
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        Health (-youDamage);
        status();
        Sleep(1000);

        if (totalHealth <= 0) {
            print_slow_at(15, 45, "모든 체력이 소모 되었다...\n");
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            Heart(-1);
            status();
            Sleep(1000);
            return 0;
        }
        else if (total_youHealth <= 0) {
            print_slow_at(15, 45, "중2병수뭉이를 처치했다 ! !");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "\"자, 이제 누가 진짜지?\"");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "\"바로 나다.\" 20G를 얻었다");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            Gold(20);
            status();
            Sleep(1000);
            return 0;
        }
    }
}
//다리달린물고기
void fight5() {
    clear_text();
    Fight_UI();
    print_at(49, 22, " !~        ,,;*=*#~       ");
    print_at(49, 23, "  !~!*    ;!#==;!*   #,    ");
    print_at(49, 24, "    ::~;**~=*;!-,:, ● : ●   ");
    print_at(49, 25, "    ::;$;~:~=*-*:=-    :___  ");
    print_at(49, 26, "    !!$#*!$=:!~;-!;    |___  ");
    print_at(49, 27, "   ;~;*.  .@$$#:-!,!; -,    ");
    print_at(49, 28, "  ;;.     .-~,@;*=:=:      ");
    print_at(49, 29, "           ;:    ;        ");
    print_at(49, 30, "           ;-    :.       ");
    print_at(49, 31, "           |__   |__      ");
    move_cursor(15, 45);
    print_slow("저게 뭐야? 다리달린 물고기..? 가? 나타났다?");
    Sleep(1000);
    print_at(15, 45, "                                                           ");
    Sleep(3000);
    move_cursor(15, 45);
    print_slow("뭐 저런게 다있담...");
    Sleep(1000);
    print_at(15, 45, "                                                           ");

    while (totalHealth > 0 && youHealth > 0) {
        print_at(15, 45, "다리달린 물고기는 발차기를 준비한다... 왜 ???");
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        int choice;
        move_cursor(15, 46);
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            move_cursor(15, 45);
            print_slow("공격하기를 선택했다 ! 타이밍에 맞춰 키를 눌러야해 !");
            Sleep(1000);
            print_at(15, 45, "                                                           ");

            break;
        case 2:
            move_cursor(15, 45);
            print_slow("방어하기를 선택했다 ! 어디로 공격이 들어올까 ?? ");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            break;

        case 3:
            move_cursor(15, 45);
            print_slow("도망가기를 선택했다 ! 도망칠수 있을까 ...?");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            int run = rand() % 100;
            Sleep(1000);

            if (run < 30) {
                move_cursor(15, 45);
                print_slow("도망가기에 성공했다 ! 나이스 !!!");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                return;
            }

            else {
                move_cursor(15, 45);
                print_slow("도망치기에 실패했다... 다시 맞써 싸워야해");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                break;
            }
        default:
            print_at(15, 45, "지금 뭐하는거야 !!! 소중한 한 턴을 낭비 했다...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            break;

        }
        print_slow_at(15, 45, "물고기는 날라차기를 갈겼다!\n");
        int youDamage = rand() % 10 + 20;
        move_cursor(15, 45);
        printf("으악! 철퍽 소리 ! %d의 피해를 입었다 !\n", youDamage);
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        totalHealth -= youDamage;
        status();
        Sleep(1000);

        if (totalHealth <= 0) {
            print_slow_at(15, 45, "모든 체력이 소모 되었다...\n");
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            Heart(-1);
            status();
            Sleep(1000);
            return 0;
        }
        else if (total_youHealth <= 0) {
            print_slow_at(15, 45, "물고기를 처치했다 ! 저거 먹을 수 있나?");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "농담입니다. 착한 어린이는 먹어보지 않아요");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "20G를 얻었다. 그런데 비린내 나는거 같다...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            Gold(20);
            status();
            Sleep(1000);
            return 0;
        }
    }
}
//거대한바퀴벌레
void fight6() {
    clear_text();
    Fight_UI();
    print_at(49, 18, "　  　 ヘ　 ヘ            ");
    print_at(49, 19, "　　  /　＼/　＼          ");
    print_at(49, 20, "    ／　　　   　ヽ       ");
    print_at(49, 21, "   /　　ㅇ　　 ㅇ |       ");
    print_at(49, 22, "  ｜　∪　/￣￣T　|       ");
    print_at(49, 23, "  ｜∪　 ｜처　|　|       ");
    print_at(49, 24, "   ＼　　｜＿　| ノ       ");
    print_at(49, 25, "  ／　ヽ ｜　 ヽ |、      ");
    print_at(49, 26, "  (　　 |｜리 || ｜       ");
    print_at(49, 27, "  |　　 ヽ二二ノ ｜       ");
    print_at(49, 28, "  |　 L　　　　｣ ｜       ");
    print_at(49, 29, "  ヽ＿ﾉ　　　 (_ /        ");
    print_at(49, 30, " ｜　　/　 /              ");
    print_at(49, 31, " ｜　 /　 /               ");
    print_at(49, 32, " (＿＿)＿_) *눈건강을위해.");

    print_slow_at(15, 45, "거대한 바퀴벌레가 나타났다 ! 징그러워,,,,");
    Sleep(1000);
    print_at(15, 45, "                                                           ");


    while (totalHealth > 0 && youHealth > 0) {
        print_at(15, 45, "바퀴벌레는 더듬이 흔들기를 준비한다...으으으");
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        int choice;
        move_cursor(15, 46);
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            move_cursor(15, 45);
            print_slow("공격하기를 선택했다 ! 타이밍에 맞춰 키를 눌러야해 !");
            Sleep(1000);
            print_at(15, 45, "                                                           ");

            break;
        case 2:
            move_cursor(15, 45);
            print_slow("방어하기를 선택했다 ! 어디로 공격이 들어올까 ?? ");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            break;

        case 3:
            move_cursor(15, 45);
            print_slow("도망가기를 선택했다 ! 도망칠수 있을까 ...?");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            int run = rand() % 100;
            Sleep(1000);

            if (run < 30) {
                move_cursor(15, 45);
                print_slow("도망가기에 성공했다 ! 나이스 !!!");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                return;
            }

            else {
                move_cursor(15, 45);
                print_slow("도망치기에 실패했다... 다시 맞써 싸워야해");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                break;
            }
        default:
            print_at(15, 45, "지금 뭐하는거야 !!! 소중한 한 턴을 낭비 했다...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            break;

        }
        print_slow_at(15, 45, "바퀴벌레는 움직였다.\n");
        move_cursor(15, 45);
        int youDamage = rand() % 10 + 20;
        printf("으아아아아아아아아아아! %d의 피해를 입었다!!!!!!!\n", youDamage);
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        totalHealth -= youDamage;
        status();
        Sleep(1000);

        if (totalHealth <= 0) {
            print_slow_at(15, 45, "모든 체력이 소모 되었다...\n");
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            Heart(-1);
            status();
            Sleep(1000);
            return 0;
        }
        else if (total_youHealth <= 0) {
            print_slow_at(15, 45, "바퀴벌레를... 처치했다...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "아 제발 다시는 만나지 말기를 제발요");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "일단 20G를 얻긴 했는데 기분이 안좋아...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            Gold(20);
            status();
            Sleep(1000);
            return 0;
        }
    }
}
//귀엽지만은않은수뭉이
void fight7() {
    clear_text();
    Fight_UI();
    print_at(49, 17, "     @     @      @@@@@        ");
    print_at(49, 18, "      @   @       @   @        ");
    print_at(49, 19, "        @         @@@@@        ");
    print_at(49, 20, "      @@@@@         @          ");
    print_at(49, 21, "        @         @@@@@        ");
    print_at(49, 22, "        @           @          ");
    print_at(49, 23, "        @@@@@@@@@@@@@          ");
    print_at(49, 24, "  @ @@@@             @@@@ @    ");
    print_at(49, 25, "   @                      @    ");
    print_at(49, 26, "    @                    @     ");
    print_at(49, 27, "   @     ●       ●      @    ");
    print_at(49, 28, "   @    ///  ㅅ   ///     @    ");
    print_at(49, 29, "    @                    @     ");
    print_at(49, 29, "      @                 @      ");
    print_at(49, 30, "     @                   @     ");
    print_at(49, 31, "    @   @             @   @   ");
    print_at(49, 32, "   @   @               @   @   ");
    print_at(49, 33, "   @$$$@               @$$$@    ");
    print_at(49, 34, "        @    ____     @        ");
    print_at(49, 35, "        @    @   @    @        ");
    print_at(49, 36, "         @@@@@   @@@@@         ");
    move_cursor(15, 45);
    print_slow("귀엽지만은않은수뭉이가 나타났다 ! 젠장 귀여워 !");
    Sleep(1000);
    print_at(15, 45, "                                                           ");

    while (totalHealth > 0 && youHealth > 0) {
        print_at(15, 45, "수뭉이는 츤데레행동을 준비한다 !!! 어떻게 하지...?????????");
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        int choice;
        move_cursor(15, 46);
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            move_cursor(15, 45);
            print_slow("공격하기를 선택했다 ! 타이밍에 맞춰 키를 눌러야해 !");
            Sleep(1000);
            print_at(15, 45, "                                                ");

            break;
        case 2:
            move_cursor(15, 45);
            print_slow("방어하기를 선택했다 ! 어디로 공격이 들어올까 ?? ");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            break;

        case 3:
            move_cursor(15, 45);
            print_slow("도망가기를 선택했다 ! 도망칠수 있을까 ...?");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            int run = rand() % 100;
            Sleep(1000);

            if (run < 30) {
                move_cursor(15, 45);
                print_slow("도망가기에 성공했다 ! 나이스 !!!");
                Sleep(1000);
                print_at(15, 45, "                                                ");
                return;
            }

            else {
                move_cursor(15, 45);
                print_slow("도망치기에 실패했다... 다시 맞써 싸워야해");
                Sleep(1000);
                print_at(15, 45, "                                                ");
                break;
            }
        default:
            print_at(15, 45, "지금 뭐하는거야 !!! 소중한 한 턴을 낭비 했다...");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            break;

        }
        print_slow_at(15, 45, "수뭉이 : 흐,흥! 난 귀엽지 않다고 !\n");
        int youDamage = rand() % 10 + 20;
        move_cursor(15, 45);
        printf("흡,,, 귀여움을 참느라 %d의 피해를 입었다\n", youDamage);
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        totalHealth -= youDamage;
        status();
        Sleep(1000);

        if (totalHealth <= 0) {
            print_slow_at(15, 45, "모든 체력이 소모 되었다...\n");
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            Heart(-1);
            status();
            Sleep(1000);
            return 0;
        }
        else if (total_youHealth <= 0) {
            print_slow_at(15, 45, "수뭉이가 귀엽다고 엉망진창 인정하게 만들어줬다");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "수뭉이 : 난 귀여워...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "수뭉이와의 커플링을 사느라 20G를 소비했다");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            Gold(-20);
            return 0;
        }
    }
}
//무언가
void fight8() {
    clear_text();
    Fight_UI();
    print_at(46, 24, " ▒▒▒▒▒▒▒▒▒▒▒ ");
    print_at(46, 25, " ▒▒▒▒▓▒▒▓▒▒▒▒  ");
    print_at(46, 26, " ▒▒▒▒▓▒▒▓▒▒▒▒  ");
    print_at(46, 27, " ▒▒▒▒▒▒▒▒▒▒▒  ");
    print_at(46, 28, " ▒▓▒▒▒▒▒▒▒▒▓▒   ");
    print_at(46, 29, " ▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒  ");
    print_at(46, 30, " ▒▒▒▒▒▒▒▒▒▒▒ ");
    move_cursor(15, 35);
    print_slow("무언가가 나타났다...");
    Sleep(1000);
    print_at(15, 45, "                        ");
    Sleep(3000);
    move_cursor(15, 35);
    print_slow("진짜 뭐지... 무섭다...");
    Sleep(1000);
    print_at(15, 45, "                       ");

    while (totalHealth > 0 && youHealth > 0) {
        print_at(15, 45, "무언가는 무언가를 준비한다...");
        Sleep(1000);
        print_at(15, 45, "                                                ");
        int choice;
        move_cursor(15, 46);
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            move_cursor(15, 45);
            print_slow("공격하기를 선택했다 ! 타이밍에 맞춰 키를 눌러야해 !");
            Sleep(1000);
            print_at(15, 45, "                                                ");

            break;
        case 2:
            move_cursor(15, 45);
            print_slow("방어하기를 선택했다 ! 어디로 공격이 들어올까 ?? ");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            break;

        case 3:
            move_cursor(15, 45);
            print_slow("도망가기를 선택했다 ! 도망칠수 있을까 ...?");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            int run = rand() % 100;
            Sleep(1000);

            if (run < 30) {
                move_cursor(15, 45);
                print_slow("도망가기에 성공했다 ! 나이스 !!!");
                Sleep(1000);
                print_at(15, 45, "                                                ");
                return;
            }

            else {
                move_cursor(15, 45);
                print_slow("도망치기에 실패했다... 다시 맞써 싸워야해");
                Sleep(1000);
                print_at(15, 45, "                                                ");
                break;
            }
        default:
            print_at(15, 45, "지금 뭐하는거야 !!! 소중한 한 턴을 낭비 했다...");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            break;

        }
    }
    print_slow_at(15, 45, "무언가는...무언가 무언가를 무언가 했다...!\n");
    int youDamage = rand() % 10 + 20;
    move_cursor(15, 45);
    printf("무엇... %d의 피해를 입었다...\n", youDamage);
    Sleep(1000);
    print_at(15, 45, "                                                           ");
    totalHealth -= youDamage;
    status();
    Sleep(1000);

    if (totalHealth <= 0) {
        print_slow_at(15, 45, "모든 체력이 소모 되었다...\n");
        Sleep(1000);
        print_at(15, 45, "                                                       ");
        Heart(-1);
        return 0;
    }
    else if (total_youHealth <= 0) {
        print_slow_at(15, 45, "무언가를 무언가 해버렸다...");
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        print_slow_at(15, 45, "무언가는 무언가 무언가인채로 있다...");
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        print_slow_at(15, 45, "뭔지 모르겠고 20G를 얻었다...");
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        Gold(20);
        status();
        Sleep(1000);
        return 0;
    }
}
//상점주인
void fight9() {
    clear_text();
    Fight_UI();
    print_at(46, 19, "     ∩＿＿＿∩    $   $      ");
    print_at(46, 20, "     | ノ　　　ヽ /⌒)   $    ");
    print_at(46, 21, "    / ⌒)(ﾟ)　(ﾟ)   /         ");
    print_at(46, 22, "  /　/　  (_●)ミ  /  $  $    ");
    print_at(46, 23, " （　ヽ   | ∪ | ／           ");
    print_at(46, 24, "    ＼　　 ヽノ /     $       ");
    print_at(46, 25, "    /　　　   /               ");
    print_at(46, 26, "   ｜　　　  /    $           ");
    print_at(46, 27, "   ｜　／＼  ＼        $      ");
    print_at(46, 28, "   ｜ /    )　 )              ");
    print_at(46, 29, "   (_ﾉ　　(   ＼   $    $     ");
    print_at(46, 30, "           ＼ ＿)  $  $ $ $ $ ");

    print_slow_at(15, 45, "상점 주인이 나타났다...? 왜..?");
    print_at(15, 45, "                                                ");
    print_slow_at(15, 45, "모르겠지만... 일단 준비하자");
    print_at(15, 45, "                                                ");
    while (totalHealth > 0 && youHealth > 0) {
        print_slow_at(15, 45, "상점주인은 골드던지기를 준비한다... 어떻게 할까...?");
        print_at(15, 45, "                                                ");
        int choice;
        move_cursor(15, 46);
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            move_cursor(15, 45);
            print_slow("공격하기를 선택했다 ! 타이밍에 맞춰 키를 눌러야해 !");
            Sleep(1000);
            print_at(15, 45, "                                                ");

            break;
        case 2:
            move_cursor(15, 45);
            print_slow("방어하기를 선택했다 ! 어디로 공격이 들어올까 ?? ");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            break;

        case 3:
            move_cursor(15, 45);
            print_slow("도망가기를 선택했다 ! 도망칠수 있을까 ...?");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            int run = rand() % 100;
            Sleep(1000);

            if (run < 30) {
                move_cursor(15, 45);
                print_slow("도망가기에 성공했다 ! 나이스 !!!");
                Sleep(1000);
                print_at(15, 45, "                                                ");
                return;
            }

            else {
                move_cursor(15, 45);
                print_slow("도망치기에 실패했다... 다시 맞써 싸워야해");
                Sleep(1000);
                print_at(15, 45, "                                                ");
                break;
            }
        default:
            print_at(15, 45, "지금 뭐하는거야 !!! 소중한 한 턴을 낭비 했다...");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            break;

        }
        print_slow_at(15, 45, "상점 주인은 돈을 던진다 !!! \n");
        int youDamage = rand() % 10 + 20;
        move_cursor(15, 45);
        printf("우왓! 아프지만 기분은 좋아... %d의 피해를 입었다\n", youDamage);
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        totalHealth -= youDamage;
        status();
        Sleep(1000);

        if (totalHealth <= 0) {
            print_slow_at(15, 45, "모든 체력이 소모 되었다...\n");
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            Heart(-1);
            status();
            Sleep(1000);
            return 0;
        }
        else if (total_youHealth <= 0) {
            print_slow_at(15, 45, "상점주인을 상대로 마작을 이겼다");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "\"너를 인정하도록 하마...\"");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "왜 그런대사를 하시는거죠...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "상점주인은 돈이 많다 ! 100G를 얻었다");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            Gold(100);
            status();
            Sleep(1000);
            return 0;
        }
    }
}
//벌레떼
void fight10() {
    clear_text();
    Fight_UI();
    print_at(46, 15, "   ﾟ●゜ 　 ｡｡   ");
    print_at(46, 16, " ｡｡　　　　ﾟ●゜ ");
    print_at(46, 17, "ﾟ●゜ 　         ");
    print_at(46, 18, "　　　 ｡｡        ");
    print_at(46, 19, "　　　ﾟ●゜      ");
    print_at(46, 20, " ｡｡              ");
    print_at(46, 21, "ﾟ●゜ 　　 ｡｡    ");
    print_at(46, 22, "　　　　　ﾟ●゜  ");
    print_at(46, 23, "                 ");
    print_at(46, 24, "　ﾟ●゜   ｡｡     ");
    print_at(46, 25, "         ﾟ●゜   ");
    print_at(46, 26, "　 ｡｡            ");
    print_at(46, 27, "　ﾟ●゜          ");
    print_at(46, 28, "                 ");
    print_at(46, 29, "         ﾟ●゜   ");
    print_at(46, 30, "ﾟ●              ");

    print_slow_at(15, 35, "벌레떼가 나타났다 !!!!!! 으악!!!!!");
    print_slow_at(15, 45, "벌레떼가 다가온다....!!!!!!");
    while (totalHealth > 0 && youHealth > 0) {
        int choice;
        move_cursor(15, 46);
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            move_cursor(15, 45);
            print_slow("공격하기를 선택했다 ! 타이밍에 맞춰 키를 눌러야해 !");
            Sleep(1000);
            print_at(15, 45, "                                                ");

            break;
        case 2:
            move_cursor(15, 45);
            print_slow("방어하기를 선택했다 ! 어디로 공격이 들어올까 ?? ");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            break;

        case 3:
            move_cursor(15, 45);
            print_slow("도망가기를 선택했다 ! 도망칠수 있을까 ...?");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            int run = rand() % 100;
            Sleep(1000);

            if (run < 30) {
                move_cursor(15, 45);
                print_slow("도망가기에 성공했다 ! 나이스 !!!");
                Sleep(1000);
                print_at(15, 45, "                                                ");
                return;
            }

            else {
                move_cursor(15, 45);
                print_slow("도망치기에 실패했다... 다시 맞써 싸워야해");
                Sleep(1000);
                print_at(15, 45, "                                                ");
                break;
            }
        default:
            print_at(15, 45, "지금 뭐하는거야 !!! 소중한 한 턴을 낭비 했다...");
            break;

        }
        print_slow_at(15, 45, "벌,벌레떼가 다가온다.......!!!!! \n");
        int youDamage = rand() % 10 + 20;
        move_cursor(15, 45);
        printf("오지마세요제발요제발요제발ㅇ %d의 피해를 입었다아아아아\n", youDamage);
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        totalHealth -= youDamage;
        status();
        Sleep(1000);

        if (totalHealth <= 0) {
            print_slow_at(15, 45, "살아있고 싶지 않아...\n");
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            Heart(-1);
            status();
            Sleep(1000);
            return 0;
        }
        else if (total_youHealth <= 0) {
            print_slow_at(15, 45, "정신없이 에프킬라를 마구마구 뿌렸더니 벌레떼는 사라졌다");
            Sleep(1000);
            print_at(15, 45, "                                                              ");
            print_slow_at(15, 45, "정말 다행이야... 다시는 보지않길");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "에프킬라를 여러통 쓰느라 돈이 나갔다...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            Gold(-40);
            status();
            Sleep(1000);
            return 0;
        }
    }
}
//무기
//황금검
void weapons1(int* Strength, int* Health) {
    printf("\n");
    print_slow("반짝반짝 빛나는 검이다\n");
    *Strength += 10;
}
//언더아머
void weapons2(int* Strength, int* Health) {
    printf("\n");
    print_slow("3대 1000미만 착용 금지. -김민준-\n");
    *Health += 20;
}
//애플비전프로
void weapons3(int* Strength, int* Health) {
    printf("\n");
    print_slow("애플에서 만든 완전비싼거, 적 파라미터 가동기능\n");
    *Strength += 50;
    *Health -= 15;
}
//휠리스
void weapons4(int* Strength, int* Health) {
    printf("\n");
    print_slow("이거 타고다니면 재미있음 진짜로\n");
    *Health += 20;

}
//저주받은검
void weapons5(int* Strength, int* Health) {
    printf("\n");
    print_slow("이름에 편견 가져야죠 암\n");
    *Health -= 30;

}
//신비한갑옷
void weapons6(int* Strength, int* Health) {
    printf("\n");
    print_slow("시스루 나시와 바지 세트\n");
    *Health += 5;

}
//활력의귀걸이
void weapons7(int* Strength, int* Health) {
    printf("\n");
    print_slow("자라에서 파는 20cm 왕 귀걸이\n");
    *Health += 10;
}
//24과잠
void weapons8(int* Strength, int* Health) {
    printf("\n");
    print_slow("대체 언제 만드냐고요? 저도 몰라요\n");
    *Health += 10;
}
//나팔바지
void weapons9(int* Strength, int* Health) {
    printf("\n");
    print_slow("박진영이 입던 거다. 초레어희귀템. 따뜻하다\n");
    *Strength += 8;
}
//보급형강인함의반지
void weapons10(int* Strength, int* Health) {
    printf("\n");
    print_slow("전사라면 누구나 하나씩은 가지고 있는 반지\n");
    *Health += 10;
    *Strength += 10;
}
//누군가의커플링
void weapons11(int* Strength, int* Health) {
    printf("\n");
    print_slow("눈물자국이 묻어있는것만 같다...\n");
    *Strength += 30;
    *Health -= 20;
}
//본크러셔오함마
void weapons12(int* Strength, int* Health) {
    printf("\n");
    print_slow("뼈를 가루내버릴수 있다...\n");
    *Strength += 30;
    *Health -= 10;
}
//저주받은목걸이
void weapons13(int* Strength, int* Health) {
    printf("\n");
    print_slow("편견을 가지지 말 것.\n");
    *Strength += 20;
    *Health += 20;
}
//수뭉이키링
void weapons14(int* Strength, int* Health) {
    printf("\n");
    print_slow("샹명대애 어셔오새오\n");
    *Strength += 20;
    *Health += 20;
}

Weapon inventory[ITEM_COUNT];
int inventory_count = 0;
//장비 배열
Weapon weapons[ITEM_COUNT] = {
    {"황금검", weapons1,100},
    {"언더아머",weapons2, 150 },
    {"애플비전프로",weapons3,400 },
    {"휠리스",weapons4,300 },
    {"저주받은검",weapons5, 500 },
    {"신비한갑옷",weapons6, 300 },
    {"활력의귀걸이",weapons7, 200 },
    {"24과잠",weapons8, 50 },
    {"나팔바지",weapons9, 800 },
    {"보급형강인함의반지",weapons10, 150 },
    {"누군가의커플링",weapons11, 300 },
    {"본크러셔오함마",weapons12, 350 },
    {"저주받은목걸이",weapons13, 20 },
    {"수뭉이키링",weapons14, 800 },
};
//특수적
//정재웅
void specials1() {
    clear_text();
    Fight_UI2();
    print_at(46, 24, " ___________");
    print_at(46, 25, "|          |");
    print_at(46, 26, "|   ^오^   |");
    print_at(46, 27, "|          |");
    print_at(46, 28, "|  개재웅  |");
    print_at(46, 29, "|__________|");
    print_slow_at(15, 45, "당신은 길을 걷다 목마르고 돈이 없는 개재웅을 마주쳤다.");
    Sleep(1000);
    print_at(15, 45, "                                                             ");
    print_slow_at(15, 45, "이걸 어떻게 해야할까");
    Sleep(1000);
    print_at(15, 45, "                                                         ");
    Sleep(1000);

    int choice;
    do {
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            move_cursor(15, 45);
            print_slow("정재웅이 돈을 훔쳐갔다. 10골드를 잃었다.");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            Gold(-10);
            break;
        case 2:
            move_cursor(15, 45);
            print_slow("정재웅이 돈을 훔쳐갔다. 10골드를 잃었다.");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            Gold(-10);
            break;
        case 3:
            move_cursor(15, 45);
            print_slow("정재웅이 돈을 훔쳐갔다. 10골드를 잃었다.");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            Gold(-10);
            break;
        default:
            move_cursor(15, 45);
            print_slow("저항을 안 해서 정재웅이 돈을 훔쳐갔다. 10골드를 잃었다.");
            Sleep(1000);
            print_at(15, 45, "                                                    ");
            Gold(-10);
            break;
        }

    } while ((choice < 1) || (choice > 3));

}
//김민준
void specials2() {
    clear_text();
    Fight_UI2();
    print_slow_at(15, 45, "당신은 길을 걷다 를 마주쳤다");
}
//이경민
void specials3() {
    clear_text();
    Fight_UI2();
    int choice;
    print_at(46, 24, " ____________________");
    print_at(46, 25, "|    ________        |");
    print_at(46, 26, "|   /        \\___    |");
    print_at(46, 27, "|  /   ｀Д´    \\   |");
    print_at(46, 28, "|  |              |  |");
    print_at(46, 29, "|  \\______________/  |");
    print_at(46, 30, "|____________________|");
    print_slow_at(15, 45, "당신은 길을 걷다 화가 나있는 이경민과 마주쳤다. ");
    Sleep(1000);
    print_at(15, 45, "                                                     ");
    print_slow_at(15, 45, "전투를 피할 수 없을 것 같다...");
    Sleep(1000);
    print_at(15, 45, "                                                      ");
    print_slow_at(15, 45, "연속으로 공격에 성공하면 이길 수 있을 것 같다.");
    Sleep(1000);
    print_at(15, 45, "                                                      ");
    print_slow_at(15, 48, "");
    Sleep(1000);
    scanf("%d", &choice);
    if (choice == 1) {
        exgameLoop();
    }
    else if (choice == 2) {
        print_slow_at(15, 45, "그럴 수는 없다. 공격하자. ");
        Sleep(1000);
        print_at(15, 45, "                                                      ");
        exgameLoop();
        return;

    }
    else if (choice == 3) {
        print_slow_at(15, 45, "그럴 수는 없다. 공격하자. ");
        Sleep(1000);
        print_at(15, 45, "                                                      ");
        exgameLoop();
        return;
    }
    else {
        print_slow_at(15, 45, "그럴 수는 없다. 공격하자. ");
        Sleep(1000);
        print_at(15, 45, "                                                      ");
        exgameLoop();
        return;
    }
}
//박조화
void specials4() {
    clear_text();
    Fight_UI();
    print_at(46, 24, "   　　　쿨쿨zZz");
    print_at(46, 25, "zzz .∧ﾍ／⌒⌒ヽ");
    print_at(46, 26, "　＜⌒/　　　　ヽ");
    print_at(46, 27, "／＜_/＿＿＿＿／");
    print_at(46, 28, "    ￣￣￣￣￣￣");
    print_slow_at(15, 45, "당신은 길을 걷다 자고있는 나는조화를 마주쳤다...");
    printf("                                                         ");
    Sleep(1000);
    print_slow_at(15, 45, "음... 말을 걸어볼까 ? 공격대신 말을 걸어보자");
    while (1) {
        int choice;
        move_cursor(15, 46);
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            move_cursor(15, 45);
            print_slow("당신은 나는조화에게 대화를 건넸다 ");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            print_slow("\"저기요 좀 지나갈게요...\"");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            print_slow("나는 조화는 그냥 자고있다...");
            Sleep(1000);
            print_at(15, 45, "                                                ");

            break;
        case 2:
            move_cursor(15, 45);
            print_slow("방어하기를 선택했다... 대체 무엇을 방어하려고 ? ");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            break;

        case 3:
            move_cursor(15, 45);
            print_slow("도망가기를 선택했다... 누구에게서 ?");
            Sleep(1000);
            print_at(15, 45, "                                                ");
            int run = rand() % 100;
            Sleep(1000);

            if (run < 30) {
                move_cursor(15, 45);
                print_slow("도망가기에 성공했다... 사실 그냥 뛰어넘었을 뿐이다");
                Sleep(1000);
                print_at(15, 45, "                                                ");
                return;
            }

            else {
                move_cursor(15, 45);
                print_slow("도망치기에 실패했다... 다시 말을 한번 걸어볼까");
                Sleep(1000);
                print_at(15, 45, "                                                ");
                print_slow("애초에 왜 도망치려 하는 거지...");
                Sleep(1000);
                print_at(15, 45, "                                                ");
                break;
            }
        default:
            print_at(15, 45, "음... 뭐 가끔은 아무것도 안해도 괜찮을것 같아");
            break;

        }
        print_slow_at(15, 45, "나는조화는 대꾸도 안하고 자고있다...\n");
        printf("                                                      ");
        Sleep(1000);
        status();

        if (totalHealth <= 50) {
            print_slow_at(15, 45, "갑자기 나는조화가 벌떡 일어났다 !!!!\n");
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            print_slow_at(15, 45, "\"아 깜짝이야\"");
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            print_slow_at(15, 45, "나는조화 : 너 조심해.\n");
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            print_slow_at(15, 45, "\"네????\"\n");
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            print_slow_at(15, 45, "그 말을 끝으로 나는조화는 사라졌다...\n");
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            print_slow_at(15, 45, "당신의 체력이 모두 회복되었습니다...흐응 ?\n");
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            Health(100);
            status();
            Sleep(1000);
            return 0;
        }
        else {
            print_slow_at(15, 45, "나는 조화를 한번 건드려 봤다");
            Sleep(1000);
            print_at(15, 45, "                                                              ");
            print_slow_at(15, 45, "가만히 자고있다...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "그냥 무시하고 지나가는게 나을것 같다...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "갑자기 벌에 쏘였다. 아야 ! 뭐지 진짜 ???");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "나는조화를 쳐다봤지만 여전히 자고 있을 뿐 이었다...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            Health(-10);
            status();
            return 0;
        }
    }
}
//동앗줄의길태숙
void specials5() {

}
//쌍동앗줄날개성한울
void specials6() {

}
//각성한녹말이쑤시개의김석규
void specials7() {

}
//각성한사슬낫의박소영
void specials8() {

}
//이벤트들
//노상강도
void event1() {
    clear_text();
    int choice;
    print_slow_at(0, 10, "당신은 길을 걷다 일진무리를 마주쳤습니다. PTSD가 온다...\n"
        "\n"
        "\"야 너 이리와봐 (찍), 멍청해 보이는게 마음에 안드는데?\"\n"
        "\n"
        "\"가지고 있는 돈 전부다 내놔\"\n"
        "\n"
        "\"네?\"\n"
        "\n");
    print_slow_at(10, 18, "> 1. 도망치기\n");
    print_slow_at(10, 20, "> 2. 싸우기\n");
    print_slow_at(10, 22, "> 3. 주기\n");
    print_slow_at(10, 25, "어떻게 할까..? ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        clear_text();
        move_cursor(0, 10);
        print_slow("당신은 도망치기로 했습니다.\n");
        if (rand() % 2) {
            printf("\n");
            print_slow("당신은 민첩하게 샤샤샥 일진들을 피해갔습니다. 다행이네요!\n");
            printf("\n");
            Sleep(1000);
        }
        else {
            print_slow("당신은 도망치려 하다가 일진들에게 붙잡혔습니다. 일진들은 화가 많이 난거 같습니다.\n");
            printf("\n");
            print_slow("결국 당신은 돈이 모두 나올때까지 두들겨 맞았습니다.\n");
            printf("\n");
            print_slow("당신은 체력을 20 잃고 소지금을 모두 잃었다. 저런...\n");
            printf("\n");
            Health(-20);
            Gold(-totalGold);
            Sleep(1000);
        }
        break;
    case 2:
        clear_text();
        move_cursor(0, 10);
        print_slow("당신은 일진들과 맞서 싸우기로 결심했습니다. 트라우마를 위하여 !\n");
        Sleep(1000);
        printf("\n");
        if (totalStrength >= 20) {
            print_slow("당신은 트라우마 타파에 성공했습니다! 역으로 일진들의 돈도 뺏었습니다! 이시대의 홍길동!\n");
            printf("\n");
            Sleep(1000);
            Gold(50);
        }
        else {
            print_slow("당신의 어설프게 싸우려 하는 모습이 재밌었는지 일진들은 눈물까지 흘리며 웃습니다...\n");
            printf("\n");
            print_slow("\"아 찐따가 아니라 광대였네 재미있었다. 자 여기 관람료. 재능있어 너.\"\n");
            printf("\n");
            print_slow("일진에게서 돈을 10G 받았습니다.\n");
            printf("\n");
            print_slow("울어야 할지 웃어야 할지 모른채로 당신은 길을 나섰습니다... 화이팅\n");
            Sleep(1000);
            printf("\n");
            Gold(10);
        }
        break;
    case 3:
        clear_text();
        move_cursor(0, 10);
        print_slow("압도당한 당신은 아무것도 할 수 없었습니다...\n");
        printf("\n");
        print_slow("얌전히 돈을 주었습니다.\n");
        printf("\n");
        print_slow("당신의 소지금이 0G가 되었습니다...\n");
        printf("\n");
        print_slow("당신은 씁쓸하게 앞으로 나아갑니다...\n");
        printf("\n");
        Gold(-totalGold);
        Sleep(1000);
        break;
    default:
        print_slow("다른 선택은 없다...\n");
        Sleep(1000);
        event1();
        break;
    }
}
//길잃음
void event2() {
    clear_text();
    int health_lost = 10;
    totalHealth -= health_lost;

    move_cursor(0, 10);
    print_slow("당신은 무성한 나무들에 길이 가려져 가야하는 길을 놓쳐버렸습니다.\n");
    printf("\n");
    Sleep(1000);
    print_slow("...당신은 너무 무섭습니다....\n");
    printf("\n");
    print_slow("길을 찾기 위해 수풀을 헤치며 나아가느라 팔이 상처 투성이가 되었습니다.\n");
    printf("\n");
    print_slow("길을 찾았지만 몸은 망신창이 입니다.\n");
    printf("\n");
    int replace[] = { health_lost, totalHealth };
    print_slow3("당신의 체력이 %을 잃어 %가 되었습니다.\n", 30, replace, 2);
    printf("\n");
    print_slow("당신은 아픈 몸을 이끌고 앞으로 나아갔습니다...");
    Sleep(1000);
}
//돈주움
void event3() {
    clear_text();
    move_cursor(0, 10);
    int gold_found = rand() % 100 + 1;
    totalGold += gold_found;
    move_cursor(0, 10);
    print_slow("당신은 길에서 금화를 발견했습니다. 럭키!\n");
    printf("\n");
    int replace[] = { totalGold };
    print_slow3("당신의 소지금이 %G가 되었습니다.\n", 30, replace, 1);
    printf("\n");
    print_slow("당신은 신나는 기분으로 앞으로 나아갔습니다. 좋은일도 가끔씩은 있어야죠 !");
    Sleep(1000);
}
//음식주움
void event4() {
    clear_text();
    move_cursor(0, 10);
    int health_plus = 20;
    totalHealth += health_plus;
    print_slow("당신은 길을 걷다 땅바닥에 떨어진 마라탕을 발견했습니다 ! ! ! ! ! \n");
    printf("\n");
    print_slow("\"마라탕 !!!!!! 당장 먹어야겠어. 버섯은 무조건 빼고. 숙주 많이. 고수 추가.\"\n");
    printf("\n");
    int replace[] = { health_plus };
    print_slow3("당신의 체력이 % 회복 되었습니다 ! 맛있게 먹어요 \n", 30, replace, 1);
    Sleep(1000);
}
//보물상자
void event5() {
    clear_text();
    move_cursor(0, 10);
    srand(time(NULL));
    int random_weapon_index = rand() % 14;
    const char* item = weapons[random_weapon_index].weapon_name;

    move_cursor(0, 10);
    print_slow("당신은 보물상자를 발견했습니다 !!\n");
    printf("\n");
    print_slow("당신은 뭐가 나올지 기대하며 보물상자를 열었습니다. 두근두근\n");
    printf("\n");
    printf("당신은 %s를 획득해 능력치가 변화했습니다 \n", item);
    printf("\n");
    Sleep(1000);

}
//엑스칼리버
void event6() {
    clear_text();
    move_cursor(0, 10);
    int choice;
    move_cursor(0, 10);
    print_slow("길 앞에는 검이 꽃힌 거대한 비석이 있었습니다.\n");
    printf("\n");
    print_slow("\"이건 바로 엑스칼리버??? 역시 이세계 물은 이정도는 해줘야지. ㅋㅋ\"\n");
    printf("\n");
    print_slow("당신은 검을 뽑으려 했지만, 뽑히지 않았습니다.\n");
    printf("\n");
    print_slow("지친 당신은 길바닥에 앉아 쉬다 비석에 적힌 글을 발견했습니다.\n");
    printf("\n");
    print_slow("\"피를 바치세요 ?\"\n");
    printf("\n");
    print_slow_at(20, 24, "> 1. 피를 바친다\n");
    print_slow_at(20, 26, "> 2. 그냥 간다\n");
    print_slow_at(20, 27, "어떻게 할까..? ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        clear_text();
        move_cursor(0, 10);
        print_slow("\"이건 못참지. 가즈아 ㅋㅋ\"\n");
        printf("\n");
        print_slow("손바닥을 그어, 피가 철철 흐르는 손을 칼에 가져다 대자 칼이 스르륵 뽑혔다. 저런, 많이 아프지 않길\n");
        printf("\n");
        print_slow("당신은 엑스칼리버를 획득 했습니다... 굳이 그렇게 해야 했을까요 ?\n");
        printf("\n");
        Strength(+15);
        Health(-20);
        int replace[] = { Strength };
        print_slow("당신은 칼을 얻어 공격력이 %가 되었습니다.\n", 30, replace, 1);
        printf("\n");
        int replace2[] = { Health };
        print_slow3("으, 피가 너무 흐른다. \"아파...\" 체력이 %가 되었습니다.\n", 30, replace2, 1);
        printf("\n");
        Sleep(900);
        break;
    case 2:
        clear_text();
        move_cursor(0, 10);
        print_slow("지친 당신은 비석을 보며 입맛만을 다시다, 앞으로 나아갔습니다.\n");
        printf("\n");
        Sleep(900);
        break;
    default:
        print_slow("다른 선택은 하기가 싫다\n");
        Sleep(900);
        event6();
        break;
    }
}
//마약꽃
void event7() {
    clear_text();
    move_cursor(0, 10);
    int health_loss = 15;
    int strength_gain = 5;

    move_cursor(0, 10);
    print_slow("당신은 앞으로 걷다 빨간꽃이 만개한 초원을 발견했다. 어쩐지 기분이 좋아지는 기분이다.\n");
    printf("\n");
    print_slow("\"아름다워...\"\n");
    printf("\n");
    print_slow("당신은 시간이 흐른다는 사실도 잊은 채로 꽃밭에 누워 행복한 꿈을 꿨다\n");
    printf("\n");
    print_slow("한참 뒤 당신은 정신을 차렸고 주변에는 썩은 꽃뿐이라는 사실을 깨닫고 공포감에 휩싸인채 굳어버렸다.\n");
    printf("\n");
    print_slow2("아름다운 것을 보여주고 싶었는데...", 200);
    printf("\n");
    totalHealth -= health_loss;
    totalStrength += strength_gain;
    int replace[] = { totalHealth, totalStrength };
    print_slow3("당신의 체력과 공격력이 %, %가 되었다.\n", 30, replace, 2);
    printf("\n");
    print_slow("어디선가 들려오는 목소리를 듣고 정신이 든 당신은 서둘러 앞으로 나아갔다.\n");
    Sleep(1000);

}
//구덩이
void event8() {
    clear_text();
    int choice;
    move_cursor(0, 10);
    print_slow("당신을 길을 가다 저 멀리 빛나는 물건이 보이는걸 발견 했습니다. \n");
    printf("\n");
    print_slow("당신은 설레는 마음으로 빛나는 물건을 향해 서둘러 달려갔습니다...\n");
    printf("\n");
    print_slow("아쉽게도, 빛나는 물건은 함정이였습니다. 당신은 헤어나올수 없는 심연의...히,아니, 구덩이에 빠졌습니다.");
    printf("\n");
    print_slow("당신은 어떻게든 빠져나오려 애를 써봤지만 아쉽게도 역부족이였습니다.");
    Sleep(1000);
    printf("\n");
    print_slow("당신이 구덩이에서 헤어나왔을 때는 이미 날이 샌 뒤였습니다...\n");
    Health(-20);
    printf("\n");
    int replace[] = { totalHealth };
    print_slow3("진이 모두 빠져버린 당신은 망신창이가 되었습니다. 체력을 20 잃어 % 가 되었습니다.\n", 30, replace, 1);
    Sleep(1000);

}
//황금수뭉이
void event9() {
    clear_text();
    int choice;

    move_cursor(0, 10);
    print_slow("길을 걷다 행운의 황금수뭉이 조각상을 마주쳤습니다. 소원을 빌수있을거 같습니다, 빌어볼까 ?\n");
    printf("\n");
    print_at(10, 12, "> 1. 돈 많이 벌게 해주세요 !\n");
    print_slow_at(15, 14, "] 이런, 욕심많은 사람 같으니라고\n");
    print_at(10, 17, "> 2. 건강하게 살수있게 해주세요 !\n");
    print_slow_at(15, 19, "] 귀여운 소원이네요 \n");
    print_at(10, 22, "> 3. 조각상을 훔쳐간다.\n");
    print_slow_at(15, 24, "] 하아 ? \n");
    print_slow_at(10, 28, "어떤 소원을 빌까 ? ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        clear_text();
        move_cursor(0, 10);
        print_slow("\"돈 많이 벌게 해주세요 !! 깁미머니 !\"\n");
        printf("\n");
        print_slow("수뭉이는 실존했다 !!!!!! 하늘에서 돈이 떨어진다 !\n");
        printf("\n");
        print_slow("당신은 잽싸게 하늘에서 떨어지는 모든 돈을 주웠습니다.\n");
        Gold(200);
        printf("\n");
        int replace[] = { totalGold };
        print_slow3("당신은 200G를 얻어, 소지금이 %G가 되었습니다\n", 30, replace, 1);
        Sleep(1000);
        break;
    case 2:
        clear_text();
        move_cursor(0, 10);
        print_slow("\"나는 건강한게 제일 인거같아 나이먹으니까 힘들어...\"\n");
        printf("\n");
        print_slow("소원을 빌자, 하늘에서 뭔가 떨어졌다. 이게 뭐지? 당신은 무언가를 주워 무엇인지 확인했습니다.\n");
        printf("\n");
        print_slow("무언가의 정체는 정x장홍삼주스 였다...! 당신은 재빠르게 x관장홍삼주스를 원샷 때렸습니다\n");
        printf("\n");
        print_slow("\"뭔가 벌써 건강해진 기분이야...\" 다행이네요 !\n");
        Health(20);
        printf("\n");
        int replace2[] = { totalHealth };
        print_slow3("당신의 체력이 20 회복 되어 체력이 %가 되었습니다.\n", 30, replace2, 1);
        Sleep(1000);
        break;
    case 3:
        clear_text();
        move_cursor(0, 10);
        print_slow("당신은 조각상을 훔쳐가기로 결심했습니다. 하지만 조각상은 엄청... 무거워 보이는데요.\n");
        printf("\n");
        print_slow("당신은 그런것 따윈 개의치 않고 조각상을 훔칠 생각에 신이 났습니다. 저런, 욕심많은 사람\n");
        printf("\n");
        print_slow("\"흐흐흐 상점에 팔면 한탕 하는건데 이걸 참아 ? 바로 갈겨 ㅋㅋ\"\n");
        srand(time(0));
        if (rand() % 10 < 4) {
            print_slow("성공했습니다! 당신은 조각상을 상점에 매우 비싸게 팔습니다! 어떻게 했지...?\n");
            Gold(1000);
            printf("\n");
            print_slow("무려 1000G 나 되는 거금을 얻었습니다. 어디에다 쓰려고 ?\n");
            Sleep(1000);
        }
        else {
            print_slow("실패했습니다... 당신은 조각상을 들려다, 조각상에 깔려 사망 직전까지 갔다가 겨우 살았습니다...\n");
            Health(-30);
            printf("\n");
            print_slow("당신은 체력을 30이나 잃었습니다... 바보같은 사람\n");
            int replace[] = { totalHealth };
            print_slow3("체력을 30 잃어 %가 되었습니다...어휴\n", 30, replace, 1);
            Sleep(1000);
        }
        break;
    default:
        print_slow("다른 선택은 없을거 같은데, 나는 이거면 돼\n");
        Sleep(1000);
        event9();
        break;
    }
}
//연못
void event10() {
    clear_text();
    int choice;

    move_cursor(0, 10);
    print_slow("당신은 길을 가다 앞에 연못을 발견했습니다.\n");
    printf("\n");
    print_slow_at(10, 12, "> 1. 달려가서 마신다.\n");
    print_slow_at(10, 14, "> 2. 그냥 지나간다.\n");
    print_slow_at(10, 16, "어떻게 할까..? ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        clear_text();
        move_cursor(0, 10);
        print_slow("당신은 신나게 달려가다가 나무에 걸려 넘어졌습니다...\n");
        printf("\n");
        print_slow("주머니에서 돈이 빠져나와 연못에 들어갔습니다...아이고야\n");
        printf("\n");
        Gold(-20);
        int replace[] = { totalGold };
        print_slow3("소지금을 20원만큼 잃어서 %G가 되었습니다...어휴\n", 30, replace, 1);
        Sleep(1000);
        break;
    case 2:
        clear_text();
        move_cursor(0, 10);
        print_slow("당신은 그냥 무시하고 걸어가기로 했습니다.\n");
        printf("\n");
        print_slow("하지만 무시하고 걸어가자, 당신은 목이 굉장히 마르기 시작했습니다.\n");
        printf("\n");
        print_slow("\"젠장... 목이 너무 말라...\"\n");
        printf("\n");
        Health(-5);
        int replace2[] = { Health };
        print_slow3("당신은 갈증을 참을 수 없습니다... 체력이 5감소 해서 %가 되었습니다,..\n", 30, replace2, 1);
        Sleep(1000);
        break;
    default:
        print_slow("다른 선택은 없다...\n");
        Sleep(1000);
        event10();
        break;
    }
}
//상점주인
void event11() {
    clear_text();
    move_cursor(0, 5);
    print_slow("당신은 길을 걷다 떠돌이 상인을 만났습니다 !\n");
    printf("\n");
    print_slow("\"킬킬킬,,,구경만 하다 가는건 안될세,,,\"\n");
    printf("\n");
    print_slow("좀... 이상한 사람인거 같은데 그냥 지나칠까 ?\n");
    printf("\n");
    print_slow_at(3, 12, "> 1. 지나친다");
    print_slow_at(3, 14, "> 2. 구매하고 가지 뭐");

    int choice;
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        print_slow("당신은 그냥 지나치기로 했습니다...\n");
        printf("\n");
        print_slow("\"생긴게 너무 음험해...\"\n");
        printf("\n");
        print_slow("사람은 외모로 평가하는게 아니에요.\n");
        printf("\n");
        print_slow("아무튼, 나쁜사람\n");
        printf("\n");
        print_slow("아무일도 벌어지지 않았고 당신은 킬킬웃는 상인을 쳐다보다 길을 나섰습니다\n");
        return;
    case 2:
        print_slow("당신은 호기심에 무언가를 구매하기로 결정했습니다\n");
        printf("\n");
        print_slow("\"흠, 한번 보고 갈까\"\n");
        printf("\n");
        print_slow("\"켈켈켈,,,구경만 하면 안되네,,,자 오늘의 상품일세,,,\"\n");
        printf("\n");
        Sleep(1000);
        clear_text();
        move_cursor(0, 2);
        print_slow("\"끌끌끌,,,골라보게나,,,효과는 비밀일세,,,\"\n");
        printf("\n");
        print_at(10, 8, "____________판 매 품 목____________");
        print_at(10, 9, "|                                 |");

        // 랜덤 값으로 아이템 할당
        for (int i = 0; i < ITEMS_TO_DISPLAY; ++i) {
            int random_weapon_index = rand() % ITEM_COUNT;
            move_cursor(10, 10 + i);
            printf("|  > %d. %-15s %4d gold |\n", i + 1, weapons[random_weapon_index].weapon_name, weapons[random_weapon_index].price);
        }

        for (int i = ITEMS_TO_DISPLAY; i < 11; ++i) {
            print_at(10, 11 + i, "|                                 |");
        }

        print_at(10, 21, "|_________________________________|");

        while (1) {
            printf("\n");
            int replace[] = { totalGold };
            print_slow3("소지금: %G\n", 50, replace, 1);
            move_cursor(0, 30);
            printf("\n");
            print_slow("\"구매할 아이템 번호를 입력하면 된다네,,, 끌끌끌,,,\"\n");
            printf("\n");
            print_slow2("쇼핑을 멈추시려면 p키를 입력하면 된답니다 \n!", 50);
            printf("\n");
            print_slow("\",,,크흠,,\"\n");
            printf("\n");

            char input[10];
            scanf("%s", input);

            if (input[0] == 'p') {
                break;
            }

            choice = atoi(input);
            if (choice < 1 || choice > ITEMS_TO_DISPLAY) {
                printf("\n");
                print_slow("잘못 선택 하셨어요. 다시 선택해 볼까요 ?\n");
                printf("\n");
                continue;
            }

            int random_weapon_index = rand() % ITEM_COUNT;

            if (totalGold >= weapons[random_weapon_index].price) {
                totalGold -= weapons[random_weapon_index].price;
                inventory[inventory_count++] = weapons[random_weapon_index];
                printf("\n");
                printf("%s을(를) %d gold에 구매했습니다 !\n", weapons[random_weapon_index].weapon_name, weapons[random_weapon_index].price);
                printf("\n");
            }
            else {
                printf("\n");
                printf("돈이 부족해요 !\n");
                printf("\n");
            }
        }

        printf("\n구매한 아이템:\n");
        printf("\n");
        for (int i = 0; i < inventory_count; ++i) {
            printf("\n");
            printf("- %s\n", inventory[i].weapon_name);
        }

        status();
        return;
    default:
        print_slow("잘못된 선택입니다.");
        return;
    }
}

//이벤트 배열
Event events[] = {
    {"노상강도", event1},
    {"길잃음",event2 },
    {"돈주움",event3 },
    {"음식찾음",event4 },
    {"보물상자",event5 },
    {"파묻힌검",event6 },
    {"마약꽃",event7 },
    {"빛나는물건",event8 },
    {"황금수뭉이",event9 },
    {"연못",event10 },
    {"상점주인",event11}
};
//전투 배열
Fight fights[] = {
    {"야생멧돼지", fight1},
    {"과제에미친수뭉이",fight2 },
    {"조별과제응원빌런",fight3 },
    {"다크데빌수뭉",fight4 },
    {"다리달린물고기",fight5 },
    {"거대한바퀴벌레",fight6 },
    {"귀엽지만은않은수뭉이",fight7 },
    {"무언가",fight8 },
    {"상점주인",fight9 },
    {"벌레떼",fight10 },
};

//특수적 배열
Special specials[6] = {
    {"정재웅", specials1},
    {"김민준",specials2 },
    {"이경민",specials3 },
    {"박조화",specials4 },
    {"동앗줄의길태숙",specials5 },
    {"쌍동앗줄날개성한울",specials6 },

};
//마왕
void bosses3() {

}
//보스대화분기초기화
int text_boss2 = 0;
//녹말이쑤시개의박소영
void bosses2() {
    clear_text();
    Fight_UI2();
    print_at(49, 24, "   ¯ ╭━━━━╮___         ");
    print_at(49, 25, "   . | :::::::::: / ___ /  ");
    print_at(49, 26, " * < | :::::::::(｡ ●ω●｡)");
    print_at(49, 27, "    >し------し-- - Ｊ     ");
    move_cursor(15, 35);
    print_slow("보스, 녹말이쑤시개의 박소영이 등장했다.");
    Sleep(1000);
    print_slow_at(15, 45, "\"안녕 난 녹말이쑤시개의 박소영이라고 해\"");
    Sleep(1000);
    print_at(15, 45, "                                                           ");
    print_slow_at(15, 45, "\"지금 마왕한테 가는길 인거지?\"");
    Sleep(1000);
    print_at(15, 45, "                                                           ");
    print_slow_at(15, 45, "\"자 그러면 바로 본론으로 들어가자\"");
    Sleep(1000);
    print_at(15, 45, "                                                           ");
    while (totalHealth > 0 && total_youHealth > 0 || text_boss2 <= 4) {
        int choice;
        print_slow_at(15, 45, "무엇을 할까 ? ");
        Sleep(1000);
        print_at(15, 45, "                                                           ");
        move_cursor(15, 46);
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            move_cursor(15, 45);
            print_slow("공격하기를 선택했다 ! 타이밍을 잘 맞추도록 하자 ");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            break;
        case 2:
            move_cursor(15, 45);
            print_slow("방어하기를 선택했다 ! 박소영 교수님이 어디로 공격을 하실까... ?? ");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow("아차, 여기서는 녹말이쑤시개의 박소영이다, 말을 조심하도록 하자");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            break;
        case 3:
            move_cursor(15, 45);
            print_slow("대화하기를 선택했다 ! 일단은 사슬낫의 김석규도 대화가 통했으니까...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            Sleep(1000);

            switch (text_boss2) {

            case 0:
                move_cursor(15, 45);
                print_slow("\"저기 녹말이쑤시개의 박소영씨? 님? 아무튼, 마왕은 어떤 사람이죠?\"");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow("\"그게 왜 궁금 한건데? 너도 이제는 알 때도 되지 않았어?\"");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow("녹말이쑤시개는 이상한 소리를 하는 것 같습니다.");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow("\"흐음 뭐 잘해봐 난 별로 관심 없어서\"");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                text_boss2++;
                break;

            case 1:
                print_slow_at(15, 45, "당신은 곰곰히 생각 해 봅니다. 정말로 모르는지,");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "\"...정말로 모르겠어요. 여러분들이 무슨 말을 하는건지\"");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "맙소사.");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "\"와, 너 진짜 멍청하구나?\"");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                text_boss2++;
                break;

            case 2:
                print_slow_at(15, 45, "\"네???\"");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "\"갑자기 재미있어지네\"");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "\"뭐, 됐어");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "\"저기, 저는 마왕을 물리치고 싶지 않아요. 꼭 만나서 대화를 해보고싶어요\"");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "\"흠, 그래? 뭐 그러면 비켜줄 수도 있지\"");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "당신은 갑자기 투지가 불타오릅니다");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                text_boss2++;
                break;

            case 3:
                print_slow_at(15, 45, "\"아아... 좀 더 같이 있자, 버틸 수 있지?\"");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "\"아뇨... 대화를 하는데 왜 공격을...\"");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "\"'누군가' 가 그걸 원하거든\"");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "녹말이쑤시개의 박소영은 종일 이상한 얘기만 하네요. 얘기할 가치가 없습니다.");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                text_boss2++;
                break;

            case 4:
                move_cursor(15, 45);
                print_slow("\"저기 녹말이쑤시개의 박소영씨? 님? 아무튼, 마왕은 어떤 사람이죠?\"");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow("\"몰라?\"");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow("녹말이쑤시개의 박소영은 c 뿌리기 공격을 준비중이다...");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                break;

            }
            break;

        default:
            print_slow_at(15, 45, "지금 뭐하는거야 !!! 소중한 한 턴을 낭비 했다...");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            break;

        }
        if (text_boss2 == 0) {
            print_slow_at(15, 45, "녹말이쑤시개의 박소영은 c뿌리기를 사용했다 !\n");
            int boss3_Damage = rand() % 10 + 30;
            int replace[] = { boss3_Damage };
            print_slow3("켁, 숨이 안쉬어 진다 %를 받았다... c는 너무 어려워...\n", 50, replace, 1);
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            Health(-boss3_Damage);
            status();
            Sleep(1000);
        }
        else {
            print_slow_at(15, 45, "녹말이쑤시개의 박소영은 공격할 마음이 없어보인다\n");
            print_at(15, 45, "                                                           ");
            status();
            Sleep(1000);
            break;
        }
        if (totalHealth <= 0) {
            print_slow_at(15, 45, "목숨이 하나 줄어들었다...\n");
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            Heart(-1);
            Health(totalHealth);
            status();
            Sleep(1000);
            break;
        }
        if (total_bossHealth <= 0) {
            print_slow_at(15, 45, "\"아, 시간됐다 그럼 이만\"");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "녹말이쑤시개의 박소영은 도망쳤다 !");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "녹말이쑤시개의 박소영이 있던자리에는 아무것도 없었다...실망스러워");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "아하하하,,, 가끔은 안 좋은 일도 있는 법이죠 ");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            Heart(totalHeart);
            Health(totalHealth);
            status();
            Sleep(1000);
            print_at(15, 45, "                                                       ");

            void handle_load_count(load_count);
        }
    }

}

//보스1대화변수초기화
int text_boss1 = 0;

//보스1사슬낫의김석규
void bosses1() {
    clear_text();
    Fight_UI2();
    print_at(49, 24, "   ¯ ╭━━━━╮___         ");
    print_at(49, 25, "   . | :::::::::: / ___ /  ");
    print_at(49, 26, " * < | :::::::::(｡ ●ω●｡)");
    print_at(49, 27, "    >し------し-- - Ｊ     ");
    move_cursor(15, 35);
    print_slow("보스, 사슬낫의 김석규가 등장했다.");
    Sleep(1000);
    print_slow_at(15, 45, "\"음, 나는 사슬낫의김석규다. 여기까지 온 걸 일단 환영하지\"\n");
    Sleep(1000);
    print_at(15, 45, "                                                                   ");
    print_slow_at(15, 45, "\"자네는...음 마왕님을 무찌르러 가는길인건가 ?\"\n");
    Sleep(1000);
    print_at(15, 45, "                                                                   ");
    print_slow_at(15, 45, "\"그렇다면 어쩔수 없지만 나를 먼저 지나가야 합세\"\n");
    Sleep(1000);
    print_at(15, 45, "                                                                   ");
    while (totalHealth > 0 && total_youHealth > 0) {
        int choice;
        print_slow_at(15, 45, "무엇을 할까 ? ");
        Sleep(1000);
        print_at(15, 45, "                                                                ");
        move_cursor(15, 46);
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            move_cursor(15, 45);
            print_slow("공격하기를 선택했다 ! 타이밍을 잘 맞추도록 하자 \n");
            Sleep(1000);
            print_at(15, 45, "                                                            ");
            break;
        case 2:
            move_cursor(15, 45);
            print_slow("방어하기를 선택했다 ! 김석규교수님이 어디로 공격을 하실까... ?? \n");
            Sleep(1000);
            print_at(15, 45, "                                                            ");
            print_slow("아차, 여기서는 사슬낫의 김석규다, 말을 조심하도록 하자\n");
            Sleep(1000);
            print_at(15, 45, "                                                            ");
            break;
        case 3:
            move_cursor(15, 45);
            print_slow("대화하기를 선택했다 ! 일단은 보스도 사람이니까 대화를 해보자\n");
            Sleep(1000);
            print_at(15, 45, "                                                            ");
            Sleep(1000);

            switch (text_boss1) {
            case 0:
                move_cursor(15, 45);
                print_slow_at(15, 45, "\"사슬낫의 김석규... 마왕은 어떤 사람이지 ?\"\n");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "사슬낫의 김석규는 한참을 뜸들이다 대답했다\n");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "\"음... 마왕님은 좀... 특이 취향이시지\"\n");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                text_boss1++;
                break;

            case 1:
                print_slow_at(15, 45, "\"어떤 특이 취향을 말하는 거지?\"\n");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "\"글쎄... 자네가 여기까지 잘 왔다면 알 수도\"\n");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                text_boss1++;
                break;

            case 2:
                print_slow_at(15, 45, "\"그게 무슨 의미인거지...?\"\n");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "사슬낫의 김석규는 이상한 소리를 하는 것 같다\n");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "\"...크흠, 나는 잘 모르겠네만 아무튼,간에\"\n");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                text_boss1++;
                break;

            case 3:
                print_slow_at(15, 45, "\"좀 더 자세히 이야기 해줘 ! 마왕은 어떤 인물이지 ?\"\n");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "\"마왕이 악인이 아니라면 이야기 해보고 싶어 !\"\n");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "그건... 정말 좋은 생각인 것 같네요 !\n");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "\"어이쿠야,,, 시간이 벌써, 허허허 이만 가보겠네\"\n");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                text_boss1++;
                break;

            case 4:
                print_slow_at(15, 45, "\"크흠, 다음에 보도록 하지,,,\"");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "사슬낫의 김석규는 도망쳤다 !");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "사슬낫의 김석규가 있던자리에는 금은보화가 가득이였다 !");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                print_slow_at(15, 45, "우후후,, 좋은 날이네요, 이런 날도 있어야죠 ");
                Sleep(1000);
                print_at(15, 45, "                                                           ");
                Gold(500);
                Heart(totalHeart);
                Health(totalHealth);
                status();
                Sleep(1000);
                print_at(15, 45, "                                                       ");
                void handle_load_count(load_count);
                return;
            }
            break;

        default:
            print_slow_at(15, 45, "지금 뭐하는거야 !!! 소중한 한 턴을 낭비 했다...\n");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            break;

        }
        if (text_boss1 == 0) {
            move_cursor(15, 45);
            print_slow_at(15, 45, "\"사슬낫의 김석규, 우리 대화를 하자.\"\n");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "\"사슬낫의김석규는 대답하지 않고 공격을 준비하고 있다.\"\n");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
        }
        if (text_boss1 == 0) {
            print_slow_at(15, 45, "사슬낫의김석규는 파이썬뿌리기를 사용했다 !\n");
            int boss1_Damage = rand() % 20 + 40;
            int replace[] = { boss1_Damage };
            print_slow3("크윽, 역시 보스인가... % 의 피해를 입었다...\n", 50, replace, 1);
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            Health(-boss1_Damage);
            status();
            Sleep(1000);
        }
        else {
            print_slow_at(15, 45, "사슬낫의김석규는 공격할 마음이 없는 것 같다\n");
            Sleep(1000);
            print_at(15, 45, "                                                     ");
            print_slow_at(15, 45, "다행이네요 ! 계속 대화해봐요 !\n");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            status();

        }
        if (totalHealth <= 0) {
            print_slow_at(15, 45, "목숨이 하나 줄어들었다...\n");
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            Heart(-1);
            Health(totalHealth);
            status();
            Sleep(1000);
            break;
        }
        if (text_boss1 == 4) {
            print_slow_at(15, 45, "\"크흠, 다음에 보도록 하지,,,\"");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "사슬낫의 김석규는 도망쳤다 !");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "사슬낫의 김석규가 있던자리에는 금은보화가 가득이였다 !");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            print_slow_at(15, 45, "우후후,, 좋은 날이네요, 이런 날도 있어야죠 ");
            Sleep(1000);
            print_at(15, 45, "                                                           ");
            Gold(500);
            Heart(totalHeart);
            Health(totalHealth);
            status();
            Sleep(1000);
            print_at(15, 45, "                                                       ");
            void handle_load_count(load_count);
            return;
        }
    }
}
//보스 배열
Boss bosses[3] = {
    {"사슬낫의김석규", bosses1},
    {"녹말이쑤시개의박소영",bosses2 },
    {"마왕",bosses3 },
};

//길 선택수 초기화
int load_count = 0;
//특수적 선택 초기화
int special_count = 1;

//갈림길함수정의
void handle_load_count(int count) {

    while (load_count < 12) {
        clear_text();
        status();
        Sleep(3000);
        if (load_count != 3 && load_count != 7 && load_count != 11) {
            clear_text();
            print_slow_at(3, 2, "또 다시 갈림길이다.");
            print_at(10, 5, "> a. 뭔가 무시무시한 소리가 들리는 길\n");
            print_slow_at(15, 8, " *피가 끓어 오르는 소리가 들린다. 오른팔이...");
            print_at(10, 12, "> b. 여러가지 소리가 들려오는 복잡하고 어두운 길\n");
            print_slow_at(15, 15, " *가면 무언가 벌어질 것만 같아...");
            print_at(10, 19, "> c. 여기로 가면 안될 것 같다,,,\n");
            print_slow_at(15, 22, " *진짜로 여기는 가지말자 무서워요...");
            print_slow_at(10, 25, "어디로 가지? \n");

            char choice;
            scanf(" %c", &choice);

            switch (choice) {
            case 'a':case 'A':
                clear_text();
                move_cursor(0, 10);
                print_slow("투지가 불 타 오른다\n");
                Sleep(1000);
                int random_fight = rand() % 10;
                fights[random_fight].fight_Function();
                load_count++;
                break;
            case 'b':
                clear_text();
                move_cursor(0, 10);
                print_slow("이상한 기분이다...\n");
                Sleep(1000);
                event11();
                srand(time(0));
                int random_event = rand() % 10;
                events[random_event].event_Function();
                load_count++;
                break;
            case 'c':
                clear_text();
                move_cursor(0, 10);
                print_slow("앞으로 나아가니 이상한게 보인다... 가까이 가면 안될거같은데\n");
                Sleep(1000);
                switch (special_count) {
                case 1:
                    specials1();
                    break;
                case 2:
                    specials2();
                    break;
                case 3:
                    specials3();
                    break;
                case 4:
                    specials4();
                    break;
                case 5:
                    specials5();
                    break;
                case 6:
                    specials6();
                    break;
                default:
                    break;
                }
                special_count++;
                break;

            default:
                clear_text();
                move_cursor(0, 10);
                print_slow("다른길은 없는 것 같다. 나아가야 할 때다.\n");
                handle_load_count(load_count);
                break;
            }
        }
        else if (load_count == 3) {
            clear_text();
            move_cursor(0, 10);
            print_slow("갈림길들을 거친 당신 앞에는 한개의 길밖에 남지 않았습니다.\n");
            printf("\n");
            print_slow("무시무시한 기운이 느껴집니다. 어떻게 할까요 ?\n");
            printf("\n");
            Sleep(1000);
            print_slow("\"당연히 가지, 그 동안 난 강해졌다. 출발할때다. 목표는 마왕, 가자 !\"\n");
            Sleep(1000);
            clear_text();
            status();
            bosses1();
        }
        else if (load_count == 7) {
            clear_text();
            move_cursor(0, 10);
            print_slow("갈림길들을 거친 당신 앞에는 한개의 길밖에 남지 않았습니다.\n");
            printf("\n");
            print_slow("무시무시한 기운이 느껴집니다. 어떻게 할까요 ?\n");
            printf("\n");
            Sleep(1000);
            print_slow("\"당연히 가지, 그 동안 난 더 강해졌다, 사슬낫의김석규도 물리쳤고 말이야. 가자 !!\"\n");
            printf("\n");
            Sleep(1000);
            print_slow("\"아주,,, 좋은 생각이에요 자 마왕을 향해 가보자구요\"\n");
            Sleep(1000);
            clear_text();
            status();
            bosses2();
        }
        else if (load_count == 11) {
            clear_text();
            move_cursor(0, 10);
            print_slow("갈림길들을 거친 당신 앞에는 한개의 길밖에 남지 않았습니다.\n");
            printf("\n");
            print_slow("어떠한 기운이 느껴집니다. 어떻게 할까요 ?\n");
            printf("\n");
            Sleep(1000);
            print_slow("\"당연히 가지, 나는 모든 보스를 물리쳤어, 가자 마왕에게로\"\n");
            printf("\n");
            print_slow2("\"우후후,,, 기다렸답니다\"\n", 200);
            Sleep(1000);
            clear_text();
            status();
            bosses3();
        }
    }
}

//대사수정필요
void start_dead() {
    clear_text();
    print_slow_at(0, 5, "마왕성 까지 달려오니 정말 금방이였다.\n"
        "\n"
        "\"뭐야 별 거 없네... 이제 들어가서 마왕을 무찌르면 되려나...\"\n");
    clear_text();
    if (totalStrength == 10) {
        print_slow_at(0, 8, "당신에게는 100골드와 철검뿐이다. 그래도 들어갈까?\n");
        printf("\n");
        printf(" > 들어가자 (1)\n");
        printf("\n");
        printf(" > 너무 급한것 같다 (2)\n");
        printf("\n");
        int a;
        scanf("%d", &a);
        if (a == 1) {
            clear_text();
            print_slow_at(5, 8, "당신이 들어가기로 결심하자 마자. . .\n");
            Sleep(1000);
            print_slow_at(5, 10, "시야가 어두워 지며. . .\n");
            Sleep(1000);
            print_slow_at(5, 12, "\"뭐야 이 벌레는 ?\"\n");
            move_cursor(8, 15);
            print_slow2("당 신 은   의 식 을  잃 었 다. . .", 800);
            print_at(12, 19, "게임 오버");
            Sleep(1000);
            exit(0);

        }
        else if (a == 2) {
            clear_text();
            print_slow_at(5, 8, "당신이 뒤로 돌아가려 하자마자, 돌아가는 길이 사라졌다\n");
            print_slow_at(5, 10, "\"뭐야?\"\n");
            print_slow_at(5, 12, "돌아가는 길은 없다. 나아가자");
            clear_text();
            print_slow_at(5, 8, "당신이 들어가기로 결심하자 마자. . .\n");
            Sleep(1000);
            print_slow_at(5, 10, "시야가 어두워 지며. . .\n");
            Sleep(1000);
            print_slow_at(5, 12, "\"뭐야 이 벌레는 ?\"\n");
            move_cursor(8, 15);
            print_slow2("당 신 은   의 식 을  잃 었 다. . .", 500);
            print_at(12, 19, "게임 오버");
            Sleep(1000);
            exit(0);
        }
    }
    else {
        print_slow_at(0, 8, "당신에게는 50골드와 목검뿐이다. 그래도 들어갈까?\n");
        printf("\n");
        printf(" > 들어가자 (1)\n");
        printf("\n");
        printf(" > 너무 급한것 같다 (2)\n");
        printf("\n");
        int a;
        scanf("%d", &a);
        if (a == 1) {
            clear_text();
            print_slow_at(5, 8, "당신이 들어가기로 결심하자 마자. . .\n");
            Sleep(1000);
            print_slow_at(5, 10, "시야가 어두워 지며. . .\n");
            Sleep(1000);
            print_slow_at(5, 12, "\"뭐야 이 벌레는 ?\"\n");
            move_cursor(8, 15);
            print_slow2("당 신 은   의 식 을  잃 었 다. . .", 800);
            print_at(12, 19, "게임 오버");
            Sleep(1000);
            exit(0);

        }
        else if (a == 2) {
            clear_text();
            print_slow_at(5, 8, "당신이 뒤로 돌아가려 하자마자, 돌아가는 길이 사라졌다\n");
            print_slow_at(5, 10, "\"뭐야?\"\n");
            print_slow_at(5, 12, "돌아가는 길은 없다. 나아가자");
            clear_text();
            print_slow_at(5, 8, "당신이 들어가기로 결심하자 마자. . .\n");
            Sleep(1000);
            print_slow_at(5, 10, "시야가 어두워 지며. . .\n");
            Sleep(1000);
            print_slow_at(5, 12, "\"뭐야 이 벌레는 ?\"\n");
            move_cursor(8, 15);
            print_slow2("당 신 은   의 식 을  잃 었 다. . .", 500);
            print_at(12, 19, "게임 오버");
            Sleep(1000);
            exit(0);
        }
    }
}

void story() {
    clear_text();
    move_cursor(0, 10);
    print_slow("무섭지만 왜인지 쭉 뻗은길은 가면 안될 것만 같다...\n");
    printf("\n");
    print_slow("당신은 숲으로 들어갔다...\n");
    Sleep(1000);
    clear_text();
    move_cursor(0, 10);
    print_slow("숲으로 들어오자, 당신은 세 갈림 길을 발견했다\n");
    printf("\n");
    print_slow("이번에는 어디로 가야할까. . .?\n");
    Sleep(1000);

    handle_load_count(load_count);
}
//첫번째 길선택 
void street() {
    clear_text();
    status();
    Sleep(2000);
    clear_text();
    move_cursor(0, 10);
    print_slow("뭔지 하나도 모르겠지만, 아무튼 출발 해볼까...\n");
    printf("\n");
    print_slow2("당신은 당신이 깨어났던 곳을 나와 출발하려던 차에 갈림길을 발견했다. . .\n", 90);
    printf("\n");
    print_slow("어디로 갈까 ?");
    Sleep(1000);
    clear_text();

    move_cursor(10, 5);
    printf(" > a. 마왕성이 보이는 뻥 뚫린 길");
    move_cursor(14, 8);
    print_slow(" * 이 길로 가면 바로 마왕을 만날 수 있을 것만 같다.");
    move_cursor(10, 14);
    printf(" > b. 울창한 숲이 보이고 이상한 소리들이 들려오는 길");
    move_cursor(14, 17);
    print_slow(" * 무섭다. . . 마왕성 까지 가려면 한참 걸릴 것 같다. . .");
    move_cursor(10, 21);
    print_slow("어디로 갈까 ?");

    char choice;
    scanf(" %c", &choice);

    switch (choice) {
    case 'a': {
        start_dead();
    }
    case 'b': {
        story();
    }
    default: {
        clear_text();
        move_cursor(5, 20);
        printf("이 외에 다른 길은 없다. 제대로 선택하자. ");
        Sleep(1000);
        street();
    }
    }

}

//첫번째 스탯변환 선택지
void startChoice() {
    printf("\n");
    print_slow("\"시간이 없어요 용사님. 빨리 마왕을 무찌르러 가주세요.\"\n");
    printf("\n");
    print_slow("1. > 알겠습니다. 당장 출발하겠습니다.\n");
    printf("\n");
    print_slow("2. > 네? 그게 무슨 소리에요. 여긴 어딘데요?\n");
    printf("\n");

    char choice;
    scanf(" %c", &choice);

    if (choice == '1') {
        printf("\n");
        print_slow("\"감사합니다! 당신의 앞길에 축복이 깃들길! 여기 지원품 입니다!\"\n");
        printf("\n");
        print_slow("당신은 100G와 철 검을 얻었습니다.\n\n");
        Sleep(1000);
        Gold(100);
        Strength(10);
        street();
    }
    else if (choice == '2') {
        printf("\n");
        print_slow("\"하아, 어서 출발하세요. 여기요.\"\n");
        printf("\n");
        print_slow("당신은 50G와 목검을 얻었습니다.\n\n");
        Gold(50);
        Strength(5);
        Sleep(1000);
        street();

    }
    else {
        printf("\n");
        print_slow("\"뭐라 하시는거에요?\"\n");
        Sleep(1000);
        startChoice();
    }
}

//인트로
void intro() {
    clear_text();
    print_slow_at(0, 5, "끼--익 !!!! 쾅- !!!!\n"
        "\n"
        "'아아... 이렇게 트릭에 치여 삶을 마감하는 건가'\n"
        "\n"
        "눈앞이 점멸하며 주마등이 스쳐 지나가. . .\n"
        "지 않고 낯선 천장이 날 맞이했다\n"
        "\n"
        "'아?'\n"
        "\n"
        "나 트럭에 치였는데?\n"
        "\n"
        "주변을 둘러보니 꽤나 고풍스럽다. 그리고 사람이... 왜이리 많아!??\n"
        "\n"
        "뭐지여기어디지당신들누구야저너무무서워요\n"
        "\n"
        "수 많은 사람들 중 한 여성이 날 향해 입을 열었다\n"
        "\n"
        "\"일어나세요 용사님. 당신을 기다렸답니다.\"\n"
        "\n"
        "'네?'\n"
        "\n");
    print_slow2("\"2147483536번 째 용사님, 마왕을 무찔러 저희를 구해주세요\"\n", 100);
    printf("\n");
    print_slow("\"네?'\"\n");
    printf("\n");

    startChoice();
}
//메인함수
int main() {
    int width = 100;
    int height = 50;

    set_console_size(width, height);

    char choice;

    while (1) {
        clear_text();
        move_cursor(10, 5);
        print_slow2("일어나세요, 용사님\n", 200);
        move_cursor(8, 8);
        print_slow("> 일어나기(a)\n");
        scanf(" %c", &choice);

        if (choice == 'a') {
            intro();
            break;
        }
        else {
            clear_text();
            move_cursor(10, 5);
            print_slow2("\"하아, 일어나시라니까.\"\n", 100);

        }
    }
    return 0;
}
