#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//new directory at pathName
void mkdir(char pathName[]) {
    if (strcmp(pathName, "/") == 0 || strlen(pathName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64], dirName[64];
    struct NODE* parent = splitPath(pathName, baseName, dirName);

    if (parent == NULL) {
        return;  // printed by splitPath()
    }

    // Check if directory already exists
    struct NODE* child = parent->childPtr;
    while (child != NULL) {
        if (strcmp(child->name, baseName) == 0 && child->fileType == 'D') {
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
            return;
        }
        child = child->siblingPtr;
    }

    // new parent dir
    struct NODE* newNode = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newNode->name, baseName);
    newNode->fileType = 'D';             // 'D' = dir
    newNode->parentPtr = parent;
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;

    //new node as child/sibling
    if (parent->childPtr == NULL) {
        parent->childPtr = newNode;
    } else {
        struct NODE* last = parent->childPtr;
        while (last->siblingPtr != NULL) {
            last = last->siblingPtr;
        }
        last->siblingPtr = newNode;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}


// split full path into dirName and baseName - return parent node
struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    //root dir
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    //no slashes
    if (strchr(pathName, '/') == NULL) {
        strcpy(dirName, "");
        strcpy(baseName, pathName);
        return cwd;
    }

    //safe copies
    char tempPath[64], tempBase[64], tempDir[64];
    strcpy(tempDir, pathName);
    strcpy(tempBase, pathName);
    strcpy(tempPath, pathName);

    //dirName and baseName
    strcpy(dirName, dirname(tempDir));
    strcpy(baseName, basename(tempBase));

    //begin at root/cwd
    struct NODE* current = (pathName[0] == '/') ? root : cwd;

    //tokenize dirName - traverse path
    char tokenPath[128];
    strcpy(tokenPath, dirName);
    char* token = strtok(tokenPath, "/");

    while (token != NULL) {
        struct NODE* child = current->childPtr;
        int found = 0;

        while (child != NULL) {
            if (strcmp(child->name, token) == 0 && child->fileType == 'D') {
                found = 1;
                break;
            }
            child = child->siblingPtr;
        }

        if (!found) {
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }

        current = child;
        token = strtok(NULL, "/");
    }

    return current;
}

