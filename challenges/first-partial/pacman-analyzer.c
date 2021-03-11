#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define REPORT_FILE "packages_report.txt"

struct Package {
    char *name,
         *install_date, 
         *last_update,
         *removal_date;
    int updates;

};

struct Package *packages;
int installed, removed, upgraded, current, position;
int alpm, pacman, scripttlet;
char *new, *old;

void analizeLog(char *logFile, char *report);
void registerPackage(char *name, char *date);
void updatePackage(char *name, char *date);
void insertRemoved(char *name, char *date);
int searchPackage(char *name);
void writeReport(char *report);

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./pacman-analizer.o \n");
	return 1;
    }

    if(argc < 5){
        printf("Paramentros incompletos\n");
        return 1;
    }

    analizeLog(argv[2], REPORT_FILE);

    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    packages = (struct Package*)malloc(3000 * sizeof(struct Package));
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char *word, *date, *name;

    fp = fopen(logFile, "r");
    if (fp == NULL){
        printf("Archivos no validos\n");
        printf("Value of errno: %d\n ", errno);
        return;

    }

    while ((read = getline(&line, &len, fp)) != -1) {
        date = strsep(&line, "]");

        while ((word = strsep(&line, " ")) != NULL){
            if (strcmp(word, "installed") == 0){
                name = strsep(&line, " ");
                if (installed == 0){
                    old = name;
                }
                registerPackage(name, date);
                installed++;
                new = name;
            }
            else if (strcmp(word, "removed") == 0){
                name = strsep(&line, " ");
                insertRemoved(name, date);
                removed++;
            }
            else if (strcmp(word, "upgraded") == 0){
                name = strsep(&line, " ");
                updatePackage(name, date);
            }
            else if (strcmp(word, "[ALPM]") == 0){
                alpm++;
            }
            else if (strcmp(word, "[PACMAN]") == 0){
                pacman++;
            }
            else if (strcmp(word, "[ALPM-SCRIPTLET]") == 0){
                scripttlet++;
            }
        }

    }

    writeReport(report);

    printf("Report is generated at: [%s]\n", report);

    fclose(fp);
}

void registerPackage(char *name, char *date){
    struct Package p = {.name = name, .install_date = date, .last_update = "-", .removal_date = "-"};
    packages[position] = p;
    position++;
}

void updatePackage(char *name, char *date){
    int i = searchPackage(name);
    if (packages[i].updates == 0){
        upgraded++;
    }
    int up = packages[i].updates + 1;
    packages[i] = (struct Package){.name = name, .install_date = packages[i].install_date, .removal_date = packages[i].removal_date, .last_update = date, .updates = up};
}

void insertRemoved(char *name, char *date){
    int i = searchPackage(name);
    packages[i] = (struct Package){.name = name, .install_date = packages[i].install_date, .removal_date = date, .last_update = packages[i].last_update, .updates = packages[i].updates};
}

int searchPackage(char *name){
    for (int i = 0; i < position; i++){
        if (strcmp(packages[i].name, name) == 0){
            return i;
        }
    }
    return 0;
}

void writeReport(char *report){

    FILE *fptr;

   // use appropriate location if you are using MacOS or Linux
   fptr = fopen("packages_report.txt","w");

   if(fptr == NULL)
   {
      printf("Error!");   
      exit(1);             
   }
   current = installed - removed;

   fprintf(fptr, "Pacman Packages Report\n");
   fprintf(fptr, "----------------------\n");
   fprintf(fptr, "- Installed packages : %d\n", installed);
   fprintf(fptr, "- Removed packages   : %d\n", removed);
   fprintf(fptr, "- Upgraded packages  : %d\n", upgraded);
   fprintf(fptr, "- Current installed  : %d\n", current);
   fprintf(fptr, "-------------\n");
   fprintf(fptr, "General Stats\n");
   fprintf(fptr, "-------------\n");
   fprintf(fptr, "- Oldest package               : %s\n", old);
   fprintf(fptr, "- Newest package               : %s\n", new);
   fprintf(fptr, "- Package with no upgrades     : ");
   for (int i = 0; i < position; i++){
       if (packages[i].updates == 0){
           fprintf(fptr, "%s, ", packages[i].name);
       }
   }
    fprintf(fptr, "\n");
    fprintf(fptr, "- [ALPM-SCRIPTTLET] type count : %d\n", scripttlet);
    fprintf(fptr, "- [ALPM] count                 : %d\n", alpm);
    fprintf(fptr, "- [PACMAN] count               : %d\n", pacman);
    fprintf(fptr, "-------------\n");
    fprintf(fptr, "List of packages\n");
    fprintf(fptr, "-------------\n");
    for (int i = 0; i < position; i++){
        fprintf(fptr, "- Package Name        : %s\n", packages[i].name);
        fprintf(fptr, "  - Install date      : %s\n", packages[i].install_date);
        fprintf(fptr, "  - Last update date  : %s\n", packages[i].last_update);
        fprintf(fptr, "  - How many updates  : %d\n", packages[i].updates);
        fprintf(fptr, "  - Removal date      : %s\n", packages[i].removal_date);
   }


   fclose(fptr);

}
