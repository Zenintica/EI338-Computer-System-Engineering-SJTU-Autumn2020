#include <iostream>
#include <limits.h>
#include <cstring>
#include "cma.h"

#define COMMAND_MAX_LENGTH 128

using namespace std;

//int main() {
//    // coder needs to ensure if the initial memory is less than INT_MAX
//    int size = 100000;
//    if (size <= INT_MAX) {
//        MemoryBlock mem(size);
//        mem.set_debug();
//        mem.request_memory(10000, "p1", 'F');
//        mem.request_memory(20000, "p2", 'F');
//        mem.request_memory(30000, "p3", 'F');
//        mem.request_memory(30000, "p4", 'F');
//        mem.print_info();
//        mem.release_memory("p4");
//        mem.print_info();
//        mem.request_memory(40000, "p4", 'F');
//        mem.print_info();
//        mem.request_memory(51400, "p114", 'W');
//
//        mem.request_memory(40000, "p4", 'F');
//        mem.release_memory("p1");
//        mem.request_memory(5000, "p20", 'F');
//        mem.request_memory(2500, "p21", 'F');
//        mem.release_memory("p20");
//        mem.release_memory("p3");
//        mem.print_info();
//
//        mem.request_memory(999, "p31", 'F');
//        mem.print_info();
//        mem.release_memory("p31");
//        mem.request_memory(999, "p32", 'W');
//        mem.print_info();
//        mem.release_memory("p32");
//        mem.request_memory(999, "p33", 'B');
//        mem.print_info();
//        mem.release_memory("p33");
//
//        mem.print_info();
//        mem.compact();
//        mem.print_info();
//    }
//    return 0;
//}

int main(int argc, char **argv) {
    // coder needs to ensure if the initial memory is less than INT_MAX
    if (argc != 2) {
        printf("Incorrect number of arguments.\n");
        return -1;
    }
    int mem_size = 0;
    sscanf(argv[1], "%u", &mem_size);
    bool debug = false;
    MemoryBlock mem(mem_size, debug);
    printf("The size of memory is initialized to %u bytes\n", mem_size);
    if (mem_size <= INT_MAX) {
        while (true) {
            printf("Contiguous_Memory_Allocation >");
            char op[COMMAND_MAX_LENGTH];
            scanf("%s", op);

            if (strcmp(op, "EXIT") == 0) {
                printf("[INFO] exiting the program.");
                break;
            }
            else if (strcmp(op, "STAT") == 0) {
                mem.print_info();
            }
            else if (strcmp(op, "RQ") == 0) {
                char name[COMMAND_MAX_LENGTH], strategy;
                int size_request;
                scanf("%s %u %c", name, &size_request, &strategy);
                int status_code = mem.request_memory(size_request, name, strategy);
                switch (status_code) {
                    case 0: {
                        printf("[INFO] process %s successfully created.\n", name);
                        break;
                    }
                    case -1: {
                        printf("[ERROR] unknown strategy.\n");
                        break;
                    }
                    case -2: {
                        printf("[ERROR] process name occupied.\n");
                        break;
                    }
                    case -3: {
                        printf("[ERROR] no satisfied empty blocks.\n");
                        break;
                    }
                    case -4: {
                        printf("[ERROR] requesting size is over maximum space.\n");
                        break;
                    }
                    case -5: {
                        printf("[ERROR] unknown error.\n");
                        break;
                    }
                    default: { // status_code = -6
                        printf("[ERROR] invalid name (empty string given, used for empty blocks only).\n");
                        break;
                    }
                }
            }
            else if (strcmp(op, "RL") == 0) {
                char name[COMMAND_MAX_LENGTH];
                scanf("%s", name);
                int status_code = mem.release_memory(name);
                switch (status_code) {
                    case 0: {
                        printf("[INFO] process %s successfully released.\n", name);
                        break;
                    }
                    case -2: {
                        printf("[ERROR] invalid name (empty string given, used for empty blocks only).\n");
                        break;
                    }
                    default: { // status_code = -1
                        printf("[ERROR] no such process.\n");
                        break;
                    }
                }
            }
            else if (strcmp(op, "C") == 0) {
                mem.compact();
            }
            else if (strcmp(op, "D") == 0) {
                mem.set_debug();
            }
            else {
                printf("============================================================\n");
                printf("[ERROR] unknown command.\n");
                printf("[INFO] operations:\n");
                printf("    RQ <process name> <memory size (in bytes)> <strategy>\n"
                       "        Request for a contiguous block of memory (available strategies are F, W and B)\n"
                       "    RL <process name>\n"
                       "        Release the process's contiguous block of memory\n"
                       "    C\n"
                       "        Compact unused holes of memory into one single block\n"
                       "    STAT\n"
                       "        Report the regions of free and allocated memory\n"
                       "    D\n"
                       "        Switch debugging mode\n"
                       "    EXIT\n"
                       "        Exit\n"
                );
                printf("============================================================\n");
            }
        }
        return 0;
    }
    else {
        printf("[ERROR] invalid argument of memory size.\n");
        return -1;
    }
}