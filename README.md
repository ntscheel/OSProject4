# OSProject4
Repository for Operating Systems Project 4. 

###Task
Using C++, create a memory management unit simulator that will implement dynamic storage allocation using paging. The page size will be determined based on a command line parameter, but must be a power of 2 (between 1024 and 32768). You will not actually be spawning processes that consume memory. Rather you will be creating simulated "processes" that each make a series of memory allocations and deallocations. The following are the actions that a process can do:

- Initialize
	- Assign a PID - unique number (start at 1024 and increment up)
	- Allocate some amount of startup memory for the process
		- Text/Code: random number (2048 - 16384 bytes)
		- Data/Globals: random number (0 - 1024 bytes)
		- Stack: constant (65536 bytes)
- Allocate memory on the heap
	- N chars (N bytes)
	- N shorts (N * 2 bytes)
	- N ints / floats (N * 4 bytes)
	- N longs / doubles (N * 8 bytes)
- Set value for allocated memory
	- Store integer, float, or character values in memory
- Deallocate memory from the heap
	- N chars (N bytes)
	- N shorts (N * 2 bytes)
	- N ints / floats (N * 4 bytes)
	- N longs / doubles (N * 8 bytes)
- Terminate
	- Deallocate all memory associated with the process

Your simulator should continually ask the user to input a command. Your program should interpret the following statements:

- create
	- Initializes a new process
	- Prints the PID
- allocate \<PID\> \<var_name\> \<data_type\> \<number_of_elements\>
	- Allocated memory on the heap (how much depends on the data type and the number of elements)
	- Print the virtual memory address
- set \<PID\> \<var_name\> \<offset\> \<value_0\> \<value_1\> \<value_2\> ... \<value_N\>
	- Set the value for variable \<var_name> starting at \<offset>
	- Multiple contiguous values can be set with one command
- free \<PID\> \<var_name\>
	- Deallocate memory on the heap that is associated with \<var_name>
- terminate \<PID\>
	- Kill the specified process
- print \<object\> 
	- if \<object\> is "mmu", print the MMU memory table
	- If \<object\> is "page", print the page table (for pages that are not in memory, output text should be red). Do not need to print anything for free frames.
	- If \<object\> is "processes", print a list of PIDs for processes that are still running
	- If \<object\> is a "\<PID\>\:\<var_name\>", print the value of the variable for that process
	- If variable has more than 4 elements, just print the first 4 followed by "... \[N items\]" \(where N is the number of elements\).
- exit
	- quit program
	
Your simulated machine will only have 64 MB of RAM (67108864 bytes). However, you will have an addressable memory of 512 MB (536870912 bytes). Once space in RAM runs out, you must use the hard disk as the backing store to swap pages in and out of memory. A single file should be created on startup (if it doesn't already exist) that is 448 MB (469762048 bytes) and initialized to all 0s to serve as the backing store. If the file does exist, you should check to verify that it is 448 MB.
