#include "structs.h"

/*====================================
             ADD CLIENT
======================================*/

/*-----------------------------------
City/Municipality selection
This function takes in an input for the Client's city or municipality by choosing from a list of array of cities

@ param ClientTag[] : Array of the Client struct
return : none
@ pre : clientCity is already initialized as input from the main menu
-------------------------------------*/
void chooseCity(char *clientCity)
{
    char *City[]={"Manila","Mandaluyong","Marikina","Pasig","Quezon City",                  //Array of cities to choose from
                        "San Juan", "Caloocan", "Malabon", "Navotas", "Valenzuela",
                        "Las Piñas", "Makati", "Muntinlupa", "Parañaque", "Pasay",
                        "Pateros", "Taguig"};
    int i, j,                                                                               //loop counters
        index,                                                                              //number of the city to choose from
        id_city;                                                                            //id of the city chosen from the array
    
    printf("Choose City/Municipality:\n");
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            index = j*4+i;
            printf("%2d. %-14s", index+1, City[index]);
        }
        printf("\n");
    }
    printf(" 17. Taguig\n");

    printf("Enter Choice: ");
    scanf("%d", &id_city);

    strcpy(clientCity, City[id_city-1]);

}

/*-----------------------------------
Client Recommender
This function takes in an input for the Client's recommender by choosing from the existing array of clients in the Client struct

@ param ClientTag[] : Array of the Client struct
@ param numClients  : total number of clients currently stored inside the array of clients
return : The id number of the client
@ pre : numClient is a non-negative integer and initialized; Client array is initialized ; currentID is a non-negative integer
-------------------------------------*/
Client *selectRecommender(Client ClientTag[], int numClients, int currentID)
{    
    int i,                              //loop counter
        choice;                         //id of chosen client recommender 
    Client *id_recommender = NULL;      //initiate id_recommender to NULL as default indicating no recommender chosen yet

    printf("Recommender: \n");
    if(numClients==0){                  //no clients input yet
        printf("No clients yet\n");
        return NULL;
    }
    else{                               //show client list
        for(i=0;i<numClients;i++){
            if(i!=currentID)
                printf("%d. %s\n", i+1,ClientTag[i].clientName);
        }
        printf("%d. None",numClients+1);
    }
    printf("Enter choice: ");
    scanf("%d", &choice);

    if(choice<=numClients && (choice-1) != currentID){     //Has a recommender
        id_recommender=&ClientTag[choice-1];
    }
    return id_recommender;
}

/*---------------------------------------------------------------
Select Stylist By Accumulated Points
Point-accumulation recursion starting at a new client's recommender, then pick whichever stylist ended up with the most points

@ param Stylist[]   : Array of stylist
@ param numStylist  : Total number of stylist in record
return              : pointer to recommended stylist
@ pre               : all array and variables are initialized and existing records
------------------------------------------------------------------*/
Stylist* stylistSelectByPoints(Stylist Stylist[], int *numStylist)
{
    int points[MAX_STYLISTS]={};                //
    int i, best_id;

    //function to accumulateStylistPoints

    best_id=0;
    for(i=1;i<*numStylist;i++){
        if(points[i]>points[best_id])
            best_id=i;
    }

    return &Stylist[best_id];
}

/*-----------------------------------
Choose Stylist
This function displays a list of active stylist from where a 

@ param ClientTag[] : Array of the Client struct
return : The character string of City chosen
@ pre  : all array and variables are initialized and existing records
-------------------------------------*/
Stylist* chooseStylist(Stylist StylistTag[], int numStylist)
{
    int i,                      //loop counter
        id_stylist,             //number of chosen stylist within the active list
        count = 0;              //count of active stylists
    int displayIndex[MAX_STYLISTS];     //actual index of active stylist chosen

    printf("Stylist:\n");

    for(i=0;i<numStylist;i++){
        if(StylistTag[i].isActive==1){
            printf("%d. %s\n", count+=1, StylistTag[i].name);
            printf("\n");
            displayIndex[count] = i;
            count+=1;
        }
    }

    printf("Enter Choice: ");
    scanf("%d", &id_stylist);

    return &StylistTag[displayIndex[id_stylist-1]];
}

/*-----------------------------------
Client ID Generator
This function will generate the ID number for every new client added into the database. The ID generated will
be assigned in sequential order.

@ param ClientTag[] : Array of the Client struct
@ param numClient   : Total number of existing clients recorded
return : integer of ID number to be assigned
@ pre : all variables and array are initialized and existing already
-------------------------------------*/
int newClientID(Client Client[], int numClient)
{
    int i,
        id=0;

    if(numClient>=MAX_CLIENTS){            //current clients is at its max record
        return -1;
    }    

    if(numClient==0){                       //first ID/Client within the record
        return 1;
    }

    for(i=0;i<numClient;i++){              
        if(Client[i].id_client > id){
            id=Client[i].id_client;
        }
    }

    return id+1;
}
/*-----------------------------------
Full implementation of Add Client option
This function is the full implementation of the Add Client option. This function adds new client data
within the database of Clients including their pets to be added aside from the Add Pet function

@ param ClientTag[] : Array of the Client struct
@ param numClients  : total number of clients currently stored inside the array of clients
@ param Stylist[]   : Array of Stylists
@ param numStylist  : total number of stylists currently stored inside the array of clients
@ param Pet[]       : Array of Pets
@ param name      : String that contains the name of the client in <Lastname>, <Firstname> format

return : none
@ pre : all variables and array are initialized and existing already
-------------------------------------*/
void addClient(Client Client[], int *numClients, Stylist Stylist[], int numStylist, const char* name)
{
    int numPetsToAssign=0,              //number of pets to assign for given client
        current_id=*numClients,         //the current INDEX of the client being added within the array/struct
        id_client;                      //generated id of the client
    int i;

    if(*numClients>=MAX_CLIENTS){           //client list is at MAX
        printf("Client list is full.\n");
    }else{          
        printf("Add Client: \n");           //adding clients
        
        //ID Generator
        id_client = newClientID(Client,*numClients);
        Client[current_id].id_client=id_client;

        //Name
        printf("Name: ");
        sprintf(Client[current_id].clientName,MAX_NAME_LEN+1,"%s",name);

        //Choose City/Municipality
        chooseCity(Client[current_id].clientCity);

        //Recommender
        Client[current_id].clientRecommender = selectRecommender(Client,*numClients, current_id);

        //Stylist selection
        printf("Stylist:\n");
        /*
        if(Client[current_id].clientRecommender!=NULL){
            Client[current_id].chosenStylist=stylistSelectByPoints(Stylist,numStylist);

            printf("%s\n", Client[current_id].chosenStylist->name);*/
        
            printf("No recommender, pick from list below:\n");
            Client[current_id].chosenStylist=chooseStylist(Stylist,numStylist);
        

        //Pets of Client
        printf("Pets of Client\n");
        printf("Number of Pets: ");
        scanf("%d", &numPetsToAssign);
            for(i=0;i<numPetsToAssign;i++)
                addPetClient(Client,current_id,current_id+1);
    }
    (*numClients)++;
}

/*====================================
             EDIT CLIENT
======================================*/

/*
Find Client ID
This function finds takes in an ID number and goes through the list of current active Client IDs to match with the target ID 

@ param Client[]   : Array of the Client struct
@ param numClient  : Total number of clients as recorded
@ param targetID   : target ID of the client to be found
@ return : ID/index of the found target client
@ pre : variables and arrays are intialized with valid values
*/
int findClientIndex(Client Clients[], int numClient, int targetID){
    int i;              //loop counter
    int found = -1;     //found flag, initialized to -1 as a default index number, if ID is found, i counter will be stored inside found

    for(i=0;i<numClient;i++){
        if(targetID==Clients[i].id_client){
            found = i;
        }
        i+=1;
    }
    return found;
}

/*------------------------------------------------
EDIT CLIENT SYSTEM
This function edits an already existing client from the list

@ param Client[]   : Array of the Client struct
@ param numClient  : Total number of clients as recorded
@ param Stylist[]  : Array of stylist struct
@ para numStylist  : Total number of already existing stylists
@ param name       : String that contains the name of the pet in <Lastname>, <Firstname> format
return : none
@ pre : all variables and array are initialized and existing already
------------------------------------------------*/
void editClient(Client Client[], int numClients, Stylist Stylist[], int numStylist, const char *name)
{
    int i,                      //loop variable
        id_client,              //id of client to edit
        id_choice;

    printf("Select Client to Edit:\n");
    for(i=0;i<numClients;i++){
        printf("%d. %s\n", i+1,Client[i].clientName);
    }
    printf("Enter Choice: ");
    scanf("%d", &id_choice);

    id_client = findClientIndex;

    printf("Current data: \n");
    printf("Name: %s\n",Client[id_client].clientName);
    printf("City/Municipality: %s\n", Client[id_client].clientCity);
    printf("Recommender: %s\n", Client[id_client].clientRecommender->clientName);
    printf("Stylist: \n");
    chooseStylist(Stylist,numStylist);

    //ENTERING NEW DATA
    printf("\nNew Data:\n");

    //Name
    printf("Name: ");
    sprintf(Client[id_client-1].clientName,MAX_NAME_LEN+1,"%s",name);

    //Choose City/Municipality
    chooseCity(Client[id_client-1].clientCity);

    //Recommender
    Client[id_client-1].clientRecommender=selectRecommender(Client,numClients,id_client-1);

    //Stylist
    Client[id_client-1].chosenStylist=chooseStylist(Stylist,numStylist);
    
}

/*====================================
             DELETE CLIENT
======================================*/

/*------------------------------------------------
DELETE CLIENT SYSTEM
This function deletes a chosen client and their respective pets within the system

@ param Client[]   : Array of the Client struct
@ param *numClient : Total number of clients as recorded
return : none
@ pre : numClient is a non-negative integer and initialized; Client array is initialized
------------------------------------------------*/
void deleteClient(Client Client[], int *numClient)
{
    int i,                      //loop variable
        id_delete;              //id# of client to be deleted
    char confirm;               //confirmation of choice Y = yes N = no

    printf("Delete Client:\n\n");

    printf("Select client to delete:\n");
    for(i=0;i<*numClient;i++){
        printf("%d. %s\n", i+1, Client[i].clientName);
    }
    printf("Enter Choice: ");
    scanf("%d", &id_delete);

    if(id_delete>0 && id_delete<=*numClient){
        printf("Confirmation: Are you sure you want to delete %s? <Y/N>: ", Client[id_delete-1].clientName);
        scanf(" %c", &confirm);
        
        if(confirm=='Y'||confirm=='y'){

            printf("Client %s has been removed from system.\n",Client[id_delete-1].clientName);

            for(i=0;i<Client[id_delete-1].numPets;i++){
                printf("Pet %s has been removed from system.\n", Client[id_delete-1].ClientPets[i].petName);
            }

            i=id_delete-1;
            while(i<*numClient-1){
                Client[i] = Client[i+1];
                i+=1;
            }

            *numClient-=1;
        } else printf("Deletion cancelled.\n");
    }

}


/*=======================================
    SERVICES AVAILED BY A CLIENT
=========================================*/

/*------------------------------------------------
MONTH CONVERTER
This function converts the month inside the struct of date from int to its string literal afterwards printing its month in string

@ param month : gets the value of month stored inside the date struct to convert it into its string month
return : none
@ pre : month is already existing within the date sturct it will point to
------------------------------------------------*/
void int_to_date(int month){

    char *monthNames[] = {"January","February","March","April","May","June","July","August","September","October","November","December"};       //array of months

    printf("%s", monthNames[month-1]);
}

void servicesAvailedbyClient(Client CLIENTS[], int numClients){
    int i,j;            //loop variables
    int id_client;      //id of client chosen to be displayed
    float total=0;      //total price of services availed 

    for(i=0;i<numClients;i++){
        printf("%d. %s\n", i+1, CLIENTS[i].clientName);
    }
    printf("\nEnter choice: ");
    scanf("%d", &id_client);

    printf("Services Availed by a Client\n\n");
    printf("Client Info:\n");
    printf("    Name : %s\n", CLIENTS[id_client-1].clientName);
    printf("    ID   : %d\n", id_client);
    printf("    PETS :\n");

    for(i=0;i<CLIENTS[id_client-1].numPets;i++){
    printf("        Name : %s\n", CLIENTS[id_client-1].ClientPets[i].petName);
    printf("        ID   : %d\n", CLIENTS[id_client-1].ClientPets[i].id_pet);
    printf("        Age  : %d years, %d months\n", CLIENTS[id_client-1].ClientPets[i].PetAge.years, CLIENTS[id_client-1].ClientPets[i].PetAge.months);
    printf("        Services Availed:\n");
    
    for(j=0;j<CLIENTS[i].ClientPets[i].numServicesAvailed;j++){
    printf("            + %s (", CLIENTS[id_client-1].ClientPets[i].ServicesAvailed[j].service_name);
    int_to_date(CLIENTS[id_client-1].ClientPets[i].ServicesAvailed[j].DateAvailed.month);
    printf(" %d, %d - ", CLIENTS[id_client-1].ClientPets[i].ServicesAvailed[j].DateAvailed.day, CLIENTS[id_client-1].ClientPets[i].ServicesAvailed[j].DateAvailed.year); 

        //total += 

    }
    }

    printf("\n Total price of services availed: PHP %.2f\n", total);
    printf("\n End of Report.\n");
}