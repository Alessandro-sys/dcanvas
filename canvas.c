#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds.c"


/*
Defines a canvas w*h, filled with "fill" character

        +----+----+--------------------------+
 blocco |WWWW|HHHH| w*h celle (char)         |
        +----+----+--------------------------+
                  ^ puntatore restituito

*/
char *canvas_new(uint32_t w, uint32_t h, char fill){
    char *c = malloc(4+4+(w*h));
    *(uint32_t *)c = w;
    *(uint32_t *)(c+4) = h;
    for(uint32_t i = 0; i < (w*h); i++){
        c[8+i] = fill;
    }
    return c+8;
}

/* returns canvas width */
uint32_t canvas_w(char *c){
    return *(uint32_t *)(c-8);
} 

/* returns canvas height */
uint32_t canvas_h(char *c){
    return *(uint32_t *)(c-4);
}

/* 
    Given coordinates in (x,y) format, returns one-dimensional coordinates
    Needs to handle:
    (1, 0) -> 1     (10, 0) -> 10
    (0, 1) -> 11    (0,10) -> 20
    (-1, 0) -> 10   (-10, 0) -> 1
    (20, 0) -> 10
    (0, 20) -> 100
*/
int canvas_index(char *c, int x, int y){
    uint32_t w = canvas_w(c);
    uint32_t h = canvas_h(c);

    if(x < 0){
        x = (-x) % w;
        x = w - x;
    }
    if(y < 0){
        y = (-y) % h;
        y = h - y;
    }
    if(x > 0) x = x % w;
    if(y > 0) y = y % h;
    

    return y * w + x;
}

/* writes character "state" in the position (x,y) of the canvas c*/
void canvas_set(char *c, int x, int y, char state){
    int index = canvas_index(c,x,y);
    c[index] = state;
}

/* gets the char at the position (x,y) */
char canvas_get(char *c, int x, int y){
    int index = canvas_index(c,x,y);
    return c[index];
}

/* fills the canvas with a given character "state" */
void canvas_fill(char *c, char state){
    uint32_t w = canvas_w(c);
    uint32_t h = canvas_h(c);
    for(uint32_t i = 0; i < (w*h); i++){
        c[i] = state;
    }
}

/*  fills a canvas row from given row and col for a given length */
void canvas_hline(char *c, int x, int y, int len, char state){
    // x -> x+1 x+2 x+3... x+len
    // y fisso
    int count = 0;
    while(count < len){
        c[canvas_index(c, x, y)] = state;
        x++;
        count++;
    }
}

/* fills a canvas col from given row and col for a given length */
void canvas_vline(char *c, int x, int y, int len, char state){
    // x fisso
    // y -> y+1 y+2 y+3
    int count = 0;
    while(count < len){
        c[canvas_index(c,x,y)] = state;
        y++;
        count++;
    }
}

/* writes a rectangle with canvas_(h/v)line starting from given coordinates (x,y) and dimension (w,h) */
void canvas_rect(char *c, int x, int y, int w, int h, char state){
    w = w-1;
    h = h-1;
    canvas_hline(c, x, y, w, state);
    canvas_hline(c, x, y+h, w, state);
    canvas_vline(c, x, y, h, state);
    canvas_vline(c, x+w, y, h+1, state);
}

/* writes a given text in "str", starting from coordinates (x,y) in the canvas "c" */
void canvas_text(char *c, int x, int y, const char *str){
    int index = canvas_index(c, x, y);
    int count = 0;
    while(str[count] != 0){
        c[index+count] = str[count];
        count++;
    }
}

void canvas_free(char *c){
    free(c-8);
}

/* 
    transforms a canvas into a ds 
    Will make two for, the first one iterates through columns, the second one through rows
    I will copy every character into a buffer
    At the end of each row, it will add a \n character, so the buffer will need to be (w*h + h) big
*/
char *canvas_render(char *c){
    uint32_t w = canvas_w(c);
    uint32_t h = canvas_h(c);
    
    uint32_t cap = w*h + h;
    char *buf = malloc(cap);
    
    for(uint32_t y = 0; y < h; y++){
        for(uint32_t x = 0; x < w; x++){
            int index = canvas_index(c, x, y);
            buf[index+y] = c[index];
        }
        buf[w*y + w + y] = '\n';
    }

    char *s = ds_new(buf, cap);
    free(buf);
    return s;
}

void canvas_print(char *c){
    uint32_t w = canvas_w(c);
    uint32_t h = canvas_h(c);
    for(uint32_t y = 0; y < h; y++){
        for(uint32_t x = 0; x < w; x++){
            printf("%c", c[canvas_index(c,x,y)]);
        }
        printf("\n");
    }
    printf("\n");
}


int main(void){
    char *canvas = canvas_new(10,20,'#');
    canvas_print(canvas);

    canvas_set(canvas,5,2,'l');
    canvas_print(canvas);
    
    printf("%c\n", canvas_get(canvas,5,2));

    canvas_fill(canvas, 'c');
    canvas_print(canvas);

    canvas_hline(canvas, 5, 2, 7, 'f');
    canvas_print(canvas);

    canvas_vline(canvas, 7,5, 19, 'z');
    canvas_print(canvas);

    canvas_fill(canvas, 'o');
    canvas_rect(canvas, 8, 15, 5, 7, '#');
    canvas_print(canvas);


    canvas_fill(canvas, 'o');
    canvas_text(canvas, 5, 5, "buongiorno italia");
    canvas_print(canvas);

    canvas_free(canvas);

    char *c = canvas_new(20, 10, '.');
    canvas_rect(c, 1, 1, 18, 8, '#');
    canvas_text(c, 5, 4, "CIAO");

    char *frame = canvas_render(c);
    printf("%s", frame);
    ds_free(frame);
    canvas_free(c);
    
    return 0;
    
}
