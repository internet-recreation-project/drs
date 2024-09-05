#ifndef DRS_H
#define DRS_H

#include <time.h>
#include <pthread.h>

#define MAX_TLD_LEN 10
#define MAX_DOMAIN_LEN 255
#define MAX_NAME_LEN 100

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

// Functions for managing TLDs
void add_tld(const char *tld, const char *registry);
void list_tlds();

// Functions for managing registrars
void add_registrar(const char *name);
void list_registrars();

// Functions for managing domains
void register_domain(const char *domain, const char *owner, const char *registrar, const char *tld, int years);
void whois(const char *domain, const char *tld);
void transfer_domain(const char *domain, const char *tld, const char *new_owner);
void renew_domain(const char *domain, const char *tld, int years);

// Functions for file operations
void load_data();
void save_data();

// Function to initialize mutexes
void init_mutexes();
void destroy_mutexes();

#endif
