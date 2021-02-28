#include <stdio.h>

int main(){
    int i, tmp;
    char c, word[100];

    i = 0;

    while ((c = getchar()) != EOF) {

        word[i] = c;
        i++;
    }


    i -= 1;

    for (int j = 0;  j < i/2; j++) {
        tmp = word[j];
        word[j] = word[i - j - 1];
        word[i - j - 1] = tmp;
    }

    printf("%s", word);
    return 0;
}
