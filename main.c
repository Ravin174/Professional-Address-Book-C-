/*
Name: RAVI J S
Student ID: 25021_387
Description: This Addressbook project focuses on the creating a prototype for the addressbook
             with basic functionalities like Add, Search, Edit, Delete and Save Contacts function.
             This also includes the basic validations to check for valid name, phone and email.
             Then stores it in a file for future access and further use. This program is designed 
             by considering the linux terminal in mind.
*/
#include <stdio.h>
#include "contact.h"
/* Structure declaration */

int main()
{
    /* Variable and structre defintion */
    int option;
    AddressBook addressbook;
    addressbook.contact_count = 0;

    init_initialization(&addressbook); // Function call for loading all contacts from flle to program

    while (1)
    {
        printf(BLUE"\nAddress book menu\n"RESET); /* Give a prompt message for a user */
        printf(BLUE"1. Add contact\n2. Search contact\n3. Edit contact\n4. Delete contact\n5. Display contact\n6. Save contact\n7. Exit\n"RESET);
        printf(CYAN"Enter the option : "RESET);
        scanf("%d", &option);

        switch (option) /* Based on choosed option */
        {
        case 1: // Create or Add Contacts
            create_contact(&addressbook);
            break;
            
        case 2: // Search Contacts
            search_contacts(&addressbook);
            break;

        case 3: // Edit Contacts
            //printf("Edit Contact menu : \n1.Name \n2.Mobile number\n3.Mail ID\n4.Exit\nEnter the option : "); /* Providing menu */

            edit_contact(&addressbook);
            break;

        case 4: // Delete Contacts
            //printf("Delete Contact menu : \n1.Name \n2.Mobile number\n3.Mail ID\n4.Exit\nEnter the option : "); /* Providing menu */

            delete_contact(&addressbook);
            break;

        case 5: // List or Display all Contacts
            printf(CYAN"\nList Contacts:"RESET);
            list_contacts(&addressbook);
            break;

        case 6: // Save Contacts
            printf(GREEN"Saving contacts\n"RESET);
            save_contacts(&addressbook);
            printf(GREEN"Contacts saved successfully\n"RESET);
            break;

        case 7: // Exit with saving
            printf(GREEN"\nINFO : Save and Exit...\n\n"RESET);
            save_contacts(&addressbook);
            return 0;

        default: // If wrong input option
            printf(RED"Invalid option \n"RESET);
            break;
        }
    }
    return 0;
}
