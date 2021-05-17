#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>

#define VAR 0
#define FUNC 1
#define NUM 2

struct option long_option[] = {
    {"help", 0, NULL, 'h'},
    {0, 0, 0, 0}
};

typedef struct {
    char origin[32];
    char modified[16];
    int32_t isOccupied;
} variable;

void get_line(char* str, FILE* pFile);
void random_space(FILE* pFile);
void set_name(char* str, variable* var, int32_t type);
int32_t isVar(char* str, int32_t idx, int32_t type);

void get_line(char* str, FILE* pFile){  //我們要先得到檔案裡的某一段字串，才能進行處理
                                        //直覺上會認為只要一直fread，每次一個byte，讀到\n就算結束了
                                        //但我們沒辦法保證會不會所有程式都在同一行，或是其他狀況，所以要思考其他字元來取代\n的功能(做為一行的結尾)
    
    char c = 0;
    int32_t idx = 0;

    memset(str,0,sizeof(str));  //str要準備存分析好的每一行
    
    do{

        fread(&c,sizeof(char),1,pFile);     //先讀先存，再來檢查是不是我們結尾會出現的字元
        str[idx++] = c;

    }while(c != ';' && c != '}' && c != '{' && c != '>' && !feof(pFile));   //這些就是做為代替\n的字元，!feof是為了避免segmentation fault

    if(str[0] == '#'){  //若今天讀到的是以#開頭(ex: #include <stdio.h>)，就要繼續讀，直到遇到非空格或換行的字元(像下一行的#)
                        //若不這樣做，後面就無法用str[0] == '#'來判斷
        
        do{

            fread(&c,sizeof(char),1,pFile);
            if(c == '\n' || c == ' ') str[idx++] = c;

        }while(c == '\n' || c == ' ');
        fseek(pFile,-1,SEEK_CUR);   //會多讀一個沒有要讀進str的字元，所以往回1byte

    }

    return;
}

void random_space(FILE* pFile){
    
    char space = ' ';
    char nl = '\n';
    int32_t times = rand() % 5 + 1;

    for(int32_t i = 0;i < times;i++){
        
        int32_t choice = rand() % 10;

        if(choice < 9) fwrite(&space,sizeof(char),1,pFile);
        else fwrite(&nl,sizeof(char),1,pFile);

    }

    return;
}

void set_name(char* str, variable* var, int32_t type){  //搜尋是否有宣告變數或函式，並儲存名稱，為其隨機產生一組名稱

    int32_t varIdx = 0;
    char tmp[32] = "";

    for(int32_t i = 0;i < 128;i++){ //找空的位置，找到了就把index存起來
        if(var[i].isOccupied == 0){
            varIdx = i;
            break;
        }
    }

    for(int32_t i = 0;i < strlen(str);i++){
        if(strncmp(&str[i],"int",3) == 0 || strncmp(&str[i],"char",4) == 0){    //遇到int或char代表可能要宣告變數了

            int32_t continueSearch = 0;
            int32_t strIdx = 0;

            memset(var[varIdx].origin,0,sizeof(var[varIdx].origin));    //Initialize
            memset(tmp,0,sizeof(tmp));
            
            if(type == VAR){

                for(int32_t j = i + 4;j < strlen(str);j++){     //讀int或char後面的字串，有可能是變數名稱
                    if(str[j] == ' ' || str[j] == '\n') continue;   //因為int和char後面可以塞一堆\n和空格再宣告，所以不用理他
                    if(str[j] == ';' || str[j] == '=' || str[j] == ',' || str[j] == ')') break; //遇到這些代表變數宣告結束
                    if(str[j] == '('){  //遇到這個代表這其實是在宣告函式
                        continueSearch = 1;
                        break;
                    }

                    tmp[strIdx++] = str[j];
                }

            }
            else{   //如果是要讀函式的名字

                for(int32_t j = i + 4;j < strlen(str);j++){
                    if(str[j] == ' ' || str[j] == '\n') continue;
                    if(str[j] == '(') break;
                    if(str[j] == ';' || str[j] == '=' || str[j] == ',' || str[j] == ')'){
                        continueSearch = 1;
                        break;
                    }
                    
                    tmp[strIdx++] = str[j];
                }

            }
            

            for(int32_t j = 0;j < varIdx;j++){
                if(strncmp(tmp,var[j].origin,strlen(tmp)) == 0){    //如果已經有重複的變數名稱就不必再處理，讓他繼續找
                    continueSearch = 1;
                    break;
                }
            }
            if(strncmp(tmp,"main",4) == 0) continueSearch = 1;  //若讀到main代表是main function，不能做更改

            if(continueSearch) continue;    //繼續搜尋可能的變數或函式名稱

            strncpy(var[varIdx].origin,tmp,strlen(tmp));    //存在struct array

            var[varIdx].modified[0] = rand() % 26 + 'a';    //隨機變數名稱的第一個不能為數字，所以是a-z隨機取一

            for(int32_t j = 1;j < 16;j++){  //隨機安排名稱
                int32_t choice = rand() % 62;

                if(choice < 10) var[varIdx].modified[j] = choice + '0';
                else if(choice < 36) var[varIdx].modified[j] = choice - 10 + 'A';
                else if(choice < 62) var[varIdx].modified[j] = choice - 36 + 'a';
            }

            var[varIdx].isOccupied = 1; //標記此位子已被使用
            varIdx++;   //移到下一個存名稱的位置
        }
    }

    return;
}

int32_t isVar(char* str, int32_t idx, int32_t type){

    if(type == VAR){

        for(int32_t i = idx;i < strlen(str);i++){
            if(str[i] == ' ' || str[i] == '\n') continue;
            if(str[i] == '=' || str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '%' || str[i] == '<' || str[i] == '>') return 1;
            if(str[i] == '&' || str[i] == '^' || str[i] == '|' || str[i] == ')' || str[i] == ',' || str[i] == ';' || str[i] == ':' || str[i] == '!') return 1;
            
            return 0;
        }

    }
    else if(type == FUNC){

        for(int32_t i = idx;i < strlen(str);i++){
            if(str[i] == ' ' || str[i] == '\n') continue;
            if(str[i] == '(') return 1;            
            return 0;
        }

    }
    else{

        for(int32_t i = idx;i >= 0;i--){
            if(str[i] == ' ' || str[i] == '\n') continue;
            if(str[i] == '=' || str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '%' || str[i] == '<' || str[i] == '>') return 1;
            if(str[i] == '&' || str[i] == '^' || str[i] == '|' || str[i] == '!' || str[i] == '(' || str[i] == ',') return 1;
            if(str[i] == 'e' && i - 3 >= 0){
                if(strncmp(&str[i - 3],"case",4) == 0) return 1;
            }
            if(str[i] == 'n' && i - 5 >= 0){
                if(strncmp(&str[i - 5],"return",6) == 0) return 1;
            }
            
            return 0;
        }

    }
}

int main(int argc, char *argv[]){

    int32_t c = 0;
    int32_t level = 0;
    int32_t opt_l = 0;
    int32_t opt_i = 0;
    int32_t opt_o = 0;
    int32_t opt_h = 0;
    char input_filename[32] = "";
    char output_filename[32] = "";

    srand(time(0));

    while((c = getopt_long(argc,argv,"l:i:o:h",long_option,NULL)) != -1){   //get option
        switch(c){
            case 'l':
                opt_l = 1;
                level = (int32_t)strtol(optarg,NULL,10);
                break;
            case 'i':
                opt_i = 1;
                strncpy(input_filename,optarg,strlen(optarg));
                break;
            case 'o':
                opt_o = 1;
                strncpy(output_filename,optarg,strlen(optarg));
                break;
            case 'h':
                opt_h = 1;
                break;
        }
    }

    if(opt_l && opt_i && opt_o){

        FILE* input_file = NULL;
        FILE* output_file = NULL;
        FILE* tmp_file = NULL;
        int32_t isString = 0;
        char str[1024] = "";
        variable var[128] = {0};

        if((input_file = fopen(input_filename,"rb")) == NULL){
            perror("Error");
            return 0;
        }
        if((output_file = fopen(output_filename,"wb+")) == NULL){
            perror("Error");
            return 0;
        }
        if((tmp_file = fopen("tmp.c","wb+")) == NULL){  //暫存檔
            perror("Error");
            return 0;
        }

        while((c = fgetc(input_file)) != -1) fputc(c,tmp_file); //先把input裡的內容全部複製到tmp.c，未來就讀取tmp.c裡的內容(為了不要破壞原檔案)
                                                                //每做完一個level，把output裡的內容複製到tmp.c，下一level就把tmp.c當input，才能累積修改的結果

        for(int32_t i = level;i >= 1;i--){
            rewind(tmp_file);
            rewind(output_file);
            switch(i){
                case 1:
                    memset(str,0,sizeof(str));
                    get_line(str,tmp_file); //因為feof的某些缺陷，需要先讀一次字串，再進入迴圈，每次迴圈結束再讀一次
                    while(!feof(tmp_file)){

                        if(str[0] == '#') fwrite(str,strlen(str),1,output_file);    //遇到以#開頭，就直接fwrite(因為我不想處理)

                        else{
                        
                            isString = 0;
                            for(int32_t j = 0;j < strlen(str);j++){ //讀每個字元

                                fwrite(&str[j],sizeof(char),1,output_file); //不管怎樣先fwrite
                                
                                if(str[j] == '\'') isString = !isString;    //判斷現在是否是字串並記錄
                                if(str[j] == '"') isString = !isString;
                                if(str[j] == ' ' && !isString) random_space(output_file);   //遇到這些字元，代表後面可以亂塞換行和空格
                                if(str[j] == '(' && !isString) random_space(output_file);
                                if(str[j] == ')' && !isString) random_space(output_file);
                                if(str[j] == ';' && !isString) random_space(output_file);
                                if(str[j] == ',' && !isString) random_space(output_file);
                                if(str[j] == '\n') random_space(output_file);

                            }
                        }

                        memset(str,0,sizeof(str));
                        get_line(str,tmp_file);
                    }
                    break;
                case 2:
                    memset(str,0,sizeof(str));
                    get_line(str,tmp_file);
                    while(!feof(tmp_file)){
                        
                        if(str[0] == '#') fwrite(str,strlen(str),1,output_file);

                        else{

                            set_name(str,var,VAR);  //var是一個struct array，定義在18行
                            isString = 0;
                            for(int32_t j = 0;j < strlen(str);j++){

                                int32_t varIdx = 0;
                                int32_t foundVariable = 0;

                                if(str[j] == '\'') isString = !isString;
                                if(str[j] == '"') isString = !isString;

                                while(var[varIdx].isOccupied == 1){

                                    if(strncmp(&str[j],var[varIdx].origin,strlen(var[varIdx].origin)) == 0 && isVar(str,j + strlen(var[varIdx].origin),VAR) && !isString){
                                        foundVariable = 1;
                                        fwrite(var[varIdx].modified,sizeof(char),16,output_file);
                                        j += strlen(var[varIdx].origin) - 1;
                                        break;
                                    }
                                    varIdx++;
                                }

                                if(!foundVariable) fwrite(&str[j],sizeof(char),1,output_file);
                            }

                        }

                        memset(str,0,sizeof(str));
                        get_line(str,tmp_file);
                    }

                    break;
                case 3:
                    memset(str,0,sizeof(str));
                    get_line(str,tmp_file);
                    while(!feof(tmp_file)){
                        
                        if(str[0] == '#') fwrite(str,strlen(str),1,output_file);

                        else{

                            set_name(str,var,FUNC);
                            isString = 0;
                            for(int32_t j = 0;j < strlen(str);j++){

                                int32_t varIdx = 0;
                                int32_t foundVariable = 0;

                                if(str[j] == '\'') isString = !isString;
                                if(str[j] == '"') isString = !isString;

                                while(var[varIdx].isOccupied == 1){

                                    if(strncmp(&str[j],var[varIdx].origin,strlen(var[varIdx].origin)) == 0 && isVar(str,j + strlen(var[varIdx].origin),FUNC) && !isString){
                                        foundVariable = 1;
                                        fwrite(var[varIdx].modified,sizeof(char),16,output_file);
                                        j += strlen(var[varIdx].origin) - 1;
                                        break;
                                    }
                                    varIdx++;
                                }

                                if(!foundVariable) fwrite(&str[j],sizeof(char),1,output_file);
                            }

                        }

                        memset(str,0,sizeof(str));
                        get_line(str,tmp_file);
                    }
                    
                    break;
                case 4:
                    memset(str,0,sizeof(str));
                    get_line(str,tmp_file);
                    while(!feof(tmp_file)){
                        
                        if(str[0] == '#') fwrite(str,strlen(str),1,output_file);

                        else{

                            isString = 0;
                            for(int32_t j = 0;j < strlen(str);j++){

                                if(str[j] == '\'') isString = !isString;
                                if(str[j] == '"') isString = !isString;

                                if((str[j] == '-' && str[j+1] >= '0' && str[j+1] <= '9') || (str[j] >= '0' && str[j] <= '9')){

                                    if(!isString && isVar(str,j-1,NUM)){

                                        int64_t num = 0;
                                        char* end = NULL;
                                        int64_t multiplicand = 0;
                                        int64_t multiplier = 0;
                                        int64_t remainder = 0;
                                        char buf[64] = "";

                                        num = strtol(&str[j],&end,10);
                                        multiplicand = rand() % 50;
                                        multiplier = num / 50;
                                        remainder = num - 50 * multiplier;
                                        
                                        if(remainder >= 0) snprintf(buf,64,"( %ld * %ld + %ld )",multiplicand,multiplier,remainder);
                                        else snprintf(buf,64,"( %ld * %ld - %d )",multiplicand,multiplier,abs(remainder));

                                        fwrite(buf,sizeof(char),strlen(buf),output_file);
                                        
                                        j += end - &str[j] - 1;

                                    }
                                    else fwrite(&str[j],sizeof(char),1,output_file);
                                }
                                else fwrite(&str[j],sizeof(char),1,output_file);
                            }

                        }

                        memset(str,0,sizeof(str));
                        get_line(str,tmp_file);
                    }
                    
                    break;
            }

            rewind(output_file);
            tmp_file = freopen(NULL,"wb+",tmp_file);
            while((c = fgetc(output_file)) != -1) fputc(c,tmp_file);
        }


        fclose(input_file);
        fclose(output_file);
        fclose(tmp_file);
        remove("tmp.c");
    }

    if(opt_h){
        printf("./hw0402 -l [options] -i [input file] -o [output file]\n");
        printf("./hw0402 -h\n");
        printf("./hw0402 --help\n");
        printf("    Display this help manual.\n");
    }

    

    return 0;
}