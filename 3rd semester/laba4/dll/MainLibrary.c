#include <windows.h> 
#include <stdio.h> 

int fileProcessing(CHAR* buf, char targetChar) {
    int exchanges = 0;
    //std::cout << "I am in  FUNCTION";
    for (int i = 0; buf[i] != '\0'; i++) {
        //printf("I am in  cicle: %d", i);
        if (isdigit(buf[i])) {
            buf[i] = targetChar;
            exchanges++;
        }
    }

    return exchanges;
}
