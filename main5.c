#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 128

typedef struct Task Task;
typedef struct Node Node;

struct Task {
    int taskId;
    char* description;
    float resourceUtilization;
};

struct Node {
    Task task;
    Node* next;
};

Task readTaskFromFile(FILE* f) {
    
    Task task;
    char buffer[LINESIZE];

    fgets(buffer, LINESIZE, f);
    task.taskId = atoi(buffer);

    fgets(buffer, LINESIZE, f);
    task.description = malloc((strlen(buffer)) * sizeof(char));
    strncpy(task.description, buffer, strlen(buffer) - 1);
    task.description[strlen(buffer) - 1] = '\0';

    fgets(buffer, LINESIZE, f);
    task.resourceUtilization = atof(buffer);

    return task;
}

void readTasksFromFile(const char* filename, Task** tasksArray, int* tasksNumber) {

    FILE* f;

    f = fopen(filename, "r");

    char buffer[LINESIZE];
    fgets(buffer, LINESIZE, f);

    *tasksNumber = atoi(buffer);

    *tasksArray = malloc((*tasksNumber) * sizeof(Task));

    for (int i = 0; i < *tasksNumber; i++) {
        (*tasksArray)[i] = readTaskFromFile(f);
    }

    fclose(f);

}

void printTaskToConsole(Task task) {
    printf("Task %d\n\tDescription: %s\n\tResource Utilization: %.2f%%\n", task.taskId, task.description, task.resourceUtilization);
}

Task deepCopyTask(Task task) {
    Task copyTask;
    copyTask.taskId = task.taskId;

    copyTask.description = malloc((strlen(task.description) + 1) * sizeof(char));
    strcpy(copyTask.description, task.description);

    copyTask.resourceUtilization = task.resourceUtilization;
    return copyTask;
}

void insertAtBeginning(Node** head, Task task) {

    Node* newNode = malloc(sizeof(Node));

    newNode->task = task;
    newNode->next = *head;

    *head = newNode;

}

void insertAtEnd(Node** head, Task task) {

    Node* newNode = malloc(sizeof(Node));
    newNode->task = task;
    newNode->next = NULL;

    if (*head) {

        Node* aux = *head;

        while (aux->next) {
            aux = aux->next;
        }

        aux->next = newNode;
    }
    else {

        *head = newNode;
    }

}

void parseListAndPrint(Node* head) {
    if (head) {
        while (head) {
            printTaskToConsole(head->task);
            head = head->next;
        }
    }
    else {
        printf("List is empty\n\n");
    }
}

Task deleteFromBeginning(Node** head) {
    
    if (*head) {
        Node* currentHead = *head;
        Task deletedTask = currentHead->task;

        *head = (*head)->next;
        free(currentHead);

        return deletedTask;
    }
    else {
        Task deletedTask = { .taskId = 0, .description = "ERROR TASK", .resourceUtilization = 0 };
        return deletedTask;
    }
 
}

Task deleteFromEnd(Node** head) {
    if (*head) {
        if ((*head)->next) {

            Node* aux = *head;

            while (aux->next->next) {
                aux = aux->next;
            }

            
            Node* lastNode = aux->next;
            
            Task deletedTask = lastNode->task;
            aux->next = NULL;
            free(lastNode);

            return deletedTask;

        }
        else {
            Task deletedTask = (*head)->task;

            free(*head);
            *head = NULL;

            return deletedTask;
        }
    }
    else {
        Task deletedTask = { .taskId = 0, .description = "ERROR TASK", .resourceUtilization = 0 };
        return deletedTask;
    }
}

int main() {

    int tasksNumber;
    Task* tasksArray;

    readTasksFromFile("scheduler.txt", &tasksArray, &tasksNumber);

    for (int i = 0; i < tasksNumber; i++) {
        printTaskToConsole(tasksArray[i]);
    }

    Node* head = NULL;

    printf("----------------------------\n\n");

    insertAtBeginning(&head, deepCopyTask(tasksArray[0]));
    insertAtBeginning(&head, deepCopyTask(tasksArray[1]));
    insertAtEnd(&head, deepCopyTask(tasksArray[2]));

    parseListAndPrint(head);

    printf("----------------------------\n\n");

    Task deletedTask = deleteFromBeginning(&head);
    printf("Deleted element:\n");
    printTaskToConsole(deletedTask);

    free(deletedTask.description);

    //printTaskToConsole(tasksArray[1]);
    printf("----------------------------\n\n");

    deletedTask = deleteFromEnd(&head);
    printf("Deleted element:\n");
    printTaskToConsole(deletedTask);


    /*printf("----------------------------\n\n");
    parseListAndPrint(head);

    printf("----------------------------\n\n");
    deletedTask = deleteFromBeginning(&head);
    deletedTask = deleteFromBeginning(&head);

    parseListAndPrint(head);

    printf("----------------------------\n\n");
    deletedTask = deleteFromBeginning(&head);
    printTaskToConsole(deletedTask);*/


    return 0;
}
