#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "drs_headers/domains.h"
#include "drs_headers/tlds.h"
#include "drs_headers/whois.h"
#include "drs_headers/registrars.h"
#include "drs_headers/mutex.h"
#include "drs_headers/save_load.h"
#include "drs_headers/drs.h"

// Define global variables
TLD tlds[MAX_TLD_LEN];
int tld_count = 0;

Registrar registrars[MAX_NAME_LEN];
int registrar_count = 0;

Domain domains[MAX_DOMAIN_LEN];
int domain_count = 0;
pthread_mutex_t tld_mutex;
pthread_mutex_t registrar_mutex;
pthread_mutex_t domain_mutex;

// Example usage of IRP's DRS (Internet Recreation Project's Domain Registration System)
int main() {
    // Initialize mutexes
    init_mutexes();

    // Load data
    load_data();

    // ADD
    add_tld("com", "GlobalRegistry");
    add_registrar("RegistrarCo");
    register_domain("example", "Alice", "RegistrarCo", "com", 2);
    register_domain("example2", "Alice", "RegistrarCo", "com", 2);

    // LOOKUP
    whois("example", "com");

    // TRANSFER
    transfer_domain("example", "com", "Bob");

    // RENEW
    renew_domain("example", "com", 3);

    // LOOKUP
    whois("example", "com");

    // LIST
    list_domains();
    list_tlds();
    list_registrars();

    // SEARCH
    search_whois("example", "com");

    // REMOVE
    remove_domain("example", "com");
    remove_domain("example2", "com");
    remove_tld("com");
    remove_registrar("RegistrarCo");

    // LIST
    list_domains();
    list_tlds();
    list_registrars();

    // Save data and clean up
    save_data();
    destroy_mutexes();

    return 0;
}
