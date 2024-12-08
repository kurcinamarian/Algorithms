#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 100

//information about characters
struct char_info {
    char character;
    int frequency;
    int bit_number;
    char code[MAX];
};

//information about groups
struct group_info {
    char character[MAX];
    int frequency;
};

//queue
struct group_info heap[MAX];
int size;

//add to queue
void add(struct group_info group) {
    int i = size;
    size++;
    //heapify the queue
    while (i > 0 && heap[(i - 1) / 2].frequency > group.frequency) {
        heap[i] = heap[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap[i] = group;
}

//get minimum from queue
struct group_info get_min() {
    struct group_info minNode = heap[0];
    size--;
    //heapify the queue
    if (size > 0) {
        struct group_info lastNode = heap[size];
        int i = 0;
        while (i * 2 + 1 < size) {
            int child = 2 * i + 1;
            if (child + 1 < size && heap[child + 1].frequency < heap[child].frequency)
                child++;
            if (lastNode.frequency < heap[child].frequency)
                break;
            heap[i] = heap[child];
            i = child;
        }
        heap[i] = lastNode;
    }
    return minNode;
}

//increment all bit numbers for all characters in group
void increment_bit_numbers(struct char_info *info, int n, const char *characters) {
    for (int i = 0; characters[i] != '\0'; i++) {
        for (int j = 0; j < n; j++) {
            if (info[j].character == characters[i]) {
                info[j].bit_number++;
                break;
            }
        }
    }
}

int main() {
    int frequencies[] = {45, 13, 12, 16, 9, 5};
    char characters[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    //get number of characters
    int n = sizeof(frequencies) / sizeof(frequencies[0]);
    size = 0;
    //sort characters based on frequencies
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (frequencies[i] < frequencies[j]) {
                int temp_frequency = frequencies[i];
                frequencies[i] = frequencies[j];
                frequencies[j] = temp_frequency;

                char temp_char = characters[i];
                characters[i] = characters[j];
                characters[j] = temp_char;
            }
        }
    }
    //information about the characters
    struct char_info info[n];
    char type;
    //select type
    printf("Select type of bit number generator (a/b)");
    scanf(" %c", &type);
    if(type == 'a'){
        //group information
        struct group_info groups[n];
        //inicialization of groups and characters
        for (int i = 0; i < n; i++) {
            info[i].frequency = frequencies[i];
            info[i].character = characters[i];
            info[i].bit_number = 0;
            groups[i].frequency = frequencies[i];
            memset(groups[i].character, '\0', MAX);
            groups[i].character[0] = characters[i];
        }
        //add groups to queue
        for (int i = 0; i < n; i++) {
            add(groups[i]);
        }
        //for all groups merge till there is only one left
        for (int i = n; i > 1; i--) {
            //get 2 smallest from queue
            struct group_info first = get_min();
            struct group_info second = get_min();
            printf("%s %s\n", first.character, second.character);
            //merged group
            struct group_info new_group;
            new_group.frequency = first.frequency + second.frequency;
            //merge characters of groups
            int k = 0;
            for (int j = 0; first.character[j] != '\0'; j++, k++) {
                new_group.character[k] = first.character[j];
            }
            for (int j = 0; second.character[j] != '\0'; j++, k++) {
                new_group.character[k] = second.character[j];
            }
            new_group.character[k] = '\0';
            //increment code lenght for all characters in new group 
            increment_bit_numbers(info, n, first.character);
            increment_bit_numbers(info, n, second.character);
            //add to queue
            add(new_group);
        }
        //sort again if error
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (info[j].bit_number > info[j + 1].bit_number) {
                
                    struct char_info temp = info[j];
                    info[j] = info[j + 1];
                    info[j + 1] = temp;
                }       
            }
        }
        //print code bit lenghts
        printf("Final bit numbers for each character:\n");
        for (int i = 0; i < n; i++) {
            printf("Character: %c, Frequency: %d, Bit Number: %d\n",
                info[i].character, info[i].frequency, info[i].bit_number);
        }
    }
    else if (type == 'b'){
        int total = 0;
        //get total frequency
        for(int i = 0; i < n; i++){
            total += frequencies[i];
        }
        //for all characters get bit lenghts numbers
        for(int i = 0; i < n; i++){
            info[i].frequency = frequencies[i];
            info[i].character = characters[i];
            float p = (float)info[i].frequency/(float)total;
            info[i].bit_number = (int)ceil(-log2(p));
        }
         //print code bit lenghts
        printf("Final bit numbers for each character:\n");
        for (int i = 0; i < n; i++) {
            printf("Character: %c, Frequency: %d, Bit Number: %d\n",
                info[i].character, info[i].frequency, info[i].bit_number);
        }
    }
    //increment first character code lenght
    if(info[0].bit_number == 1){
        info[0].bit_number++;
    }
    //code letters
    int c = 2;
    int temp = 0;
    for (int i = 0; i < n; i++) {
        int len = info[i].bit_number;
        //if change of code lenght, bit movement to the left
        while(c<len){
            temp *= 2;
            c++;
        }
        //encode to binary
        int temp2 = temp;
        char code[MAX];
        for (int j = len-1; j >= 0; j--) {
            code[j] = (temp2 % 2 == 0) ? '0' : '1'; 
            temp2 /= 2;
        }
        code[len] = '\0';
        strcpy(info[i].code, code); 
        temp++;
    }
    //print codes
    printf("Huffman Code:\n");
    for (int i = 0; i < n; i++) {
        printf("%c: %s\n", info[i].character, info[i].code);
    }
    //decode code
    printf("Enter a string to decode: \n");
    char encoded_string[MAX] = {'\0'};
    scanf("%s", encoded_string);
    char temp_string[MAX] = {'\0'};
    int temp_pos = 0;
    //go through encoded string
    for (int i = 0; encoded_string[i] != '\0'; i++) {
        temp_string[temp_pos] = encoded_string[i];
        temp_pos++;
        temp_string[temp_pos] = '\0'; 
        //compare with all letters
        for (int j = 0; j < n; j++){
            //if found reset temp string
            if (strcmp(temp_string, info[j].code) == 0){
                printf("%c", info[j].character);
                temp_pos = 0;
                break;
            }
        }
    }
    //string encoding
    printf("\nEnter a string to encode: \n");
    char string[MAX] = {'\0'};
    scanf("%s", string);
    //for letters in string find letter and print its code
    for (int i = 0; string[i] != '\0'; i++) {
        for (int j = 0; j < n; j++) {
            if (string[i] == info[j].character) {
                printf("%s", info[j].code);
                break;
            }
        }
    }
    return 0;
}

