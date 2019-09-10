#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// グローバル変数
// リテラルを用いることで静的領域にメモリを確保
int gems[14];
char* letters[14] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N"};
char* playerName;
double partyHp = 600.0;
int partyDefence = (10 + 10 + 5 + 15) / 4;

int suraimuHp = 100;
int goburinHp = 200;
int ookomoriHp = 300;
int weawolfHp = 400;
int doragonHp = 800;

enum {FIRE, WATER, WIND, EARTH, LIFE, EMPTY};
// 構造体宣言
typedef struct {
    int id;
    char* name;
    int hp;
    int maxHp;
    int type;
    int attack;
    int defence;
} Monster;

Monster suzaku = {0, "朱雀", 150, 150, 0, 25, 10};
Monster seiryu = {1, "青龍", 150, 150, 2, 15, 10};
Monster byakko = {2, "白虎", 150, 150, 3, 20, 5};
Monster genbu = {3, "玄武", 150, 150, 1, 20, 15};

Monster suraimu = {0, "スライム", 100, 100, 1, 10, 5};
Monster goburin = {1, "ゴブリン", 200, 200, 3, 20, 15};
Monster ookomori = {2, "オオコウモリ", 300, 300, 2, 30, 25};
Monster weawolf = {3, "ウェアウルフ", 400, 400, 2, 40, 30};
Monster doragon = {4, "ドラゴン", 800, 800, 0, 50, 40};


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

// 敵から味方への攻撃
void enemyAttack(Monster* m) {
    srand((unsigned int)time(NULL));
    int attack = (*m).attack;
    int rondomNumber = rand() % 21;
    // printf("%d\n", rondomNumber);
    int diff = attack - partyDefence;
    // printf("%d\n", diff);
    // (double)はキャスト int -> double
    double swingWidth = ((double) (rondomNumber - 10) * 0.01) + 1.0;
    // printf("%f\n", swingWidth);
    double damage = (double) (diff) * swingWidth;
    if(damage <= 0 || attack == partyDefence) {
        damage = 1.0;
    }
    partyHp -= damage;
    // %.0f 小数桁数は表示しない
    printf("[%sのターン]\n~%s~の攻撃!%.0fのダメージを受けた\n", (*m).name, (*m).name, damage);
}

double judgeMonstersType(Monster* ally, Monster* enemy) {
    int atype = (*ally).type;
    int etype = (*enemy).type;
    if((atype == 1 && etype == 0) || (atype == 0 && etype == 2) || (atype == 2 && etype == 3) || (atype == 3 && etype == 1)) {
        return 2.0;
    } else if((etype == 1 && atype == 0) || (etype == 0 && atype == 2) || (etype == 2 && atype == 3) || (etype == 3 && atype == 1)) {
        return 0.5;
    } else {
        return 1.0;
    }
}


Monster* IdToAlly(int allyId) {
    switch (allyId) {
        case 0:
            return &suzaku;
        case 1:
            return &seiryu;
        case 2:
            return &byakko;
        case 3:
            return &genbu;
        default:
            break;
    }
}

// 味方から敵への攻撃
double allyAttack(int allyId, Monster* enemy, int igems, int combo) {
    if(allyId == 4) {
        return 0.0;
    }
    Monster* ally = IdToAlly(allyId);
    int attack = (*ally).attack;
    printf("attack: %d\n", attack);
    int defence = (*enemy).defence;
    printf("defence: %d\n", defence);
    int diff = attack - defence;
    printf("diff: %d\n", diff);
    double revision = judgeMonstersType(ally, enemy);
    printf("revision: %f\n", revision);
    int rondomNumber = rand() % 21;
    printf("rondom: %d\n", rondomNumber);
    double swingWidth = ((double) (rondomNumber - 10) * 0.01) + 1.0;
    printf("swingwidth: %f\n", swingWidth);
    double power = pow(1.5, (double) (igems - 3 + combo));
    printf("power: %f\n", power);
    double damage = (double) (diff) * revision * power * swingWidth;
    printf("%sに%.0fのダメージ！\n", (*enemy).name, damage);
    return damage;
}


// パズルを評価する
int judgeGems(int i, int combo) {
    switch(gems[i]) {
        case FIRE:
            if(combo > 1) {
                printf("$朱雀$の攻撃！ %d COMBO!\n", combo);
                return 0;
            } else {
                printf("$朱雀$の攻撃！ \n");
                return 0;
            }
            break;      
        case WATER:
            if(combo > 1) {
                printf("~玄武~の攻撃！ %d COMBO!\n", combo);
                return 3;
            } else {
                printf("~玄武~の攻撃！ \n");
                return 3;
            }
            break;
        case WIND:
            if(combo > 1) {
                printf("@青龍@の攻撃！ %d COMBO!\n", combo);
                return 1;
            } else {
                printf("@青龍@の攻撃！ \n");
                return 1;
            }
            break;
        case EARTH:
            if(combo > 1) {
                printf("#白虎#の攻撃！ %d COMBO!\n", combo);
                return 2;
            } else {
                printf("#白虎#の攻撃！ \n");
                return 2;
            }
            break;
        case LIFE:
            if(combo > 1) {
                printf("味方のライフが回復した！ %d COMBO!\n", combo);
                return 4;
            } else {
                printf("味方のライフが回復した！\n", combo);
                return 4;
            }
            break;
    }
}

// 0 => 敵死亡 1 => 敵生きてる
int decreaseEnemyHp(Monster* enemy, int damage) {
    if(enemy == &suraimu) {
        suraimuHp -= damage;
        if(suraimuHp <= 0) {
            return 0;
        }
        return 1;
    } else if(enemy == &goburin) {
        goburinHp -= damage;
        if(goburinHp <= 0) {
            return 0;
        }
        return 1;
    } else if(enemy == &ookomori) {
        ookomoriHp -= damage;
        if(ookomoriHp <= 0) {
            return 0;
        }
        return 1;
    } else if(enemy == &weawolf) {
        weawolfHp -= damage;
        if(weawolfHp <= 0) {
            return 0;
        } 
        return 1;
    } else if(enemy == &doragon) {
        doragonHp -= damage;
        if(doragonHp <= 0) {
            return 0;
        }
        return 1;
    }
}

int judgeAndJustyfyGems(Monster* enemy) {
    srand((unsigned int)time(NULL));
    int combo = 0;
    int comboed = 1;
    int count = 0;
    int allyId;
    int damage;
    int enemyDied;
    for(int i = 0; i < 12; i++) {
        if(memcmp(&gems[i], &gems[i + 1], 1) == 0 && memcmp(&gems[i + 1], &gems[i + 2], 1) == 0 && memcmp(&gems[i + 2], &gems[i + 3], 1) == 0 && memcmp(&gems[i + 3], &gems[i + 4], 1) == 0) {
            combo++;
            comboed = 0;
            allyId = judgeGems(i, combo);
            damage = allyAttack(allyId, enemy, 5, combo);
            enemyDied = decreaseEnemyHp(enemy, damage);
            printGems();
            for(int j = 4; j > -1; j--) {
                for(int k = 0; k < 14 - (i + j); k++) {
                    gems[(i + j) + k] = gems[(i + j) + (k + 1)];
                }
                int gemType = rand() % 5;
                gems[13] = gemType;
                printGems();
            }
            if(enemyDied == 0) {
                return 0;
            }
            judgeAndJustyfyGems(enemy);
        } else if(memcmp(&gems[i], &gems[i + 1], 1) == 0 && memcmp(&gems[i + 1], &gems[i + 2], 1) == 0 && memcmp(&gems[i + 2], &gems[i + 3], 1) == 0){
            combo++;
            comboed = 0;
            allyId = judgeGems(i, combo);
            damage = allyAttack(allyId, enemy, 4, combo);
            enemyDied = decreaseEnemyHp(enemy, damage);
            printGems();
            for(int j = 3; j > -1; j--) {
                for(int k = 0; k < 14 - (i + j); k++) {
                    gems[(i + j) + k] = gems[(i + j) + (k + 1)];
                }
                int gemType = rand() % 5;
                gems[13] = gemType;
                printGems();
            }
            if(enemyDied == 0) {
                return 0;
            }
            judgeAndJustyfyGems(enemy);
        } else if(memcmp(&gems[i], &gems[i + 1], 1) == 0 && memcmp(&gems[i + 1], &gems[i + 2], 1) == 0) {
            combo++;
            comboed = 0;
            allyId = judgeGems(i, combo);
            damage = allyAttack(allyId, enemy, 3, combo);
            enemyDied = decreaseEnemyHp(enemy, damage);
            printGems();
            for(int j = 2; j > -1; j--) {
                for(int k = 0; k < 14 - (i + j); k++) {
                    gems[(i + j) + k] = gems[(i + j) + (k + 1)];
                }
                int gemType = rand() % 5;
                gems[13] = gemType;
                printGems();
            }
            if(enemyDied == 0) {
                return 0;
            }
            judgeAndJustyfyGems(enemy);
        } 
    }

    return 1;
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

int enemyCurrentHp(Monster* enemy) {
    if(enemy == &suraimu) {
        return suraimuHp;
    } else if(enemy == &goburin) {
        return goburinHp;
    } else if(enemy == &ookomori) {
        return ookomoriHp;
    } else if(enemy == &weawolf) {
        return weawolfHp;
    } else if(enemy == &doragon) {
        return doragonHp;
    }
}

void oneTurnOfButtle(Monster *m) {
    int enemyMaxHp = (*m).hp;
    int enemyHp = enemyCurrentHp(m);
    int enemyDied;
    printf("[%sのターン]", playerName);
    printf("------------------------------\n");
    printf("~%s~\nHP= %d / %d\n\n\n\n", (*m).name, enemyHp, enemyMaxHp);
    printf("$朱雀$ @青龍@ #白虎# ~玄武~\n");
    printf("HP= %.0f / %d\n", partyHp, 600);
    printf("------------------------------\n");
    printf("A B C D E F G H I J K L M N\n");
    printGems();
    printf("------------------------------\n");
    moveGem();
    enemyDied = judgeAndJustyfyGems(m);
    if(enemyDied != 0) {
        enemyAttack(m);
        oneTurnOfButtle(m);
    } else {
        printf("%sに勝利した！\n\n", (*m).name);
    }
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

    rondomShuffleGems();

    printf("~%s~が現れた！\n\n\n", (*&suraimu).name);
    oneTurnOfButtle(&suraimu);
    printf("~%s~が現れた！\n\n\n", (*&goburin).name);
    oneTurnOfButtle(&goburin);
    printf("~%s~が現れた！\n\n\n", (*&ookomori).name);
    oneTurnOfButtle(&ookomori);
    printf("~%s~が現れた！\n\n\n", (*&weawolf).name);
    oneTurnOfButtle(&weawolf);
    printf("~%s~が現れた！\n\n\n", (*&doragon).name);
    oneTurnOfButtle(&doragon);
    return 0;
}