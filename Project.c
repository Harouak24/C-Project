#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define SIZE 100
int login_signup(void);
int check_password(char* password, char* password_X);
int login(int N_clients, char email[][50], char* email_X, char password[][25], char* password_X, char secret_question[][50], char answer[][50], char banned_email[][50]);
void signup(int* N_clients, char email[][50], char password[][25], char fname[][25], char lname[][25], char* email_X, char* password_X, char secret_question[][50], char answer[][50], char bank_account_number[][20], int* balance);
int email_exits(char* New_email, char email[][50], int N_clients);
int bank_account_number_exits(char* New_bank_nm, char bank_account_number[][20], int N_clients);
int password_safety(char* password);
void output(int N_clients, char email[][50], char password[][25], char fname[][25], char lname[][25], char secret_question[][50], char answer[][50], char bank_account_number[][20], int* balance);
void swap_str(char str1[], char str2[]);
void swap_int(int* balance1, int* balance2);
int login_menu();
int change_info_menu();
int pay_bill(int index, int balance[], int *N_clients, char email[][50], char password[][25], char fname[][25], char lname[][25], char secret_question[][50], char answer[][50], char bank_account_number[][20], char banned_email[50]);
void end_subscription(int index, int balance[], int *N_clients, char email[][50], char password[][25], char fname[][25], char lname[][25], char secret_question[][50], char answer[][50], char bank_account_number[][20]);
int main(void) 
{
	char email[SIZE][50], password[SIZE][25], fname[SIZE][25], lname[SIZE][25], temp[100], email_X[50], password_X[50], bank_account_number[SIZE][20],secret_question[SIZE][50], answer[SIZE][50], banned_email[SIZE][50];
	int balance[SIZE], choice, choice2, choice3, change, i = 0, N_clients, result, check, ban_count = 0;
	FILE* infp;
	infp = fopen("database.txt", "r");
	if (infp == NULL) {
		printf("\n\t\tFile could not be opened.\n");
		return 0;
	}
  while (!feof(infp)) {
		fscanf(infp, "%s", email[i]);
		fscanf(infp, "%s", fname[i]);
		fscanf(infp, "%s", lname[i]);
		fscanf(infp, "%s", password[i]);
		fscanf(infp, "%s", bank_account_number[i]);
		fscanf(infp, "%d", &balance[i]);
		fscanf(infp, "%s", secret_question[i]);
    fscanf(infp, "%s", answer[i]);
    i++;
  }
  fclose(infp);
  N_clients = i;
  do {
    choice = login_signup();
    switch(choice) {
      case 1:
        printf("\n\t\tEnter your email: ");
		    scanf("%s", email_X);
		    printf("\n\t\tEnter your password: ");
		    scanf("%s", password_X);
        result = login(N_clients, email, email_X, password, password_X, secret_question, answer, banned_email);
        if(result == -101) {
          signup(&N_clients, email, password, fname, lname, email_X, password_X, secret_question, answer, bank_account_number, balance);
        }
        else if(result == 404) {
          printf("\n\t\tYou are being redirected to the first menu...\n");
        }
        else {
          do {
            choice2 = login_menu();
            switch(choice2) {
              case 1:
                //change user information
                printf("\n\t\tPlease re-enter your password for access: ");
                scanf("%s", temp);
                if(strncmp(temp, password[result], strlen(temp)) == 0) {
                  choice3 = change_info_menu();
                  switch(choice3) {
                    case 1:
                      printf("\n\t\tEnter new first name: ");
                      scanf("%s", temp);
                      strcpy(fname[result], temp);
                      break;
                    case 2:
                      printf("\n\t\tEnter new last name: ");
                      scanf("%s", temp);
                      strcpy(lname[result], temp);
                      break;
                    case 3:
                      printf("\n\t\tEnter new email: ");
                      scanf("%s", temp);
                      strcpy(email[result], temp);
                      break;
                    case 4:
                      printf("\n\t\tEnter new password: ");
                      scanf("%s", temp);
                      while(password_safety(temp)) {
                        printf("\n\t\tFor safety concerns the password must contain 12 characters that must include 3 lowercase letters, 3 uppercase letters, 3 digits, and three other characters that are not digits and not letters. You must make sure your password does not contain the following special characters: @ or ; or backslash.\n");
                        printf("\n\t\tTry again: ");
                        scanf("%s", temp);
                      }
                      strcpy(password[result], temp);
                      break;
                    case 5:
                      printf("\n\t\tEnter new bank account number: ");
                      scanf("%s", temp);
                      strcpy(bank_account_number[result], temp);
                      break;
                    default:
                      printf("\n\t\tError! unavailable choice.\n");
                  }
                break;
              case 2:
                if(pay_bill(result, balance, &N_clients, email, password, fname, lname, secret_question, answer, bank_account_number, banned_email[ban_count]))
                  printf("\n\t\tPayement has been successful!\n");
                else {
                  ban_count++;
                  choice2 = 4;
                }
                break;
              case 3:
                //End subscription
                end_subscription(result, balance, &N_clients, email, password, fname, lname, secret_question, answer, bank_account_number);
                printf("\n\t\tGoodbye! Netflix will miss you.\n");
                choice2 = 4;
                break;
              case 4:
                break;
              default:
                printf("\n\t\tError! unavailable choice\n");
                }
              else
                printf("\n\t\tWrong Password!\n");
            }
          } while(choice2 != 4);
        }
      break;
      case 2: 
        signup(&N_clients, email, password, fname, lname, email_X, password_X, secret_question, answer, bank_account_number, balance);
        break;
      case 3:
        printf("\n\tA file with all profiles sorted by family name was generated.\n");
        output(N_clients, email, password, fname, lname, secret_question, answer,bank_account_number, balance);
        break;
      default:
        printf("\n\tError! Choice unavailable.\n");
    }
  } while(choice != 3);
  /*
  for(i = 0; i < 3; i++) {
    printf("%s\t%s\t%s\t%s\t%s\t%d\t%s\t%s\n",email[i], fname[i], lname[i], password[i], bank_account_number[i], balance[i], secret_question[i], answer[i]);
  }
  */
  return 0;
}
int login_signup(void) {
  int choice;
  printf("\n\t\t--------------MENU--------------\n");
	printf("\n\t\t1. Log In\n");
	printf("\n\t\t2. Sign Up\n");
  printf("\n\t\t3. Exit");
	printf("\n\n\t\tEnter your choice: ");
	scanf("%d", &choice);
  return choice;
}
int login(int N_clients, char email[][50], char* email_X, char password[][25], char* password_X, char secret_question[][50], char answer[][50], char banned_email[][50]) {
  int i;
  char temp[50];
  for(i = 0; i < N_clients; i++) {
    if(strcmp(banned_email[i], email_X) == 0) {
      printf("\n\t\tThis account no longer exits!\n");
      return -101;
    }
    if (strcmp(email[i], email_X) == 0) {
			if (strcmp(password[i], password_X) == 0)
        return i;
      else if (check_password(password[i], password_X) == 2) {
        printf("\n\t\tYou inverted 2 consecutive characters in your password. Please answer this secret question to login.\n");
				printf("\n\t\t%s ", secret_question[i]);
				scanf("%s", temp);
				if (strcmp(temp, answer[i]) == 0)
          return i;
				else {
					printf("\n\t\tAnswers do not match.\n");
          return 0;
        }
      }
      else {
        printf("\n\t\tWrong password!\n");
        return 404;
      }
    }
  }
  return -101;
}
int check_password(char* password, char* password_X) 
{
	int i, count = 0;
	for (i = 0; password_X[i] != '\0'; i++) {
		if (password[i] != password_X[i] && (password[i] == password_X[i + 1] || password[i] == password_X[i - 1]))
			count++;
	}
	return count;
}
void signup(int* N_clients, char email[][50], char password[][25], char fname[][25], char lname[][25], char* email_X, char* password_X, char secret_question[][50], char answer[][50], char bank_account_number[][20], int* balance)
{
  int index = *N_clients - 1;
  char temp[50];
	printf("\n\t\t--------------Sign Up--------------\n");
	printf("\n\t\tEnter your email: ");
	scanf("%s", temp);
  while(email_exits(temp, email, *N_clients)) {
    printf("\n\t\tEmail already exists. Please try again: ");
    scanf("%s", temp);
  }
  strcpy(email[index], temp);
	printf("\n\t\tEnter your first name: ");
	scanf("%s", fname[index]);
	printf("\n\t\tEnter your last name: ");
	scanf("%s", lname[index]);
	printf("\n\t\tEnter your password: ");
	scanf("%s", temp);
  while(password_safety(temp)) {
    printf("\n\t\tFor safety concerns the password must contain 12 characters that must include 3 lowercase letters, 3 uppercase letters, 3 digits, and three other characters that are not digits and not letters. You must make sure your password does not contain the following special characters: @ or ; or backslash.\n");
    printf("\n\t\tTry again: ");
    scanf("%s", temp);
  }
  strcpy(password[index], temp);
	printf("\n\t\tEnter your bank account number: ");
	scanf("%s", temp);
  while(bank_account_number_exits(temp, bank_account_number, *N_clients)) {
    printf("\n\t\tBank account number already exists. Please try again: ");
    scanf("%s", temp);
  }
  strcpy(bank_account_number[index], temp);
	balance[index] = 0;
	printf("\n\t\tEnter a secret question: ");
	scanf("%s", secret_question[index]);
	printf("\n\t\tEnter an answer: ");
	scanf("%s", answer[index]);
  *N_clients += 1;
}
int email_exits(char* New_email, char email[][50], int N_clients) {
  int i;
  for(i = 0; i < N_clients; i++) {
    if(strncmp(New_email, email[i], strlen(New_email)) == 0)
      return 1;
  }
  return 0;
}
int bank_account_number_exits(char* New_bank_nm, char bank_account_number[][20], int N_clients) {
  int i;
  for(i = 0; i < N_clients; i++) {
    if(strncmp(New_bank_nm, bank_account_number[i], strlen(New_bank_nm)) == 0)
      return 1;
  }
  return 0;
}
int password_safety(char* New_password) {
  int i, allowed[4], result = 0;
  for(i = 0; i < 4; i++) {
    allowed[i] = 0;
  }
  if (strlen(New_password) != 12)
    result++;
  for (i = 0; i < 12; i++) {
    if (New_password[i] >= 48 && New_password[i] <= 57)
      allowed[0]++;
    else if (New_password[i] >= 65 && New_password[i] <= 90) 
      allowed[1]++;
    else if (New_password[i] >= 97 && New_password[i] <= 122) 
      allowed[2]++;
    else if ((New_password[i] != '@') && (New_password[i] != ';') && (New_password[i] != '\\') && ((New_password[i] >= 32 && New_password[i] <= 47) || (New_password[i] >= 58 && New_password[i] <= 64) || (New_password[i] >= 91 && New_password[i] <= 96)))
      allowed[3]++;
    }
    for (i = 0; i < 4; i++) {
      if (allowed[i] != 3)
        result++;
    }
    return result;
}
void output(int N_clients, char email[][50], char password[][25], char fname[][25], char lname[][25], char secret_question[][50], char answer[][50], char bank_account_number[][20], int* balance) {
  int i, j;
  FILE* outfp = fopen("output.txt", "w");
  for(i = 0; i < N_clients - 1; i++) {
    for(j = 0; j < N_clients - i - 1; j++) {
      lname[j][0] = tolower(lname[j][0]);
      lname[j + 1][0] = tolower(lname[j + 1][0]);
      if( strcmp(lname[j], lname[j+1]) > 0) {
        swap_str(email[j], email[j + 1]);
        swap_str(password[j], password[j + 1]);
        swap_str(fname[j], fname[j + 1]);
        swap_str(lname[j], lname[j + 1]);
        swap_str(bank_account_number[j], bank_account_number[j + 1]);
        swap_int(&balance[j], &balance[j + 1]);
        swap_str(secret_question[j], secret_question[j + 1]);
        swap_str(answer[j], answer[j + 1]);
      }   
    }
  }
  for(i = 0; i < N_clients; i++) {
    fprintf(outfp, "%s %s %s %s %s %d %s %s\n", email[i], password[i], fname[i], lname[i], bank_account_number[i], balance[i], secret_question[i], answer[i]);
  }
  fclose(outfp);
}
void swap_str(char str1[], char str2[]) {
  char temp[50];
  strcpy(temp, str1);
  strcpy(str1, str2);
  strcpy(str2, temp);
}
void swap_int(int* balance1, int* balance2) {
  int temp;
  temp = *balance1;
  *balance1 = *balance2;
  *balance2 = temp;
}
int login_menu() {
  int choice;
  printf("\n\t\t-------------Welcome to Netflix-------------\n");
  printf("\n\t\t1. Change user information\n");
  printf("\n\t\t2. Pay bill\n");
  printf("\n\t\t3. End subscription\n");
  printf("\n\t\t4. Log out\n");
  printf("\n\n\t\tEnter your choice: ");
  scanf("%d", &choice);
  return choice;
}
int change_info_menu() {
  int choice;
  printf("\n\t\tWhat information would you like to change\n");
  printf("\n\t\t1. First Name\n");
  printf("\n\t\t2. Last Name\n");
  printf("\n\t\t3. Email\n");
  printf("\n\t\t4. Password\n");
  printf("\n\t\t5. Bank Account Number\n");
  printf("\n\n\t\tEnter your choice: ");
  scanf("%d", &choice);
  return choice;
}
int pay_bill(int index, int balance[], int *N_clients, char email[][50], char password[][25], char fname[][25], char lname[][25], char secret_question[][50], char answer[][50], char bank_account_number[][20], char banned_email[50]) {
  int i;
  balance[index] -= 500;
  if(balance[index] < 0) {
    strcpy(email[index], banned_email);
    for(i = index - 1; i < *N_clients - 1; i++) {
      strcpy(email[i], email[i + 1]);
      strcpy(password[i], password[i + 1]);
      strcpy(fname[i], fname[i + 1]);
      strcpy(lname[i], lname[i + 1]);
      strcpy(bank_account_number[i], bank_account_number[i + 1]);
      balance[i] = balance[i + 1];
      strcpy(secret_question[i], secret_question[i + 1]);
      strcpy(answer[i], answer[i + 1]);
    } 
    *N_clients -= 1;
    return 0; 
  }
  return 1;
}
void end_subscription(int index, int balance[], int *N_clients, char email[][50], char password[][25], char fname[][25], char lname[][25], char secret_question[][50], char answer[][50], char bank_account_number[][20]) {
  int i;
  for(i = index - 1; i < *N_clients - 1; i++) {
    strcpy(email[i], email[i + 1]);
    strcpy(password[i], password[i + 1]);
    strcpy(fname[i], fname[i + 1]);
    strcpy(lname[i], lname[i + 1]);
    strcpy(bank_account_number[i], bank_account_number[i + 1]);
    balance[i] = balance[i + 1];
    strcpy(secret_question[i], secret_question[i + 1]);
    strcpy(answer[i], answer[i + 1]);
  }
  *N_clients -= 1;
}
