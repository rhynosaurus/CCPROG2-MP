/*
    Description  : Struct definitions and constants for the Pet Salon tracking system.
    Author/s     : MESINA, ANNA GABRIELLA B.    PEREZ, ERIN ALYANNA A.
    Section      : S20F
    Last Modified: JULY 24, 2026
*/

#ifndef PET_SALON_STRUCTS_H
#define PET_SALON_STRUCTS_H

/* =================================================================
    CONSTANTS
   ================================================================= */
#define MAX_NAME_LEN                200     // name max chars (shared)
#define MAX_DESC_LEN                300     // service description max chars
#define MAX_SERVICES                20      // offered + not-offered combined
#define MAX_STYLISTS                100
#define MAX_RENDERED_THIS_MONTH     50      // per stylist, capped
#define MAX_PETS_OWN                5
#define MAX_CLIENTS                 20

#define JUNIOR_SALARY               18000.0f
#define SENIOR_SALARY               28000.0f
#define MASTER_SALARY               38000.0f
#define MASTER_BONUS_PER_SERVICE    100.0f
#define JUNIOR_TO_SENIOR_THRESHOLD  10
#define SENIOR_TO_MASTER_THRESHOLD  20

/* =================================================================
    ENUMS
    Represents the three possible stylist ranks, lowest to highest.
   ================================================================= */
typedef enum {
    RANK_JUNIOR = 0,
    RANK_SENIOR = 1,
    RANK_MASTER = 2
} StylistRank;

/* =================================================================
    DATE
    Represents a calendar date. Day/month/year stored as integers.
   ================================================================= */
typedef struct {
    int day;
    int month;
    int year;
} Date;

/* =================================================================
    PET AGE
    Indicates the age of pets in YY years and MM months
   ================================================================= */
typedef struct {
    int years;
    int months;
} petAge;

/* =================================================================
    SERVICE
    Represents a single pet grooming service offered by the salon.
   ================================================================= */
typedef struct {
    char  name[MAX_NAME_LEN + 1];           // service name
    char  description[MAX_DESC_LEN + 1];    // short description
    float price;                            // price in PHP, 2 decimal points
    int   isOffered;                        // 0 = false, 1 = true
    int   isUsed;                           // slot occupancy flag
} Service;

/* =================================================================
    SERVICE RECORD
    One instance of a service being rendered by a stylist,
    pairing a pointer to the service with the date it happened.
   ================================================================= */
typedef struct {
    Service *service;   // pointer into the Service[] array
    Date     date;      // date rendered
} ServiceRecord;

/* =================================================================
    PAST PET SERVICES AVAILED
    Stores the record of past services a pet availed, and the date.
  ================================================================= */
typedef struct {
    char service_name[MAX_NAME_LEN + 1];    // name of service
    Date DateAvailed;                       // service date availed
} petServicesAvailed;

/* =================================================================
    STYLIST
    Represents a stylist employed by the salon. Stylists are never
    truly deleted; deleting one just sets isActive to false.
   ================================================================= */
typedef struct {
    int         stylistID;                  // auto-assigned, sequential
    char        name[MAX_NAME_LEN + 1];     // format: Lastname, Firstname
    int         isActive;                   // 0 = false, 1 = true
    StylistRank rank;
    float       salary;
    // Services rendered THIS MONTH - capped at 50, cleared by End Month
    ServiceRecord renderedServices[MAX_RENDERED_THIS_MONTH];
    int           renderedCount;
} Stylist;

/* =================================================================
    PET
    Indicates the information stored of a pet as indicated by their
    owner.
   ================================================================= */
typedef struct {
    int id_pet;                             // sequentially assigned
    char petName[MAX_NAME_LEN + 1];         // pet name
    petAge PetAge;                          // age of pet, YY years / MM months
    petServicesAvailed ServicesAvailed[5];  // past services availed, max 5
} pet;

/* =================================================================
    CLIENT
    Indicates the client information.
   ================================================================= */
typedef struct client {
    int id_client;                          // sequentially assigned
    char clientName[MAX_NAME_LEN + 1];      // Lastname, Firstname
    int *clientCity;                        // points into City[] array
    struct client *clientRecommender;       // points to another client
    Stylist *chosenStylist;                 // points to a chosen stylist
    pet ClientPets[MAX_PETS_OWN];           // pets of client
} client;

#endif