/*
    Description: Struct definitions for Clients and Pets portion
    Author: MESINA, ANNA GABRIELLA B.
    Section: S20F
    Last Modified: July 22 2026
*/

#define MAX_NAME_LEN 200
#define MAX_SERVICES 10
#define MAX_PETS_OWN 5
#define MAX_CLIENTS 20

/*
PET AGE:
Indicates the age of pets in YY years and MM months
*/
typedef struct{
    int years;
    int months;
}petAge;

/*
DATE:
Indicates the date in DD/MM/YYYY format each representing as integers
*/
typedef struct{
    int day;
    int month;
    int year;
}date;

/*
PAST PET SERVICES AVAILED:
Stores the record of the past 5 services they availed as well as the date it was availed
*/
typedef struct{
    char service_name[MAX_NAME_LEN+1];      //name of service
    date DateAvailed;                       //service date availed
}petServicesAvailed;

/*
PET:
Indicates the information stored of a pet as indicated by their owner
*/
typedef struct{
    int id_pet;                             //sequentially assigned
    char petName[MAX_NAME_LEN+1];           //pet name
    petAge PetAge;                          //age of pet in YY years and MM months format
    petServicesAvailed ServicesAvailed[5];  //Past services availed with a max of 5
}pet;

/*
CLIENT:
Indicates the client information 
*/
typedef struct{
    int id_client;                      //sequentially assigned
    char clientName[MAX_NAME_LEN+1];    //Lastname, Firstname
    int *clientCity;                    //points to an array City[] where client can choose a city depending on integer it represents
    client *clientRecommender;          //points to another client from the struct of clients
    Stylist *chosenStylist;             //points to a stylist from the structure made
    pet ClientPets[5];                  //pets of clients 
}client;

