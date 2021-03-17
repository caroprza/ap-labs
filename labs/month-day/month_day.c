#include <stdio.h>
#include <stdlib.h>

static char daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};
int month, day;

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday);
char *month_name(int n);

int main(int argc, char **argv) {
    if (argc < 3){
        printf("Faltan elementos");
        return 0;
    }
    int year = atoi(argv[1]);
    int yearday = atoi(argv[2]);
    if (yearday > 366){
        printf("Un año tiene max 366 dias\n");
        return 0;
    }
    int *pmonth, *pday;
    pmonth = &month;
    pday = &day;
    month_day(year, yearday, pmonth, pday);
    char *m = month_name(month);
    printf("%s %d, %d\n", m, day, year);

}

char *month_name(int n) {

   static char *name[] = {
       "Illegal month",
       "Jan", "Feb", "Mar",
       "Apr", "May", "Jun",
       "Jul", "Aug", "Sep",
       "Oct", "Nov", "Dec"
   };

   return (n < 1 || n > 12) ? name[0] : name[n];
}

void month_day(int year, int yearday, int *pmonth, int *pday){
    int i, leap;
    leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
    for (i = 1; i < 13; i++){
        if (yearday <= daytab[leap][i]){
            *pmonth = i;
            *pday = yearday;
            return;
        }
        else {
            yearday -= daytab[leap][i];
        }
    }
}
