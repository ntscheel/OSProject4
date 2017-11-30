/*TODO: Create proc struct
 *
 */
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

int main(argc, argv[]){

    uint8_t *mem = new uint8_t[67108864]; //init RAM
    int page_size; //in bytes
    //TODO: createBackingStore

    //TODO: read in page size and check that it's a power of 2




    return 0;
}

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