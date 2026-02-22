Overview:
A robust, Linux-terminal-based contact management system developed in C.This project serves as a functional prototype for an address book, 
allowing users to manage personal data through a structured command-line interface. It features persistent data storage, ensuring that contacts are saved and retrieved across different sessions.


Key Features:
              CRUD Operations: Full support for Adding, Searching, Editing, and Deleting contacts.
              Data Validation: Includes built-in logic to validate the format of names, phone numbers, and email addresses.
              Persistent Storage: Implements file handling to save contact data to a permanent file, preventing data loss when the program exits.
              Linux-Optimized: Designed specifically for the Linux terminal environment, utilizing ANSI color codes for an enhanced user experience.
              Memory Efficiency: Uses structured data types (AddressBook) to manage contacts efficiently in-memory before syncing to disk.

Technical Implementation:

Language: C.
            Core Concepts: File I/O (loading/saving), Structures, Loops, and Switch-case logic.
            Modular Design: The project is split into header files (contact.h) and implementation files to follow professional coding standards.

How to Use
            Compile: Use a standard C compiler like gcc.
            Run: Launch the executable.
Navigate: \
Use the numbered menu to manage your contacts. 
Ensure you select "Save contact" or "Exit" to sync changes to the storage file.
