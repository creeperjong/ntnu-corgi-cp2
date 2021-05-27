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

    int32_t idx = 0;
    int32_t num = 0;
    int32_t authorIdx = 0;
    int32_t parsingAuthor = 0;
    char* target = NULL;
    char author[16][32];
    memset(author, 0, sizeof(author));

    char* start = "<!-- search results -->";
    char* end = "<!-- settings menu -->";
    char* authorPrefix = "<span itemprop=\"name\" title=\"";
    char* titlePrefix = "<span class=\"title\" itemprop=\"name\">";
    char* sourceNamePrefix = "<span itemprop=\"name\">";
    char* sourceVolumePrefix = "<span itemprop=\"volumeNumber\">";
    char* sourceIssue = "<span itemprop=\"issueNumber\">";
    char* pagePrefix = "<span itemprop=\"pagination\">";
    char* yearPrefix = "<span itemprop=\"datePublished\">";

    for(int32_t i = 0;i < filesize;i++){
        if(strncmp(&buf[i], start, strlen(start)) == 0){
            idx = i;
            break;
        }
    }

    for(int32_t i = idx;i < filesize;i++){
        
        if(strncmp(&buf[i], authorPrefix, strlen(authorPrefix)) == 0){
            
            if(!parsingAuthor){
                num++;
                if(num >  10) break;
                parsingAuthor = 1;
                authorIdx = 0;
                memset(author, 0, sizeof(author));
            }

            target = strtok(&buf[i + strlen(authorPrefix)], "\"<>");
            target = strtok(NULL, "\"<>");
            strncpy(author[authorIdx++], target, strlen(target));
        }
        else if(strncmp(&buf[i], titlePrefix, strlen(titlePrefix)) == 0){
            printf("Paper %02d:\n", num);
            printf("\tTitle: %s\n", strtok(&buf[i + strlen(titlePrefix)], "<"));
            printf("\tAuthor: ");
            for(int32_t j = 0;j < authorIdx;j++){
                if(j == authorIdx - 1) printf("%s.",author[j]);
                else printf("%s, ",author[j]);
            }

            parsingAuthor = 0;
        }
        else if(strncmp(&buf[i], sourceNamePrefix, strlen(sourceNamePrefix)) == 0){
            printf("\n\tSource: %s", strtok(&buf[i + strlen(sourceNamePrefix)], "<"));
        }
        else if(strncmp(&buf[i], sourceVolumePrefix, strlen(sourceVolumePrefix)) == 0){
            printf(" %s", strtok(&buf[i + strlen(sourceVolumePrefix)], "<"));
        }
        else if(strncmp(&buf[i], sourceIssue, strlen(sourceIssue)) == 0){
            printf("(%s)", strtok(&buf[i + strlen(sourceIssue)], "<"));
        }
        else if(strncmp(&buf[i], pagePrefix, strlen(pagePrefix)) == 0){
            printf(": %s", strtok(&buf[i + strlen(pagePrefix)], "<"));
        }
        else if(strncmp(&buf[i], yearPrefix, strlen(yearPrefix)) == 0){
            printf("\n\tYear: %s\n", strtok(&buf[i + strlen(yearPrefix)], "<"));
        }
        else if(strncmp(&buf[i], end, strlen(end)) == 0) break;
    }

    munmap(buf, filesize);
    close(fd);
    remove("buf");

    return;
}

int main(int argc, char *argv[]){

    int32_t opt_q = 0;
    int32_t opt_h = 0;
    int32_t optargIdx = 0;
    char c = 0;
    char keyword[32] = "";

    while((c = getopt(argc, argv, "q:h")) != -1){
        switch(c){
            case 'q':
                opt_q = 1;
                strncpy(keyword, optarg, 32);
                optargIdx = optind;
                break;
            case 'h':
                opt_h = 1;
                break;
        }
    }

    if(opt_q){

        int32_t fd = 0;
        int64_t filesize = 0;
        char url[128] = "https://dblp.org/search?q=";
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

        while(optargIdx < argc){
            keyword[strlen(keyword)] = '-';
            strncat(keyword, argv[optargIdx], strlen(argv[optargIdx]));
            optargIdx++;
        }

        strncat(url, keyword, strlen(keyword));
        curl_easy_setopt(curl, CURLOPT_URL, url);
        
        for(int32_t i = 0;i < strlen(keyword);i++){
            if(keyword[i] == '-') keyword[i] = ' ';
        }
        printf("Searching for %s...\n", keyword);
        
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        getResult();

        fclose(buf);
        curl_easy_cleanup(curl);
    }

    if(opt_h) printf("./hw0503 -q [keyword]\n./hw0503 -h\n\tDisplay this help manual.\n");



    return 0;
}