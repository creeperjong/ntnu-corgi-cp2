#pragma once 

#include <stdio.h>
#include <stdint.h>


char* mystrchr(const char* s, int c);
char* mystrrchr(const char* s, int c);
size_t mystrspn(const char* s, const char* accept);
size_t mystrcspn(const char* s, const char* reject);
char* mystrpbrk(const char* s, const char* accpet);
char* mystrstr(const char* haystack, const char* needle);
char* mystrtok(char* str, const char* delim);

char* mystrchr(const char* s, int c){

    int32_t i = 0;

    while(s[i] != '\0'){
        if(s[i] == c) return (char*)s + i;
        else i++; 
    }

    return NULL;
}

char* mystrrchr(const char* s, int c){

    int32_t i = 0;

    while(s[i] != '\0') i++;

    while(i != -1){
        if(s[i] == c) return (char*)s + i;
        else i--; 
    }

    return NULL;
}

size_t mystrspn(const char* s, const char* accept){
    size_t ans = 0;
    int32_t found = 0;
    int32_t i = 0;
    int32_t j = 0;

    while(s[i] != '\0'){
        while(accept[j] != '\0'){
            if(s[i] == accept[j]){
                found = 1;
                ans++;
                break;
            }
            j++;
        }
        if(found == 0) break;
        i++;
        j = 0;
        found = 0;
    }
    
    return ans;
}

size_t mystrcspn(const char* s, const char* reject){
    size_t ans = 0;
    int32_t found = 0;
    int32_t i = 0;
    int32_t j = 0;

    while(s[i] != '\0'){
        while(reject[j] != '\0'){
            if(s[i] == reject[j]){
                found = 1;
                break;
            }
            j++;
        }
        if(found == 1) break;
        else ans++;
        i++;
        j = 0;
        found = 0;
    }
    
    return ans;
}

char* mystrpbrk(const char* s, const char* accpet){

    int32_t i = 0;
    int32_t j = 0;

    while(s[i] != '\0'){
        while(accpet[j] != '\0'){
            if(s[i] == accpet[j]) return (char*)&s[i];
            j++;
        }
        i++;
        j = 0;
    }

    return NULL;
}

char* mystrstr(const char* haystack, const char* needle){
    int32_t idx = 0;
    int32_t needle_len = 0;
    int32_t isFailed = 0;

    while(needle[idx++] != '\0') needle_len++;
    idx = 0;

    while(haystack[idx+needle_len-1] != '\0'){
        for(int32_t i = 0;i < needle_len;i++){
            if(needle[i] != haystack[idx+i]){
                isFailed = 1;
                break;
            }
        }

        if(isFailed){
            idx++;
            isFailed = 0;
        }
        else return (char*)&haystack[idx]; 

    }
    return NULL;
}

char* mystrtok(char* str, const char* delim){
    static char* record = NULL;
    char* ans = NULL;
    char* separate_ptr = NULL;
    int32_t i = 0;
    int32_t isEnd = 1;
    
    if(record == NULL){
        if(str != NULL) record = str;
        else return NULL;
    }

    ans = record;

    separate_ptr = mystrpbrk(record,delim);

    if(separate_ptr == NULL){
        record = NULL;
        return ans;
    }else{
        do{
            isEnd = 1;
            *(separate_ptr) = '\0';
            separate_ptr++;
            while(delim[i] != '\0'){
                if(*(separate_ptr) == delim[i]){
                    isEnd = 0;
                    break;
                }
                i++;
            }
            i = 0;
        }while(!isEnd);

        record = separate_ptr;
        return ans;
    }


}

