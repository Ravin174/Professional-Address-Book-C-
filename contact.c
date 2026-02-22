#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "contact.h"
void init_initialization(AddressBook *addressbook)
{
    FILE *fp = fopen("addressbook.txt","r");
    if(fp == NULL){ // Checking if  file is present or not
        printf(RED"Error: Unable to access Addressbook or Addressboook not found!\n"RESET);
        return;
    }
    fscanf(fp,"#%d\n",&addressbook->contact_count); // reading the no. of contacts/contact count

    fscanf(fp, "%*[^\n]\n");    // This line reads and discards the header row

    for(int i = 0; i < addressbook->contact_count; i++){ // loading the contacts to the structures
        fscanf(fp,"%[^,],%[^,],%[^\n]\n",
            addressbook->contact_details[i].Name,
            addressbook->contact_details[i].Mobile_number,
            addressbook->contact_details[i].Mail_ID);
    }
    fclose(fp); // close the file
}

/* Creating Contacts */
int create_contact(AddressBook *addressbook) 
{
    char name[100], phone[20], email[50]; // Temporary variables for input
    char choice;

    do {
        int i = addressbook->contact_count;

        // Check if address book is full
        if (i >= MAX_CONTACTS) {
            printf(RED "\nError: Address book is full! Cannot add more than %d contacts.\n" RESET, MAX_CONTACTS);
            return -1;
        }

        // Validate Name
        do {
            printf(CYAN "\nEnter Name: " RESET);
            scanf(" %[^\n]", name);
        } while (!validate_name(name));

        // Validate Phone
        do {
            printf(CYAN "Enter Phone Number: " RESET);
            scanf(" %[^\n]", phone);
        } while (!validate_phone(addressbook, phone));

        // Validate Email
        do {
            printf(CYAN "Enter Mail ID: " RESET);
            scanf(" %[^\n]", email);
        } while (!validate_email(addressbook, email));

        // Copying from temporary variable to structure (safe copy with size limit)
        strncpy(addressbook->contact_details[i].Name, name, sizeof(addressbook->contact_details[i].Name) - 1);
        addressbook->contact_details[i].Name[sizeof(addressbook->contact_details[i].Name) - 1] = '\0';

        strncpy(addressbook->contact_details[i].Mobile_number, phone, sizeof(addressbook->contact_details[i].Mobile_number) - 1);
        addressbook->contact_details[i].Mobile_number[sizeof(addressbook->contact_details[i].Mobile_number) - 1] = '\0';

        strncpy(addressbook->contact_details[i].Mail_ID, email, sizeof(addressbook->contact_details[i].Mail_ID) - 1);
        addressbook->contact_details[i].Mail_ID[sizeof(addressbook->contact_details[i].Mail_ID) - 1] = '\0';

        // Increment contact count
        addressbook->contact_count++;
        printf(GREEN "\nContact Added Successfully! Total Contacts: %d\n" RESET, addressbook->contact_count);

        // Ask user if they want to add more
        printf(MAGENTA "Do you want to add more contacts? (y/n): " RESET);
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');

    sort_contacts(addressbook); // Sorting contacts after storing the details

    return 0;
}
/* Listing All Contacts */
void list_contacts(AddressBook *addressbook) 
{
    if (addressbook->contact_count == 0) { // If no contacts are available
        printf(RED "\nNo contacts available in the Address Book!\n" RESET);
        return;
    }

    // Printing header
    printf(BLUE "\n---------------------------------- CONTACT LIST ----------------------------------\n" RESET);
    printf(CYAN "%-5s %-30s %-20s %-30s\n" RESET, "S.No", "Name", "Mobile Number", "Mail ID");
    printf(BLUE "----------------------------------------------------------------------------------\n" RESET);

    // Print all contacts
    for (int i = 0; i < addressbook->contact_count; i++) {
        printf("%-5d %-30s %-20s %-30s\n",
               i + 1,
               addressbook->contact_details[i].Name,
               addressbook->contact_details[i].Mobile_number,
               addressbook->contact_details[i].Mail_ID);
    }

    printf(BLUE "----------------------------------------------------------------------------------\n" RESET);
}


/* Search Contacts */
int search_contacts(AddressBook *addressbook) 
{
    if (addressbook->contact_count == 0) { // Checking if contacts present or not
        printf(RED "\nNo contacts available to search!\n" RESET);
        return 0;
    }

    int ch;
    do {
        printf(BLUE "\nSearch Contact Menu:\n" RESET);
        printf(CYAN "1. Name\n2. Mobile Number\n3. Mail ID\n4. Exit\n" RESET);
        printf(MAGENTA "Enter your choice: " RESET);
        scanf("%d", &ch);

        switch (ch) {
            case 1: 
                search_name(addressbook, false); // mode -> false (no edit/delete)
                break;
            case 2: 
                search_phone(addressbook, false); 
                break;
            case 3: 
                search_email(addressbook, false); 
                break;
            case 4: 
                printf(GREEN "Returning to main menu...\n" RESET);
                break;
            default: 
                printf(RED "Invalid Choice! Please enter 1–4.\n" RESET);
        }

    } while (ch != 4);

    return 0;
}
/* Edit Contacts */
int edit_contact(AddressBook *addressbook) {
    if (addressbook->contact_count == 0) {
        printf(YELLOW "\nNo contacts available to edit!\n" RESET);
        return 0;
    }

    char main_choice;
    do {
        printf(BLUE "\nSearch Contact to Edit:\n" RESET);
        printf(CYAN "1. Name\n2. Mobile Number\n3. Mail ID\n4. Exit\n" RESET);
        printf(MAGENTA "Enter your option: " RESET);

        int opt, idx = -1;
        scanf("%d", &opt);

        switch (opt) {
            case 1:
                idx = search_name(addressbook, true);
                break;
            case 2:
                idx = search_phone(addressbook, true);
                break;
            case 3:
                idx = search_email(addressbook, true);
                break;
            case 4:
                return 0;
            default:
                printf(RED "Invalid option! Please enter 1–4.\n" RESET);
                continue;
        }

        if (idx == -1) {
            printf(YELLOW "No matching contact found. Returning to edit menu.\n" RESET);
            continue;
        }

        // Display the selected contact
        printf(CYAN "\nSelected Contact:\n" RESET);
        printf(BLUE "----------------------------------------------------------------------------------\n" RESET);
        printf(CYAN "%-5s %-30s %-20s %-30s\n" RESET, "S.No", "Name", "Mobile Number", "Mail ID");
        printf(BLUE "----------------------------------------------------------------------------------\n" RESET);
        printf("%-5d %-30s %-20s %-30s\n",
               idx + 1,
               addressbook->contact_details[idx].Name,
               addressbook->contact_details[idx].Mobile_number,
               addressbook->contact_details[idx].Mail_ID);
        printf(BLUE "----------------------------------------------------------------------------------\n" RESET);

        // Confirmation before editing
        char confirm_edit;
        printf(MAGENTA "Are you sure you want to edit this contact? (y/n): " RESET);
        scanf(" %c", &confirm_edit);

        if (confirm_edit == 'n' || confirm_edit == 'N') {
            printf(YELLOW "Editing cancelled. Returning to edit menu.\n" RESET);
            continue; // go back to main edit menu
        }

        char sub_choice;
        do {
            printf(BLUE "\nEdit Contact Menu:\n" RESET);
            printf(CYAN "1. Name\n2. Mobile Number\n3. Mail ID\n4. Exit\n" RESET);
            printf(MAGENTA "Enter your option: " RESET);

            int option;
            scanf("%d", &option);

            char edit_n[100], edit_p[20], edit_e[50];

            switch (option) {
                case 1:
                    do {
                        printf(CYAN "Enter the new Name: " RESET);
                        scanf(" %[^\n]", edit_n);
                    } while (!validate_name(edit_n));

                    strncpy(addressbook->contact_details[idx].Name, edit_n,
                            sizeof(addressbook->contact_details[idx].Name) - 1);
                    addressbook->contact_details[idx].Name[sizeof(addressbook->contact_details[idx].Name) - 1] = '\0';
                    printf(GREEN "Name changed successfully!\n" RESET);
                    break;

                case 2:
                    do {
                        printf(CYAN "Enter the new Phone Number: " RESET);
                        scanf(" %[^\n]", edit_p);
                    } while (!validate_phone(addressbook, edit_p));

                    strncpy(addressbook->contact_details[idx].Mobile_number, edit_p,
                            sizeof(addressbook->contact_details[idx].Mobile_number) - 1);
                    addressbook->contact_details[idx].Mobile_number[sizeof(addressbook->contact_details[idx].Mobile_number) - 1] = '\0';
                    printf(GREEN "Phone number changed successfully!\n" RESET);
                    break;

                case 3:
                    do {
                        printf(CYAN "Enter the new E-Mail ID: " RESET);
                        scanf(" %[^\n]", edit_e);
                    } while (!validate_email(addressbook, edit_e));

                    strncpy(addressbook->contact_details[idx].Mail_ID, edit_e,
                            sizeof(addressbook->contact_details[idx].Mail_ID) - 1);
                    addressbook->contact_details[idx].Mail_ID[sizeof(addressbook->contact_details[idx].Mail_ID) - 1] = '\0';
                    printf(GREEN "E-Mail changed successfully!\n" RESET);
                    break;

                case 4:
                    sub_choice = 'n';
                    continue;
                default:
                    printf(RED "Invalid choice! Please enter 1–4.\n" RESET);
            }

            printf(MAGENTA "Do you want to edit more fields of this contact? (y/n): " RESET);
            scanf(" %c", &sub_choice);

        } while (sub_choice == 'y' || sub_choice == 'Y');

        sort_contacts(addressbook); // keep sorted after editing

        printf(MAGENTA "Do you want to edit another contact? (y/n): " RESET);
        scanf(" %c", &main_choice);

    } while (main_choice == 'y' || main_choice == 'Y');

    return 0;
}
/* Delete Contacts */
int delete_contact(AddressBook *addressbook) 
{
    if (addressbook->contact_count == 0) {
        printf(RED "\nNo contacts available to delete!\n" RESET);
        return 0;
    }

    char choice;
    do {
        // Step 1: Pick which contact to delete
        printf(BLUE "\nDelete Contact Menu:\n" RESET);
        printf(CYAN "1. Name\n2. Mobile Number\n3. Mail ID\n4. Exit\n" RESET);
        printf(MAGENTA "Enter your option: " RESET);

        int opt, idx = -1;
        scanf("%d", &opt);

        switch (opt) {
            case 1: 
                idx = search_name(addressbook, true); 
                break;
            case 2: 
                idx = search_phone(addressbook, true); 
                break;
            case 3: 
                idx = search_email(addressbook, true); 
                break;
            case 4: 
                return 0;
            default: 
                printf(RED "Invalid option! Please enter 1–4.\n" RESET); 
                continue;
        }

        if (idx == -1) {
            printf(YELLOW "No matching contact found. Returning to delete menu.\n" RESET);
            continue;
        }

        // Step 2: Show selected contact before deleting
        printf(CYAN "\nSelected Contact:\n" RESET);
        printf(BLUE "----------------------------------------------------------------------------------\n" RESET);
        printf(CYAN "%-5s %-30s %-20s %-30s\n" RESET, "S.No", "Name", "Mobile Number", "Mail ID");
        printf(BLUE "----------------------------------------------------------------------------------\n" RESET);
        printf("%-5d %-30s %-20s %-30s\n",
               idx + 1,
               addressbook->contact_details[idx].Name,
               addressbook->contact_details[idx].Mobile_number,
               addressbook->contact_details[idx].Mail_ID);
        printf(BLUE "----------------------------------------------------------------------------------\n" RESET);

        // Step 3: Confirm deletion
        char confirm;
        printf(MAGENTA "Are you sure you want to delete this contact? (y/n): " RESET);
        scanf(" %c", &confirm);

        if (confirm == 'y' || confirm == 'Y') {
            // Shift contacts left
            for (int j = idx; j < addressbook->contact_count - 1; j++) {
                addressbook->contact_details[j] = addressbook->contact_details[j + 1];
            }
            addressbook->contact_count--;

            printf(GREEN "Contact deleted successfully!\n" RESET);
        } else {
            printf(YELLOW "Deletion cancelled.\n" RESET);
        }

        if (addressbook->contact_count == 0) { // If all contacts are deleted
            printf(CYAN "\nAll contacts have been deleted. Address Book is now empty.\n" RESET);
            break;
        }

        // Step 4: Ask to delete more
        printf(MAGENTA "Do you want to delete another contact? (y/n): " RESET);
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');

    return 0;
}
/* Saving Contacts to a Text File */
int save_contacts(AddressBook *addressbook) 
{
    FILE *fp = fopen("addressbook.txt", "w"); // write mode (overwrite existing)
    if (fp == NULL) {
        perror(RED "Error opening addressbook.txt" RESET);
        return -1;
    }

    // Save the contact count
    fprintf(fp, "#%d\n", addressbook->contact_count);

    // Save a header row (discarded while loading)
    fprintf(fp, "Name,Phone,Email\n");

    // Save each contact in CSV format
    for (int i = 0; i < addressbook->contact_count; i++) {
        fprintf(fp, "%s,%s,%s\n",
            addressbook->contact_details[i].Name,
            addressbook->contact_details[i].Mobile_number,
            addressbook->contact_details[i].Mail_ID);
    }

    fclose(fp);

    printf(GREEN "\nContacts saved successfully to addressbook.txt (%d contacts).\n" RESET,
           addressbook->contact_count);

    return 0;
}

/* -- Search Functions -- */

/* Search by Name */
int search_name(AddressBook *addressbook, bool select_mode) {
    char search_name[32];
    printf(CYAN "\nEnter Name to search: " RESET);
    scanf(" %[^\n]", search_name);

    if (strlen(search_name) == 0) {
        printf(RED "Error: Name cannot be empty.\n" RESET);
        return -1;
    }

    int indices[100], count = 0;
    for (int i = 0; i < addressbook->contact_count; i++) {
        if (strcasestr(addressbook->contact_details[i].Name, search_name)) {
            indices[count++] = i;
        }
    }

    if (count == 0) {
        printf(YELLOW "No contact found with name containing: %s\n" RESET, search_name);
        return -1;
    }

    print_contact_table();
    for (int i = 0; i < count; i++) {
        print_contact(&addressbook->contact_details[indices[i]], i + 1, indices[i]);
    }
    printf(BLUE "------------------------------------------------------------------------------------------------\n" RESET);

    if (!select_mode) return -1;

    int choice;
    printf(CYAN "Enter your choice (1-%d): " RESET, count);
    scanf("%d", &choice);
    if (choice < 1 || choice > count) {
        printf(RED "Invalid choice.\n" RESET);
        return -1;
    }
    return indices[choice - 1];
}

/* Search by Phone Number */
int search_phone(AddressBook *addressbook, bool select_mode) {
    char search_phone[15];
    printf(CYAN "\nEnter Phone Number to search: " RESET);
    scanf("%s", search_phone);

    if (strlen(search_phone) == 0) {
        printf(RED "Error: Phone number cannot be empty.\n" RESET);
        return -1;
    }

    int indices[100], count = 0;
    for (int i = 0; i < addressbook->contact_count; i++) {
        if (strstr(addressbook->contact_details[i].Mobile_number, search_phone)) {
            indices[count++] = i;
        }
    }

    if (count == 0) {
        printf(YELLOW "No contact found with phone number containing: %s\n" RESET, search_phone);
        return -1;
    }

    print_contact_table();
    for (int i = 0; i < count; i++) {
        print_contact(&addressbook->contact_details[indices[i]], i + 1, indices[i]);
    }
    printf(BLUE "------------------------------------------------------------------------------------------------\n" RESET);

    if (!select_mode) return -1;

    int choice;
    printf(CYAN "Enter your choice (1-%d): " RESET, count);
    scanf("%d", &choice);
    if (choice < 1 || choice > count) {
        printf(RED "Invalid choice.\n" RESET);
        return -1;
    }
    return indices[choice - 1];
}

/* Search by Email ID */
int search_email(AddressBook *addressbook, bool select_mode) {
    char search_email[35];
    printf(CYAN "\nEnter Email ID to search: " RESET);
    scanf(" %[^\n]", search_email);

    if (strlen(search_email) == 0) {
        printf(RED "Error: Email cannot be empty.\n" RESET);
        return -1;
    }

    int indices[100], count = 0;
    for (int i = 0; i < addressbook->contact_count; i++) {
        if (strcasestr(addressbook->contact_details[i].Mail_ID, search_email)) {
            indices[count++] = i;
        }
    }

    if (count == 0) {
        printf(YELLOW "No contact found with email containing: %s\n" RESET, search_email);
        return -1;
    }

    print_contact_table();
    for (int i = 0; i < count; i++) {
        print_contact(&addressbook->contact_details[indices[i]], i + 1, indices[i]);
    }
    printf(BLUE "------------------------------------------------------------------------------------------------\n" RESET);

    if (!select_mode) return -1;

    int choice;
    printf(CYAN "Enter your choice (1-%d): " RESET, count);
    scanf("%d", &choice);
    if (choice < 1 || choice > count) {
        printf(RED "Invalid choice.\n" RESET);
        return -1;
    }
    return indices[choice - 1];
}
/* Validation Functions */

/* Name Validation and Auto-Correction */
bool validate_name(char *name) {
    // Trim leading & trailing spaces
    int start = 0, end = strlen(name) - 1;
    while (isspace((unsigned char)name[start])) start++;
    while (end >= start && isspace((unsigned char)name[end])) end--;
    name[end + 1] = '\0';
    memmove(name, name + start, end - start + 2);

    if (strlen(name) == 0) {
        printf(RED "Invalid Name: Name cannot be empty.\n" RESET);
        return false;
    }

    // Only alphabets and spaces allowed
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha((unsigned char)name[i]) && !isspace((unsigned char)name[i])) {
            printf(RED "Invalid Name: Only alphabets and spaces allowed.\n" RESET);
            return false;
        }
    }

    // Convert to Title Case
    int capitalize = 1;
    for (int i = 0; name[i] != '\0'; i++) {
        if (isspace((unsigned char)name[i])) {
            capitalize = 1;
        } else if (capitalize) {
            name[i] = toupper((unsigned char)name[i]);
            capitalize = 0;
        } else {
            name[i] = tolower((unsigned char)name[i]);
        }
    }

    return true;
}

/* Phone Number Validation and Auto-Correction */
bool validate_phone(AddressBook *addressbook, char *phone) {
    // Remove non-digit characters (spaces, dashes, etc.)
    int j = 0;
    for (int i = 0; phone[i] != '\0'; i++) {
        if (isdigit((unsigned char)phone[i])) {
            phone[j++] = phone[i];
        }
    }
    phone[j] = '\0';

    int len = strlen(phone);
    if (len != 10) {
        printf(RED "Invalid Phone: Must be exactly 10 digits.\n" RESET);
        return false;
    }

    if (phone[0] < '6' || phone[0] > '9') {
        printf(RED "Invalid Phone: Must start with 6, 7, 8, or 9.\n" RESET);
        return false;
    }

    // Check duplicates
    for (int i = 0; i < addressbook->contact_count; i++) {
        if (strcmp(addressbook->contact_details[i].Mobile_number, phone) == 0) {
            printf(YELLOW "Warning: This phone number already exists.\n" RESET);
            return false;
        }
    }

    return true;
}

/* Email ID Validation and Auto-Correction */
bool validate_email(AddressBook *addressbook, char *email) {
    // Remove spaces and convert to lowercase
    int j = 0;
    for (int i = 0; email[i] != '\0'; i++) {
        if (!isspace((unsigned char)email[i])) {
            email[j++] = tolower((unsigned char)email[i]);
        }
    }
    email[j] = '\0';

    int len = strlen(email);
    if (len < 6) {
        printf(RED "Invalid Email: Too short.\n" RESET);
        return false;
    }

    char *at = strchr(email, '@');
    if (!at || at == email) {
        printf(RED "Invalid Email: Must contain '@' with characters before it.\n" RESET);
        return false;
    }

    // must end with ".com"
    if (strcmp(email + len - 4, ".com") != 0) {
        printf(RED "Invalid Email: Must end with '.com'.\n" RESET);
        return false;
    }

    // domain checks
    if (at[1] == '.' || at > email + len - 5) {
        printf(RED "Invalid Email: Invalid domain name.\n" RESET);
        return false;
    }

    // invalid chars before @
    for (char *c = email; c < at; c++) {
        if (!isalnum((unsigned char)*c) && *c != '.' && *c != '_' && *c != '-') {
            printf(RED "Invalid Email: Local part has invalid characters.\n" RESET);
            return false;
        }
    }

    // invalid chars after @
    for (char *c = at + 1; c < email + len; c++) {
        if (!isalnum((unsigned char)*c) && *c != '.' && *c != '-') {
            printf(RED "Invalid Email: Domain has invalid characters.\n" RESET);
            return false;
        }
    }

    if (strstr(email, "..")) {
        printf(RED "Invalid Email: Cannot contain consecutive dots.\n" RESET);
        return false;
    }

    char *dotcom = strstr(email, ".com");
    if (dotcom && dotcom != email + len - 4) {
        printf(RED "Invalid Email: Multiple '.com' found.\n" RESET);
        return false;
    }

    // duplicate check
    for (int i = 0; i < addressbook->contact_count; i++) {
        if (strcmp(addressbook->contact_details[i].Mail_ID, email) == 0) {
            printf(YELLOW "Warning: This email already exists.\n" RESET);
            return false;
        }
    }

    return true;
}

/*  Contact Sort */
void sort_contacts(AddressBook *addressbook) {
    for (int i = 0; i < addressbook->contact_count - 1; i++) {
        for (int j = i + 1; j < addressbook->contact_count; j++) {
            if (strcasecmp(addressbook->contact_details[i].Name,
                           addressbook->contact_details[j].Name) > 0) {
                Contacts temp = addressbook->contact_details[i];
                addressbook->contact_details[i] = addressbook->contact_details[j];
                addressbook->contact_details[j] = temp;
            }
        }
    }
}

/*Contacts Printing */
void print_contact_table() {
    printf(BLUE "\n------------------------------------------------------------------------------------------------\n" RESET);
    printf(BLUE BOLD "| %-6s | %-6s | %-20s | %-13s | %-35s |\n" RESET,
           "S.No.", "C.No.", "Name", "Mobile Number", "E-Mail ID");
    printf(BLUE "------------------------------------------------------------------------------------------------\n" RESET);
}

void print_contact(Contacts *c, int serial, int index) {
    printf(BLUE "| " RESET "%-6d" BLUE " | " RESET "%-6d" BLUE " | " RESET "%-20s" BLUE " | " RESET "%-13s" BLUE " | " RESET "%-35s" BLUE " |\n" RESET,
           serial, index + 1, c->Name, c->Mobile_number, c->Mail_ID);
}
