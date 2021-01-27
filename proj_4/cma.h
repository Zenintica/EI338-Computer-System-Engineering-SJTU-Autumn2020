#ifndef _CMA_
#define _CMA_

#include <iostream>
#include <string>
#include <limits.h>

using namespace std;

class MemoryBlock {
private:
    struct MemBlock {
        int number;
        int low;
        int high;
        string process_name;
        bool is_empty;
        MemBlock *prev;
        MemBlock *next;

        MemBlock(int no, int lo, int hi, string p_name, bool emp) {
            number = no;
            low = lo;
            high = hi;
            process_name = p_name;
            is_empty = emp;
            prev = next = nullptr;
        }
    };

    MemBlock *init_block;
    int max_size;
    int blocks_count;

    bool debug;

public:
    MemoryBlock(int init_size = INT_MAX, bool de = false) {
        blocks_count = 1;
        init_block = new MemBlock(blocks_count, 0, init_size - 1, "", true);
        max_size = init_size;
        debug = de;
    }

    void print_info() {
        printf("============================================================\n");
        string info_debug = debug ? "on" : "off";
        printf("Debug mode: %s\n", info_debug.c_str());
        printf("Maximum Bytes Allocated: %010u\n", max_size);
        MemBlock *cursor = init_block;
        while (cursor) {
            string string_status = cursor->is_empty ? "Empty" : "Process " + cursor->process_name;
            printf("Address[%010u] - address[%010u]: %s\n", cursor->low, cursor->high, string_status.c_str());
            cursor = cursor->next;
        }
        printf("============================================================\n");
    }

    void set_debug() {
        debug = !debug;
        string de_s = debug ? "ON" : "OFF";
        printf("[INFO] debugging mode is now %s.\n", de_s.c_str());
    }

    bool name_is_occupied(string name) {
        MemBlock *cursor = init_block;
        while (cursor) {
            if (cursor->process_name == name) {
                if (debug)
                    printf("---\n[DEBUG] info from method name_is_occupied:\n"
                           "\trequest from %s not satisfied: name occupied.\n---\n", name.c_str());
                return true;
            }
            cursor = cursor->next;
        }
        return false;
    }

    int request_memory(int request_size, string name, char strategy) {
        // error code:
        //  -1: unknown strategy;
        //  -2: process name occupied;
        //  -3: no satisfied empty blocks;
        //  -4: over maximum space;
        //  -5: unknown error;
        //  -6: invalid name (empty string, used for empty blocks);
        if (name.empty()) {
            if (debug)
                printf("---\n[DEBUG] info from method request_memory: \n"
                       "\trequest from %s not satisfied: invalid name "
                       "(empty string, used for empty blocks).\n---\n", name.c_str());
            return -6;
        }
        if (request_size > max_size) {
            if (debug)
                printf("---\n[DEBUG] info from method request_memory: \n"
                       "\trequest from %s not satisfied: size greater than maximum.\n---\n", name.c_str());
            return -4;
        }
        if (name_is_occupied(name)) return -2;
        switch (strategy) {
            case 'F': {
                if (debug)
                    printf("---\n[DEBUG] info from method request_memory: \n"
                           "\ttraversing blocks for request from %s with strategy \"first fit\"\n", name.c_str());
                MemBlock *cursor = init_block;
                while (cursor) {
                    if (debug) {
                        string stat_emp = cursor->is_empty ? "yes" : "no";
                        printf("\tvisiting block number %u: \n"
                               "\t\tname: %s;\n"
                               "\t\tis_empty? %s;\n"
                               "\t\taddress range: %010u, %010u\n",
                               cursor->number, cursor->process_name.c_str(), stat_emp.c_str(), cursor->low,
                               cursor->high);
                    }
                    if (cursor->is_empty && cursor->high - cursor->low + 1 >= request_size) {
                        if (cursor->high - cursor->low + 1 > request_size) {
                            blocks_count++;
                            MemBlock *new_block = new MemBlock(blocks_count, cursor->low,
                                                               cursor->low + request_size - 1, name, false);
                            cursor->low = cursor->low + request_size;
                            if (cursor->prev) {
                                new_block->prev = cursor->prev;
                                cursor->prev->next = new_block;
                            } else {
                                init_block = new_block;
                            }
                            new_block->next = cursor;
                            cursor->prev = new_block;
                            if (debug) printf("\trequest from %s satisfied\n---\n", name.c_str());
                            return 0;
                        } else {
                            cursor->is_empty = false;
                            cursor->process_name = name;
                            if (debug) printf("\trequest from %s satisfied\n---\n", name.c_str());
                            return 0;
                        }
                    }
                    cursor = cursor->next;
                }
                if (debug) printf("\tnot satisfied: no suitable blocks.\n---\n");
                return -3;
            }
            case 'B': {
                if (debug)
                    printf("---\n[DEBUG] info from method request_memory: \n"
                           "\ttraversing blocks for request from %s with strategy \"best fit\"\n", name.c_str());

                MemBlock *cursor = init_block;
                int current_least_waste = max_size;
                int optimal_number = -1;
                bool flag = false;

                while (cursor) {
                    if (debug) {
                        string stat_emp = cursor->is_empty ? "yes" : "no";
                        printf("\tvisiting block number %u: \n"
                               "\t\tname: %s;\n"
                               "\t\tis_empty? %s;\n"
                               "\t\taddress range: %010u, %010u\n",
                               cursor->number, cursor->process_name.c_str(), stat_emp.c_str(), cursor->low,
                               cursor->high);
                    }
                    if (cursor->is_empty && cursor->high - cursor->low + 1 >= request_size) {
                        flag = true;
                        if (cursor->high - cursor->low + 1 - request_size <= current_least_waste) {
                            current_least_waste = cursor->high - cursor->low + 1 - request_size;
                            optimal_number = cursor->number;
                        }
                    }
                    cursor = cursor->next;
                }

                if (!flag) {
                    if (debug) printf("\tnot satisfied: no suitable blocks.\n---\n");
                    return -3;
                }
                if (optimal_number == -1) {
                    if (debug) printf("\tnot satisfied: unknown errors.\n---\n");
                    return -5;
                }
                MemBlock *cursor_new = init_block;
                if (debug) {
                    printf("\tfinding best-fit block with number %u\n", optimal_number);
                }
                while (cursor_new) {
                    if (debug) {
                        string stat_emp = cursor_new->is_empty ? "yes" : "no";
                        printf("\tvisiting block number %u: \n"
                               "\t\tname: %s;\n"
                               "\t\tis_empty? %s;\n"
                               "\t\taddress range: %010u, %010u\n",
                               cursor_new->number, cursor_new->process_name.c_str(), stat_emp.c_str(), cursor_new->low,
                               cursor_new->high);
                    }
                    if (cursor_new->number == optimal_number) {
                        if (debug)
                            printf("\tfound empty block with address range %010u, %010u\n",
                                   cursor_new->low, cursor_new->high);
                        if (cursor_new->high - cursor_new->low + 1 > request_size) {
                            blocks_count++;
                            MemBlock *new_block = new MemBlock(blocks_count, cursor_new->low,
                                                               cursor_new->low + request_size - 1, name, false);
                            cursor_new->low = cursor_new->low + request_size;
                            if (cursor_new->prev) {
                                new_block->prev = cursor_new->prev;
                                cursor_new->prev->next = new_block;
                            } else {
                                init_block = new_block;
                            }
                            new_block->next = cursor_new;
                            cursor_new->prev = new_block;
                            if (debug) printf("\trequest from %s satisfied\n---\n", name.c_str());
                            return 0;
                        } else {
                            cursor_new->is_empty = false;
                            cursor_new->process_name = name;
                            if (debug) printf("\trequest from %s satisfied\n---\n", name.c_str());
                            return 0;
                        }
                    }
                    cursor_new = cursor_new->next;
                }
                if (debug) printf("\tnot satisfied: unknown errors.\n---\n");
                return -5;
            }
            case 'W': {
                if (debug)
                    printf("---\n[DEBUG] info from method request_memory: \n"
                           "\ttraversing blocks for request from %s with strategy \"worst fit\"\n", name.c_str());

                MemBlock *cursor = init_block;
                int current_most_waste = -1;
                int optimal_number = -1;
                bool flag = false;
                while (cursor) {
                    if (debug) {
                        string stat_emp = cursor->is_empty ? "yes" : "no";
                        printf("\tvisiting block number %u: \n"
                               "\t\tname: %s;\n"
                               "\t\tis_empty? %s;\n"
                               "\t\taddress range: %010u, %010u\n",
                               cursor->number, cursor->process_name.c_str(), stat_emp.c_str(), cursor->low,
                               cursor->high);
                    }
                    if (cursor->is_empty && cursor->high - cursor->low + 1 >= request_size) {
                        flag = true;
                        if (cursor->high - cursor->low + 1 - request_size >= current_most_waste) {
                            current_most_waste = cursor->high - cursor->low + 1 - request_size;
                            optimal_number = cursor->number;
                        }
                    }
                    cursor = cursor->next;
                }

                if (!flag) {
                    if (debug) printf("\tnot satisfied: no suitable blocks.\n---\n");
                    return -3;
                }
                if (optimal_number == -1) {
                    if (debug) printf("\tnot satisfied: unknown errors.\n---\n");
                    return -5;
                }

                MemBlock *cursor_new = init_block;
                if (debug) {
                    printf("\tfinding best-fit block with number %u\n", optimal_number);
                }
                while (cursor_new) {
                    if (debug) {
                        string stat_emp = cursor_new->is_empty ? "yes" : "no";
                        printf("\tvisiting block number %u: \n"
                               "\t\tname: %s;\n"
                               "\t\tis_empty? %s;\n"
                               "\t\taddress range: %010u, %010u\n",
                               cursor_new->number, cursor_new->process_name.c_str(), stat_emp.c_str(), cursor_new->low,
                               cursor_new->high);
                    }
                    if (cursor_new->number == optimal_number) {
                        if (debug)
                            printf("\tfound empty block with address range %010u, %010u\n",
                                   cursor_new->low, cursor_new->high);
                        if (cursor_new->high - cursor_new->low + 1 > request_size) {
                            blocks_count++;
                            MemBlock *new_block = new MemBlock(blocks_count, cursor_new->low,
                                                               cursor_new->low + request_size - 1, name, false);
                            cursor_new->low = cursor_new->low + request_size;
                            if (cursor_new->prev) {
                                new_block->prev = cursor_new->prev;
                                cursor_new->prev->next = new_block;
                            } else {
                                init_block = new_block;
                            }
                            new_block->next = cursor_new;
                            cursor_new->prev = new_block;
                            if (debug) printf("\trequest from %s satisfied\n---\n", name.c_str());
                            return 0;
                        } else {
                            cursor_new->is_empty = false;
                            cursor_new->process_name = name;
                            if (debug) printf("\trequest from %s satisfied\n---\n", name.c_str());
                            return 0;
                        }
                    }
                    cursor_new = cursor_new->next;
                }

                if (debug) printf("\tnot satisfied: unknown errors.\n---\n");
                return -5;
            }
            default: {
                if (debug)
                    printf("---\n[DEBUG] info from method request_memory: \n"
                           "\trequest from %s not satisfied: unknown strategy.\n---\n", name.c_str());
                return -1;
            }
        }
    }

    int release_memory(string name) {
        // error code:
        //  -1: no such process;
        //  -2: invalid name (empty string, used for empty blocks);
        if (name.empty()) {
            if (debug)
                printf("---\n[DEBUG] info from method release_memory:\n"
                       "\trequest from %s not satisfied: invalid name "
                       "(empty string, used for empty blocks).\n---\n", name.c_str());
            return -2;
        }
        if (debug)
            printf("---\n[DEBUG] info from method release_memory:\n"
                   "\ttraversing blocks for releasing process %s\n", name.c_str());
        MemBlock *cursor = init_block;
        while (cursor) {
            if (cursor->process_name == name) {
                if (debug) printf("\tfound process %s\n", name.c_str());
                if (cursor->next) {
                    if (cursor->next->is_empty) {
                        cursor->high = cursor->next->high;
                        MemBlock *temp = cursor->next->next;
                        delete cursor->next;
                        if (temp) temp->prev = cursor;
                        cursor->next = temp;
                    }
                }
                if (cursor->prev) {
                    if (cursor->prev->is_empty) {
                        cursor->low = cursor->prev->low;
                        MemBlock *temp = cursor->prev->prev;
                        delete cursor->prev;
                        if (temp) temp->next = cursor;
                        cursor->prev = temp;
                    }
                }
                cursor->is_empty = true;
                cursor->process_name = "";
                if (debug) printf("\tprocess resource released\n---\n");
                return 0;
            }
            cursor = cursor->next;
        }
        if (debug)
            printf("---\n[DEBUG] info from method release_memory:\n"
                   "\trequest from %s not satisfied: no such process.\n---\n", name.c_str());
        return -1;
    }

    void compact() {
        if (debug)
            printf("---\n[DEBUG] info from method compact:\n"
                   "\ttraversing distributed empty blocks...\n");
        MemBlock *cursor = init_block;
        int empty_block_count = 0;
        int offset = 0;
        while (cursor->next) {
            if (cursor->is_empty) {
                empty_block_count++;
                offset += (cursor->high - cursor->low + 1);
                if (cursor->prev) {
                    cursor = cursor->next;
                    cursor->prev->prev->next = cursor;
                    cursor->prev = cursor->prev->prev;
                } else {
                    cursor = cursor->next;
                    cursor->prev = nullptr;
                    init_block = cursor;
                }
            } else {
                cursor->low -= offset;
                cursor->high -= offset;
                cursor = cursor->next;
            }
        }
        if (!cursor) {
            printf("[INFO] not satisfied: unknown errors.\n");
            return;
        }
        if (cursor->is_empty) {
            empty_block_count++;
            cursor->low -= offset;
        } else {
            cursor->low -= offset;
            cursor->high -= offset;
            blocks_count++;
            MemBlock *new_block = new MemBlock(blocks_count, cursor->high + 1, cursor->high + offset, "", true);
            new_block->prev = cursor;
            new_block->next = nullptr;
            cursor->next = new_block;
        }
        printf("[INFO] found %u empty blocks, all compacted and "
               "moved to the highest address of the memory.\n", empty_block_count);
    }
};

#endif