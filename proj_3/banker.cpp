#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4
#define INIT_FILE "resources.txt"

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

int vector_leq(const int *a, const int *b, int n) {
    for (int i = 0; i != n; ++i) {
        if (a[i] > b[i]) {
            return 0;
        }
    }
    return 1;
}

int is_safe() {
    int work[NUMBER_OF_RESOURCES];
    bool finish[NUMBER_OF_CUSTOMERS];
    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
        work[i] = available[i];
    }
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        finish[i] = false;
    }
    for (int round = 0; round < NUMBER_OF_CUSTOMERS; ++round) {
        bool flag = false;
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
            if (!finish[i] && vector_leq(need[i], work, NUMBER_OF_RESOURCES)) {
                flag = true;
                finish[i] = true;
                for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) {
                    work[j] += allocation[i][j];
                }
                break;
            }
        }
        if (!flag) return 0;
    }
    return 1;
}

int request_resources(int customer, int request[NUMBER_OF_RESOURCES]) {
    int err = 0;
    for (int i = 0; i != NUMBER_OF_RESOURCES; ++i) {
        if (request[i] > available[i]) {
            printf("No enough resources to allocate: <customer: %d, resource: %d, available: %d, to request: %d>\n",
                    customer, i, available[i], request[i]);
            err = 1;
        }
        if (err != 0) {
            while (i--) {
                available[i] += request[i];
                allocation[customer][i] -= request[i];
                need[customer][i] += request[i];
            }
            return err;
        }
        available[i] -= request[i];
        allocation[customer][i] += request[i];
        need[customer][i] -= request[i];
    }
    if (!is_safe()) {
        printf("Unsafe state after request!\n");
        for (int i = 0; i != NUMBER_OF_RESOURCES; ++i) {
            available[i] += request[i];
            allocation[customer][i] -= request[i];
            need[customer][i] += request[i];
        }
        return -1;
    }
    return 0;
}

int release_resources(int customer, int release[NUMBER_OF_RESOURCES]) {
    for (int i = 0; i != NUMBER_OF_RESOURCES; ++i) {
        if (release[i] < 0 || release[i] > allocation[customer][i]) {
            printf("Invalid number of resources to release: <customer: %d, resource: %d, allocated: %d, to release: %d>\n",
                    customer, i, allocation[customer][i], release[i]);
            while (i--) {
                allocation[customer][i - 1] += release[i - 1];
                available[i] -= release[i];
            }
            return -1;
        }
        allocation[customer][i] -= release[i];
        available[i] += release[i];
    }
    return 0;
}

void display_usage() {
    printf("=============================================================\n");
    printf("<this program> <available resources of size %d>\n", NUMBER_OF_RESOURCES);
    printf("Operations:\n");
    printf("    Request resources: RQ <customer> <resources>\n");
    printf("    Release resources: RL <customer> <resources>\n");
    printf("    Display resources: *\n");
    printf("=============================================================\n");
}

void display_resources() {
    printf("=============================================================\n");
    printf("Available resources:\n");
    for (int i = 0; i != NUMBER_OF_RESOURCES; ++i) {
        printf("%d ", available[i]);
    }
    printf("\n");
    printf("-------------------------------------------------------------\n");
    printf("Maximum resources for each customer:\n");
    for (int customer = 0; customer != NUMBER_OF_CUSTOMERS; ++customer) {
        printf("%d: ", customer);
        for (int r = 0; r != NUMBER_OF_RESOURCES; ++r) {
            printf("%d ", maximum[customer][r]);
        }
        printf("\n");
    }
    printf("-------------------------------------------------------------\n");
    printf("Allocated resources for each customer:\n");
    for (int customer = 0; customer != NUMBER_OF_CUSTOMERS; ++customer) {
        printf("%d: ", customer);
        for (int r = 0; r != NUMBER_OF_RESOURCES; ++r) {
            printf("%d ", allocation[customer][r]);
        }
        printf("\n");
    }
    printf("-------------------------------------------------------------\n");
    printf("Needed resources for each customer:\n");
    for (int customer = 0; customer != NUMBER_OF_CUSTOMERS; ++customer) {
        printf("%d: ", customer);
        for (int r = 0; r != NUMBER_OF_RESOURCES; ++r) {
            printf("%d ", need[customer][r]);
        }
        printf("\n");
    }
    printf("=============================================================\n");
}

int main(int argc, char *argv[]) {
    if (argc != 1 + NUMBER_OF_RESOURCES) {
        printf("Incorrect number of parameters.\n");
        display_usage();
        return -1;
    }
    for (int i = 0; i != NUMBER_OF_RESOURCES; ++i) {
        available[i] = atoi(argv[i + 1]);
    }
    FILE *f = fopen(INIT_FILE, "r");
    if (f == NULL) {
        printf("Unable to open file: %s\n", INIT_FILE);
        display_usage();
        return -1;
    }
    for (int c = 0; c != NUMBER_OF_CUSTOMERS; ++c) {
        for (int r = 0; r != NUMBER_OF_RESOURCES; ++r) {
            fscanf(f, "%d", &maximum[c][r]);
            need[c][r] = maximum[c][r];
        }
    }
    fclose(f);
    printf("Initialized, ready to run.\n");

    char op[5];
    printf("Banker_Algorithm> ");
    while (scanf("%s", op) == 1) {
        if (strcmp(op, "RQ") == 0) {
            int request[NUMBER_OF_RESOURCES], customer;
            scanf("%d", &customer);
            for (int i = 0; i != NUMBER_OF_RESOURCES; ++i) {
                scanf("%d", &request[i]);
            }
            if (request_resources(customer, request) != 0) {
                printf("DENIED.\n");
            } else {
                printf("SUCCESS.\n");
            }
        } else if (strcmp(op, "RL") == 0) {
            int release[NUMBER_OF_RESOURCES], customer;
            scanf("%d", &customer);
            for (int i = 0; i != NUMBER_OF_RESOURCES; ++i) {
                scanf("%d", &release[i]);
            }
            if (release_resources(customer, release) != 0) {
                printf("DENIED.\n");
            } else {
                printf("SUCCESS.\n");
            }
        } else if (strcmp(op, "*") == 0) {
            display_resources();
        } else {
            display_usage();
        }
        printf("Banker_Algorithm> ");
    }
    return 0;
}