#include <stdio.h>
#include <string.h>

int main() {
    char buffer[50];
    char* s = "geeksforgeeks";
 
    // Counting the character and storing
    // in buffer using snprintf
    printf("Writing %s onto buffer"
           " with capacity 6",
           s);
    int j = snprintf(buffer, 6, "%s\n", s);
 
    // Print the string stored in buffer and
    // character count
    printf("\nString written on "
           "buffer = %s", buffer);
    
}




