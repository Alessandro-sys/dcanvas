1. When allocatin space with malloc, i have to use the length of the variable i want to allocate, not its size
char *s = malloc(4+4+len+1)

2. if a function returns uint32_t, i have to cast every variable in uint32_t. I cannot return *(s-4), i have to return (uint32_t *)(s-4)

3. s = realloc(s, 4+4+*currlen+len+1); does not work