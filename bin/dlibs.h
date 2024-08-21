typedef struct {
    char username[50];
    char email[50];
    char password[30];
} Account;

void newAccount(char user[], char email[], char pass[]);
int stringCount(char *string, char *substring, int lower);
int checkEmail(char *email);