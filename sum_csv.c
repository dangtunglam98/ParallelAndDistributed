
// Name: Lam Dang
// For CS360 Parallel Programming
// Intro to C

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXVALUES 1000000


int sum(int a, int b){
    return a + b;
}

void sum_pointer(int a, int b, int *summation){
    *summation = sum(a,b);
}

void sum_array(int len,int a[], int b[], int ** sum){
    *sum = malloc(sizeof(int)*len);
    for(int i = 0; i < len; i++){
        (*sum)[i] = *a + *b;
        a++;
        b++;
    }
}

int compareArray(int len, int ** a, int ** b){
    for(int i = 0; i < len; i++){
        if((*a)[i] != (*b)[i]){
            return 0;
        }
    }

    return 1;
}

int main(int argc, char* argv[])
{
    FILE *infileHandler;
    FILE *canonHandler;
    char infileName[128];
    char canonName[128];
    int value_count = 0;

    int rowOne[MAXVALUES];
    int rowTwo[MAXVALUES];
    int valueRowOne;
    int valueRowTwo;

    if (argc < 3) {
        fprintf(stderr, "missing filename to read");
        return(-1);
    } else {
        strcpy(infileName, argv[1]);
        strcpy(canonName, argv[2]);
    }

    if (!(infileHandler = fopen(infileName, "r"))) {
		fprintf(stderr, "error opening input file %s\n", infileName);
		return(-1);
	}

    while (fscanf(infileHandler, "%d , %d", &rowOne[value_count], &rowTwo[value_count]) != EOF){
        value_count++;
    }

    fprintf(stderr, "read %d values from data\n", value_count); 
	fclose(infileHandler);


    
    int * sumArr;
    sum_array(value_count, rowOne, rowTwo, &sumArr);
    free(sumArr);

    if (!(canonHandler = fopen(canonName, "r"))) {
		fprintf(stderr, "error opening input file %s\n", canonName);
		return(-1);
	}


    int* canonArr = (int*)malloc(value_count*sizeof(int));
    int item_count = 0;
    while (fscanf(canonHandler, "%d", &canonArr[item_count]) != EOF){
        item_count++;
    }
    fprintf(stderr, "read %d values\n", item_count); 
    fclose(canonHandler);

    

    
    int success = 0;
    success = compareArray(value_count, &canonArr, &sumArr);
    if(success == 1){
        printf("Success");
    } else {
        printf("Failed");
    }
}