#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define RED "\033[91m"
#define GREEN "\033[92m"
#define YELLOW "\033[93m"
#define BLUE "\033[94m"
#define PURPLE "\033[95m"
#define CYAN "\033[96m"
#define RED_BOLD "\033[1;91m"
#define GREEN_BOLD "\033[1;92m"
#define YELLOW_BOLD "\033[1;93m"
#define BLUE_BOLD "\033[1;94m"
#define PURPLE_BOLD "\033[1;95m"
#define CYAN_BOLD "\033[1;96m"
#define RESET "\033[m"
#define _NEW_PAGE_ printf("%s",clear)

typedef struct {
    uint16_t ym;
    uint16_t exp;
    uint16_t mi;
    uint8_t lifespan;
    uint8_t active;
    uint8_t mi_force;
    uint8_t intellegence;
    uint8_t politic;
    uint8_t charm;
    uint8_t train;
    uint8_t morale;
    uint8_t none02;
    uint8_t loyalty;
    uint32_t none03;
    uint16_t none04;
    uint8_t none05;
    uint32_t skill;
}__attribute__ ((packed)) sCharacter;

typedef struct {
    uint32_t ready;
    uint64_t none01;
    uint16_t population;
    uint16_t defense;
    uint16_t none02;
    uint16_t explore;
    uint16_t business;
    uint16_t none03;
    uint8_t none04;
    uint8_t water;
    uint8_t loyalty;
}__attribute__ ((packed)) sCity;

char clear[12] = {0x1b, 0x5b, 0x48, 0x1b, 0x5b, 0x32, 0x4a, 0x1b, 0x5b, 0x33, 0x4a, 0x00};
uint8_t end[18] = {0,0,0,0,0,0,0,0,0,0,0,0xFF,0,0xFF,0xFF,0x64,0xFF,0xFF};

void monarch(uint8_t* savedata, int32_t nowIdx, uint64_t filesize);
void city(uint8_t* savedata, int32_t nowIdx, uint64_t filesize);
void general(uint8_t* savedata, int32_t nowIdx, uint64_t filesize, int32_t year);
uint32_t skill_finder(char* str);
uint16_t formation_finder(char* str);
void print_skill(uint32_t skill);
void print_formation(uint16_t formation);

void monarch(uint8_t* savedata, int32_t nowIdx, uint64_t filesize){

    uint16_t prestige = 0;
    uint16_t gold = 0;
    uint16_t food = 0;
    int32_t foundIdx = 0;

    while(1){

        int32_t isFound = 0;
        uint32_t prefixZero = 0;

        printf("\nPlease enter the following value of the target monarch according to the given format\n");
        printf("(format: ?????? ??? ??????)\n");
        printf("Type at here -> ");
        scanf("%hu %hu %hu",&prestige,&gold,&food);

        for(int32_t i = nowIdx;i < filesize - 12;i++){
            if(memcmp(&savedata[i],&prefixZero,sizeof(prefixZero)) == 0 && memcmp(&savedata[i+8],&gold,sizeof(gold)) == 0 && memcmp(&savedata[i+12],&food,sizeof(food)) == 0){
                foundIdx = i + 4;
                isFound = 1;
                break;
            }
            if(memcmp(&savedata[i],end,18) == 0) break;
        }
        if(isFound){
            _NEW_PAGE_;
            printf(GREEN_BOLD"Success!\n"RESET);
            break;
        }
        else{
            _NEW_PAGE_;
            printf(RED"Error: No such monarch\n"RESET);
        }
    }

    _NEW_PAGE_;
    printf("\nSearch result:\n");

    while(1){

        int32_t choice = 0;
        int32_t isEnd = 0;
        double addition = 0;

        addition = prestige - (savedata[foundIdx] + savedata[foundIdx+1] * 256) * 0.7;

        printf("\n");
        printf("(1) ??????: %hu\n",prestige);
        printf("(2) ???: %hu\n",gold);
        printf("(3) ??????: %hu\n",food);
        printf("(0) ??????\n");
        sleep(1);
        printf("\n");
        printf("Please choose the value you want to edit: ");
        scanf("%d",&choice);

        if(choice < 0 || choice > 3){
            _NEW_PAGE_;
            printf(RED"Error: Undefined behavior\n"RESET);
            continue;
        }

        if(choice == 0) break;
        if(choice == 1) printf(RED_BOLD"Warning: There might exist potential deviation from the actual value in the game\n"RESET);

        uint16_t modified = 0;

        printf("Please enter the modified value: ");
        scanf("%hd",&modified);

        if(choice == 1){
            prestige = modified;
            modified = (uint16_t)((modified - addition) * 10 / 7); 
            memcpy(&savedata[foundIdx],&modified,sizeof(modified));
        }
        if(choice == 2){
            gold = modified;
            memcpy(&savedata[foundIdx+4],&modified,sizeof(modified));
        }
        if(choice == 3){
            food = modified;
            memcpy(&savedata[foundIdx+8],&modified,sizeof(modified));
        }

        _NEW_PAGE_;
        printf(GREEN_BOLD"Success!\n"RESET);

        if(isEnd) break;
    }

    return;
}

void city(uint8_t* savedata, int32_t nowIdx, uint64_t filesize){
    uint16_t defense = 0;
    uint16_t explore = 0;
    uint16_t business = 0;
    uint8_t water = 0;
    int32_t foundIdx = 0;

    while(1){

        printf("\nPlease enter the followg value of the target city accroding to the given format\n");
        printf("(format: ?????? ?????? ?????? ??????)\n");
        printf("Type at here -> ");
        scanf("%hu %hu %hu %hhu",&defense,&explore,&business,&water);

        int32_t isFound = 0;

        for(int32_t i = nowIdx;i < filesize - 11;i++){
            if(memcmp(&savedata[i],&defense,sizeof(defense)) == 0 && memcmp(&savedata[i+4],&explore,sizeof(explore)) == 0 && memcmp(&savedata[i+6],&business,sizeof(business)) == 0 && savedata[i+11] == water){
                foundIdx = i - 14;
                isFound = 1;
                break;
            }
            if(memcmp(&savedata[i],end,18) == 0) break;
        }
        if(isFound){
            _NEW_PAGE_;
            printf(GREEN_BOLD"Success!\n"RESET);
            break;
        }
        else{
            _NEW_PAGE_;
            printf(RED"Error: No such city\n"RESET);
        }
    }

    _NEW_PAGE_;
    printf("\nSearch result:\n");

    while(1){

        sCity ct = {0};
        int32_t choice = 0;
        int32_t isEnd = 0;

        memcpy(&ct,&savedata[foundIdx],sizeof(ct));

        printf("\n");
        printf("(1) ??????: %u\t(5) ??????: %hu\n",(uint32_t)ct.population * 100,ct.explore);
        printf("(2) ????????????: %hhu\t(6) ??????: %hhu\n",ct.loyalty,ct.water);
        printf("(3) ??????: --\t\t(7) ??????: %hu\n",ct.business);
        printf("(4) ?????????: %u\t\t(8) ??????: %hu\n",ct.ready,ct.defense);
        printf("(0) ??????\n");
        sleep(1);
        printf("\n");
        printf("Please choose the value you want to edit: ");
        scanf("%d",&choice);

        if(choice < 0 || choice > 8){
            _NEW_PAGE_;
            printf(RED"Error: Undefined behavior\n"RESET);
            continue;
        }

        if(choice == 0) break;
        if(choice == 3){
            _NEW_PAGE_;
            printf(BLUE"Sorry, this function hasn't been completed yet :(\n"RESET);
            continue;
        }

        uint8_t modified8 = 0;
        uint16_t modified16 = 0;
        uint32_t modified32 = 0;

        printf("Please enter the modified value: ");

        switch(choice){
            case 1:
            case 4:
                scanf("%d",&modified32);
                if(choice == 1){
                    modified16 =(uint16_t)(modified32 / 100);
                    memcpy(&savedata[foundIdx+12],&modified16,sizeof(modified16));
                }
                if(choice == 4) memcpy(&savedata[foundIdx],&modified32,sizeof(modified32));

                _NEW_PAGE_;
                printf(GREEN_BOLD"Success!\n"RESET);
                break;
            case 2:
            case 6:
                scanf("%hhu",&modified8);

                if(choice == 2) memcpy(&savedata[foundIdx+26],&modified8,sizeof(modified8));
                if(choice == 6) memcpy(&savedata[foundIdx+25],&modified8,sizeof(modified8));

                _NEW_PAGE_;
                printf(GREEN_BOLD"Success!\n"RESET);
                break;
            default:
                scanf("%hu",&modified16);

                if(choice == 1){
                    modified16 /= 100;
                    memcpy(&savedata[foundIdx+12],&modified16,sizeof(modified16));
                }
                if(choice == 5) memcpy(&savedata[foundIdx+18],&modified16,sizeof(modified16));
                if(choice == 7) memcpy(&savedata[foundIdx+20],&modified16,sizeof(modified16));
                if(choice == 8) memcpy(&savedata[foundIdx+14],&modified16,sizeof(modified16));
                
                _NEW_PAGE_;
                printf(GREEN_BOLD"Success!\n"RESET);
                break;
        }

        if(isEnd) break;
    }

    return;
}

void general(uint8_t* savedata, int32_t nowIdx, uint64_t filesize, int32_t year){

    uint16_t search[3] = {0};
    uint8_t age = 0;
    uint16_t formation = 0;
    int32_t formerIdx = 0;
    int32_t latterIdx = 0;
    int32_t isActive = 0;

    while(1){
        printf("\nPlease enter the following value of the target character according to the given format\n");
        printf("(format: ?????? ?????? ????????? ?????? ?????? (0->??? 1->???))\n");
        printf("Type at here -> ");
        scanf("%hu %hu %hu %hhu %d",&search[0],&search[1],&search[2],&age,&isActive);

        formation = formation_finder("Please enter the target character's formation (use 0 as the end of the input and space as separator): ");

        formerIdx = 0;
        latterIdx = 0;
        int32_t isFound = 0;

        for(int32_t i = nowIdx;i < filesize - 16;i++){
            if(memcmp(&savedata[i],search,sizeof(search)) == 0 && formerIdx == 0){
                formerIdx = i - 4;
                isFound++;
            }
            if(savedata[i] == (year - 99 - age) % 256 && savedata[i+15] == formation % 256 && savedata[i+16] == formation / 256 && latterIdx == 0){
                latterIdx = i - 1;
                isFound++;
                break;           
            }
            if(memcmp(&savedata[i],end,18) == 0) break;
        }

        if(isFound == 2) break;
        else{
            _NEW_PAGE_;
            printf(RED"Error: No such character\n"RESET);
        }
    }

    _NEW_PAGE_;
    printf("\nSearch result:\n");

    while(1){

        sCharacter person = {0};
        int32_t choice = 0;
        int32_t isEnd = 0;

        memcpy(&person,&savedata[formerIdx+4],sizeof(person));
        formation = savedata[latterIdx+16] + savedata[latterIdx+17] * 256;
        age = year - 99 - savedata[latterIdx+1];

        printf("\n");
        printf("(1) ??????: %hhu\t(8)  ??????: %hhu\t(13) ?????????: %hu\n",age,person.mi_force,person.mi);
        printf("(2) ??????: %hhu\t(9)  ??????: %hhu\t(14) ?????????: --\n",person.loyalty,person.intellegence);
        printf("(3) ??????: --\t(10) ??????: %hhu\t(15) ??????: %hhu\n",person.politic,person.train);
        printf("(4) ??????: %s\t(11) ??????: %hhu\t(16) ??????: %hhu\n",(isActive) ? "???" : "???",person.charm,person.morale);
        printf("(5) ??????: %hu\t(12) ??????: %hu\n",person.ym,person.exp);
        printf("(6) ??????: ");
        print_skill(person.skill);
        printf("(7) ??????: ");
        print_formation(formation);
        printf("(0) ??????\n");
        sleep(1);
        printf("\n");
        printf("Please choose the value you want to edit: ");
        scanf("%d",&choice);

        if(choice < 0 || choice > 16){
            _NEW_PAGE_;
            printf(RED"Error: Undefined behavior\n"RESET);
            continue;
        }

        uint8_t modified8 = 0;
        uint16_t modified16 = 0;
        uint32_t modified32 = 0;
        
        switch(choice){
            case 0:
                isEnd = 1;
                break;
            case 3:
            case 14:
                _NEW_PAGE_;
                printf(BLUE"Sorry, this function hasn't been completed yet :(\n"RESET);
                break;
            case 6:
                modified32 = skill_finder("Please enter all the skills after modified (use 0 as the end of the input and space as separator): ");
                memcpy(&savedata[formerIdx+27],&modified32,sizeof(modified32));
                _NEW_PAGE_;
                printf(GREEN_BOLD"Success!\n"RESET);
                break;
            case 7:
                modified16 = formation_finder("Please enter all the formations after modified (use 0 as the end of the input and space as separator): ");
                memcpy(&savedata[latterIdx+16],&modified16,sizeof(modified16));
                _NEW_PAGE_;
                printf(GREEN_BOLD"Success!\n"RESET);
                break;
            case 1:
            case 5:
            case 12:
            case 13:
                printf("Please enter the modified value: ");
                scanf("%hd",&modified16);

                if(choice == 1){
                    modified16 = year - 99 - modified16;
                    memcpy(&savedata[latterIdx+1],&modified16,sizeof(modified16));
                }
                if(choice == 5) memcpy(&savedata[formerIdx+4],&modified16,sizeof(modified16));
                if(choice == 12) memcpy(&savedata[formerIdx+6],&modified16,sizeof(modified16));
                if(choice == 13) memcpy(&savedata[formerIdx+8],&modified16,sizeof(modified16));

                _NEW_PAGE_;
                printf(GREEN_BOLD"Success!\n"RESET);
                break;
            case 4:
                if(isActive == 0){
                    isActive = 1;
                    savedata[formerIdx+11] += 4;
                }
                else{
                    isActive = 0;
                    savedata[formerIdx+11] -= 4;
                }
                
                _NEW_PAGE_;
                printf(GREEN_BOLD"Success!\n"RESET);
                break;
            default:
                printf("Please enter the modified value: ");
                scanf("%hhd",&modified8);

                if(choice == 2) memcpy(&savedata[formerIdx+19],&modified8,sizeof(modified8));
                if(choice == 4) memcpy(&savedata[formerIdx+11],&modified8,sizeof(modified8));
                if(choice == 8) memcpy(&savedata[formerIdx+12],&modified8,sizeof(modified8));
                if(choice == 9) memcpy(&savedata[formerIdx+13],&modified8,sizeof(modified8));
                if(choice == 10) memcpy(&savedata[formerIdx+14],&modified8,sizeof(modified8));
                if(choice == 11) memcpy(&savedata[formerIdx+15],&modified8,sizeof(modified8));
                if(choice == 15) memcpy(&savedata[formerIdx+16],&modified8,sizeof(modified8));
                if(choice == 16) memcpy(&savedata[formerIdx+17],&modified8,sizeof(modified8));
                
                _NEW_PAGE_;
                printf(GREEN_BOLD"Success!\n"RESET);
                break;
        }
        
        if(isEnd) break;
    }

    return;
}

uint32_t skill_finder(char* str){
    
    uint32_t ans = 0;
    int32_t input = 0;

    printf("\n");
    printf("(1)  ??????   (2)  ??????   (3)  ??????   (4)  ??????\n");
    printf("(5)  ??????   (6)  ??????   (7)  ??????   (8)  ??????\n");
    printf("(9)  ??????   (10) ??????   (11) ??????   (12) ??????\n");
    printf("(13) ??????   (14) ??????   (15) ??????   (16) ??????\n");
    printf("(17) ??????   (18) ??????   (19) ??????   (20) ??????\n");
    printf("(21) ??????   (22) ??????   (23) ??????   (24) ??????\n");
    printf("(25) ??????   (26) ??????   (27) ??????   (28) ??????\n");
    printf("(29) ??????   (30) ??????   (31) ??????   (32) ??????\n");
    printf("\n");
    printf("%s",str);
    do{
        scanf("%d",&input);

        if(input > 0 && input < 33) ans = ans | 0x0001 << (input-1); 

    }while(input != 0);

    return ans;

}

uint16_t formation_finder(char* str){

    uint16_t ans = 1;
    int32_t input = 0;

    printf("\n");
    printf("(1) ??????    (2)  ??????   (3)  ??????   (4)  ??????\n");
    printf("(5) ??????    (6)  ??????   (7)  ??????   (8)  ??????\n");
    printf("(9) ??????    (10) ??????   (11) ??????   (12) ??????\n");
    printf("\n");
    printf("%s",str);
    do{
        scanf("%d",&input);

        if(input > 0 && input < 13) ans = ans | 0x0001 << input; 

    }while(input != 0);
    
    return ans;
}

void print_skill(uint32_t skill){

    for(int32_t i = 0;i < 32;i++){
        if(skill & 1){
            if(i == 0) printf("?????? ");
            if(i == 1) printf("?????? ");
            if(i == 2) printf("?????? ");
            if(i == 3) printf("?????? ");
            if(i == 4) printf("?????? ");
            if(i == 5) printf("?????? ");
            if(i == 6) printf("?????? ");
            if(i == 7) printf("?????? ");
            if(i == 8) printf("?????? ");
            if(i == 9) printf("?????? ");
            if(i == 10) printf("?????? ");
            if(i == 11) printf("?????? ");
            if(i == 12) printf("?????? ");
            if(i == 13) printf("?????? ");
            if(i == 14) printf("?????? ");
            if(i == 15) printf("?????? ");
            if(i == 16) printf("?????? ");
            if(i == 17) printf("?????? ");
            if(i == 18) printf("?????? ");
            if(i == 19) printf("?????? ");
            if(i == 20) printf("?????? ");
            if(i == 21) printf("?????? ");
            if(i == 22) printf("?????? ");
            if(i == 23) printf("?????? ");
            if(i == 24) printf("?????? ");
            if(i == 25) printf("?????? ");
            if(i == 26) printf("?????? ");
            if(i == 27) printf("?????? ");
            if(i == 28) printf("?????? ");
            if(i == 29) printf("?????? ");
            if(i == 30) printf("?????? ");
            if(i == 31) printf("?????? ");  
        }
        skill >>= 1;
    }
    printf("\n");
    return;
}

void print_formation(uint16_t formation){
    formation >>= 1;
    for(int32_t i = 0;i < 12;i++){
        if(formation & 1){
            if(i == 0) printf("?????? ");
            if(i == 1) printf("?????? ");
            if(i == 2) printf("?????? ");
            if(i == 3) printf("?????? ");
            if(i == 4) printf("?????? ");
            if(i == 5) printf("?????? ");
            if(i == 6) printf("?????? ");
            if(i == 7) printf("?????? ");
            if(i == 8) printf("?????? ");
            if(i == 9) printf("?????? ");
            if(i == 10) printf("?????? ");
            if(i == 11) printf("?????? ");
        }
        formation >>= 1;
    }
    printf("\n");
    return;
}

int main(){
    
    int32_t fd = -1;
    int32_t nowIdx = 0;
    int32_t year = 0;
    int32_t month = 0;
    int32_t script = 0;
    uint64_t filesize = 0;
    char filename[32] = "";
    uint8_t* savedata = NULL;
    struct stat filestat = {0};

    _NEW_PAGE_;

    printf(CYAN_BOLD"Welcome to SAN5PK Savedata Editor!\n"RESET);
    printf(RED"The editor is encoded in UTF-8. Make sure you are using it too.\n"RESET);

    reload_file:

    while(1){
        printf("\nPlease enter the file name (It should be \"SAVEDATA.S5P\"): ");

        if(fgets(filename,32,stdin) != NULL){
            filename[strlen(filename) - 1] = 0;
            if(strncmp(&filename[strlen(filename) - 4],".S5P",4) == 0){
                fd = open(filename, O_RDWR);
                
                if(fd < 0) printf(RED"Error: No such file or directory\n"RESET);
                else break;
            }
            else printf(BLUE"Warning: The file extension is not \".S5P\"\n"RESET);
        }
        else printf(RED"Error!\n"RESET);
    }

    fd = open("SAVEDATA.S5P",O_RDWR);

    fstat(fd,&filestat);
    filesize = filestat.st_size;
    savedata = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    reload_savedata:

    while(1){

        printf("\nPlease enter the year, the month and the script number in the savedata (format: yyy mm s): ");
        scanf("%d %d %d",&year,&month,&script);

        int32_t isFound = 0;
        uint8_t search[8] = {0};
        search[5] = year % 256;
        search[6] = year / 256;
        search[7] = month;
        script %= 256;

        for(int32_t i = 0;i < filesize;i++){
            if(memcmp(&savedata[i],search,8) == 0 && savedata[i + 9] == script){
                nowIdx = i + 5;
                isFound = 1;
                break;
            }
        }
        if(isFound){
            _NEW_PAGE_;
            printf(GREEN_BOLD"Success!\n"RESET);
            break;
        }
        else{
            _NEW_PAGE_;
            printf(RED"Error: No such savedata\n"RESET);
        }
    }

    int32_t choice = 0;
    int32_t isEnd = 0;
    char garbage = 0;
    sleep(1);

    while(1){
        printf("\n");
        printf("You are now in ");
        printf(PURPLE"%d??? %d??? ??????%d\n"RESET,year,month,script);
        printf("(1) ??????????????????    (4) ??????????????????\n");
        printf("(2) ??????????????????    (5) ??????????????????\n");
        printf("(3) ??????????????????    (0) ??????\n");
        printf("\n");
        printf("Please choose your option: ");
        scanf("%d",&choice);

        switch(choice){
            case 1:
                _NEW_PAGE_;
                monarch(savedata, nowIdx, filesize);
                _NEW_PAGE_;
                break;
            case 2:
                _NEW_PAGE_;
                city(savedata, nowIdx, filesize);
                _NEW_PAGE_;
                break;
            case 3:
                _NEW_PAGE_;
                general(savedata, nowIdx, filesize, year);
                _NEW_PAGE_;
                break;
            case 4:
                _NEW_PAGE_;
                garbage = getchar();
                goto reload_savedata;
                break;
            case 5:
                _NEW_PAGE_;
                garbage = getchar();
                goto reload_file;
                break;
            case 0:
                printf(BLUE_BOLD"See you next time :)\n"RESET);
                isEnd = 1;
                sleep(1);
                break;
            default:
                _NEW_PAGE_;
                printf(RED"Error: Undefined behavior\n"RESET);
                break;
        }
        if(isEnd) break;
    }

    munmap(savedata, filesize);
    close(fd);

    return 0;
}