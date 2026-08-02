/*
    Description  : C file for the Stylist and Service of the Pet Salon tracking system.
    Author/s     : PEREZ, ERIN ALYANNA A.
    Section      : S20F
    Last Modified: AUGUST 2, 2026
*/

#include <stdio.h>
#include <string.h>
#include "structs.h"

// FUNCTIONS FOR STYLIST AND SERVICE
/* =================================================================
    LOAD ALL DATA
    Loads all salon data from the four required binary files.

    @param stylists[]       : Array to store stylist data
    @param *stylistCount    : Pointer to the current count of stylists
    @param *nextStylistID   : Pointer to the next available sequential stylist ID
    @param services[]       : Array to store service data
    @param *serviceCount    : Pointer to the current count of services
    @param clients[]        : Array to store client data
    @param *clientCount     : Pointer to the current count of clients
    @param *nextClientID    : Pointer to the next available sequential client ID
    @param *nextPetID       : Pointer to the next available sequential pet ID
    @return                 : 1 to indicate the function has finished the loading process
    @pre                    : All array parameters must be non-null and pointers must be declared in main.
   ================================================================= */
int loadAllData(Stylist stylists[], int *stylistCount, int *nextStylistID, Service services[], int *serviceCount, Client clients[],
                int *clientCount, int *nextClientID, int *nextPetID) {
    int status = 1;

    if (!loadStylists(stylists, stylistCount, nextStylistID, "stylists.bin"))
        status = 0;
    if (!loadServices(services, serviceCount, "services.bin"))
        status = 0;
    if (!loadClients(clients, clientCount, nextClientID, nextPetID, "clients.bin"))
        status = 0;

    return status;
}

/* =================================================================
    DISPLAY MENU
    Displays the main menu for the Pet Salon Management System.

    @param hasRecords   : Flag indicating if any records (stylists, services, etc.) exist (1 for true, 0 for false)
    @return             : None
    @pre                : hasRecords should be an integer value 0 or 1.
   ================================================================= */
void displayMenu(int hasRecords) {
    printf("\nPet Salon System - Main Menu\n");

    if (hasRecords == 0) {      // If there are no records yet, show only Add and Exit
        printf("1. Add\n");
        printf("7. Exit\n");
    } else {                    // If there are records, show all main options
        printf("1. Add\n");
        printf("2. Edit\n");
        printf("3. Delete\n");
        printf("4. Book\n");
        printf("5. End Month\n");
        printf("6. Generate Reports\n");
        printf("7. Exit\n");
    }
}

/* =================================================================
    DISPLAY ADD MENU
    Displays the Add Options submenu for creating new records.

    @return : None
    @pre    : This function is called from main() when the user selects 1 (Add).
   ================================================================= */
void displayAddMenu() {
    printf("\nAdd Options:\n");
    printf("1. Add Client\n");
    printf("2. Add Pet\n");
    printf("3. Add Stylist\n");
    printf("4. Add Service\n");
    printf("5. Exit\n");
}

/* =================================================================
    SELECT STYLIST
    Recommends a stylist for a new client using the recursive point algorithm.

    @param stylists[]       : Array of all stylists in the record
    @param nStylists        : Number of stylists currently employed
    @param *pRecommender    : Pointer to the client who recommended the salon
    @return                 : Pointer to the stylist who accumulated the most points
    @pre                    : stylists array is initialized; nStylists > 0.  
   ================================================================= */
Stylist* stylistSelectByPoints(Stylist stylists[], int nStylists, Client *pRecommender) {
    int points[MAX_STYLISTS] = {0}; 
    int i;
    int nBestIndex = 0;
    Stylist *pRecommended = NULL;

    // Only runs if the new client was recommended by someone
    if (pRecommender != NULL) {
        // Start the recursive point accumulation starting with the recommender
        accumulatePoints(pRecommender, points, stylists, nStylists);

        // Determine which stylist index earned the most points
        for (i = 1; i < nStylists; i++) {
            if (points[i] > points[nBestIndex]) {
                nBestIndex = i;
            }
        }
    }

    // Assign result to a variable to ensure a single return statement
    pRecommended = &stylists[nBestIndex];

    return pRecommended;
}

/* =================================================================
    ADD STYLIST
    Adds a new stylist to the system with automatically determined attributes.

    @param stylists[]       : Array to store stylist data
    @param *stylistCount    : Pointer to the current number of stylists in the array
    @param *nextStylistID   : Pointer to the next available sequential ID number
    @param name             : String containing the stylist's name in <Lastname>, <Firstname> format
    @return                 : None
    @pre                    : stylists array is not full; nextStylistID is a positive integer.
   ================================================================= */
void addStylist(Stylist stylists[], int *stylistCount, int *nextStylistID, const char *name) {
    int i;
    int duplicate = 0;

    // Check for capacity before adding
    if (*stylistCount >= MAX_STYLISTS) {
        printf("Error: stylist list is full\n");
    } else {
        for (i = 0; i < *stylistCount && !duplicate; i++) {
            if (strcmp(stylists[i].name, name) == 0) {
                duplicate = 1;
            }
        }

        if (duplicate) {
            printf("Error: stylist already exists\n");
        } else {
            stylists[*stylistCount].stylistID = (*nextStylistID)++;                 // ID is automatically assigned sequentially

            snprintf(stylists[*stylistCount].name, MAX_NAME_LEN + 1, "%s", name);   // Copy name and ensure it fits the 200-character limit

            stylists[*stylistCount].rank = RANK_JUNIOR;                             // Rank is automatically set to the lowest rank (Junior)
            stylists[*stylistCount].salary = JUNIOR_SALARY;                         // Appropriate salary is set based on rank
            stylists[*stylistCount].isActive = 1;                                   // Active attribute is set to true (1)
            stylists[*stylistCount].renderedCount = 0;                              // Initialize service records for the new month

            (*stylistCount)++;
            printf("Stylist added successfully!\n");

            saveStylists(stylists, *stylistCount, "stylists.bin");     // Save data immediately after input
        }
    }
}

/* =================================================================
    ADD SERVICE
    Adds a new service to the application's service record.

    @param services[]       : Array to store service data
    @param *serviceCount    : Pointer to the current number of services recorded
    @param name             : The name of the service (max 200 characters)
    @param description      : A short description of the service (max 300 characters)
    @param price            : The cost of the service in pesos
    @return                 : None
    @pre                    : serviceCount must not be null; name and description must not be null.
   ================================================================= */
void addService(Service services[], int *serviceCount, const char *name, const char *description, float price) {
    int i;
    int duplicate = 0;

    for (i = 0; i < *serviceCount && !duplicate; i++) {
        if (strcmp(services[i].name, name) == 0) {
            duplicate = 1;
        }
    }

    if (duplicate) {
        printf("Error: a service named '%s' already exists.\n", name);
    } else if (*serviceCount < MAX_SERVICES) {
        // All attributes except isOffered are user input
        snprintf(services[*serviceCount].name, MAX_NAME_LEN + 1, "%s", name);
        snprintf(services[*serviceCount].description, MAX_DESC_LEN + 1, "%s", description);
        services[*serviceCount].price = price;

        services[*serviceCount].isOffered = 1;  // Is Offered is automatically set to true (1) 
        
        services[*serviceCount].isUsed = 1;     // Mark the slot as occupied in the array 

        (*serviceCount)++;
        printf("Service added successfully!\n");
        saveServices(services, *serviceCount, "services.bin");  // Save data immediately after input
    } else {
        printf("Cannot add more services. Maximum limit reached.\n");
    }
}

/* =================================================================
    SELECT RECOMMENDER
    Displays a list of existing clients and allows the user to select a recommender.
 
    @param clients[]    : Array of all current salon clients
    @param nClients     : Number of clients currently in the record
    @param currentIdx   : The index of the client being added/edited (to prevent self-recommendation)
    @return             : Pointer to the selected recommender client, or NULL if none
    @pre                : clients array is initialized; nClients is a non-negative integer
   ================================================================= */
Client* selectRecommender(Client clients[], int nClients, int currentIdx) {
    int i;
    int selection;
    Client *pSelected = NULL;

    // If no other clients exist, the recommender is automatically NULL
    if (nClients == 0 || (nClients == 1 && currentIdx == 0)) {
        printf("Recommender: No existing clients to choose from.\n");
    } else {
        printf("Select Recommender:\n");
        printf("0. None (No Recommender)\n");

        // Display only "existing" clients
        for (i = 0; i < nClients; i++) {
            // A client cannot be their own recommender during an edit
            if (i != currentIdx) {
                printf("%d. %s\n", i + 1, clients[i].clientName);
            }
        }

        printf("Enter Choice: ");
        scanf("%d", &selection);

        // Check if the choice matches a valid existing client
        if (selection > 0 && selection <= nClients) {
            // Ensure the user didn't manually type the current client's number
            if ((selection - 1) != currentIdx) {
                pSelected = &clients[selection - 1];
            }
        }
    }

    return pSelected;
}

/* =================================================================
    DISPLAY EDIT MENU
    Displays the Edit Options submenu for modifying existing records.

    @return : None
    @pre    : This function is called from main() when the user selects 2 (Edit).
   ================================================================= */
void displayEditMenu() {
    printf("\nEdit Options:\n");
    printf("1. Edit Client\n");
    printf("2. Edit Pet\n");
    printf("3. Edit Stylist\n");
    printf("4. Edit Service\n");
    printf("5. Exit\n");
}

/* =================================================================
    EDIT STYLIST
    Edits the information of an existing stylist.

    @param stylists[]   : Array to store stylist data
    @param stylistCount : The current number of stylists in the record
    @param stylistID    : The unique ID of the stylist to be edited
    @param newName      : The new name to assign to the stylist
    @return             : None
    @pre                : stylists array must be initialized; stylistID must be a positive integer.
   ================================================================= */
void editStylist(Stylist stylists[], int stylistCount, int stylistID, const char *newName) {
    int i;
    int found = 0;

    for (i = 0; i < stylistCount && !found; i++) {
        if (stylists[i].stylistID == stylistID) {
            snprintf(stylists[i].name, MAX_NAME_LEN + 1, "%s", newName);
            found = 1;
        }
    }

    if (!found) {
        printf("Stylist with ID %d not found.\n", stylistID);
    } else {
        printf("Stylist updated successfully!\n");
        saveStylists(stylists, stylistCount, "stylists.bin");     // Save data immediately after input
    }
}

/* =================================================================
    EDIT SERVICE
    Edits the description and price of an existing service record.

    @param services[]       : Array to store service data
    @param serviceCount     : The current number of services in the record
    @param name             : The name of the service to be edited
    @param newDescription   : The new description to assign to the service
    @param newPrice         : The new price to assign to the service
    @return                 : None
    @pre                    : services array must be initialized; serviceCount must be a positive integer.
   ================================================================= */
void editService(Service services[], int serviceCount, const char *name, const char *newDescription, float newPrice) {
    int i;
    int found = 0;

    for (i = 0; i < serviceCount && !found; i++) {
        if (strcmp(services[i].name, name) == 0) {
            // Only description and price can be edited
            snprintf(services[i].description, MAX_DESC_LEN + 1, "%s", newDescription);
            services[i].price = newPrice;
            found = 1;
        }
    }

    if (!found) {
        printf("Service with name '%s' not found.\n", name);
    } else {
        printf("Service updated successfully!\n");
        saveServices(services, serviceCount, "services.bin");     // Save data immediately after input
    }
}

/* =================================================================
    DISPLAY DELETE MENU
    Displays the Delete Options submenu for managing record removals.

    @return : None
    @pre    : This function is called from main() when the user selects 3 (Delete).
   ================================================================= */
void displayDeleteMenu() {
    printf("\nDelete Options:\n");
    printf("1. Delete Client\n");
    printf("2. Delete Pet\n");
    printf("3. Delete Stylist\n");
    printf("4. Delete Service\n");
    printf("5. Exit\n");
}

/* =================================================================
    DELETE STYLIST
    Marks a stylist as inactive after user confirmation.
    
    @param stylists[]       : Array to store stylist data
    @param *stylistCount    : Pointer to the number of stylists
    @param stylistID        : The unique ID of the stylist to be marked inactive
    @return                 : None
    @pre                    : stylistCount is greater than 0; stylists array is initialized.
   ================================================================= */
void deleteStylist(Stylist stylists[], int stylistCount, int stylistID) {
    int i;
    int found = 0;
    char confirm;

    for (i = 0; i < stylistCount && !found; i++) {
        if (stylists[i].stylistID == stylistID) {
            found = 1;

            // Before deleting, a confirmation notification must be given
            printf("Are you sure you want to delete stylist %s? (y/n): ", stylists[i].name);
            scanf(" %c", &confirm);

            if (confirm == 'y' || confirm == 'Y') {
                // Stylists are not actually deleted so set active to false
                stylists[i].isActive = 0;
                printf("%s is now inactive.\n", stylists[i].name);
                saveStylists(stylists, stylistCount, "stylists.bin");     // Save data immediately after input
            } else {
                printf("Deletion cancelled.\n");
            }
        }
    }

    // Only print 'not found' if the loop finished without finding the ID
    if (!found) {
        printf("Stylist with ID %d not found.\n", stylistID);
    }
}

/* =================================================================
    DELETE SERVICE
    Marks a service as no longer offered after user confirmation.
    
    @param services[]       : Array to store service data
    @param serviceCount     : The current number of services recorded
    @param name             : The name of the service to be deactivated
    @return                 : None
    @pre                    : services array is initialized; name is not null.
   ================================================================= */
void deleteService(Service services[], int serviceCount, const char *name) {
    int i;
    int found = 0;
    char confirm;

    for (i = 0; i < serviceCount && !found; i++) {
        if (strcmp(services[i].name, name) == 0) {
            found = 1;

            // Before deleting, a confirmation notification must be given
            printf("Are you sure you want to stop offering %s? (y/n): ", services[i].name);
            scanf(" %c", &confirm);

            if (confirm == 'y' || confirm == 'Y') {
                // Services are not actually deleted so Is Offered is set to false
                services[i].isOffered = 0;
                printf("%s is now not offered.\n", services[i].name);
                saveServices(services, serviceCount, "services.bin");     // Save data immediately after input
            } else {
                printf("Deletion cancelled.\n");
            }
        }
    }

    // Only print 'not found' if the loop finished without finding the name
    if (!found) {
        printf("Service with name '%s' not found.\n", name);
    }
}

/* =================================================================
    GET RANK STRING
    Returns the string representation of a stylist's rank.
    
    @param rank : The StylistRank enumeration value (Junior, Senior, or Master)
    @return     : A constant string containing the name of the rank
    @pre        : rank must be a valid member of the StylistRank enum.
   ================================================================= */
const char* getRankString(StylistRank rank) {
    const char* rankName;

    switch (rank) {
        case RANK_JUNIOR:
            rankName = "Junior Stylist";
            break;
        case RANK_SENIOR:
            rankName = "Senior Stylist";
            break;
        case RANK_MASTER:
            rankName = "Master Stylist";
            break;
        default:
            rankName = "Unknown";
            break;
    }

    return rankName;
}

/* =================================================================
    STYLIST ACTIVE CHECK
    Checks if a specific stylist is currently marked as active.

    @param stylists[]   : Array containing all stylist records
    @param stylistCount : The current number of stylists in the system
    @param stylistID    : The unique ID of the stylist to check
    @return             : 1 if the stylist is found and active, 0 otherwise
    @pre                : stylists array must be initialized; stylistID must be a positive integer.
   ================================================================= */
int stylistActiveCheck(Stylist stylists[], int stylistCount, int stylistID) {
    int i;
    int activeStatus = 0; 
    int found = 0;        

    for (i = 0; i < stylistCount && !found; i++) {
        if (stylists[i].stylistID == stylistID) {
            activeStatus = stylists[i].isActive;
            found = 1; 
        }
    }

    return activeStatus;
}

/* =================================================================
    STYLIST SELECTION BY POINTS
    A recursive helper function that traverses the recommendation chain to award points.
 
    @param *pClient     : Pointer to the client currently being evaluated in the chain
    @param points[]     : Array storing the accumulated points for each stylist
    @param stylists[]   : Array of all stylists in the salon record
    @param nStylists    : Total number of stylists currently in the record
    @return             : none
    @pre                : points array is initialized to all zeros; stylists array is initialized.
   ================================================================= */
void accumulatePoints(Client *pClient, int points[], Stylist stylists[], int nStylists) {
    int i;

    // Recursion stops when a client with no recommender (NULL) is reached
    if (pClient != NULL) {
        // Identify which stylist matches the client's preference and award a point
        for (i = 0; i < nStylists; i++) {
            if (&stylists[i] == pClient->chosenStylist) {
                points[i]++;
            }
        }

        // Move to the next person in the word-of-mouth chain
        accumulatePoints(pClient->clientRecommender, points, stylists, nStylists);
    }
}

/* =================================================================
    VALID PRICE CHECK
    Validates that a service price is non-negative.

    @param price    : The cost of the service in pesos
    @return         : 1 if the price is 0 or greater, 0 otherwise
    @pre            : price should be a float value.
   ================================================================= */
int validPriceCheck(float price) {
    int isValid;

    isValid = (price >= 0.0f);

    return isValid;
}

/* =================================================================
    FIND SERVICE INDEX
    Finds the index of a service in the services array by its name.
    
    @param services[]   : Array containing all service records
    @param serviceCount : The current number of services in the system
    @param *serviceName : The name string to search for
    @return             : The index of the service if found, or -1 if not found
    @pre                : services array is initialized; serviceName is not null.
   ================================================================= */
int findServiceIndex(Service services[], int serviceCount, const char *serviceName) {
    int i;
    int index = -1;
    int found = 0;

    for (i = 0; i < serviceCount && !found; i++) {
        if (strcmp(services[i].name, serviceName) == 0) {
            index = i;
            found = 1;
        }
    }

    return index;
}

/* =================================================================
    BOOK SERVICE
    Books a service booking for both a stylist and a pet.

    @param stylists[]   : stylist records
    @param stylistCount : Number of stylists in the system
    @param services[]   : service records
    @param serviceCount : Number of services in the system
    @param *targetPet   : Pointer to the pet availing the service
    @param stylistID    : ID of the stylist performing the service
    @param serviceName  : Name of the service being availed
    @param date         : The date the service is performed
    @return             : None
    @pre                : targetPet and date are not null so arrays are initialized.
   ================================================================= */
void bookService(Stylist stylists[], int stylistCount, Service services[], int serviceCount, Pet *targetPet, int stylistID,
                 const char *serviceName, Date date) {
    int i;
    int foundStylist = 0;
    int foundService = 0;
    Stylist *stylistPtr = NULL;
    Service *servicePtr = NULL;

    for (i = 0; i < stylistCount && !foundStylist; i++) {
        if (stylists[i].stylistID == stylistID && stylists[i].isActive) {
            stylistPtr = &stylists[i];
            foundStylist = 1;
        }
    }

    for (i = 0; i < serviceCount && !foundService; i++) {
        if (strcmp(services[i].name, serviceName) == 0 && services[i].isOffered) {
            servicePtr = &services[i];
            foundService = 1;
        }
    }

    if (foundStylist && foundService) {
        // 1. Update Stylist Record (Cap: 50)
        if (stylistPtr->renderedCount < MAX_RENDERED_THIS_MONTH) {
            stylistPtr->renderedServices[stylistPtr->renderedCount].service = servicePtr;
            stylistPtr->renderedServices[stylistPtr->renderedCount].date = date;
            stylistPtr->renderedCount++;
            
            // 2. Update Pet Record (Cap: 5, FIFO logic)
            // Shift existing services left if the record is full
            if (targetPet->PetAge.months >= 0) // Logical check placeholder for pet age, can be replaced with actual condition if needed
            {
                int j;
                int currentPetCount = 0;
                
                // Check current occupancy in pet history
                for(j = 0; j < 5; j++) {
                    if (targetPet->ServicesAvailed[j].service_name[0] != '\0')
                        currentPetCount++;
                }

                if (currentPetCount == 5) {
                    // Shift: Oldest service is removed, others move down */
                    for (j = 0; j < 4; j++) {
                        targetPet->ServicesAvailed[j] = targetPet->ServicesAvailed[j+1];
                    }

                    currentPetCount = 4;
                }
                
                // Add new service to pet at the last available slot
                snprintf(targetPet->ServicesAvailed[currentPetCount].service_name, 201, "%s", serviceName);
                targetPet->ServicesAvailed[currentPetCount].DateAvailed = date;
            }

            printf("Booking successful for %s!\n", targetPet->petName);
            // bookService only has access to the pet, so it can't save the full clients[] array.
            saveStylists(stylists, stylistCount, "stylists.bin");
        }
    } else {
        printf("Error: Stylist or Service not found/active.\n");
    }
}

/* =================================================================
    END MONTH
    Processes end-of-month promotions and salary adjustments for all active stylists.
    
    @param stylists[]   : Array containing stylist records
    @param stylistCount : The current number of stylists in the system
    @return             : None
    @pre                : stylists array is initialized; stylistCount is non-negative.
   ================================================================= */
void endMonth(Stylist stylists[], int stylistCount) {
    int i;

    printf("\nEnd Month Processing\n");
    printf("\nPromoted Stylists:\n");

    // Inactive stylists are unaffected by this action
    for (i = 0; i < stylistCount; i++) {
        if (stylists[i].isActive) {
            // Promotion from junior to Senior (Threshold: 10)
            if (stylists[i].rank == RANK_JUNIOR && stylists[i].renderedCount >= JUNIOR_TO_SENIOR_THRESHOLD) {
                stylists[i].rank = RANK_SENIOR;
                stylists[i].salary = SENIOR_SALARY;
                printf("- %s was promoted to Senior Stylist\n", stylists[i].name);
            }
            // Promotion Logic from Senior to Master (Threshold: 20)
            else if (stylists[i].rank == RANK_SENIOR && stylists[i].renderedCount >= SENIOR_TO_MASTER_THRESHOLD) {
                stylists[i].rank = RANK_MASTER;
                stylists[i].salary = MASTER_SALARY;
                printf("- %s was promoted to Master Stylist\n", stylists[i].name);
            }
        }
    }

    printf("\nMaster Stylist Salaries:\n");
    for (i = 0; i < stylistCount; i++) {
        if (stylists[i].isActive && stylists[i].rank == RANK_MASTER) {
            // Master Stylists get base salary + 100 per service rendered
            stylists[i].salary = MASTER_SALARY + (stylists[i].renderedCount * MASTER_BONUS_PER_SERVICE);
            
            printf("- %s new salary is PHP %.2f.\n", stylists[i].name, stylists[i].salary);
        }

        // List of rendered services are emptied after processing
        stylists[i].renderedCount = 0;
    }

    saveStylists(stylists, stylistCount, "stylists.bin");     // Save data immediately after input

    printf("\nEnd of month processing completed.\n");
}

/* =================================================================
    DISPLAY REPORT MENU
    Displays the report submenu and routes the user to specific report generators.

    @param clients[]    : Array of all client records
    @param nClients     : Total number of clients currently in the system
    @param stylists[]   : Array of all stylist records
    @param nStylists    : Total number of stylists currently employed
    @param services[]   : Array of all service records
    @param nServices    : Total number of services currently in the system
    @return             : none
    @pre                : All arrays are initialized; counts are non-negative
   ================================================================= */
void displayReportMenu(Client clients[], int nClients, Stylist stylists[], int nStylists, Service services[], int nServices) {
    int nChoice;
    int stylistID;

    printf("\n--- Report Options ---\n");
    printf("1. Services Availed by a Client\n");
    printf("2. Services Rendered by a Stylist\n");
    printf("3. Pets by Owner\n");
    printf("4. Salaries versus Income\n");
    printf("5. Exit Submenu\n");
    printf("Enter choice: ");
    scanf("%d", &nChoice);

    if (nChoice == 1) {
        // Display and save services for a selected client
        reportServicesAvailed(clients, nClients, services, nServices);
    } else if (nChoice == 2) {
        // Display and save services rendered by a specific stylist
        printf("Enter Stylist ID: ");
        scanf("%d", &stylistID);
        servicesRenderedByStylist(stylists, nStylists, stylistID);
    } else if (nChoice == 3) {
        // List all clients and their pets; save to "Pets by owner.txt"
        displayPetsbyOwner(clients, nClients);
    } else if (nChoice == 4) {
        // Compare active stylist salaries against total service income
        salariesVsIncome(stylists, nStylists);
    }
}

/* =================================================================
    SERVICES RENDERED BY STYLIST
    Generates a report of the services rendered by a specific stylist.
    The report is displayed on screen and saved to a text file.

    @param stylists[]   : Array containing stylist records
    @param stylistCount : Number of stylists in the record
    @param stylistID    : The unique ID of the stylist to report on
    @return             : None
    @pre                : stylistCount > 0; stylists array is initialized.
   ================================================================= */
void servicesRenderedByStylist(Stylist stylists[], int stylistCount, int stylistID) {
    int i;
    int found = 0;
    float totalCost = 0.0f;
    Stylist *sPtr = NULL;
    FILE *fp;
    char filename[100];
    
    const char *monthNames[] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", 
                                "November", "December"};

    for (i = 0; i < stylistCount && !found; i++) {
        //Report includes both active and inactive stylists
        if (stylists[i].stylistID == stylistID) {
            sPtr = &stylists[i];
            found = 1;
        }
    }

    if (found) {
        // Data must be saved in "Services Rendered by Stylist <ID>.txt"
        sprintf(filename, "Services Rendered by Stylist %d.txt", stylistID);
        fp = fopen(filename, "w");

        //Screen Display: Name, ID, Rank, and Salary are mandatory
        printf("\nServices Rendered by Stylist Report\n");
        printf("Name: %s\n", sPtr->name);
        printf("ID Number: %d\n", sPtr->stylistID);
        printf("Rank: %s\n", getRankString(sPtr->rank)); // Uses your helper
        printf("Salary: PHP %.2f\n", sPtr->salary);
        printf("\nList of Recorded Services:\n");

        if (fp != NULL) {
            fprintf(fp, "Services Rendered by Stylist Report\n");
            fprintf(fp, "Name: %s\n", sPtr->name);
            fprintf(fp, "ID Number: %d\n", sPtr->stylistID);
            fprintf(fp, "Rank: %s\n", getRankString(sPtr->rank));
            fprintf(fp, "Salary: PHP %.2f\n", sPtr->salary);
            fprintf(fp, "\nList of Recorded Services:\n");
        }

        for (i = 0; i < sPtr->renderedCount; i++){
            // Track total cost for the final display
            totalCost += sPtr->renderedServices[i].service->price;

            // Displaying record with month spelled out
            printf("- %s (%s %d, %d)\n", sPtr->renderedServices[i].service->name, monthNames[sPtr->renderedServices[i].date.month],
                                         sPtr->renderedServices[i].date.day, sPtr->renderedServices[i].date.year);

            if (fp != NULL) {
                fprintf(fp, "- %s (%s %d, %d)\n", sPtr->renderedServices[i].service->name, monthNames[sPtr->renderedServices[i].date.month],
                                                  sPtr->renderedServices[i].date.day, sPtr->renderedServices[i].date.year);
            }
        }

        // The total cost of the services rendered is also displayed
        printf("\nTotal Cost of Services Rendered: PHP %.2f\n", totalCost);
        
        if (fp != NULL) {
            fprintf(fp, "\nTotal Cost of Services Rendered: PHP %.2f\n", totalCost);
            fclose(fp);
            printf("Report saved to %s successfully.\n", filename);
        }
    } else {
        printf("Stylist with ID %d not found.\n", stylistID);
    }
}

/* =================================================================
    SALARIES VS INCOME
    Generates a report comparing the salaries of stylists against total service income.
    The report is displayed on screen and saved to "Salaries versus Income.txt".

    @param: stylists[]      : Array containing all stylist records
    @param: stylistCount    : The current number of stylists in the system
    @return                 : None
    @pre                    : stylists array is initialized; stylistCount is a non-negative integer.
   ================================================================= */
void salariesVsIncome(Stylist stylists[], int stylistCount) {
    int i;
    int j;
    float individualIncome;
    float totalSalaries = 0.0f;
    float totalServiceIncome = 0.0f;
    float netDifference;
    FILE *fp;

    fp = fopen("Salaries versus Income.txt", "w");

    printf("\nSalaries versus Income Report\n");
    printf("--------------------------------------------------\n");
    
    if (fp != NULL) {
        fprintf(fp, "Salaries versus Income Report\n");
        fprintf(fp, "--------------------------------------------------\n");
    }

    for (i = 0; i < stylistCount; i++) {
        // Only active stylists are displayed in this specific report
        if (stylists[i].isActive) {
            individualIncome = 0.0f;

            // Calculate income generated by this specific stylist
            for (j = 0; j < stylists[i].renderedCount; j++) {
                individualIncome += stylists[i].renderedServices[j].service->price;
            }

            // Display mandatory details: Name, ID, Rank, and Salary
            printf("Stylist: %s (ID: %d)\n", stylists[i].name, stylists[i].stylistID);
            printf("Rank: %s | Salary: PHP %.2f\n", getRankString(stylists[i].rank), stylists[i].salary);
            printf("Income Generated: PHP %.2f\n\n", individualIncome);

            if (fp != NULL) {
                fprintf(fp, "Stylist: %s (ID: %d)\n", stylists[i].name, stylists[i].stylistID);
                fprintf(fp, "Rank: %s | Salary: PHP %.2f\n", getRankString(stylists[i].rank), stylists[i].salary);
                fprintf(fp, "Income Generated: PHP %.2f\n\n", individualIncome);
            }

            // Accumulate totals for the summary section
            totalSalaries += stylists[i].salary;
            totalServiceIncome += individualIncome;
        }
    }

    // Calculate the difference (Income - Salaries)
    netDifference = totalServiceIncome - totalSalaries;

    // Display the summary totals at the end of the report
    printf("--------------------------------------------------\n");
    printf("Total Stylist Salaries: PHP %.2f\n", totalSalaries);
    printf("Total Service Income:   PHP %.2f\n", totalServiceIncome);
    printf("Net Difference:         PHP %.2f\n", netDifference);

    if (fp != NULL) {
        fprintf(fp, "--------------------------------------------------\n");
        fprintf(fp, "Total Stylist Salaries: PHP %.2f\n", totalSalaries);
        fprintf(fp, "Total Service Income:   PHP %.2f\n", totalServiceIncome);
        fprintf(fp, "Net Difference:         PHP %.2f\n", netDifference);
        fclose(fp);
        printf("\nReport successfully saved to 'Salaries versus Income.txt'.\n");
    }
}

/* =================================================================
    SAVE ALL DATA
    Saves all application data into their respective binary files.

    @param stylists[]   : Array of stylist records
    @param stylistCount : The number of stylists
    @param services[]   : Array of service records
    @param serviceCount : The number of services
    @param clients[]    : Array of client records
    @param clientCount  : The number of clients
    @return             : 1 if all files were saved successfully, 0 otherwise
    @pre                : All arrays must be initialized.
   ================================================================= */
int saveAllData(Stylist stylists[], int stylistCount, Service services[], int serviceCount, Client clients[], int clientCount) {
    int success = 1;

    if (!saveStylists(stylists, stylistCount, "stylists.bin")) {
        success = 0;
    }

    if (!saveServices(services, serviceCount, "services.bin")) {
        success = 0;
    }

    if (!saveClients(clients, clientCount, "clients.bin")) {
        success = 0;
    }

    return success;
}

/* =================================================================
    TRIGGER RECOVERY
    Triggers the recovery process to save all data to binary files.
    This is meant to be called from main() (which is the only place that
    actually has all three arrays and counts in scope at once) rather
    than from the individual add/edit/delete functions.

    @param stylists[]   : Array of stylist records
    @param stylistCount : The number of stylists
    @param services[]   : Array of service records
    @param serviceCount : The number of services
    @param clients[]    : Array of client records
    @param clientCount  : The number of clients
    @return             : 1 if recovery save was successful, 0 otherwise
    @pre                : All arrays must be initialized.
   ================================================================= */
int triggerRecovery(Stylist stylists[], int stylistCount, Service services[], int serviceCount, Client clients[], int clientCount) {
    int status;

    status = saveAllData(stylists, stylistCount, services, serviceCount, clients, clientCount);

    if (status != 1) {
        fprintf(stderr, "Warning: recovery save failed\n");
    }

    return status;
}

/* =================================================================
    SAVE STYLISTS
    Saves the stylist records to a binary file.

    @param stylists[]   : Array of stylist records
    @param stylistCount : The number of stylists
    @param *filename    : The name of the binary file to save to
    @return             : 1 if save was successful, 0 otherwise
    @pre                : stylists array is initialized so stylistCount is non-negative
   ================================================================= */
int saveStylists(Stylist stylists[], int stylistCount, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return 0;
    }
    fwrite(&stylistCount, sizeof(int), 1, fp);
    fwrite(stylists, sizeof(Stylist), stylistCount, fp);
    fclose(fp);
    return 1;
}

/* =================================================================
    LOAD STYLISTS
    Loads stylist records from a binary file.

    @param stylists[]       : Array to load stylist records into
    @param *stylistCount    : Pointer to an integer to store the number of stylists loaded
    @param *nextStylistID   : Pointer to an integer to store the next available stylist ID
    @param *filename        : The name of the binary file to load from
    @return                 : 1 if load was successful, 0 otherwise
    @pre                    : stylists array is initialized; stylistCount and nextStylistID are valid pointers
   ================================================================= */
int loadStylists(Stylist stylists[], int *stylistCount, int *nextStylistID, const char *filename) {
    FILE *fp = fopen(filename, "rb");
    int i;
    int maxID = 0;

    if (fp == NULL) {
        *stylistCount = 0;
        if (nextStylistID != NULL) *nextStylistID = 1;
        return 1;   // No file yet is a normal first run, not a failure
    }

    fread(stylistCount, sizeof(int), 1, fp);
    fread(stylists, sizeof(Stylist), *stylistCount, fp);
    fclose(fp);

    for (i = 0; i < *stylistCount; i++) {
        if (stylists[i].stylistID > maxID) maxID = stylists[i].stylistID;
    }
    if (nextStylistID != NULL) *nextStylistID = maxID + 1;

    return 1;
}

/* =================================================================
    SAVE SERVICES
    Saves the service records to a binary file.

    @param services[]   : Array of service records
    @param serviceCount : The number of services
    @param *filename    : The name of the binary file to save to
    @return             : 1 if save was successful, 0 otherwise
    @pre                : services array is initialized so serviceCount is non-negative
   ================================================================= */
int saveServices(Service services[], int serviceCount, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return 0;
    }
    fwrite(&serviceCount, sizeof(int), 1, fp);
    fwrite(services, sizeof(Service), serviceCount, fp);
    fclose(fp);
    return 1;
}

/* =================================================================
    LOAD SERVICES
    Loads service records from a binary file.

    @param services[]       : Array to load service records into
    @param *serviceCount    : Pointer to an integer to store the number of services loaded
    @param *filename        : The name of the binary file to load from
    @return                 : 1 if load was successful, 0 otherwise
    @pre                    : services array is initialized; serviceCount is a valid pointer
   ================================================================= */
int loadServices(Service services[], int *serviceCount, const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        *serviceCount = 0;
        return 1;
    }
    fread(serviceCount, sizeof(int), 1, fp);
    fread(services, sizeof(Service), *serviceCount, fp);
    fclose(fp);
    return 1;
}
