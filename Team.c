// Program for maintaining a personal team.
// Uses a linked list to hold the team players.
// Author: Yahya Masood


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

//**********************************************************************
// Linked List Definitions 
//  Define your linked list node and pointer types
//  here for use throughout the file.

struct footballer{
    char *familyName;
    char *firstName;
    char position;
    int value;
    struct footballer *next;
};    


//**********************************************************************
// Linked List Function Declarations
//
// Functions that modify the linked list.
// Declare your linked list functions here.



//**********************************************************************
// Support Function Declarations
//

void safegets (char s[], int arraySize);        // gets without buffer overflow
void familyNameDuplicate (char familyName[]);   // marker/tester friendly 
void familyNameFound (char familyName[]);       //   functions to print
void familyNameNotFound (char familyName[]);    //     messages to user
void familyNameDeleted (char familyName[]);
void printTeamEmpty (void);
void printTeamTitle(void);
void printNofootballersWithLowerValue(int value);

//**********************************************************************
// Program-wide Constants
//

const int MAX_LENGTH = 1023;
const char NULL_CHAR = '\0';
const char NEWLINE = '\n';
const char GOALKEEPER = 'G';
const char DEFENDER = 'D';
const char MIDFIELDER = 'M';
const char STRIKER = 'S';


//**********************************************************************
// Main Program
//

int main (void)
{ 
    const char bannerString[]
        = "Personal Team Maintenance Program.\n\n";
    const char commandList[]
        = "Commands are I (insert), D (delete), S (search by Name),\n"
          "  V (search by value), P (print), Q (quit).\n";

    // Declare linked list HEAD. 
    struct footballer *HEAD=NULL;

 
    // announce start of program
    printf("%s",bannerString);
    printf("%s",commandList);
    
    char response;
    char input[MAX_LENGTH+1];
    do
    {
        printf("\nCommand?: ");
        safegets(input,MAX_LENGTH+1);
        // Response is first char entered by user.
        // Convert to uppercase to simplify later comparisons.
        response = toupper(input[0]);

        if (response == 'I')
        {
            // Insert a footballer entry into the linked list.
            // Maintain the list in correct order (G, D, M, S).
         
            struct footballer *new, *temporary, *check;
            new=malloc(sizeof(struct footballer));
            new->familyName=malloc(100);
            new->firstName=malloc(100);

            // USE THE FOLLOWING PRINTF STATEMENTS WHEN PROMPTING FOR DATA:
            char famName[100], firName[100];
            char pos;
            int val;
            
            printf("  family Name: ");
            safegets(famName, 100);
            strcpy(new->familyName, famName);
            printf("  first Name: ");
            safegets(firName, 100);
            strcpy(new->firstName, firName);
            printf("  position: ");
            scanf("%c", &new->position);
            while(getchar()!='\n'){}
            printf("  value: ");
            scanf("%d", &new->value);
            while(getchar()!='\n'){}
            new->next=NULL;
            
            if (HEAD==NULL){HEAD=new;}
            else{
                //check for duplicate family Names
                int found=0;
                
                check=HEAD;
                if(strcmp(new->familyName, check->familyName)==0){
                    familyNameDuplicate(new->familyName);
                    found=1;
                    continue;
                }
                while(check->next!=NULL){
                    if(strcmp(new->familyName, check->familyName)==0){
                        familyNameDuplicate(new->familyName);
                        found=1;
                        break;
                    }
                    check=check->next;
                }
                
                //add in list
                if (found==0){
                    if(new->position=='G'){
                        if(HEAD->position!='G'){
                            new->next=HEAD; 
                            HEAD=new; 
                            continue;
                        }
                        temporary=HEAD;
                        while(temporary->next!=NULL&&temporary->next->position=='G'){
                            temporary=temporary->next;
                        }
                        new->next=temporary->next;
                        temporary->next=new;
                    }
                    else if(new->position=='D'){
                        if(HEAD->position=='M'||HEAD->position=='S'){
                            new->next=HEAD; 
                            HEAD=new; 
                            continue;
                        }
                        temporary=HEAD;
                        while(temporary->next!=NULL&&temporary->next->position=='G'){
                            temporary=temporary->next;
                        }
                        while(temporary->next!=NULL&&temporary->next->position=='D'){
                            temporary=temporary->next;
                        }
                        new->next=temporary->next;
                        temporary->next=new;
                    }
                    else if(new->position=='M'){
                        if(HEAD->position=='S'){
                            new->next=HEAD; 
                            HEAD=new; 
                            continue;
                        }
                        temporary=HEAD;
                        while(temporary->next!=NULL&&(temporary->next->position=='G'||temporary->next->position=='D')){
                            temporary=temporary->next;
                        }
                        while(temporary->next!=NULL&&temporary->next->position=='M'){
                            temporary=temporary->next;
                        }
                        new->next=temporary->next;
                        temporary->next=new;
                    }
                    else if(new->position=='S'){
                        temporary=HEAD;
                        while(temporary->next!=NULL&&(temporary->next->position=='G'&&temporary->next->position=='D'||temporary->position=='M')){
                            temporary=temporary->next;
                        }
                        while(temporary->next!=NULL&&temporary->next->position=='S'){
                            temporary=temporary->next;
                        }
                        new->next=temporary->next;
                        temporary->next=new;
                    }
                }
            }

        }
        else if (response == 'D')
        {
            // Delete a footballer from the list.
            
            char Name[100];
            printf("\nEnter family Name for entry to delete: ");
            safegets(Name, 100);
            
            struct footballer *delete, *temporary;
            int done=0;
            
            if (HEAD==NULL){
                printTeamEmpty();
                continue;
            }
            else if(HEAD->next==NULL&&(strcmp(HEAD->familyName, Name)==0)){
                delete=HEAD; HEAD=NULL; 
                free(delete); 
                familyNameDeleted(Name); 
                continue;
            }
            else if((strcmp(HEAD->familyName, Name)==0)){
                delete=HEAD; 
                HEAD=delete->next; 
                free(delete); 
                familyNameDeleted(Name); 
                continue;
            }
            else{
                delete=HEAD;
                while(delete!=NULL){
                    if(delete->next!=NULL&&strcmp(Name, delete->next->familyName)==0){
                        temporary=delete;
                        delete=delete->next;
                        temporary->next=delete->next;
                        delete=NULL;
                        free(delete);
                        familyNameDeleted(Name);
                        done=1;
                        break;
                    }
                    delete=delete->next;
                }
            }
            if (done==0){
                familyNameNotFound(Name);
            }
        }
        else if (response == 'S')
        {
            // Search for a footballer by family Name.
            char Name[100];
            printf("\nEnter family Name to search for: ");
            safegets(Name, 100);
        
            
            struct footballer *temporary;
            int done=0;
            
            if (HEAD==NULL){
                printTeamEmpty();
                continue;
            }
            else{
                temporary=HEAD;
                while(temporary!=NULL){
                    if(strcmp(Name, temporary->familyName)==0){
                        familyNameFound(Name);
                        printf("\n%s", temporary->familyName);
                        printf("\n%s", temporary->firstName);
                        printf("\n%c", temporary->position);
                        printf("\n%d\n", temporary->value);
                        done=1;
                        break;
                    }
                    temporary=temporary->next;
                }
            }
            if(done==0){
                familyNameNotFound(Name);
            }
        }
        else if (response == 'V')
        {
            // Search for footballers that are worth less than or equal a value.
            struct footballer *temporary;
            int value, found=0;
            printf("\nEnter value: ");
            scanf("%d", &value);
            while(getchar()!='\n'){}
        
            
            if (HEAD==NULL){
                printTeamEmpty();
                continue;
            }
            else{
                temporary=HEAD;
                while(temporary!=NULL){
                    if(value>=temporary->value){
                        printf("\n%s", temporary->familyName);
                        printf("\n%s", temporary->firstName);
                        printf("\n%c", temporary->position);
                        printf("\n%d\n", temporary->value);
                        found=1;
                    }
                    temporary=temporary->next;
                }
            }
            if(found==0){printNofootballersWithLowerValue(value);}

        }
        else if (response == 'P')
        {
            // Print the team.
            struct footballer *temporary;
            
            if (HEAD==NULL){printTeamEmpty();}
            else{
                printTeamTitle();
                temporary=HEAD;
                while(temporary!=NULL){
                    printf("\n%s", temporary->familyName);
                    printf("\n%s", temporary->firstName);
                    printf("\n%c", temporary->position);
                    printf("\n%d\n", temporary->value);
                    temporary=temporary->next;
                }
            }

        }
        else if (response == 'Q')
        {
            ; // do nothing, we'll catch this case below
        }
        else 
        {
            // do this if no command matched ...
            printf("\nInvalid command.\n%s\n",commandList);
        }
    } while (response != 'Q');
  
    // Delete the whole linked list that hold the team.
    
    struct footballer *temporary, *delete;
    
    if (HEAD!=NULL){
        temporary=HEAD;
        while(temporary!=NULL){
            delete=temporary;
            temporary=temporary->next;
            familyNameDeleted(delete->familyName);
            delete=NULL;
            free(delete);
        }
        HEAD=NULL;
    }

    // Print the linked list to confirm deletion.
            
    if (HEAD==NULL){printTeamEmpty();}
    else{
        printTeamTitle();
        temporary=HEAD;
        while(temporary!=NULL){
            printf("\n%s", temporary->familyName);
            printf("\n%s", temporary->firstName);
            printf("\n%c", temporary->position);
            printf("\n%d\n", temporary->value);
            temporary=temporary->next;
        }
    }


    return 0;
}

//**********************************************************************
// Support Function Definitions

// Function to get a line of input without overflowing target char array.
void safegets (char s[], int arraySize)
{
    int i = 0, maxIndex = arraySize-1;
    char c;
    while (i < maxIndex && (c = getchar()) != NEWLINE)
    {
        s[i] = c;
        i = i + 1;
    }
    s[i] = NULL_CHAR;
}

// Function to call when user is trying to insert a family Name 
// that is already in the book.
void familyNameDuplicate (char familyName[])
{
    printf("\nAn entry for <%s> is already in the team!\n"
           "New entry not entered.\n",familyName);
}

// Function to call when a footballer with this family Name was found in the team.
void familyNameFound (char familyName[])
{
    printf("\nThe footballer with family Name <%s> was found in the team.\n",
             familyName);
}

// Function to call when a footballer with this family Name was not found in the team.
void familyNameNotFound (char familyName[])
{
    printf("\nThe footballer with family Name <%s> is not in the team.\n",
             familyName);
	
}

// Function to call when a family Name that is to be deleted
// was found in the team.
void familyNameDeleted (char familyName[])
{
    printf("\nDeleting footballer with family Name <%s> from the team.\n",
             familyName);
}

// Function to call when printing an empty team.
void printTeamEmpty (void)
{
    printf("\nThe team is empty.\n");
}

// Function to call to print title when whole team being printed.
void printTeamTitle (void)
{
    printf("\nMy Team: \n");
}

// Function to call when no footballer in the team has lower or equal value to the given value
void printNofootballersWithLowerValue(int value)
{
	printf("\nNo footballer(s) in the team is worth less than or equal to <%d>.\n", value);
}

//**********************************************************************
// Add your functions below this line.
