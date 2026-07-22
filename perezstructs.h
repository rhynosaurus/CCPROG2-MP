/*
    Description  : Struct definitions and constants for the Stylist and Service portions of the Pet Salon tracking system.
    Author/s     : PEREZ, ERIN ALYANNA A.
    Section      : S20F
    Last Modified: JULY 22, 2026
*/

#ifndef STYLIST_SERVICE_H
#define STYLIST_SERVICE_H

// CONSTANTS
#define MAX_NAME_LEN    200     // stylist/service name max chars
#define MAX_DESC_LEN    300     // service description max chars

#define MAX_SERVICES                20  // offered + not-offered combined
#define MAX_STYLISTS                100
#define MAX_RENDERED_THIS_MONTH     50  // per stylist, capped

#define JUNIOR_SALARY               18000.0f
#define SENIOR_SALARY               28000.0f
#define MASTER_SALARY               38000.0f
#define MASTER_BONUS_PER_SERVICE    100.0f

#define JUNIOR_TO_SENIOR_THRESHOLD  10
#define SENIOR_TO_MASTER_THRESHOLD  20

/* =================================================================
    ENUMS:
    Represents the three possible stylist ranks, lowest to highest.
    ================================================================= */
typedef enum {
    RANK_JUNIOR = 0,
    RANK_SENIOR = 1,
    RANK_MASTER = 2
} StylistRank;

/*  =================================================================
    DATE:
    Represents a calendar date. Day/month/year are stored as integers,
    month is spelled out only at display time (not as a string).
    ================================================================= */
typedef struct {
    int day;
    int month;
    int year;
} Date;

/*  =================================================================
    SERVICE
    The spec gives services no id field.
    Represents a single pet grooming service offered by the salon.
    ================================================================= */
typedef struct {
    char  name[MAX_NAME_LEN + 1];           // service name
    char  description[MAX_DESC_LEN + 1];    // short description
    float price;                            // price in PHP, 2 decimal points
    int   isOffered;                        // 0 = false, 1 = true
    int   isUsed;                           // slot occupancy flag
} Service;

/* ============================================================
   SERVICE RECORD
   The spec gives services id field.
   Represents one instance of a service being availed/rendered,
   pairing a pointer to the service with the date it happened.
   ============================================================ */
typedef struct {
    Service *service;   // pointer into the Service[] array
    Date     date;      // date rendered
} ServiceRecord;

/* ============================================================
   STYLIST
   Represents a stylist employed by the salon. Stylists are never truly deleted,
   deleting one just sets isActive to false.
   ============================================================ */
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

#endif