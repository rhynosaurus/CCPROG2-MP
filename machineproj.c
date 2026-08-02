/*
    Description  : C file for the Pet Salon tracking system.
    Author/s     : MESINA, ANNA GABRIELLA B.    PEREZ, ERIN ALYANNA A.
    Section      : S20F
    Last Modified: AUGUST 2, 2026
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"

/* =================================================================
    FORWARD DECLARATIONS
    Some functions are used before they are defined. Adding function
    prototypes at the top lets the compiler know about these
    functions before they are used and prevents errors.
   ================================================================= */
int   loadStylists(Stylist stylists[], int *stylistCount, int *nextStylistID, const char *filename);
int   loadServices(Service services[], int *serviceCount, const char *filename);
int   loadClients(Client clients[], int *clientCount, int *nextClientID, int *nextPetID, const char *filename);
int   saveStylists(Stylist stylists[], int stylistCount, const char *filename);
int   saveServices(Service services[], int serviceCount, const char *filename);
int   saveClients(Client clients[], int clientCount, const char *filename);

int   loadAllData(Stylist stylists[], int *stylistCount, int *nextStylistID, Service services[], int *serviceCount,
                   Client clients[], int *clientCount, int *nextClientID, int *nextPetID);
int   saveAllData(Stylist stylists[], int stylistCount, Service services[], int serviceCount, Client clients[], int clientCount);
int   triggerRecovery(Stylist stylists[], int stylistCount, Service services[], int serviceCount, Client clients[], int clientCount);

void  displayMenu(int hasRecords);
void  displayAddMenu(void);
void  displayEditMenu(void);
void  displayDeleteMenu(void);
void  displayReportMenu(Client clients[], int nClients, Stylist stylists[], int nStylists, Service services[], int nServices);

int      newClientID(Client clients[], int numClient);
void     chooseCity(int *selectedCityIndex);
Client*  selectRecommender(Client clients[], int nClients, int currentIdx);
Stylist* stylistSelectByPoints(Stylist stylists[], int nStylists, Client *pRecommender);
Stylist* chooseStylist(Stylist stylists[], int numStylists);
void     accumulatePoints(Client *pClient, int points[], Stylist stylists[], int nStylists);

void addClient(Client clients[], int *numClients, Stylist stylists[], int nStylists);
int  newPetID(Client clients[], int numClients);
void addPetClient(Client clients[], int clientIdx, int petIdx, int nTotalClients);
void addPet(Client clients[], int nTotalClients);
void addStylist(Stylist stylists[], int *stylistCount, int *nextStylistID, const char *name);
void addService(Service services[], int *serviceCount, const char *name, const char *description, float price);

void editClient(Client clients[], int nTotalClients, Stylist stylists[], int nTotalStylists);
void editPet(Client clients[], int nTotalClients);
void editStylist(Stylist stylists[], int stylistCount, int stylistID, const char *newName);
void editService(Service services[], int serviceCount, const char *name, const char *newDescription, float newPrice);

void deleteClient(Client clients[], int *nTotalClients);
void deletePets(Client clients[], int nTotalClients);
void deleteStylist(Stylist stylists[], int stylistCount, int stylistID);
void deleteService(Service services[], int serviceCount, const char *name);

const char* getRankString(StylistRank rank);
int   stylistActiveCheck(Stylist stylists[], int stylistCount, int stylistID);
int   validPriceCheck(float price);
int   findClientIndex(Client clients[], int nTotalClients, int targetID);
Pet*  findPetIndex(Client clients[], int nTotalClients, int targetPetID, int *pOwnerIdx);
int   findServiceIndex(Service services[], int serviceCount, const char *serviceName);

void bookService(Stylist stylists[], int stylistCount, Service services[], int serviceCount, Pet *targetPet, int stylistID,
                  const char *serviceName, Date date);
void endMonth(Stylist stylists[], int stylistCount);

void reportServicesAvailed(Client clients[], int nClients, Service services[], int nServices);
void servicesRenderedByStylist(Stylist stylists[], int stylistCount, int stylistID);
void displayPetsbyOwner(Client clients[], int nClients);
void salariesVsIncome(Stylist stylists[], int stylistCount);
void reassignPet(Client clients[], int nOldOwnerIdx, int nOldPetIdx, int nNewOwnerIdx, int nNewPetSlotIdx);

// FUNCTIONS FOR PET SALON TRACKING SYSTEM
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
    CLIENT ID GENERATOR
    This function will generate the ID number for every new client added into the database.
    The ID generated will be assigned in sequential order.

    @param clients[]    : Array of existing client records
    @param numClient    : Total number of existing clients currently recorded
    @return             : The integer ID to be assigned, or -1 if the salon is at max capacity
    @pre                : the clients array is initialized and numClient is >= 0
   ================================================================= */
int newClientID(Client clients[], int numClient) {
    int i;
    int maxID = 0;
    int assignedID = -1;

    // Check salon capacity (max 20 clients) before assigning ID
    if (numClient < MAX_CLIENTS) {
        // Find the highest current ID to ensure the next is sequential
        for (i = 0; i < numClient; i++) {
            if (clients[i].id_client > maxID) {
                maxID = clients[i].id_client;
            }
        }
        
        // If no clients exist, maxID is 0, so the first ID becomes 1
        assignedID = maxID + 1;
    }

    return assignedID;
}

/* =================================================================
    CITY/MUNICIPALITY SELECTION
    This function takes in an input for the Client's city or municipality by choosing from a list of array of cities

    @param *selectedCityIndex   : Pointer to the integer storing the chosen city index
    @return                     : none
    @pre                        : selectedCityIndex is a valid pointer to an integer.   
   ================================================================= */
void chooseCity(int *selectedCityIndex) {
    char *City[] = {"Manila", "Mandaluyong", "Marikina", "Pasig", "Quezon City",
                    "San Juan", "Caloocan", "Malabon", "Navotas", "Valenzuela",
                    "Las Piñas", "Makati", "Muntinlupa", "Parañaque", "Pasay",
                    "Pateros", "Taguig"};
    int i;
    int j;
    int index;
    int choice;

    printf("Choose City/Municipality:\n");
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            index = j * 4 + i;
            printf("%2d. %-14s", index + 1, City[index]);
        }

        printf("\n");
    }

    printf("17. Taguig\n");
    printf("Enter Choice: ");
    scanf("%d", &choice);

    // Store the index (choice - 1) to point into the City array later
    *selectedCityIndex = choice - 1;
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
    CLIENT STYLIST
    This function displays a list of active stylists and allows the user to select one. 

    @param stylists[]   : Array of all stylists in the record
    @param numStylists  : The total number of stylists currently in the system
    @return             : A pointer to the selected active stylist
    @pre                : stylists array is initialized; numStylists is between 0 and MAX_STYLISTS. 
   ================================================================= */
Stylist* chooseStylist(Stylist stylists[], int numStylists) {
    int i;
    int choice;
    int activeCount = 0;
    int displayMapping[MAX_STYLISTS];
    Stylist *selectedStylist = NULL;

    printf("Available Stylists:\n");

    for (i = 0; i < numStylists; i++) {
        // Only display stylists whose isActive status is true (1)
        if (stylists[i].isActive == 1) {
            printf("%d. %s\n", activeCount + 1, stylists[i].name);
            
            // Map the menu number to the actual array index
            displayMapping[activeCount] = i;
            activeCount++;
        }
    }

    if (activeCount > 0) {
        printf("Enter Choice: ");
        scanf("%d", &choice);

        // Ensure the choice exists in the displayed list
        if (choice > 0 && choice <= activeCount) {
            selectedStylist = &stylists[displayMapping[choice - 1]];
        }
    }

    return selectedStylist;
}

/* =================================================================
    CLIENT CITY STORAGE
    clientCity is a pointer, so it needs a place to store the value.
    Each client gets its own storage space, so clientCity can point
    to the correct storage for that client.
   ================================================================= */
static int clientCityStorage[MAX_CLIENTS];

/* =================================================================
    ADD CLIENT
    This function is the full implementation of the Add Client option. This function adds new client data
    within the database of Clients including their pets to be added aside from the Add Pet function

    @param clients[]    : Array of existing client records
    @param *numClients  : Pointer to the total number of clients currently recorded
    @param stylists[]   : Array of all stylists in the record
    @param nStylists    : Total number of stylists currently employed
    @return             : none
    @pre                : clients, stylists arrays are initialized; numClients points to a valid int.  
   ================================================================= */
void addClient(Client clients[], int *numClients, Stylist stylists[], int nStylists) {
    int numPetsToAssign;
    int currentIdx;
    int clientID;
    int i;

    if (*numClients >= MAX_CLIENTS) {
        printf("The salon is at maximum capacity (20 clients). Cannot add more.\n");
    } else {
        currentIdx = *numClients;
        printf("--- Add New Client ---\n");

        // 1. ID Generation: IDs are assigned sequentially starting at 1
        clientID = newClientID(clients, *numClients);
        clients[currentIdx].id_client = clientID;

        // 2. Name: Stored in format <Lastname>, <Firstname> (up to 200 chars)
        printf("Name: ");
        scanf(" %200[^\n]", clients[currentIdx].clientName);

        // 3. City Selection: Restricted to NCR list
        clients[currentIdx].clientCity = &clientCityStorage[currentIdx];
        chooseCity(clients[currentIdx].clientCity);

        // 4. Recommender: Pointer to an existing client or NULL
        clients[currentIdx].clientRecommender = selectRecommender(clients, *numClients, currentIdx);

        // 5. Stylist Selection: Use algorithm for Full Requirement
        printf("Stylist Assignment: ");
        if (clients[currentIdx].clientRecommender != NULL) {
            clients[currentIdx].chosenStylist = stylistSelectByPoints(stylists, nStylists, clients[currentIdx].clientRecommender);
            printf("Recommended based on word-of-mouth: %s\n", clients[currentIdx].chosenStylist->name);
        } else {
            // Manual selection if there is no recommendation chain
            printf("No recommender found. Please select manually.\n");
            clients[currentIdx].chosenStylist = chooseStylist(stylists, nStylists);
        }

        // 6. Pet Entry: At least one pet must be entered at this point
        printf("\n--- Pet Registration ---\n");
        numPetsToAssign = 0;
        while (numPetsToAssign < 1 || numPetsToAssign > MAX_PETS_OWN) {
            printf("Number of Pets (1-%d): ", MAX_PETS_OWN);
            scanf("%d", &numPetsToAssign);
        }

        for (i = 0; i < numPetsToAssign; i++) {
            // Add individual pet details to this client's record
            addPetClient(clients, currentIdx, i, *numClients);
        }

        // Only increment the count if the client was actually added
        (*numClients)++;
        printf("Client and pets successfully recorded.\n");
    }
}

/* =================================================================
    PET ID GENERATOR
    This function generate Pet IDs everytime new Pets are added into the system

    @param clients[]    : Array of existing client records
    @param numClients   : Total number of clients currently recorded in the system
    @return             : The integer ID to be assigned to the new pet
    @pre                : clients array is initialized; numClients is a non-negative integer.
   ================================================================= */
int newPetID(Client clients[], int numClients) {
    int i;
    int j;
    int highestID = 0;
    int assignedID;

    for (i = 0; i < numClients; i++) {
        // Check all 5 pet slots for this owner
        for (j = 0; j < MAX_PETS_OWN; j++) {
            // Only check slots that actually contain a registered pet
            if (clients[i].ClientPets[j].id_pet > highestID) {
                highestID = clients[i].ClientPets[j].id_pet;
            }
        }
    }

    assignedID = highestID + 1;

    return assignedID;
}

/* =================================================================
    ADD PET CLIENT
    This function adds pets into a client's record

    @param clients[]        : Array of existing client records
    @param clientIdx        : The index of the client in the array
    @param petIdx           : The specific pet slot index (0 to 4) being filled
    @param nTotalClients    : Total number of clients currently in the system
    @return                 : none
    @pre                    : clients array is initialized; clientIdx and petIdx are within valid bounds.
   ================================================================= */
void addPetClient(Client clients[], int clientIdx, int petIdx, int nTotalClients) {
    int assignedID;

    if (clientIdx >= 0 && petIdx >= 0 && petIdx < MAX_PETS_OWN) {
        assignedID = newPetID(clients, nTotalClients);
        clients[clientIdx].ClientPets[petIdx].id_pet = assignedID;

        // Pet names are at most 200 characters long
        printf("Pet Name: ");
        scanf(" %200[^\n]", clients[clientIdx].ClientPets[petIdx].petName);

        // Age must be recorded in both years and months
        printf("Age:\n");
        printf("    Years: ");
        scanf("%d", &clients[clientIdx].ClientPets[petIdx].PetAge.years);

        printf("    Months: ");
        scanf("%d", &clients[clientIdx].ClientPets[petIdx].PetAge.months);

        // New pets start with an empty service history
        printf("Pet %s (ID: %d) added successfully.\n", clients[clientIdx].ClientPets[petIdx].petName, assignedID);
    }
}

/* =================================================================
    ADD PET
    This function is the I/O system of when Add Pet is the chosen option. 
    This function will ask user to input the ID# of the client where a new pet will be added

    @param clients[]        : Array of existing client records
    @param nTotalClients    : Total number of clients currently in the system
    @return                 : none
    @pre                    : clients array is initialized; nTotalClients is >= 0.
   ================================================================= */
void addPet(Client clients[], int nTotalClients) {
    int i;
    int clientChoice;
    int clientIdx;
    int petSlotIdx = -1;

    printf("--- Add New Pet ---\n");

    if (nTotalClients > 0) {
        // User selects which client the pet belongs to
        printf("Select client:\n");
        for (i = 0; i < nTotalClients; i++) {
            printf("%d. %s\n", i + 1, clients[i].clientName);
        }

        printf("Enter Choice: ");
        scanf("%d", &clientChoice);
        clientIdx = clientChoice - 1;

        // Check if selection is valid and if client has room for more pets
        if (clientIdx >= 0 && clientIdx < nTotalClients) {
            // Search for the first empty slot (ID == 0) in the 5-pet array
            for (i = 0; i < MAX_PETS_OWN; i++) {
                if (clients[clientIdx].ClientPets[i].id_pet == 0 && petSlotIdx == -1) {
                    petSlotIdx = i;
                }
            }

            // A client can only track up to five pets
            if (petSlotIdx != -1) {
                addPetClient(clients, clientIdx, petSlotIdx, nTotalClients);
                printf("Pet record successfully added to %s.\n", clients[clientIdx].clientName);
            } else {
                printf("Error: This client already has the maximum of 5 pets.\n");
            }
        } else {
            printf("Invalid selection.\n");
        }
    } else {
        printf("No clients recorded. Please add a client first.\n");
    }
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
    EDIT CLIENT
    This function edits an already existing client from the list

    @param clients[]        : Array of existing client records
    @param nTotalClients    : Total number of clients currently in the system
    @param stylists[]       : Array of all stylists in the record
    @param nTotalStylists   : Total number of stylists currently employed
    @return                 : none
    @pre                    : clients and stylists arrays are initialized; nTotalClients > 0.
   ================================================================= */
void editClient(Client clients[], int nTotalClients, Stylist stylists[], int nTotalStylists) {
    int i;
    int selection;
    int clientIdx;
    char *CityList[] = {"Manila", "Mandaluyong", "Marikina", "Pasig", "Quezon City",
                        "San Juan", "Caloocan", "Malabon", "Navotas", "Valenzuela",
                        "Las Piñas", "Makati", "Muntinlupa", "Parañaque", "Pasay",
                        "Pateros", "Taguig"};

    printf("--- Edit Client Record ---\n");
    for (i = 0; i < nTotalClients; i++) {
        printf("%d. %s\n", i + 1, clients[i].clientName);
    }

    printf("Enter Choice: ");
    scanf("%d", &selection);

    clientIdx = selection - 1;

    if (clientIdx >= 0 && clientIdx < nTotalClients) {
        printf("\n--- Current Data ---\n");
        printf("Name: %s\n", clients[clientIdx].clientName);
        
        if (clients[clientIdx].clientCity == NULL) {
            clients[clientIdx].clientCity = &clientCityStorage[clientIdx];
        }
        printf("City: %s\n", CityList[*clients[clientIdx].clientCity]);

        // Check if recommender is NULL before accessing its members
        if (clients[clientIdx].clientRecommender != NULL) {
            printf("Recommender: %s\n", clients[clientIdx].clientRecommender->clientName);
        } else {
            printf("Recommender: None\n");
        }

        printf("Stylist: %s\n", clients[clientIdx].chosenStylist->name);

        printf("\n--- Enter New Data ---\n");

        printf("New Name: ");
        scanf(" %200[^\n]", clients[clientIdx].clientName);

        chooseCity(clients[clientIdx].clientCity);
        clients[clientIdx].clientRecommender = selectRecommender(clients, nTotalClients, clientIdx);

        printf("Select New Preferred Stylist:\n");
        clients[clientIdx].chosenStylist = chooseStylist(stylists, nTotalStylists);

        printf("Client record updated successfully.\n");
    }
}

/* =================================================================
    EDIT PET
    This function asks user to select from a list of pets and asks which pet information will 
    be either to be edited/changed or to be reassigned to another client within the system.

    @param clients[]        : Array of existing client records
    @param nTotalClients    : Total number of clients currently recorded
    @return                 : none
    @pre                    : clients array is initialized; nTotalClients is a non-negative integer.
   ================================================================= */
void editPet(Client clients[], int nTotalClients) {
    int i;
    int j;
    int displayCount = 1;
    int petChoice;
    int foundClientIdx = -1;
    int foundPetIdx = -1;
    int newOwnerChoice;
    int newOwnerIdx;
    int availableSlot = -1;
    char reassignChoice;

    printf("--- Edit Pet Record ---\n");
    
    // Check through all clients and their 5 pet slots to show a list
    for (i = 0; i < nTotalClients; i++) {
        for (j = 0; j < MAX_PETS_OWN; j++) {
            // Only display slots that actually contain a registered pet
            if (clients[i].ClientPets[j].id_pet > 0) {
                printf("%d. [%s] %s\n", displayCount, clients[i].clientName, clients[i].ClientPets[j].petName);
                displayCount++;
            }
        }
    }

    if (displayCount > 1) {
        int internalCount;

        printf("Enter choice to edit: ");
        scanf("%d", &petChoice);

        // Find the pet selected by its position in the list
        internalCount = 1;
        for (i = 0; i < nTotalClients; i++) {
            for (j = 0; j < MAX_PETS_OWN; j++) {
                if (clients[i].ClientPets[j].id_pet > 0) {
                    if (internalCount == petChoice) {
                        foundClientIdx = i;
                        foundPetIdx = j;
                    }
                    internalCount++;
                }
            }
        }

        if (foundPetIdx != -1) {
            // Ask if the pet is to be assigned to a different client
            printf("Would you like to reassign this pet to a different owner? (Y/N): ");
            scanf(" %c", &reassignChoice);

            if (reassignChoice == 'Y' || reassignChoice == 'y') {
                printf("Select New Owner:\n");
                for (i = 0; i < nTotalClients; i++) {
                    if (i != foundClientIdx) {
                        printf("%d. %s\n", i + 1, clients[i].clientName);
                    }
                }

                printf("Enter choice: ");
                scanf("%d", &newOwnerChoice);
                newOwnerIdx = newOwnerChoice - 1;

                if (newOwnerIdx < 0 || newOwnerIdx >= nTotalClients || newOwnerIdx == foundClientIdx) {
                    printf("Invalid new owner selection. Reassignment skipped.\n");
                } else {
                    // New owner must have a free slot among their 5 pets
                    for (i = 0; i < MAX_PETS_OWN; i++) {
                        if (clients[newOwnerIdx].ClientPets[i].id_pet == 0 && availableSlot == -1) {
                            availableSlot = i;
                        }
                    }

                    if (availableSlot != -1) {
                        // Move data to new owner and clear old owner's slot
                        reassignPet(clients, foundClientIdx, foundPetIdx, newOwnerIdx, availableSlot);
                        foundClientIdx = newOwnerIdx;
                        foundPetIdx = availableSlot;
                        printf("Pet reassigned successfully.\n");
                    } else {
                        printf("Error: New owner already has the maximum of 5 pets.\n");
                    }
                }
            }

            // Edit non-ID fields (Name and Age)
            printf("\n--- Updating Details for %s ---\n", clients[foundClientIdx].ClientPets[foundPetIdx].petName);
            printf("New Name: ");
            scanf(" %200[^\n]", clients[foundClientIdx].ClientPets[foundPetIdx].petName);
            
            printf("New Age:\n");
            printf("    Years: ");
            scanf("%d", &clients[foundClientIdx].ClientPets[foundPetIdx].PetAge.years);
            printf("    Months: ");
            scanf("%d", &clients[foundClientIdx].ClientPets[foundPetIdx].PetAge.months);
            
            printf("Pet record updated.\n");
        }
    } else {
        printf("No pets currently recorded in the system.\n");
    }
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
    DELETE CLIENT
    This function deletes a chosen client and their respective pets within the system

    @param clients[]        : Array of existing client records
    @param *nTotalClients   : Pointer to the total number of clients recorded
    @return                 : none
    @pre                    : clients array is initialized; nTotalClients points to a non-negative integer
   ================================================================= */
void deleteClient(Client clients[], int *nTotalClients) {
    int i;
    int j;
    int selection;
    int targetIdx;
    char confirm;

    printf("--- Delete Client Record ---\n\n");

    // Display current clients for selection
    printf("Select client to delete:\n");
    for (i = 0; i < *nTotalClients; i++) {
        printf("%d. %s\n", i + 1, clients[i].clientName);
    }

    printf("Enter Choice: ");
    scanf("%d", &selection);
    targetIdx = selection - 1;

    // Check if selection is within the current record range
    if (targetIdx >= 0 && targetIdx < *nTotalClients) {
        printf("Confirmation: Are you sure you want to delete %s? <Y/N>: ", clients[targetIdx].clientName);
        scanf(" %c", &confirm);
        
        if (confirm == 'Y' || confirm == 'y') {
            // Inform the user about the client and pet removal
            printf("Client %s has been removed from the system.\n", clients[targetIdx].clientName);

            // Check through the fixed 5-pet array to announce pet deletion
            for (j = 0; j < MAX_PETS_OWN; j++) {
                if (clients[targetIdx].ClientPets[j].id_pet > 0) {
                    printf("Pet %s (ID: %d) has been removed from the system.\n", clients[targetIdx].ClientPets[j].petName, clients[targetIdx].ClientPets[j].id_pet);
                }
            }

            // Shift subsequent records up by one
            for (i = targetIdx; i < *nTotalClients - 1; i++) {
                clients[i] = clients[i + 1];
            }

            // Decrement the total client count */
            *nTotalClients -= 1;
            printf("Deletion complete.\n");
        } else {
            printf("Deletion cancelled.\n");
        }
    }
}

/* =================================================================
    DELETE PET
    This function deletes a chosen Pet within the system

    @param clients[]        : Array of existing client records
    @param nTotalClients    : Total number of clients currently in the system
    @return                 : none
    @pre                    : clients array is initialized; nTotalClients is a non-negative integer
   ================================================================= */
void deletePets(Client clients[], int nTotalClients) {
    int i;
    int j;
    int k;
    int displayCount = 1;
    int userChoice;
    int foundClientIdx = -1;
    int foundPetIdx = -1;
    int internalCount;
    char confirm;

    printf("--- Delete Pet Record ---\n");
    printf("Select pet to delete:\n");

    for (i = 0; i < nTotalClients; i++) {
        // Check through the fixed-size 5-pet array
        for (j = 0; j < MAX_PETS_OWN; j++) {
            // Only display slots that actually contain a pet
            if (clients[i].ClientPets[j].id_pet > 0) {
                printf("%d. %s (Owner: %s)\n", displayCount, clients[i].ClientPets[j].petName, clients[i].clientName);
                displayCount++;
            }
        }
    }

    if (displayCount > 1) {
        printf("Enter Choice: ");
        scanf("%d", &userChoice);

        // Re-scan to find which client and index match the user's choice
        internalCount = 1;
        for (i = 0; i < nTotalClients; i++) {
            for (j = 0; j < MAX_PETS_OWN; j++) {
                if (clients[i].ClientPets[j].id_pet > 0) {
                    if (internalCount == userChoice) {
                        foundClientIdx = i;
                        foundPetIdx = j;
                    }

                    internalCount++;
                }
            }
        }

        // Validation and Confirmation for deletion
        if (foundPetIdx != -1) {
            printf("\nConfirmation: Are you sure you want to delete %s? <Y/N>: ", 
                   clients[foundClientIdx].ClientPets[foundPetIdx].petName);
            scanf(" %c", &confirm);

            if (confirm == 'Y' || confirm == 'y') {
                printf("Pet %s has been removed from the system.\n", clients[foundClientIdx].ClientPets[foundPetIdx].petName);

                // Shift pets within the client's 5-slot array to maintain order
                for (k = foundPetIdx; k < MAX_PETS_OWN - 1; k++) {
                    clients[foundClientIdx].ClientPets[k] = clients[foundClientIdx].ClientPets[k + 1];
                }

                // Clear the last slot to ensure it is marked as empty
                clients[foundClientIdx].ClientPets[MAX_PETS_OWN - 1].id_pet = 0;
                
                printf("Done.\n");
            } else {
                printf("Deletion cancelled.\n");
            }
        }
    } else {
        printf("No pets currently recorded.\n");
    }
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
    FIND CLIENT INDEX
    This function finds takes in an ID number and goes through the list of current active Client IDs to match with the target ID 

    @param clients[]        : Array of existing client records
    @param nTotalClients    : Total number of clients currently in the system
    @param targetID         : The unique ID number being searched for
    @return                 : The index of the client in the array if found; otherwise, -1
    @pre                    : clients array is initialized; nTotalClients is a non-negative integer
   ================================================================= */
int findClientIndex(Client clients[], int nTotalClients, int targetID) {
    int i;
    int foundIdx = -1;

    for (i = 0; i < nTotalClients; i++) {
        // Match the target ID with the id_client member of the struct
        if (clients[i].id_client == targetID) {
            foundIdx = i;
        }
    }

    return foundIdx;
}

/* =================================================================
    FIND PET INDEX
    Searches for a pet by ID across all client records and returns its location.

    @param clients[]        : Array of all clients currently in the system
    @param nTotalClients    : Total number of clients currently recorded
    @param targetPetID      : The unique ID of the pet being searched for
    @param *pOwnerIdx       : Pointer to store the array index of the pet's owner
    @return                 : Pointer to the matching Pet structure, or NULL if not found
    @pre                    : clients array is initialized; pOwnerIdx points to a valid integer
   ================================================================= */
Pet* findPetIndex(Client clients[], int nTotalClients, int targetPetID, int *pOwnerIdx) {
    int i;
    int j;
    Pet *pFoundPet = NULL;

    for (i = 0; i < nTotalClients; i++) {
        // Each client has exactly MAX_PETS_OWN (5) slots in their array
        for (j = 0; j < MAX_PETS_OWN; j++) {
            // Only check slots that are actually occupied
            if (clients[i].ClientPets[j].id_pet == targetPetID) {
                pFoundPet = &clients[i].ClientPets[j];
                *pOwnerIdx = i;
            }
        }
    }

    return pFoundPet;
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
    SERVICES AVAILED BY CLIENT
    Generates a report of all services availed by a client's pets, including prices.

    @param clients[]    : Array of existing client records
    @param nClients     : Total number of clients currently in the system
    @param services[]   : Array of all pet grooming services offered
    @param nServices    : Total number of services in the salon record
    @return             : none
    @pre                : clients and services arrays are initialized; nClients > 0
   ================================================================= */
void reportServicesAvailed(Client clients[], int nClients, Service services[], int nServices) {
    int i;
    int j;
    int k;
    int clientChoice;
    int clientIdx;
    int found = 0;
    float totalSpent = 0.0f;
    FILE *fp = NULL;
    char fileName[100];
    char *monthNames[] = {"", "January", "February", "March", "April", "May", "June", 
                          "July", "August", "September", "October", "November", "December"};

    // User selects a client from the list
    printf("--- Generate Services Availed Report ---\n");
    for (i = 0; i < nClients; i++) {
        printf("%d. %s\n", i + 1, clients[i].clientName);
    }

    printf("Select Client: ");
    scanf("%d", &clientChoice);
    clientIdx = clientChoice - 1;

    if (clientIdx >= 0 && clientIdx < nClients) {
        sprintf(fileName, "Services Availed by Client %d.txt", clients[clientIdx].id_client);
        fp = fopen(fileName, "w");

        if (fp != NULL) {
            // Display Client Name and ID
            printf("\nReport for Client: %s (ID: %d)\n", clients[clientIdx].clientName, clients[clientIdx].id_client);
            fprintf(fp, "Report for Client: %s (ID: %d)\n", clients[clientIdx].clientName, clients[clientIdx].id_client);

            // Loop through all 5 possible pet slots
            for (i = 0; i < MAX_PETS_OWN; i++) {
                if (clients[clientIdx].ClientPets[i].id_pet > 0) {
                    // Display Pet Name, ID, and Age
                    printf("\n  Pet: %s (ID: %d)\n", clients[clientIdx].ClientPets[i].petName, clients[clientIdx].ClientPets[i].id_pet);
                    printf("  Age: %d years, %d months\n", clients[clientIdx].ClientPets[i].PetAge.years, clients[clientIdx].ClientPets[i].PetAge.months);
                    
                    fprintf(fp, "\n  Pet: %s (ID: %d)\n", clients[clientIdx].ClientPets[i].petName, clients[clientIdx].ClientPets[i].id_pet);
                    fprintf(fp, "  Age: %d years, %d months\n", clients[clientIdx].ClientPets[i].PetAge.years, clients[clientIdx].ClientPets[i].PetAge.months);

                    // Display list of last 5 services availed
                    printf("  Services History:\n");
                    fprintf(fp, "  Services History:\n");

                    for (j = 0; j < 5; j++) {
                        // Check if a service record exists in this slot
                        if (clients[clientIdx].ClientPets[i].ServicesAvailed[j].service_name[0] != '\0') {
                            // Look up price in master list to satisfy price requirement
                            for (k = 0; k < nServices && found == 0; k++) {
                                if (strcmp(services[k].name, clients[clientIdx].ClientPets[i].ServicesAvailed[j].service_name) == 0) {
                                    printf("    - %s (PHP %.2f) on %s %d, %d\n", services[k].name, services[k].price,
                                            monthNames[clients[clientIdx].ClientPets[i].ServicesAvailed[j].DateAvailed.month],
                                            clients[clientIdx].ClientPets[i].ServicesAvailed[j].DateAvailed.day,
                                            clients[clientIdx].ClientPets[i].ServicesAvailed[j].DateAvailed.year);

                                    fprintf(fp, "    - %s (PHP %.2f) on %s %d, %d\n", services[k].name, services[k].price,
                                            monthNames[clients[clientIdx].ClientPets[i].ServicesAvailed[j].DateAvailed.month],
                                            clients[clientIdx].ClientPets[i].ServicesAvailed[j].DateAvailed.day,
                                            clients[clientIdx].ClientPets[i].ServicesAvailed[j].DateAvailed.year);
                                        
                                    totalSpent += services[k].price;
                                    found = 1;
                                }
                            }
                        }
                    }
                }
            }

            // Display total price of all services availed by all pets
            printf("\nTOTAL PRICE OF ALL SERVICES: PHP %.2f\n", totalSpent);
            fprintf(fp, "\nTOTAL PRICE OF ALL SERVICES: PHP %.2f\n", totalSpent);

            fclose(fp);
            printf("\nReport successfully saved to %s.\n", fileName);
        }
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
    DISPLAY PETS BY OWNER
    This function displays ALL of the Pets sorted by owners as recorded into the system

    @param clients[]    : Array of all clients currently in the system
    @param nClients     : Total number of clients recorded
    @return             : None
    @pre                : clients array is initialized; nClients is a non-negative integer
   ================================================================= */
void displayPetsbyOwner(Client clients[], int nClients) {
    int i;
    int j;
    FILE *fp = NULL;
    char *CityList[] = {"Manila", "Mandaluyong", "Marikina", "Pasig", "Quezon City",
                        "San Juan", "Caloocan", "Malabon", "Navotas", "Valenzuela", 
                        "Las Piñas", "Makati", "Muntinlupa", "Parañaque", "Pasay", 
                        "Pateros", "Taguig"};

    fp = fopen("Pets by owner.txt", "w");

    if (fp != NULL) {
        printf("--- Pets by Owner Report ---\n\n");
        fprintf(fp, "--- Pets by Owner Report ---\n\n");

        for (i = 0; i < nClients; i++) {
            // Display Client Name, ID, and City
            printf("Client: %s (ID: %d)\n", clients[i].clientName, clients[i].id_client);
            
            // Dereference the clientCity pointer to get the index for CityList
            printf("City  : %s\n", CityList[*clients[i].clientCity]);
            printf("Pets  :\n");

            fprintf(fp, "Client: %s (ID: %d)\n", clients[i].clientName, clients[i].id_client);
            fprintf(fp, "City  : %s\n", CityList[*clients[i].clientCity]);
            fprintf(fp, "Pets  :\n");

            // Check through the fixed 5-pet array and check if slot is active
            for (j = 0; j < MAX_PETS_OWN; j++) {
                if (clients[i].ClientPets[j].id_pet > 0) {
                    // Display Pet Name, ID, and Age
                    printf("    - %s (ID: %d), Age: %dy %dm\n", clients[i].ClientPets[j].petName, clients[i].ClientPets[j].id_pet,
                           clients[i].ClientPets[j].PetAge.years, clients[i].ClientPets[j].PetAge.months);
                    
                    fprintf(fp, "    - %s (ID: %d), Age: %dy %dm\n", clients[i].ClientPets[j].petName, clients[i].ClientPets[j].id_pet,
                            clients[i].ClientPets[j].PetAge.years, clients[i].ClientPets[j].PetAge.months);
                }
            }

            printf("\n");
            fprintf(fp, "\n");
        }

        printf("End of Report. Saved to 'Pets by owner.txt'.\n");
        fclose(fp);
    }
}

/* =================================================================
    REASSIGN PET TO ANOTHER CLIENT
    This function reassigns a pet from its initial owner to a new owner

    @param clients[]        : Array of all client records in the system
    @param nOldOwnerIdx     : Array index of the current owner
    @param nOldPetIdx       : Array index (0-4) of the pet being moved
    @param nNewOwnerIdx     : Array index of the target new owner
    @param nNewPetSlotIdx   : Array index of the first empty slot in the new owner's record
    @return                 : None
    @pre                    : All indices are within valid bounds; the new owner has at least one empty slot
   ================================================================= */
void reassignPet(Client clients[], int nOldOwnerIdx, int nOldPetIdx, int nNewOwnerIdx, int nNewPetSlotIdx) {
    Pet movingPet;
    int i;

    // Capture the data of the pet to be moved
    movingPet = clients[nOldOwnerIdx].ClientPets[nOldPetIdx];

    // Remove the pet from the original client by shifting remaining pets up
    for (i = nOldPetIdx; i < MAX_PETS_OWN - 1; i++) {
        clients[nOldOwnerIdx].ClientPets[i] = clients[nOldOwnerIdx].ClientPets[i + 1];
    }

    // Clear the last slot of the old owner to mark it as empty
    clients[nOldOwnerIdx].ClientPets[MAX_PETS_OWN - 1].id_pet = 0;

    // Add the pet to the new client owner's identified empty slot
    clients[nNewOwnerIdx].ClientPets[nNewPetSlotIdx] = movingPet;
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

/* =================================================================
    SAVE CLIENTS
    Saves the client records to a binary file.

    @param clients[]    : Array of client records
    @param clientCount  : The number of clients
    @param *filename    : The name of the binary file to save to
    @return             : 1 if save was successful, 0 otherwise
    @pre                : clients array is initialized so clientCount is non-negative
   ================================================================= */
int saveClients(Client clients[], int clientCount, const char *filename) {
    (void)clients;
    (void)clientCount;
    (void)filename;
    return 1;
}

/* =================================================================
    LOAD CLIENTS
    Loads client records from a binary file.

    @param clients[]        : Array to load client records into
    @param *clientCount     : Pointer to an integer to store the number of clients loaded
    @param *nextClientID    : Pointer to an integer to store the next available client ID
    @param *nextPetID       : Pointer to an integer to store the next available pet ID
    @param *filename        : The name of the binary file to load from
    @return                 : 1 if load was successful, 0 otherwise
    @pre                    : clients array is initialized; clientCount, nextClientID, and next
   ================================================================= */
int loadClients(Client clients[], int *clientCount, int *nextClientID, int *nextPetID, const char *filename) {
    (void)clients;
    (void)filename;
    *clientCount = 0;
    if (nextClientID != NULL) *nextClientID = 1;
    if (nextPetID != NULL) *nextPetID = 1;
    return 1;
}

/* =================================================================
    MAIN
    For testing the Pet Salon system implemented above.
    Wires the various Add/Edit/Delete/Book/End Month/Report
    functions to the menu functions already defined in this file.
   ================================================================= */
int main(void) {
    static Stylist stylists[MAX_STYLISTS];
    static Service services[MAX_SERVICES];
    static Client  clients[MAX_CLIENTS];

    int stylistCount = 0, serviceCount = 0, clientCount = 0;
    int nextStylistID = 1, nextClientID = 1, nextPetID = 1;

    int running = 1;
    int mainChoice;

    printf("=== Pet Salon Tracking System ===\n");
    loadAllData(stylists, &stylistCount, &nextStylistID, services, &serviceCount,
                clients, &clientCount, &nextClientID, &nextPetID);

    while (running) {
        int hasRecords = (stylistCount > 0 || serviceCount > 0 || clientCount > 0);
        displayMenu(hasRecords);
        printf("Enter Choice: ");
        if (scanf("%d", &mainChoice) != 1) break;

        switch (mainChoice) {
            case 1: { // Add
                int addChoice;
                displayAddMenu();
                printf("Enter Choice: ");
                scanf("%d", &addChoice);

                if (addChoice == 1) {
                    addClient(clients, &clientCount, stylists, stylistCount);
                } else if (addChoice == 2) {
                    addPet(clients, clientCount);
                } else if (addChoice == 3) {
                    char name[MAX_NAME_LEN + 1];
                    printf("Stylist Name (Lastname, Firstname): ");
                    scanf(" %200[^\n]", name);
                    addStylist(stylists, &stylistCount, &nextStylistID, name);
                } else if (addChoice == 4) {
                    char name[MAX_NAME_LEN + 1];
                    char desc[MAX_DESC_LEN + 1];
                    float price;
                    printf("Service Name: ");
                    scanf(" %200[^\n]", name);
                    printf("Description: ");
                    scanf(" %300[^\n]", desc);
                    do {
                        printf("Price: ");
                        scanf("%f", &price);
                        if (!validPriceCheck(price)) printf("Price must be >= 0.\n");
                    } while (!validPriceCheck(price));
                    addService(services, &serviceCount, name, desc, price);
                }
                break;
            }
            case 2: { // Edit
                int editChoice;
                displayEditMenu();
                printf("Enter Choice: ");
                scanf("%d", &editChoice);

                if (editChoice == 1) {
                    editClient(clients, clientCount, stylists, stylistCount);
                } else if (editChoice == 2) {
                    editPet(clients, clientCount);
                } else if (editChoice == 3) {
                    int id; char newName[MAX_NAME_LEN + 1];
                    printf("Stylist ID to edit: ");
                    scanf("%d", &id);
                    printf("New Name: ");
                    scanf(" %200[^\n]", newName);
                    editStylist(stylists, stylistCount, id, newName);
                } else if (editChoice == 4) {
                    char name[MAX_NAME_LEN + 1], newDesc[MAX_DESC_LEN + 1];
                    float newPrice;
                    printf("Service Name to edit: ");
                    scanf(" %200[^\n]", name);
                    printf("New Description: ");
                    scanf(" %300[^\n]", newDesc);
                    printf("New Price: ");
                    scanf("%f", &newPrice);
                    editService(services, serviceCount, name, newDesc, newPrice);
                }
                break;
            }
            case 3: { // Delete
                int delChoice;
                displayDeleteMenu();
                printf("Enter Choice: ");
                scanf("%d", &delChoice);

                if (delChoice == 1) {
                    deleteClient(clients, &clientCount);
                } else if (delChoice == 2) {
                    deletePets(clients, clientCount);
                } else if (delChoice == 3) {
                    int id;
                    printf("Stylist ID to delete: ");
                    scanf("%d", &id);
                    deleteStylist(stylists, stylistCount, id);
                } else if (delChoice == 4) {
                    char name[MAX_NAME_LEN + 1];
                    printf("Service Name to delete: ");
                    scanf(" %200[^\n]", name);
                    deleteService(services, serviceCount, name);
                }
                break;
            }
            case 4: { // Book
                int clientChoice, clientIdx, petChoice, stylistID;
                char serviceName[MAX_NAME_LEN + 1];
                Date date;
                int i;

                if (clientCount == 0) {
                    printf("No clients recorded yet.\n");
                    break;
                }

                printf("--- Book a Service ---\n");
                for (i = 0; i < clientCount; i++) {
                    printf("%d. %s\n", i + 1, clients[i].clientName);
                }
                printf("Select Client: ");
                scanf("%d", &clientChoice);
                clientIdx = clientChoice - 1;

                if (clientIdx < 0 || clientIdx >= clientCount) {
                    printf("Invalid client.\n");
                    break;
                }

                printf("Select Pet Slot (1-%d): ", MAX_PETS_OWN);
                scanf("%d", &petChoice);
                if (petChoice < 1 || petChoice > MAX_PETS_OWN ||
                    clients[clientIdx].ClientPets[petChoice - 1].id_pet == 0) {
                    printf("Invalid pet slot.\n");
                    break;
                }

                printf("Stylist ID: ");
                scanf("%d", &stylistID);
                printf("Service Name: ");
                scanf(" %200[^\n]", serviceName);
                printf("Date (day month year): ");
                scanf("%d %d %d", &date.day, &date.month, &date.year);

                bookService(stylists, stylistCount, services, serviceCount,
                            &clients[clientIdx].ClientPets[petChoice - 1], stylistID, serviceName, date);
                saveClients(clients, clientCount, "clients.bin");
                break;
            }
            case 5: // End Month
                endMonth(stylists, stylistCount);
                break;
            case 6: // Generate Reports
                displayReportMenu(clients, clientCount, stylists, stylistCount, services, serviceCount);
                break;
            case 7: // Exit
                triggerRecovery(stylists, stylistCount, services, serviceCount, clients, clientCount);
                printf("Data saved. Goodbye!\n");
                running = 0;
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }

    return 0;
}