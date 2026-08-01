#include "structs.h"
/*====================================
              ADD PETS
======================================*/

/*------------------------------------------------
GENERATE NEW PET ID
This function generate Pet IDs everytime new Pets are added into the system

@ param Client[]  : Array of the Client struct
@ param numClient : Total number of clients as recorded
return : interger of ID number to be assigned
@ pre : numClient is a non-negative integer and initialized; Client array is initialized
------------------------------------------------*/
int newPetID(Client Client[], int numClient)
{
    int i, j;                   //loop counter
    int highestID = 0;          //ID of the last pet recorded among clients

    for(i=0;i<numClient;i++) {
        for(j=0;j<Client[i].numPets;j++){
            if(Client[i].ClientPets[j].id_pet > highestID)
                highestID = Client[i].ClientPets[j].id_pet;
        }
    }
    return highestID+1;
}

/*------------------------------------------------
ADD PET
This function adds pets into a client's record

@ param Client[]  : Array of the Client struct
@ param numClient : Total number of clients as recorded
@ param id_client : The ID number of the client in which the pet will be added to
return : none
@ pre : numClient is a non-negative integer and initialized; Client array is initialized; id_client is a non-negative integer
------------------------------------------------*/
void addPetClient(Client Client[], int id_client, int numClient)
{
    int indx = Client[id_client].numPets;       //highest index of client's pet

    Client[id_client].ClientPets[indx].id_pet = newPetID(Client, numClient);

    printf("Name: ");
    scanf(" %[^\n]", Client[id_client].ClientPets[indx].petName);  

    printf("Age: \n");
    printf("    Years: ");
    scanf("%d", &Client[id_client].ClientPets[indx].PetAge.years);

    printf("    Months: ");
    scanf("%d", &Client[id_client].ClientPets[indx].PetAge.months);

    Client[id_client].numPets++;
}

/*------------------------------------------------
ADD PET SYSTEM FULL IMPLEMENTATION
This function is the I/O system of when Add Pet is the chosen option. 
This function will ask user to input the ID# of the client where a new pet will be added

@ param Client[]  : Array of the Client struct
@ param numClient : Total number of clients as recorded
return : none
@ pre : numClient is a non-negative integer and initialized; Client array is initialized
------------------------------------------------*/
void addPet(Client Client[], int numClient)
{

    int i,              //loop counter
        id_client;      //id of client chosen

    printf("Add Pet\n");
    printf("Select client: \n");
    for(i=0;i<numClient;i++){
        printf("%d. %s\n",i+1,Client[i].clientName);
    }
    printf("Enter Choice: ");
    scanf("%d", &id_client);

    addPetClient(Client,id_client-1,numClient);

    printf("Done.\n");

}

/*------------------------------------------------
DISPLAY PETS BY OWNER
This function displays ALL of the Pets sorted by owners as recorded into the system

@ param Client[]  : Array of the Client struct
@ param numClient : Total number of clients as recorded
return : none
@ pre : numClient is a non-negative integer and initialized; Client array is initialized
------------------------------------------------*/
void displayPetsbyOwner(Client Client[], int numClients)
{
    int i,j,            //loop counter
        numPets;        //number of pets of client


    printf("Pets by Owner\n");
    
    //Client display
    for(i=0;i<numClients;i++){
        numPets=Client[i].numPets;

        printf("Name   :  %s\n", Client[i].clientName);
        printf("ID     :  %d\n", Client[i].id_client);
        printf("City   :  %s\n", Client[i].clientCity);
        printf("Pets   :\n");

        //Pets display
        for(j=0;j<numPets;j++){
            printf("    Name   :  %s\n", Client[i].ClientPets[j].petName);
            printf("    ID     :  %d\n", Client[i].ClientPets[j].id_pet);
            printf("    Age   :  %d years, %d months\n", Client[i].ClientPets[j].PetAge.years, Client[i].ClientPets[j].PetAge.months);
        }
        printf("\n");
    }
    printf("End of Report\n");
}

/*====================================
             EDIT PET
======================================*/

/*------------------------------------------------
REASSIGN PET TO ANOTHER CLIENT
This function reassigns a pet from its initial owner to a new owner

@ param Client[]     : Array of the Client struct
@ param id_pet       : ID of the pet to be reassigned
@ param id_newClient : ID of the new client where the pet will be reassigned
@ param id_oldClient : ID of the initial client who owned the pet
return : none
@ pre : All variables and arrays are initialized
------------------------------------------------*/
void reassignPet(Client Clients[], int id_pet, int id_newClient, int id_oldClient){
    Pet movingPet;          //gets the whole struct of pet chosen to be moved
    int i;                  //loop counter

    movingPet = Clients[id_oldClient].ClientPets[id_pet];

    for(i=id_pet;i<Clients[id_oldClient].numPets-1;i++){
        Clients[id_oldClient].ClientPets[i]=Clients[id_oldClient].ClientPets[i+1];
    }

    Clients[id_oldClient].numPets--;

    Clients[id_newClient].ClientPets[Clients[id_newClient].numPets] = movingPet;

    Clients[id_newClient].numPets++;

}

/*------------------------------------------------
EDIT PET SYSTEM
This function asks user to select from a list of pets and asks which pet information will 
be either to be edited/changed or to be reassigned to another client within the system.

@ param Client[]  : Array of the Client struct
@ param numClient : Total number of clients as recorded
return : none
@ pre : numClient is a non-negative integer and initialized; Client array is initialized
------------------------------------------------*/
void editPet(Client Client[], int numClients)
{

    int i, j,             //loop counter
        count,            //indicates the number of pets owned by the client
        id_pet, 
        tempclient_id,    //stores a temporary id of a client when moving
        newclient_id,     //indicates the new client id of where the pet will be reassigned (if so)
        pet_choice;       //pet of choice to be edited
    char cChoice;

    printf("Select pet to edit:\n");
    
    for(i=0;i<numClients;i++){
        for(j=0;j<Client[i].numPets;j++){
            printf("%d. %s\n", count++, Client[i].ClientPets[j].petName);
        }
    }
    printf("\n");
    printf("Enter choice: ");
    scanf("%d", &pet_choice);

    //Find out which client owns the selected pet
    count=1;
    for(i=0;i<numClients;i++){
        for(j=0;j<Client[i].numPets;j++){
            if(count==pet_choice){
                tempclient_id=i;
                id_pet=j;
            }
            count++;
        }
    }

    printf("Would you like to reassign the pet <Y/N>: ");
    scanf(" %c",&cChoice);

    //REASSIGN PET
    if(cChoice=='Y'||cChoice=='y'){

        printf("Select new owner: \n");
        for(i=0;i<numClients;i++){
            printf(" %d. %s\n", i+1,Client[i].clientName);
        }
        printf("Enter Choice: ");
        scanf("%d", &newclient_id);
        newclient_id-=1;

        if(Client[newclient_id].numPets >= MAX_PETS_OWN)
            printf("Current Client's Pets is at MAX\n");

        reassignPet(Client,id_pet,newclient_id,tempclient_id);

        tempclient_id = newclient_id;
        id_pet = Client[newclient_id].numPets-1;

        printf("Current Data:\n");
        for(i=0;i<Client[tempclient_id].numPets;i++){
            printf("    Name   :  %s\n", Client[tempclient_id].ClientPets[i].petName);
            printf("    Age   :  %d years, %d months\n", Client[tempclient_id].ClientPets[i].PetAge.years, Client[tempclient_id].ClientPets[i].PetAge.months);
        }

        printf("New Data:\n");
        printf("    Name   :  %s\n", Client[tempclient_id].ClientPets[id_pet].petName);

        printf("Age: \n");
        printf("    Years: %d\n",Client[tempclient_id].ClientPets[id_pet].PetAge.years);

        printf("    Months: %d\n", Client[tempclient_id].ClientPets[id_pet].PetAge.months);
    }
    //Simply edit the chosen pet information
    else{
        //Current information of the pet chosen to be edited
        printf("Current Data:\n");
        printf("    Name   :  %s\n", Client[tempclient_id].ClientPets[id_pet].petName);
        printf("    Age    :  %d years, %d months\n", Client[tempclient_id].ClientPets[id_pet].PetAge.years, Client[tempclient_id].ClientPets[id_pet].PetAge.months);

        //Input new data on the chosen pet to be edited
        printf("New Data:\n");
        printf("Name: ");
        scanf(" %[^\n]", Client[tempclient_id].ClientPets[id_pet].petName);  

        printf("Age: \n");
        printf("    Years: ");
        scanf("%d", &Client[tempclient_id].ClientPets[id_pet].PetAge.years);

        printf("    Months: ");
        scanf("%d", &Client[tempclient_id].ClientPets[id_pet].PetAge.months);
    }

    printf("Done.\n");
        
}

/*====================================
             DELETE PET
======================================*/

/*------------------------------------------------
DELETE PET SYSTEM
This function deletes a chosen Pet within the system

@ param Client[]  : Array of the Client struct
@ param numClient : Total number of clients as recorded
return : none
@ pre : numClient is a non-negative integer and initialized; Client array is initialized
------------------------------------------------*/
void deletePets(Client Client[], int numClients){
    int i,j,                //loop counter
        found=0,            //pet found flag 0 = not found, 1 = found
        choice,             //choice of pet
        id_client,          //client of id who owns the pet
        id_pet,             //id of pet to be found 
        count;              //temporary counter of pets owned by the client which also indicates the index of each pet by client
    char confirm;           //confirmation of choice Y = yes N = no

    printf("Delete Pet:\n");
    printf("Select pet to delete:\n");

    for(i=0;i<numClients;i++){
        for(j=0;j<Client[i].numPets;j++){
            printf("%d. %s\n", count++,Client[i].ClientPets[j].petName);
        }
    }

    printf("Enter Choice: ");
    scanf("%d", &choice);

    //find pet ownership
    count=1;
    for(i=0;i<numClients && !found;i++){
        for(j=0;j<Client[i].numPets && !found;j++){
            if(count==choice){
                id_client=i;
                id_pet=j;
                found=1;
            }
            count++;
        }
    }

    if(choice > 0 && choice <= count){
        printf("\nConfirmation: Are you sure you want to delete %s? <Y/N>: ", Client[id_client].ClientPets[id_pet].petName);
        scanf(" %c", &confirm);

        if(confirm == 'Y' || confirm == 'y'){
            if(found){
                printf("Pet %s has been removed from the system.\n",Client[id_client].ClientPets[id_pet].petName);

                for (j=id_pet;j<Client[id_client].numPets-1;j++){
                    Client[id_client].ClientPets[j] = Client[id_client].ClientPets[j+1];
                }

                Client[id_client].numPets--;
            }

            printf("Done.\n");
        } else printf("Deletion cancelled.\n");
    }
}