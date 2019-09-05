#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// グローバル変数
// リテラルを用いることで静的領域にメモリを確保
int gems[14];
char* letters[14] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N"};
char* playerName;
int partyHp = 600;
int partyDefence;

enum {FIRE, WATER, WIND, EARTH, LIFE, EMPTY};
// 構造体宣言
typedef struct {
    char *name;
    int hp;
    int maxHp;
    char *type;
    int attack;
    int defence;
} Monster;

Monster suzaku = {"朱雀", 150, 150, "火", 25, 10};
Monster seiryu = {"青龍", 150, 150, "風", 15, 10};
Monster byakko = {"白虎", 150, 150, "土", 20, 5};
Monster genbu = {"玄武", 150, 150, "土", 20, 15};

Monster suraimu = {"スライム", 100, 100, "水", 10, 5};
Monster goburin = {"ゴブリン", 200, 200, "土", 20, 15};
Monster ookomori = {"オオコウモリ", 300, 300, "風", 30, 25};
Monster weawolf = {"ウェアウルフ", 400, 400, "風", 40, 30};
Monster doragon = {"ドラゴン", 800, 800, "火", 50, 40};


void printMonsterSummary(Monster* m) {
    printf("$%s$ HP=%d 攻撃=%d 防御=%d\n", (*m).name, (*m).hp, (*m).attack, (*m).defence);
}

// Gemsを表示する
void printGems() {
    for(int i = 0; i < 14; i++) {
        switch(gems[i]) {
            case FIRE:
                printf("$");
                break;
            case WATER:
                printf("~");
                break;
            case WIND:
                printf("@");
                break;
            case EARTH:
                printf("#");
                break;
            case LIFE:
                printf("&");
                break;
        }
        printf(" ");
    }
    printf("\n");
}

// ランダムに1つのGemを生成する
int rondomCreateGem() {
    srand((unsigned int)time(NULL));
    int gemType = rand() % 5;
    return gemType;
}

// 初めにランダムにスロットを並べる
void rondomShuffleGems() {
    srand((unsigned int)time(NULL));
    for(int i = 0; i < 14; i++) {
        int gemType = rand() % 5;
        gems[i] = gemType;
    }
    printGems();
    printf("------------------------------\n");
}

// コマンドに対して配列のインデックスを返す
int askNumberOfLetter(char* letter) {
    int sizeOfLetters = sizeof letters / sizeof letters[0];
    for(int i = 0; i < sizeOfLetters; i++) {
        if(strcmp(letter, letters[i]) == 0) {
            return i;
        }
    }
    printf("正しくコマンドを入力してください");
    return 1;
}

// パズルを評価する
int judgePuzzle(int combo) {
    for(int i = 0; i < 12; i++) {
        if(memcmp(&gems[i], &gems[i + 1], 1) == 0 && memcmp(&gems[i + 1], &gems[i + 2], 1) == 0) {
            switch(gems[i]) {
                case FIRE:
                    if(combo > 1) {
                        printf("$朱雀$の攻撃！ %d COMBO!\n", combo);
                    } else {
                        printf("$朱雀$の攻撃！ \n");
                    }
                    return i;
                    break;
                case WATER:
                    if(combo > 1) {
                        printf("~玄武~の攻撃！ %d COMBO!\n", combo);
                    } else {
                        printf("~玄武~の攻撃！ \n");
                    }
                    return i;
                    break;
                case WIND:
                    if(combo > 1) {
                        printf("@青龍@の攻撃！ %d COMBO!\n", combo);
                    } else {
                        printf("@青龍@の攻撃！ \n");
                    }
                    return i;
                    break;
                case EARTH:
                    if(combo > 1) {
                        printf("#白虎#の攻撃！ %d COMBO!\n", combo);
                    } else {
                        printf("#白虎#の攻撃！ \n");
                    }
                    return i;
                    break;
                case LIFE:
                    if(combo > 1) {
                        printf("味方のライフが回復した！ %d COMBO!\n", combo);
                    } else {
                        printf("味方のライフが回復した！ \n");
                    }
                    return i;
                    break;
            }
        } 
    }

    return 15;
}

void leftJustify(int i) {
    // 2コンボ以降生成されるGemが同じパターン
    srand((unsigned int)time(NULL));
    int comboed;
    if(memcmp(&gems[i], &gems[i + 1], 1) == 0 && memcmp(&gems[i + 1], &gems[i + 2], 1) == 0) {
        printGems();
        for(int j = 2; j > -1; j--) {
            for(int k = 0; k < 14 - (i + j); k++) {
                gems[(i + j) + k] = gems[(i + j) + (k + 1)];
            }
            int gemType = rand() % 5;
            gems[13] = gemType;
            printGems();
        }
    }
}


// コマンドを受け取ってGemsを動かす
void moveGem() {
    char command[3];
    char cmd1[2];
    char cmd2[2];

    int index_cmd1;
    int index_cmd2;
    int diff_index;

    printf("コマンド？>");
    scanf("%s", command);

    if(strlen(command) == 2) {
        strncpy(cmd1, command, 1);
        cmd1[1] = '\0';
        strncpy(cmd2, command + 1, 1);
        cmd2[1] = '\0';

        index_cmd1 = askNumberOfLetter(cmd1);
        index_cmd2 = askNumberOfLetter(cmd2);

        if(index_cmd1 < index_cmd2) {
            diff_index = index_cmd2 - index_cmd1;
            printGems();
            for(int i = 0; i < diff_index; i++) {
                int s = gems[index_cmd1 + i];
                gems[index_cmd1 + i] = gems[index_cmd1 + i + 1];
                gems[index_cmd1 + i + 1] = s;
                printGems();
            }
        } else {
            diff_index = index_cmd1 - index_cmd2;
            printGems();
            for(int i = 0; i < diff_index; i++) {
                int t = gems[index_cmd2 + i];
                gems[index_cmd2 + i] = gems[index_cmd2 + i + 1];
                gems[index_cmd2 + i + 1] = t;
                printGems();
            }
        }
        printf("\n");

    } else {
        printf("コマンドはA~Nまでの大文字2文字を入力してください (ex) A-D\n");
        moveGem();
    }
}

// 敵の攻撃
void enemyAttack(Monster* m) {
    int attack = (*m).attack;
    partyHp -= attack;
    printf("[%sのターン]\n~%s~の攻撃!%dのダメージを受けた\n", (*m).name, (*m).name, attack);
}

// モンスターとのバトル
int battleWithMonster(Monster* m) {
    int enemyMaxHp = (*m).hp;
    int enemyCurrentHp = (*m).hp;
    int i;
    int combo = 0;
    printf("~%s~が現れた！\n\n\n", (*m).name);
    printf("[%sのターン]", playerName);
    printf("------------------------------\n");
    printf("~%s~\nHP= %d / %d\n\n\n\n", (*m).name, enemyCurrentHp, enemyMaxHp);
    printf("$朱雀$ @青龍@ #白虎# ~玄武~\n");
    printf("HP= %d / %d\n", partyHp, 600);
    printf("------------------------------\n");
    printf("A B C D E F G H I J K L M N\n");
    rondomShuffleGems();
    moveGem();

    // 3comboまでしか対応できない
    combo++;
    i = judgePuzzle(combo);
    if(i != 15) {
        leftJustify(i);
    }

    combo++;
    i = judgePuzzle(combo);
    if(i != 15) {
        leftJustify(i);
    }

    combo++;
    i = judgePuzzle(combo);
    if(i != 15) {
        leftJustify(i);
    }

    enemyAttack(&suraimu);
}

int main(int argc, char** argv) {
    playerName = *(argv + 1);
    printf("*** Puzzle & Monsters ***\n");
    printf("%sのパーティ(HP=600)はダンジョンに到達した\n", playerName);
    printf("<パーティ編成>----------\n");

    printMonsterSummary(&suzaku);
    printMonsterSummary(&seiryu);
    printMonsterSummary(&byakko);
    printMonsterSummary(&genbu);

    printf("------------------------\n\n\n");

    battleWithMonster(&suraimu);
    return 0;
}