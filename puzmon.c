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

double suraimuHp = 75.0;
double goburinHp = 200.0;
double ookomoriHp = 300.0;
double weawolfHp = 400.0;
double doragonHp = 800.0;

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

Monster suzaku = {0, "朱雀", 150, 150, 0, 45, 10};
Monster seiryu = {1, "青龍", 150, 150, 2, 35, 10};
Monster byakko = {2, "白虎", 150, 150, 3, 40, 5};
Monster genbu = {3, "玄武", 150, 150, 1, 40, 15};

Monster suraimu = {0, "スライム", 100, 100, 1, 10, 5};
Monster goburin = {1, "ゴブリン", 200, 200, 3, 20, 15};
Monster ookomori = {2, "オオコウモリ", 300, 300, 2, 30, 25};
Monster weawolf = {3, "ウェアウルフ", 400, 400, 2, 40, 30};
Monster doragon = {4, "ドラゴン", 800, 800, 0, 50, 40};


void printMonsterSummary(Monster* m) {
    int type = (*m).type;
    switch(type) {
        case FIRE:
            printf("\x1b[31m$%s$\033[m HP=%d 攻撃=%d 防御=%d\n", (*m).name, (*m).hp, (*m).attack, (*m).defence);
            break;
        case WATER:
            printf("\x1b[34m~%s~\033[m HP=%d 攻撃=%d 防御=%d\n", (*m).name, (*m).hp, (*m).attack, (*m).defence);
            break;
        case WIND:
            printf("\x1b[36m@%s@\033[m HP=%d 攻撃=%d 防御=%d\n", (*m).name, (*m).hp, (*m).attack, (*m).defence);
            break;
        case EARTH:
            printf("\x1b[33m#%s#\033[m HP=%d 攻撃=%d 防御=%d\n", (*m).name, (*m).hp, (*m).attack, (*m).defence);
            break;
        default:
            break;
    }
}

// Gemsを表示する
void printGems() {
    for(int i = 0; i < 14; i++) {
        switch(gems[i]) {
            case FIRE:
                printf("\x1b[41m$\x1b[m");
                break;
            case WATER:
                printf("\x1b[44m~\x1b[m");
                break;
            case WIND:
                printf("\x1b[46m@\x1b[m");
                break;
            case EARTH:
                printf("\x1b[43m#\x1b[m");
                break;
            case LIFE:
                printf("\x1b[42m&\x1b[m");
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
int enemyAttack(Monster* m) {
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
    switch((*m).type) {
        case FIRE:
            printf("[%sのターン]\n[\x1b[31m~%s~\033[mの攻撃!%.0fのダメージを受けた\n\n\n", (*m).name, (*m).name, damage);
            break;
        case WATER:
            printf("[%sのターン]\n[\x1b[34m~%s~\033[mの攻撃!%.0fのダメージを受けた\n\n\n", (*m).name, (*m).name, damage);
            break;
        case WIND:
            printf("[%sのターン]\n[\x1b[36m~%s~\033[mの攻撃!%.0fのダメージを受けた\n\n\n", (*m).name, (*m).name, damage);
            break;
        case EARTH:
            printf("[%sのターン]\n[\x1b[33m~%s~\033[mの攻撃!%.0fのダメージを受けた\n\n\n", (*m).name, (*m).name, damage);
            break;
        default:
            break;
    }
    if(partyHp <= 0) {
        return 1;
    } else {
        return 0;
    }
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
    // printf("attack: %d\n", attack);
    int defence = (*enemy).defence;
    // printf("defence: %d\n", defence);
    int diff = attack - defence;
    // printf("diff: %d\n", diff);
    double revision = judgeMonstersType(ally, enemy);
    // printf("revision: %f\n", revision);
    int rondomNumber = rand() % 21;
    // printf("rondom: %d\n", rondomNumber);
    double swingWidth = ((double) (rondomNumber - 10) * 0.01) + 1.0;
    // printf("swingwidth: %f\n", swingWidth);
    double power = pow(1.5, (double) (igems - 3 + combo));
    // printf("power: %f\n", power);
    double damage = (double) (diff) * revision * power * swingWidth;
    if(damage <= 0.0) {
        damage = 1.0;
    }
    printf("%sに%.0fのダメージ！\n\n", (*enemy).name, damage);
    return damage;
}

void healHp(int igems, int combo) {
    int rondomNumber = rand() % 21;
    double swingWidth = ((double) (rondomNumber - 10) * 0.01) + 1.0;
    double power = pow(1.5, (double) (igems - 3 + combo));
    double healPower = 20.0 * power * swingWidth;
    partyHp += healPower;
    if(partyHp > 600) {
        partyHp = 600;
    }
    printf("%.0fHP回復した！\n\n", healPower);
}


// パズルを評価する
int judgeGems(int i, int combo) {
    switch(gems[i]) {
        case FIRE:
            if(combo > 1) {
                printf("\x1b[31m$朱雀$\033[mの攻撃！ %d COMBO!\n", combo);
                return 0;
            } else {
                printf("\x1b[31m$朱雀$\033[mの攻撃！ \n");
                return 0;
            }
            break;      
        case WATER:
            if(combo > 1) {
                printf("\x1b[34m~玄武~\033[mの攻撃！ %d COMBO!\n", combo);
                return 3;
            } else {
                printf("\x1b[34m~玄武~\033[mの攻撃！ \n");
                return 3;
            }
            break;
        case WIND:
            if(combo > 1) {
                printf("\x1b[36m@青龍@\033[mの攻撃！ %d COMBO!\n", combo);
                return 1;
            } else {
                printf("\x1b[36m@青龍@\033[mの攻撃！ \n");
                return 1;
            }
            break;
        case EARTH:
            if(combo > 1) {
                printf("\x1b[33m#白虎#\033[mの攻撃！ %d COMBO!\n", combo);
                return 2;
            } else {
                printf("\x1b[33m#白虎#\033[mの攻撃！ \n");
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
int decreaseEnemyHp(Monster* enemy, double damage) {
    if(enemy == &suraimu) {
        suraimuHp -= damage;
        printf("%f\n", damage);
        printf("%f\n", suraimuHp);
        if(suraimuHp <= 0.0) {
            printf("0だよ");
            return 0;
        } else {
            printf("1だよ");
            return 1;
        }
    } else if(enemy == &goburin) {
        goburinHp -= damage;
        printf("%d\n", goburinHp);
        if(goburinHp <= 0.0) {
            return 0;
        } else {
            return 1;
        }
    } else if(enemy == &ookomori) {
        ookomoriHp -= damage;
        if(ookomoriHp <= 0.0) {
            return 0;
        } else {
            return 1;
        }
    } else if(enemy == &weawolf) {
        weawolfHp -= damage;
        if(weawolfHp <= 0.0) {
            return 0;
        } else {
            return 1;
        }
    } else if(enemy == &doragon) {
        doragonHp -= damage;
        if(doragonHp <= 0.0) {
            return 0;
        } else {
            return 1;
        }
    }
}

int judgeAndJustyfyGems(Monster* enemy) {
    srand((unsigned int)time(NULL));
    int combo = 0;
    int count = 0;
    int allyId;
    double damage;
    int enemyDied = 1;
    int stateOfEnemy = 1;

    for(int i = 0; i < 12; i++) {
        // 5個消し
        if(memcmp(&gems[i], &gems[i + 1], 1) == 0 && memcmp(&gems[i + 1], &gems[i + 2], 1) == 0 && memcmp(&gems[i + 2], &gems[i + 3], 1) == 0 && memcmp(&gems[i + 3], &gems[i + 4], 1) == 0) {
            combo++;
            printf("コンボ数: %d\n", combo);
            allyId = judgeGems(i, combo);
            if(allyId == 4) {
                healHp(5, combo);
            }
            damage = allyAttack(allyId, enemy, 5, combo);
            enemyDied = decreaseEnemyHp(enemy, damage);
            printGems();
            printf("\n");
            for(int j = 4; j > -1; j--) {
                for(int k = 0; k < 14 - (i + j); k++) {
                    gems[(i + j) + k] = gems[(i + j) + (k + 1)];
                }
                int gemType = rand() % 5;
                gems[13] = gemType;
                printGems();
                printf("\n");
            }

            printf("enemyDied: %d", enemyDied);
            if(enemyDied == 0) {
                printf("0を返したぞ");
                stateOfEnemy = 0;
                break;
            } else {
                printf("judgeする\n");
                judgeAndJustyfyGems(enemy);
            }
    
        // 4個消し
        } else if(memcmp(&gems[i], &gems[i + 1], 1) == 0 && memcmp(&gems[i + 1], &gems[i + 2], 1) == 0 && memcmp(&gems[i + 2], &gems[i + 3], 1) == 0){
            combo++;
            printf("コンボ数: %d\n", combo);
            allyId = judgeGems(i, combo);
            if(allyId == 4) {
                healHp(4, combo);
            }
            damage = allyAttack(allyId, enemy, 4, combo);
            enemyDied = decreaseEnemyHp(enemy, damage);
            printGems();
            printf("\n");
            for(int j = 3; j > -1; j--) {
                for(int k = 0; k < 14 - (i + j); k++) {
                    gems[(i + j) + k] = gems[(i + j) + (k + 1)];
                }
                int gemType = rand() % 5;
                gems[13] = gemType;
                printGems();
                printf("\n");
            }
            printf("enemyDied: %d", enemyDied);
            if(enemyDied == 0) {
                printf("0を返したぞ");
                stateOfEnemy = 0;
                break;
            } else {
                printf("judgeする\n");
                judgeAndJustyfyGems(enemy);
            }
        // 3個消し
        } else if(memcmp(&gems[i], &gems[i + 1], 1) == 0 && memcmp(&gems[i + 1], &gems[i + 2], 1) == 0) {
            combo++;
            printf("コンボ数: %d\n", combo);
            allyId = judgeGems(i, combo);
            if(allyId == 4) {
                healHp(3, combo);
            }
            damage = allyAttack(allyId, enemy, 3, combo);
            enemyDied = decreaseEnemyHp(enemy, damage);
            printGems();
            printf("\n");
            for(int j = 2; j > -1; j--) {
                for(int k = 0; k < 14 - (i + j); k++) {
                    gems[(i + j) + k] = gems[(i + j) + (k + 1)];
                }
                int gemType = rand() % 5;
                gems[13] = gemType;
                printGems();
                printf("\n");
            }
            printf("enemyDied: %d", enemyDied);
            if(enemyDied == 0) {
                printf("0を返したぞ");
                stateOfEnemy = 0;
                break;
            } else {
                printf("judgeする\n");
                judgeAndJustyfyGems(enemy);
            }
        } 
    }

    printf("enemy: %d\n", stateOfEnemy);
    if(stateOfEnemy == 0) {
        return 0;
    } else {
        return 1;
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
            printf("\n");
            for(int i = 0; i < diff_index; i++) {
                int s = gems[index_cmd1 + i];
                gems[index_cmd1 + i] = gems[index_cmd1 + i + 1];
                gems[index_cmd1 + i + 1] = s;
                printGems();
                printf("\n");
            }
        } else {
            diff_index = index_cmd1 - index_cmd2;
            printGems();
            printf("\n");
            for(int i = 0; i < diff_index; i++) {
                int t = gems[index_cmd2 + i];
                gems[index_cmd2 + i] = gems[index_cmd2 + i + 1];
                gems[index_cmd2 + i + 1] = t;
                printGems();
                printf("\n");
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

// enemy死 => 0 party死 => 1 enemyもpartyも生きてる => 続行
int oneTurnOfButtle(Monster *m) {
    int enemyMaxHp = (*m).hp;
    int enemyHp = enemyCurrentHp(m);
    int enemyDied;
    int partyDied;
    printf("[%sのターン]", playerName);
    printf("------------------------------\n");
    switch((*m).type) {
        case FIRE:
            printf("\x1b[31m$%s$\033[m\nHP= %d / %d\n\n\n\n", (*m).name, enemyHp, enemyMaxHp);
            break;
        case WATER:
            printf("\x1b[34m~%s~\033[m\nHP= %d / %d\n\n\n\n", (*m).name, enemyHp, enemyMaxHp);
            break;
        case WIND:
            printf("\x1b[36m@%s@\033[m\nHP= %d / %d\n\n\n\n", (*m).name, enemyHp, enemyMaxHp);
            break;
        case EARTH:
            printf("\x1b[33m#%s#\033[m\nHP= %d / %d\n\n\n\n", (*m).name, enemyHp, enemyMaxHp);
            break;
        default:
            break;
    }
    printf("\x1b[31m$朱雀$\033[m \x1b[36m@青龍@\033[m \x1b[33m#白虎#\033[m \x1b[34m~玄武~\033[m\n");
    printf("HP= %.0f / %d\n", partyHp, 600);
    printf("------------------------------\n");
    printf("A B C D E F G H I J K L M N\n");
    printGems();
    printf("------------------------------\n");
    moveGem();
    enemyDied = judgeAndJustyfyGems(m);
    printf("%dだぷ\n", enemyDied);
    if(enemyDied != 0) {
        partyDied = enemyAttack(m);
        if(partyDied != 0) {
            return 1;
        } else {
            oneTurnOfButtle(m);
        }
    } else if(enemyDied == 0) {
        printf("main関数に0を返した\n");
        return 0;
    }
}


int main(int argc, char** argv) {
    int partyDied;
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

    printf("\x1b[34m~%s~\033[mが現れた！\n\n\n", (*&suraimu).name);
    partyDied = oneTurnOfButtle(&suraimu);
    if(partyDied != 0) {
        printf("***GAME OVER***\n倒したモンスター数=%d\n", 0);
        return 1;
    }
    printf("\x1b[34m~%s~\033[mに勝利した！\n\n", (*&suraimu).name);
    printf("\x1b[33m#%s#\033[mが現れた！\n\n\n", (*&goburin).name);
    partyDied = oneTurnOfButtle(&goburin);
    if(partyDied != 0) {
        printf("***GAME OVER***\n倒したモンスター数=%d\n", 1);
        return 1;
    }
    printf("\x1b[33m#%s#\033[mに勝利した！\n\n", (*&goburin).name);
    printf("\x1b[36m@%s@\033[mが現れた！\n\n\n", (*&ookomori).name);
    partyDied = oneTurnOfButtle(&ookomori);
    if(partyDied != 0) {
        printf("***GAME OVER***\n倒したモンスター数=%d\n", 2);
        return 1;
    }
    printf("\x1b[36m@%s@\033[mに勝利した！\n\n", (*&ookomori).name);
    printf("\x1b[36m@%s@\033[mが現れた！\n\n\n", (*&weawolf).name);
    partyDied = oneTurnOfButtle(&weawolf);
    if(partyDied != 0) {
        printf("***GAME OVER***\n倒したモンスター数=%d\n", 3);
        return 1;
    }
    printf("\x1b[36m@%s@\033[mに勝利した！\n\n", (*&weawolf).name);
    printf("\x1b[31m$%s$\033[mが現れた！\n\n\n", (*&doragon).name);
    partyDied = oneTurnOfButtle(&doragon);
    if(partyDied != 0) {
        printf("***GAME OVER***\n倒したモンスター数=%d\n", 4);
        return 1;
    }
    printf("\x1b[31m$%s$\033[mに勝利した！\n\n", (*&doragon).name);
    return 0;
}