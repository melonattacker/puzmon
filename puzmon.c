#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// グローバル変数
// リテラルを用いることで静的領域にメモリを確保
char* gems[14] = {"#", "#", "#", "#", "@", "@", "@", "@", "~", "~", "~", "~", "&", "&"};
char* playerName;
int partyHp = 600;
int partyDefence;

// 構造体宣言
typedef struct {
    char* name;
    int hp;
    int maxHp;
    char* type;
    int attack;
    int defence;
} Monster;

void printMonsterSummary(Monster* m) {
    printf("$%s$ HP=%d 攻撃=%d 防御=%d\n", (*m).name, (*m).hp, (*m).attack, (*m).defence);
}

void rondomShuffleGems(char** gems) {
    srand((unsigned int)time(NULL));
    for(int i = 0; i < 14; i++) {
        int j = rand() % 14;
        char* t = gems[i];
        gems[i] = gems[j];
        gems[j] = t;
    }
    printf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s\n", gems[0], gems[1], gems[2], gems[3], gems[4], gems[5], gems[6], gems[7], gems[8], gems[9], gems[10], gems[11], gems[12], gems[13]);
    printf("------------------------------\n");
}

int askNumberOfLetter(char* letter) {
    enum { A, B, C, D, E, F, G, H, I, J, K, L, M, N };

    switch(letter) {
        case A:
            return 0;
        case B:
            return 1;
        case C:
            return 2;
        case D:
            return 3;
        case E:
            return 4;
        case F:
            return 5;
        case G:
            return 6;
        case H:
            return 7;
        case I:
            return 8;
        case J:
            return 9;
        case K:
            return 10;
        case L:
            return 11;
        case M:
            return 12;
        case N:
            return 13;
        default:
            break;
    }
}

void moveGem() {
    char command[3];

    printf("コマンド？>");
    scanf("%s", command);

    if(strlen(command) == 2) {
        printf("%c", command[0]);
        printf("%c", command[1]);
    } else {
        printf("コマンドはA~Nまでの大文字2文字を入力してください (ex) AD\n");
        moveGem();
    }
}

int battleWithMonster(Monster* m) {
    int enemyMaxHp = (*m).hp;
    int enemyCurrentHp = (*m).hp;
    printf("~%s~が現れた！\n\n\n", (*m).name);
    printf("[%sのターン]", playerName);
    printf("------------------------------\n");
    printf("~%s~\nHP= %d / %d\n\n\n\n", (*m).name, enemyCurrentHp, enemyMaxHp);
    printf("$朱雀$ @青龍@ #白虎# ~玄武~\n");
    printf("HP= %d / %d\n", partyHp, 600);
    printf("------------------------------\n");
    printf("A B C D E F G H I J K L M N\n");
    rondomShuffleGems(&gems[0]);
    moveGem();
}

int main(int argc, char** argv) {
    playerName = *(argv + 1);
    printf("*** Puzzle & Monsters ***\n");
    printf("%sのパーティ(HP=600)はダンジョンに到達した\n", playerName);
    printf("<パーティ編成>----------\n");

    Monster suzaku = {"朱雀", 150, 150, "火", 25, 10};
    Monster seiryu = {"青龍", 150, 150, "風", 15, 10};
    Monster byakko = {"白虎", 150, 150, "土", 20, 5};
    Monster genbu = {"玄武", 150, 150, "土", 20, 15};

    printMonsterSummary(&suzaku);
    printMonsterSummary(&seiryu);
    printMonsterSummary(&byakko);
    printMonsterSummary(&genbu);

    printf("------------------------\n\n\n");

    Monster suraimu = {"スライム", 100, 100, "水", 10, 5};
    Monster goburin = {"ゴブリン", 200, 200, "土", 20, 15};
    Monster ookomori = {"オオコウモリ", 300, 300, "風", 30, 25};
    Monster weawolf = {"ウェアウルフ", 400, 400, "風", 40, 30};
    Monster doragon = {"ドラゴン", 800, 800, "火", 50, 40};

    battleWithMonster(&suraimu);
    return 0;
}