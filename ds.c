#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
    allocates memory for header + len + terminator 
    +----+----+--------+--+
    |LLLL|AAAA|--------|\0|
    +----+----+--------+--+
    where:
    LLLL is the current string length
    AAAA is the maximum string length
    ---- is the string
    \0   is the null terminator

*/
char *ds_new(char *init, uint32_t len){
    char *s = malloc(4+4+len+1);
    uint32_t *lenptr = (uint32_t *)s;
    uint32_t *currlen = (uint32_t *)(s+4);
    *currlen = len;
    *lenptr = len;
    for(uint32_t i = 0; i < len; i++){
        s[8+i] = init[i];
    }
    s[8+len] = 0;
    return (s+8);
}

/*
    allocates an empty string but with known dimension
    +----+----+-+--+
    |0000|ccap|-|--|
    +----+----+-+--+
*/
char *ds_empty(uint32_t cap){
    char *st = malloc(4+4+cap+1);
    uint32_t *lenptr = (uint32_t *)st;
    uint32_t *maxlen = (uint32_t *)(st+4);
    *lenptr = 0;
    *maxlen = cap;
    st[8] = 0; /* to make it always printable */
    return (st+8);
}

/* returns the length of a given string "s" in O(1) time */
uint32_t ds_len(char *s){
    uint32_t *len = (uint32_t *)(s-8);
    return *len;
}

/* returns available space (LLLL - AAAA) */
uint32_t ds_avail(char *s){
    uint32_t *currlen = (uint32_t *)(s-8);
    uint32_t *max = (uint32_t *)(s-4);
    uint32_t aval = *max - *currlen;
    return aval;

}

/* 
    enqueues charactes from "add" to the string 
    if ds_avail < len reallocates the space and updates AAAA    

    |clen|mcap|-------|\0
               |
               s
    
    |clen|mcap|-------|\0
     |
     t
*/
char *ds_cat(char *s, const char *add, uint32_t len){
    uint32_t avail = ds_avail(s);
    s = s-8;

    if(avail < len){
        uint32_t currCap = *(uint32_t *)(s+4); // (uint32_t *) converts into a pointer. I have to deference it
        uint32_t newCap;
        if((currCap*2 > (currCap + len))){
            newCap = currCap*2;
        } else {
            newCap = (currCap + len) * 2;
        }

        char *tmp = realloc(s, 4+4+newCap+1); // reallocates memory, copies everything in s automatically

        *(uint32_t *)(tmp+4) = newCap;

        s = tmp;
    }
    uint32_t currLen = *(uint32_t *)(s);
    for(uint32_t i = 0; i < len; i++){
        s[8+currLen+i] = add[i];
        *(uint32_t *)s += 1;
    }

    uint32_t newLen = currLen + len;
    *(uint32_t *)s = newLen;
    s[8+newLen] = '\0';   // <-- always, unconditionally, right here


    return s+8;
}

/* Adds a char "c" at the end of a given string "s" */
char *ds_catc(char *s, char c){
    char *new = ds_cat(s, &c, 1);
    s = new;
    return new;
}

void ds_clear(char *s){
    *s = 0;
    *(uint32_t *)(s-8) = 0;
}

/* frees a previously allocated string "s" */
void ds_free(char *s){
    free(s-8);
}

/*
int main(void){ 
    char *s = ds_new("Ciao", 4);
    printf("%s\n\n", s);
    
    char *e = ds_empty(256);


    printf("len of the string s: %d. Len of the string e: %d\n", ds_len(s), ds_len(e));
    printf("av. space for the string s: %d. av. space for the string e: %d\n", ds_avail(s), ds_avail(e));

    s = ds_cat(s, " franco", 7);
    printf("\nString s after the add: %s\n", s);
    printf("len of the string s: %d:\n", ds_len(s));
    printf("av. space for the string s: %d:\n", ds_avail(s));

    printf("\n\n");
    
    s = ds_catc(s, 'l');
    printf("%s\n",s);
    
    ds_clear(s);
    printf("%s\n",s);


    ds_free(s);
    
    return 0;

}
*/