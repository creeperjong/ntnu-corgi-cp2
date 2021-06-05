#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <curl/curl.h>

void getResult(void);
void getResult(void){
    int32_t fd = 0;
    int64_t filesize = 0;
    char* buf = NULL;
    struct stat filestat = {0};

    fd = open("buf" ,O_RDWR);
    if(fd == -1){
        printf("Error: No such file or directory\n");
        return;
    }
    fstat(fd,&filestat);
    filesize = filestat.st_size;
    buf = mmap(0, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    char* startPrefix = "<h2><span class=\"mw-headline\" id=\"";
    char* wordPostfix = "</a></b>";
    char* wordEndPrefix = "</tbody></table>";
    char* detailStart = "<ol><li>";
    char* detailEnd = "</ol>";
    char* end = "NewPP limit report";

    int32_t word_state = 0;
    int32_t detail_state = 0;
    int32_t isPrint = 1;
    int32_t num = 0;

    for(int32_t i = 0;i < filesize;i++){
        //printf("%d\n", i);

        if(strncmp(&buf[i], startPrefix, strlen(startPrefix)) == 0){
            printf("\n%s: ", strtok(&buf[i + strlen(startPrefix)], "\""));
            word_state = 1;
            num = 0;
        }
        else if(strncmp(&buf[i], wordPostfix, strlen(wordPostfix)) == 0 && word_state){

            int32_t tmpIdx = i;

            while(1){
                if(buf[tmpIdx] == '>'){
                    tmpIdx++;
                    break;
                }
                else tmpIdx--;
            }

            printf("%s/", strtok(&buf[tmpIdx], "<"));
        }
        else if(strncmp(&buf[i], wordEndPrefix, strlen(wordEndPrefix)) == 0 && word_state){
            word_state = 0;
            printf("\n\n");
        }
        else if(strncmp(&buf[i], detailStart, strlen(detailStart)) == 0 && !detail_state){
            detail_state = 1;
        }
        else if(strncmp(&buf[i], detailEnd, strlen(detailEnd)) == 0 && detail_state){
            printf("\n");
            detail_state = 0;
        }
        else if(strncmp(&buf[i], end, strlen(end)) == 0){
            break;
        }

        if(detail_state){
            if(strncmp(&buf[i], "<dd>", strlen("<dd>")) == 0) printf("EX: ");
            if(strncmp(&buf[i], "<li>", strlen("<li>")) == 0) printf("%d. ", ++num);
            
            if(buf[i] == '<' || buf[i] == '>'){
                isPrint = !isPrint;
                continue;
            }

            if(isPrint){
                if(buf[i] == '\n') printf("\n");
                else printf("%c", buf[i]);
            }
        }

    }

    munmap(buf, filesize);
    close(fd);
    remove("buf");

    return;
}

int main(int argc, char *argv[]){

    int32_t opt_w = 0;
    int32_t opt_h = 0;
    char c = 0;
    char keyword[32] = "";

    while((c = getopt(argc, argv, "w:h")) != -1){
        switch(c){
            case 'w':
                opt_w = 1;
                strncpy(keyword, optarg, 32);
                break;
            case 'h':
                opt_h = 1;
                break;
        }
    }

    if(opt_w){

        int32_t fd = 0;
        int64_t filesize = 0;
        char url[128] = "https://simple.wiktionary.org/wiki/";
        CURL* curl = NULL;
        CURLcode res;
        FILE* buf = NULL;
        struct stat filestat = {0};

        curl = curl_easy_init();
        if(!curl){
            printf("Error: Unable to initialize CURL\n");
            return 0;
        }
        if((buf = fopen("buf", "w")) == NULL){
            perror("Error");
            return 0;
        }

        strncat(url, keyword, strlen(keyword));
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        getResult();

        fclose(buf);
        curl_easy_cleanup(curl);
    }

    if(opt_h) printf("fin03:\n\t-w <word>\n\t-h\n");



    return 0;
}