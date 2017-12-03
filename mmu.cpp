#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define STACK_SIZE 65536

struct Proc {
    int pid;
    int text;
    int data;
};
struct Page {
    int pid;
    int pageNum;
    int frameNum;
};
struct MMU {

};

int main(int argc, char** argv){

    uint8_t *mem = new uint8_t[67108864]; //init RAM
    int page_size; //in bytes
    //TODO: createBackingStore


    if(argc == 2){
        page_size = atoi(argv[1]);
        if((page_size == 0) || ((page_size & (page_size - 1)) != 0)){
            printf("Error: %d is not a power of 2.\n",page_size);
            exit(1);
        }
    }
    else{
        printf("Error: invalid arguments.\n");
        exit(1);
    }
    printf("Page size = %d bytes\n",page_size);

    return 0;
}
/*
void create(){

}

void allocate (PID, var_name, data_type, number_of_elements) {

}

void set (PID, var_name, offset, values[]) {

}

void free (PID, var_name){

}

void terminate(PID){

}

void print (object){
    //Maybe make separate functions for printPage, printMMU, etc.
}

void sendToBackingStore(){

}

void getFromBackingStore(){

}

void createBackingStore(){

}
 */