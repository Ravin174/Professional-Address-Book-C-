#ifndef CONTACT_H
#define CONTACT_H
#include<stdbool.h>
#define MAX_CONTACTS 1000
typedef struct Contact_data
{
    char Name[32];
    char Mobile_number[11];
    char Mail_ID[35];
} Contacts;

typedef struct AddressBook_Data
{
    Contacts contact_details[100];
    int contact_count;
} AddressBook;

/* Core functions */
void init_initialization(AddressBook *);
int create_contact(AddressBook *);
void list_contacts(AddressBook *);
int search_contacts(AddressBook *);
int edit_contact(AddressBook *);
int delete_contact(AddressBook *);
int save_contacts(AddressBook *);

/* Search Functions */
int search_name(AddressBook *, bool);
int search_phone(AddressBook *, bool);
int search_email(AddressBook *, bool);

/* Validate Functions */
bool validate_name(char *);
bool validate_phone(AddressBook *, char *);
bool validate_email(AddressBook *, char *);

/* Sort Contacts */
void sort_contacts(AddressBook *);

/* Print */
void print_contact_table();
void print_contact(Contacts *c, int serial, int index);

/* Colour Code */
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define BOLD    "\033[1m"

#endif // CONTACT_H
       // CONTACT_H

