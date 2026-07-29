#include "structs.h"
#define NULL 0
/*====================================
             ADD CLIENT
======================================*/

/*-----------------------------------
City/Municipality selection
This function takes in an input for the Client's city or municipality by choosing from a list of array of cities

@ param ClientTag[] : Array of the Client struct
return : none
-------------------------------------*/
void chooseCity(char *clientCity)
{
    char *City[]={"Manila","Mandaluyong","Marikina","Pasig","Quezon City",
                        "San Juan", "Caloocan", "Malabon", "Navotas", "Valenzuela",
                        "Las Piñas", "Makati", "Muntinlupa", "Parañaque", "Pasay",
                        "Pateros", "Taguig"};
    int i, j, 
        index,
        id_city;
    
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
-------------------------------------*/
Client *selectRecommender(Client ClientTag[], int numClients, int currentID)
{    
    int i,
        choice=0;
    Client *id_recommender = NULL;

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
------------------------------------------------------------------*/
Stylist* stylistSelectByPoints(Stylist Stylist[], int *numStylist)
{
    int points[MAX_STYLISTS]={};
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
Client Stylist
This function takes in an input for the Client's city or municipality by choosing from a list of array of cities

@ param ClientTag[] : Array of the Client struct
return : The character string of City chosen
-------------------------------------*/
Stylist* chooseStylist(Stylist StylistTag[], int numStylist)
{
    int i,
        id_stylist;

    printf("Stylist:\n");

    for(i=0;i<numStylist;i++){
        if(StylistTag[i].isActive==1)
            printf("%d. %s\n",i+1,StylistTag[i].name);
    }

    printf("Enter Choice: ");
    scanf("%d", &id_stylist);

    return &StylistTag[id_stylist-1];
}

/*-----------------------------------
Client ID Generator
This function will generate the ID number for every new client added into the database. The ID generated will
be assigned in sequential order.

@ param ClientTag[] : Array of the Client struct
@ param numClient   : Total number of existing clients recorded
return : integer of ID number to be assigned
-------------------------------------*/
int newClientID(Client Client[], int numClient)
{
    int i,
        id=0;

    if(numClient>=MAX_CLIENTS){
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
return : none
-------------------------------------*/
void addClient(Client Client[], int *numClients, Stylist Stylist[], int numStylist)
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
        scanf(" %[^\n]",Client[current_id].clientName);

        //Choose City/Municipality
        chooseCity(Client[current_id].clientCity);

        //Recommender
        Client[current_id].clientRecommender = selectRecommender(Client,*numClients, current_id);

        //Stylist selection
        printf("Stylist:\n");
        if(Client[current_id].clientRecommender!=NULL){
            Client[current_id].chosenStylist=stylistSelectByPoints(Client[current_id].clientRecommender,Stylist,numStylist);

            printf("%s\n", Client[current_id].chosenStylist->name);
        }else{
            printf("No recommender, pick from list below:\n");
            Client[current_id].chosenStylist=chooseStylist(Stylist,numStylist);
        }

        //Pets of Client
        printf("Pets of Client\n");
        printf("Number of Pets: ");
        scanf("%d", &numPetsToAssign);
            for(i=0;i<numPetsToAssign;i++)
                addPetClient(Client,current_id,current_id+1);
    }
    (*numClients)+=1;
}

/*====================================
             EDIT CLIENT
======================================*/

void editClient(Client Client[], int numClients, Stylist Stylist[], int numStylist)
{
    int i,
        id_client;

    printf("Select Client to Edit:\n");
    for(i=0;i<numClients;i++){
        printf("%d. %s\n", i+1,Client[i].clientName);
    }
    printf("Enter Choice: ");
    scanf("%d", &id_client);

    printf("Current data: \n");
    printf("Name: %s\n",Client[id_client-1].clientName);
    printf("City/Municipality: %s\n", Client[id_client-1].clientCity);
    printf("Recommender: %s\n", Client[id_client-1].clientRecommender->clientName);
    printf("Stylist: %s\n", Client[id_client-1].chosenStylist->name);
    printf("\n");

    //ENTERING NEW DATA
    printf("New Data:\n");

    //Name
    printf("Name: ");
    scanf(" %[^\n]",Client[id_client-1].clientName);

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
------------------------------------------------*/
void deleteClient(Client Client[], int *numClient)
{
    int i,
        id_delete;
    char confirm;

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
        }
    }

}
