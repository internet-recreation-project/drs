#ifndef DRS_H
#define DRS_H

#include <time.h>
#include <pthread.h>

#define MAX_TLD_LEN 10
#define MAX_DOMAIN_LEN 255
#define MAX_NAME_LEN 100

// File names for persistence
#define TLD_FILE "tlds.dat"
#define REGISTRAR_FILE "registrars.dat"
#define DOMAIN_FILE "domains.dat"

// Struct definitions
typedef struct {
    char tld[MAX_TLD_LEN];
    char registry[MAX_NAME_LEN];
} TLD;

typedef struct {
    char name[MAX_NAME_LEN];
} Registrar;

typedef struct {
    char domain[MAX_DOMAIN_LEN];
    char owner[MAX_NAME_LEN];
    char registrar[MAX_NAME_LEN];
    char status[MAX_NAME_LEN];
    time_t expiration;
} Domain;

// Data arrays and counters
extern TLD tlds[MAX_TLD_LEN];
extern int tld_count;

extern Registrar registrars[MAX_NAME_LEN];
extern int registrar_count;

extern Domain domains[MAX_DOMAIN_LEN];
extern int domain_count;

// Mutexes
extern pthread_mutex_t tld_mutex;
extern pthread_mutex_t registrar_mutex;
extern pthread_mutex_t domain_mutex;

// Function declarations
void init_mutexes();
void destroy_mutexes();
void save_tlds();
void load_tlds();
void save_registrars();
void load_registrars();
void save_domains();
void load_domains();
void save_data();
void load_data();
void add_tld(const char *tld, const char *registry);
void list_tlds();
void add_registrar(const char *name);
void list_registrars();
void register_domain(const char *domain, const char *owner, const char *registrar, const char *tld, int years);
void whois(const char *domain, const char *tld);
void transfer_domain(const char *domain, const char *tld, const char *new_owner);
void renew_domain(const char *domain, const char *tld, int years);
void remove_domain(const char *domain, const char *tld);

#endif