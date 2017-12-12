#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <string>
#include <string.h>
#include <vector>
#include <iostream>


// compile: g++ -o mmu main.cpp -std=c++11

using namespace std;

#define STACK_SIZE 65536
int cur_pid = 1023;
uint8_t *RAM = new uint8_t[67108864]; //init 64MB RAM, physical memory
uint8_t *HD = new uint8_t[536870912]; //init 448MB Hard disk, backing store
//physical address is frame_num * page_size
//      **if this exceeds the 67108864 (64MB) then the frame is in the backing store
//          and must be loaded loaded into mem (and something from mem loaded into
//          the backing store) before it can be used/accessed



/********NOTE
 * Every time we access memory, check if it is on RAM (in mem)
 *      -if not move something from RAM to Hard Disk, then the thing from Hard disk to RAM
 *          -this entails updating its page table info
 *          -Things that this affects
 *              -anything that says allocate
 *              -create
 *              -set
 *              -print variable??? (I do not think we need to worry about any of the prints)
 *
 *
 *
 *      -if it is in RAM go ahead and do whatever
 *
 * Every time we add memory to a process, make sure there is room in RAM(is there an open
 *  frame for the new page)
 *      -if there is not room in RAM (no open frame) load something into the backing store
 *        and use the newly opened frame for this new stuff
 *              -again update the page table
 *      -if there is room, then just use the open space...
 *
 *
 *
 *
 * Not sure how any of this will really be implemented though...
 */








struct Mem_alloc {
    std::string name;
    int size;
    int v_addr;
    //page_num = v_adder / page_size
    //offset = v_adder % page_size
    std::vector<std::string> values;
};
struct Proc {
    int pid;
    std::vector<Mem_alloc> mem;

};
// entry in page table
struct entry {
    int pid;
    int page_num;
    int frame_num;
};

//Functions
//	(TODO Going to have to go through this later to handle when a new frame needs to be
//	loaded, and updating the page table stuff.)
int create(std::vector<Proc>* procs, int page_size);
int randomBetweenRange(int, int);
void allocate_mem(std::vector<Mem_alloc>* mem, std::string name, int size, int page_size);
int allocate(std::vector<Proc>* procs, int PID, std::string var_name, std::string data_type, int number_of_elements, int page_size);
int set (std::vector<Proc>* procs, int PID, string var_name, int offset, vector<string> values);
int print_variable(std::vector<Proc>* procs, int PID, string var_name);
int print_mmu(std::vector<Proc>* procs);
int print_processes(std::vector<Proc>* procs);
int print_page (std::vector<entry>* table);
vector<std::string> str_split(std::string input);
/*
 * ---------- Begin main() ----------
 */
int main(int argc, char **argv) {
    srand(time(NULL));

    memset(RAM, 0, sizeof(RAM)); //set all values to zero
    memset(HD, 0, sizeof(HD)); //set all values to zero

    std::vector<int> frames;
    std::vector<int> pages;
    std::vector<Proc> procs;
    std::vector<entry> page_table;
    //TODO: createBackingStore


    //Check user arguments
    int page_size; //in bytes
    if (argc == 2) {
        page_size = atoi(argv[1]);//Cast input to int
        if ((page_size == 0) || ((page_size & (page_size - 1)) != 0)) {//Check if input is power of 2
            printf("Error: %d is not a power of 2.\n", page_size);
            exit(1);
        }
    } else { //Either missing page size arg, or too many args received.
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
    int i; //used for loops in the if-else's
    while (1) {
        cout << "> ";
        getline(cin, input);
        vector<string> inputs = str_split(input);
        if (inputs[0] == "exit") {
            cout << "Goodbye!\n";
            break;
        } else if (inputs[0] == "create") {
            int new_proc = create(&procs, page_size);
            cout << new_proc << endl;
            //procs[procs_cur] = create();
            //printf("Process created. PID: %d, data: %d\n",procs[procs_cur].pid,procs[procs_cur].data);
            //procs_cur++;

        } else if (inputs[0] == "allocate") {
            if(inputs.size() == 5) {
                int V_Addr = allocate(&procs, stoi(inputs[1]), inputs[2], inputs[3], stoi(inputs[4]), page_size);
                if(V_Addr != -1) cout << V_Addr << endl;
            }else{
                cout << "Invalid allocate options: " << input << endl;
                cout << "allocate <PID> <var_name> <data_type> <number_of_elements>" << endl;
            }
        } else if (inputs[0] == "set") {
            //Set vars
            set(&procs, stoi(inputs[1]), inputs[2], stoi(inputs[3]), inputs);
        } else if (inputs[0] == "free") {
            //Free proc
        } else if (inputs[0] == "terminate") {
            //Terminate proc
        } else if (inputs[0] == "print") {
            if(inputs[1] == "mmu"){
                print_mmu(&procs);
            } else if (inputs[1] == "processes") {
                print_processes(&procs);
            } else if(inputs.size() == 3){
                print_variable(&procs, stoi(inputs[1]), inputs[2]);
            }

        } else {
            cout << "Invalid command: " << input << "\n\n";
            for (int i=0; i<inputs.size(); i++) {
                cout << inputs[i] << ": ";
            }
            cout << endl;
        }

    }


    return 0;
}

//Use this to create a new process when the "create" command is intput
//	It will create a new process stuct, allocate the data, text, and stack
//	then put the new process at the end of the "procs" vector
int create(std::vector<Proc>* procs, int page_size) {
    Proc ret;
    //TODO is this really how PID is handled??
    cur_pid++;
    ret.pid = cur_pid;
    int data_size = randomBetweenRange(2048, 16384);
    int text_size = randomBetweenRange(0, 1024);
    //do the memory checking handling stuff in the allocate_mem method
    allocate_mem(&ret.mem, "<DATA>", data_size, page_size);
    allocate_mem(&ret.mem, "<TEXT>", text_size, page_size);
    allocate_mem(&ret.mem, "<STACK>", STACK_SIZE, page_size);
    procs->push_back(ret);
    return cur_pid;
    //TODO How to handle updating the page table???
}

//generates a random number between "min" and "max"
int randomBetweenRange(int min, int max) {
    return rand() % (max - min + 1) + min;
}

//This is used only for allocating the data, text, and stack when creating a process
void allocate_mem(std::vector<Mem_alloc>* mem, std::string name, int size, int page_size) {
    Mem_alloc unit;
    unit.name = name;
    unit.size = size;
    unit.v_addr = 0;
    if (mem->size() > 0) {
        unit.v_addr = mem->back().size + mem->back().v_addr;
    }
    /*
    //Does the new memory allocation require a new page?
    if((unit.v_addr + unit.size)/page_size > unit.v_addr/page_size){
        //if so, is there room in RAM? Is there a free frame?
        int f=0;
        while(f*page_size<RAM){


        }


        for(f=0; f<sizeof(RAM); f++){

        }

        //if not, load a "non-essential" frame into the backing store and use that frame
    }
    */

    mem->push_back(unit);
    //TODO How to handle updating the page table???
}

//This function will be called when a user inputs the "allocate" command.
//	Use the data type and number of elements to find the size of
//	the variable. Create the Mem_alloc struct for this new variable
//	and put it in the specified processes mem vector.
int allocate(std::vector<Proc>* procs, int PID, std::string var_name, std::string data_type, int number_of_elements,
              int page_size) {
    //create the new variable
    Mem_alloc var;
    var.name = var_name;
    if (data_type == "char") {
        var.size = number_of_elements;
    } else if (data_type == "short") {
        var.size = number_of_elements * 2;
    } else if (data_type == "int" || data_type == "float") {
        var.size = number_of_elements * 4;
    } else if (data_type == "long" || data_type == "double") {
        var.size = number_of_elements * 8;
    } else {
        cout << "Invalid data type: " << data_type << "\n";
        return -1;
    }
    //find the specified process, put new variable into mem vector
    int i, c;
    c = -1;
    for(i=0; i<procs->size(); i++) {
        if (procs->at(i).pid == PID) c = i;
    }
    if (c == -1) {
        cout << "Invalid PID: " << PID << "\n";
        return -1;
    }
    var.v_addr = procs->at(c).mem.back().size + procs->at(c).mem.back().v_addr;
    var.values.clear();
    for(i=0; i<number_of_elements; i++){
        var.values.push_back("");
    }
    procs->at(c).mem.push_back(var);
    return var.v_addr;
}

int set (std::vector<Proc>* procs, int PID, string var_name, int offset, vector<string> values) {
    //find the specified process
    int i, p, v;
    p = -1;
    v = -1;
    for(i=0; i<procs->size(); i++) {
        if (procs->at(i).pid == PID) p = i;
    }
    if (p == -1) {
        cout << "Invalid PID: " << PID << "\n";
        return -1;
    }
    //find the variable that has var_name
    for(i=0; i<procs->at(p).mem.size(); i++){
        if(procs->at(p).mem.at(i).name == var_name) v = i;
    }
    if (v == -1) {
        cout << "Invalid Variable Name: " << var_name << "\n";
        return -1;
    }
    //now we have the variable (procs->at(p).mem.at(v))
    //set it's value(s)
    //**values start at values[4] because it is the whole inputs vector
    if((offset + values.size()-4) > procs->at(p).mem.at(v).values.size()){
        cout << "Invalid Offset/Number of elements: " << offset << "/" << values.size()-4 << endl;
        cout << "Variable " << procs->at(p).mem.at(v).name << " has size " << procs->at(p).mem.at(v).values.size() << endl;
        return -1;
    }
    vector<string>::iterator it;
    for(i=4; i<values.size(); i++) {
        it = procs->at(p).mem.at(v).values.begin();
        it = procs->at(p).mem.at(v).values.erase(it+offset+i-4);
        procs->at(p).mem.at(v).values.insert(it, values[i]);
    }

    return 0;
}
int print_variable(std::vector<Proc>* procs, int PID, string var_name){
    //find the specified process
    int i, p, v;
    p = -1;
    v = -1;
    for(i=0; i<procs->size(); i++) {
        if (procs->at(i).pid == PID) p = i;
    }
    if (p == -1) {
        cout << "Invalid PID: " << PID << "\n";
        return -1;
    }
    //find the variable that has var_name
    for(i=0; i<procs->at(p).mem.size(); i++){
        if(procs->at(p).mem.at(i).name == var_name) v = i;
    }
    if (v == -1) {
        cout << "Invalid Variable Name: " << var_name << "\n";
        return -1;
    }
    //Now I have the variable (procs->at(p).mem.at(v))
    if(procs->at(p).mem.at(v).values.size() > 4){
        for(i=0; i<4; i++){
            cout << procs->at(p).mem.at(v).values.at(i) << ", ";
        }
        cout << "... [" << procs->at(p).mem.at(v).values.size() << " items]" << endl;
    } else {
        for(i=0; i<procs->at(p).mem.at(v).values.size()-1; i++) {
            cout << procs->at(p).mem.at(v).values.at(i) << ", ";
        }
        cout << procs->at(p).mem.at(v).values.at(procs->at(p).mem.at(v).values.size()-1) << endl;
    }
    return 0;
}
int print_mmu(std::vector<Proc>* procs){
    printf(" PID  | Variable Name | Virtual Addr | Size     \n");
    printf("------+---------------+--------------+------------\n");
    //Now for each process
    int i, j;
    for(i=0; i<procs->size(); i++){
        //and for each variable of each process
        for(j=0; j<procs->at(i).mem.size(); j++){
            //print its information
            // EX. )1024 | <TEXT>        |   0x00000000 |       5992
            printf(" %4d | %13s |   0x%08X | %d\n", procs->at(i).pid, procs->at(i).mem.at(j).name.c_str(), procs->at(i).mem.at(j).v_addr, procs->at(i).mem.at(j).size);
        }

    }
    return 0;
}
int print_processes(std::vector<Proc>* procs){
    int i;
    for(i=0; i<procs->size(); i++){
        cout << procs->at(i).pid << endl;
    }
    return 0;
}
//print the page table
int print_page (std::vector<entry>* table){
    //TODO make the output text red for pages not in memory
    printf(" PID  | Page Number | Frame Number");
    printf("------+-------------+--------------");
    //Now print each entry in the table
    int i;
    for(i=0; i<table->size(); i++){
        printf(" %4d | %11d | %12d ", table->at(i).pid, table->at(i).page_num, table->at(i).frame_num);
    }
    return 0;
}
/*
void free (PID, var_name){
}
void terminate(PID){
}
void sendToBackingStore(){
}
void getFromBackingStore(){
}
void createBackingStore(){
}
 */
//This takes a string as input and returns a vector of strings that come from splitting
//the input on space characters. (will not include space characters in the items)
vector<std::string> str_split(std::string input) {
    vector<std::string> ret;
    std::string str = "";
    int i;
    for (i = 0; i < input.length(); i++) {
        if (input[i] != ' ') str += input[i];
        if (input[i] == ' ' || input[i] == '\n' || i == input.length() - 1) {
            if (str.length() > 0) ret.push_back(str);
            str.clear();
        }
    }
    if(ret.size() < 1) ret.push_back("");
    return ret;
}