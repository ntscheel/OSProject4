#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <string>
#include <iostream>
using namespace std;

#define STACK_SIZE 65536

struct Proc {
    int pid;
    int text;
    int data;
    int stack;
    int num_pages;
    Page *pages;
};
struct Page {
    int pid;
    int pageNum;
    int frameNum;
};
struct MMU {

};
int cur_pid = 1023;

//Functions
Proc create();
int randomBetweenRange(int, int);


/*
 * ---------- Begin main() ----------
 */
int main(int argc, char** argv){
    srand(time(NULL));

    uint8_t *mem = new uint8_t[67108864]; //init 64MB RAM, physical memory

    int procs_len = 50;
    int procs_cur = 0;
    Proc procs[procs_len];

    int page_size; //in bytes

    //TODO: createBackingStore

    //Check user arguments
    if(argc == 2){
        page_size = atoi(argv[1]);//Cast input to int
        if((page_size == 0) || ((page_size & (page_size - 1)) != 0)){//Check if input is power of 2
            printf("Error: %d is not a power of 2.\n",page_size);
            exit(1);
        }
    }else{ //Either missing page size arg, or too many args received.
        printf("Error: invalid arguments.\n");
        exit(1);
    }

    //Print greeting and command list
    cout << "Welcome to the Memory Allocation Simulator! Using a page size of " << page_size << " bytes.\n";
    cout << "Commands:\n";
    cout << "   * create (initializes a new process)\n";
    cout << "   * allocate <PID> <var_name> <data_type> <number_of_elements> (allocated memory on the heap)\n";
    cout << "   * set <PID> <var_name> <offset> <value_0> <value_1> ... <value_N> (set the value for a variable)\n";
    cout << "   * free <PID> <var_name> (deallocate memory on the heap that is associated with <var_name>)\n";
    cout << "   * terminate <PID> (kill the specified process)\n";
    cout << "   * print <object> (prints data)\n";
    cout << "      - If <object> is \"mmu\", print the MMU memory table\n";
    cout << "      - if <object> is \"page\", print the page table\n";
    cout << "      - if <object> is \"processes\", print a list of PIDs for processes that are still running\n";
    cout << "      - if <object> is a \"<PID>:<var_name>\", print the value of the variable for that process\n";
    cout << "   * exit (quit this program)\n\n";

    //Begin prompting for command
    string input;
    Proc test;//Temp, remove later
    while(1){
        cout << "> ";
        getline(cin, input);

        if(input == "exit"){
            cout << "Goodbye!\n";
            break;
        }else if (input == "create"){
            procs[procs_cur] = create();
            printf("Process created. PID: %d, data: %d\n",procs[procs_cur].pid,procs[procs_cur].data);
            procs_cur++;

        }else if (input == "allocate"){
            //Do allocation
        }else if (input == "set"){
            //Set vars
        }else if (input == "free"){
            //Free proc
        }else if (input == "terminate"){
            //Terminate proc
        }else if (input == "print"){
//            for (int i = 0; i < procs_cur; ++i) {
//                printf("PID: %d, data: %d\n",procs[i].pid,procs[i].data);
//            }
        }else{
            cout << "Invalid command: " << input << "\n";
        }

    }


    return 0;
}

Proc create(){
    Proc ret;
    cur_pid++;
    ret.pid = cur_pid;
    ret.data = randomBetweenRange(2048, 16384);
    ret.text = randomBetweenRange(0, 1024);
    ret.stack = STACK_SIZE;
    return ret;
}

int randomBetweenRange(int min, int max){
    return rand() % (max-min+1) + min;
}
/*
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