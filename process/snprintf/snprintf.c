#include <stdio.h>

#define DESTINATION_SIZE 255

int main()
{

    char destination[DESTINATION_SIZE];
    snprintf(destination,DESTINATION_SIZE, "Hello! format number: %d - format string: %s, both go to destination. \n", 23423, "THISISATEST");

    printf("%s",destination);
    return 0;
}
