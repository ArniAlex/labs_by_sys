#include <stdio.h>
#include <string.h>
#include <time.h>

#pragma warning (disable: 4996)

typedef struct {
    char login[7];
    unsigned int PIN;
} User;

User users[100];
int u_count = 0;

void load_u() {  // загружаем данные из бинарника в массив 
    FILE* f = fopen("users.dat", "rb");
    if (!f) return;

    User u;
    while (fread(&u, sizeof(User), 1, f) == 1 && u_count < 100) {
        users[u_count++] = u;
    }
    fclose(f);
}

void save_u() { // перезапись данных 
    FILE* f = fopen("users.dat", "wb");
    if (!f) return;

    fwrite(users, sizeof(User), u_count, f);
    fclose(f);
}

int val_login(const char* login) { // проверка логина 
    if (strlen(login) > 6) return 0;
    for (int i = 0; login[i]; i++) {  
        if (!(isdigit(login[i]) || isalpha(login[i]))) { 
            return 0;
        }
    }
    return 1;
}

int val_pin(unsigned int pin) { // проверка пароля
    return pin <= 100000;
}

User* find_u(const char* login) { // сравнение логина 
    for (int i = 0; i < u_count; i++) {
        if (strcmp(users[i].login, login) == 0) {
            return &users[i];
        }
    }
    return NULL;
}

void reg_u() {  // регистрация пользователя 
    char login[7];
    unsigned int pin;

    printf("Create login (max 6 letters/numbers): ");
    scanf("%6s", login);

    if (!val_login(login)) {
        printf("Invalid login!\n");
        return;
    }

    if (find_u(login)) {
        printf("Login exists!\n"); //если логин уже существует
        return;
    }

    printf("Create PIN (0-100000): ");
    scanf("%u", &pin);
    if (!val_pin(pin)) {
        printf("Invalid PIN!\n");
        return;
    }
    printf("\nYour PIN: %u\n", pin);
    if (u_count<100) {
        strcpy(users[u_count].login, login);
        users[u_count].PIN = pin;
        u_count++;
    }

    save_u();
    printf("Registration success!\n");
}

User* auth_u() { // если уже зарег
    char login[7];
    unsigned int pin;

    printf("Enter login: ");
    scanf("%6s", login);

    printf("Enter PIN: ");
    scanf("%u", &pin);

    User* u = find_u(login);
    if (u && (u->PIN == pin)) {
        printf("Auth success!\n");
        return u;
    }

    printf("Wrong login or PIN!\n");
    return NULL;
}


// команды после авторизации 

void cmd_time() {
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    printf("Time: %02d:%02d:%02d\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
}

void cmd_date() {
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    printf("Date: %02d:%02d:%04d\n", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
}

void cmd_howmuch(const char* datetime, const char* flag) {
    struct tm tm = { 0 };
    sscanf(datetime, "%2d:%2d:%4d %2d:%2d:%2d",
        &tm.tm_mday, &tm.tm_mon, &tm.tm_year,
        &tm.tm_hour, &tm.tm_min, &tm.tm_sec);

    tm.tm_mon--;
    tm.tm_year -= 1900;

    time_t then = mktime(&tm);
    time_t now = time(NULL);
    double diff = difftime(now, then);

    if (strcmp(flag, "-s") == 0) printf("%.0f seconds\n", diff);
    else if (strcmp(flag, "-m") == 0) printf("%.0f minutes\n", diff / 60);
    else if (strcmp(flag, "-h") == 0) printf("%.0f hours\n", diff / 3600);
    else if (strcmp(flag, "-y") == 0) printf("%.0f years\n", diff / 31536000);
    else printf("Unknown flag!\n");
}

void sanctions(const char* username) {
    int code;
    printf("Enter code to confirm: ");
    scanf("%d", &code);

    if (code != 52) {
        printf("Wrong code!\n");
        return;
    }

    User* u = find_u(username);
    if (u) {
        save_u();
        printf("Sanctions set for %s\nSession termination.\n", username);
        return;
    }
    else {
        printf("User not found!\n");
    }
}

void show_commands() { // вывод команд 
    printf("\nCommands:\n");
    printf("Time - current time\n");
    printf("Date - current date\n");
    printf("Howmuch <dd:MM:yyyy HH:MM:SS> <flag> - time passed\n");
    printf("Logout - exit to auth\n");
    printf("Sanctions <username> - set restrictions\n");
}

int main() {
    load_u();
    char choice[10];
    User* current = NULL;
    int req_count = 0;

    while (1) {
        if (!current) {
            printf("\nRegistered? (YES/NO): ");
            scanf("%9s", choice);

            if (strcmp(choice, "YES") == 0) {
                current = auth_u();
                if (!current) {
                    printf("REPEAT\n");
                    continue;
                }
                show_commands();
            }
            else if (strcmp(choice, "NO") == 0) {
                reg_u();
            }
            else {
                printf("Unknown command!\n");
            }
        }
        else {

            printf("\n%s> ", current->login);
            scanf("%9s", choice);

            if (strcmp(choice, "Time") == 0) {
                cmd_time();
                req_count++;
            }
            else if (strcmp(choice, "Date") == 0) {
                cmd_date();
                req_count++;
            }
            else if (strcmp(choice, "Howmuch") == 0) {
                char line[100];
                fgets(line, sizeof(line), stdin);

                char datetime[20] = { 0 };
                char flag[3] = { 0 };
                int params = sscanf(line, "%19s %2s", datetime, flag);

                if (params != 2) { 
                    printf("Error: Howmuch requires 2 parameters: <datetime> <flag>\n");
                    printf("Example: Howmuch 01:01:2023 10:00:00 -s\n");
                    current = NULL;
                    req_count = 0; 
                }
                else {
                    cmd_howmuch(datetime, flag);
                    req_count++;
                }
            }
            else if (strcmp(choice, "Logout") == 0) {
                current = NULL;
                req_count = 0;
            }
            else if (strcmp(choice, "Sanctions") == 0) {
                char username[7];
                scanf("%6s", username);
                sanctions(username);
                current = NULL;
                req_count = 0;
            }
            else {
                printf("Unknown command!\n");
            }
        }
    }

    return 0;
}