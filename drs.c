#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "drs.h"

// File names for persistence
#define TLD_FILE "tlds.dat"
#define REGISTRAR_FILE "registrars.dat"
#define DOMAIN_FILE "domains.dat"

// Data arrays and counters
static TLD tlds[MAX_TLD_LEN];
static int tld_count = 0;

static Registrar registrars[MAX_NAME_LEN];
static int registrar_count = 0;

static Domain domains[MAX_DOMAIN_LEN];
static int domain_count = 0;

// Mutexes
pthread_mutex_t tld_mutex;
pthread_mutex_t registrar_mutex;
pthread_mutex_t domain_mutex;

void init_mutexes() {
    pthread_mutex_init(&tld_mutex, NULL);
    pthread_mutex_init(&registrar_mutex, NULL);
    pthread_mutex_init(&domain_mutex, NULL);
}

void destroy_mutexes() {
    pthread_mutex_destroy(&tld_mutex);
    pthread_mutex_destroy(&registrar_mutex);
    pthread_mutex_destroy(&domain_mutex);
}

void save_tlds() {
    FILE *file = fopen(TLD_FILE, "wb");
    if (!file) {
        perror("Error opening TLD file");
        return;
    }
    fwrite(&tld_count, sizeof(int), 1, file);
    fwrite(tlds, sizeof(TLD), tld_count, file);
    fclose(file);
}

void load_tlds() {
    FILE *file = fopen(TLD_FILE, "rb");
    if (!file) {
        perror("Error opening TLD file");
        return;
    }
    fread(&tld_count, sizeof(int), 1, file);
    fread(tlds, sizeof(TLD), tld_count, file);
    fclose(file);
}

void save_registrars() {
    FILE *file = fopen(REGISTRAR_FILE, "wb");
    if (!file) {
        perror("Error opening registrar file");
        return;
    }
    fwrite(&registrar_count, sizeof(int), 1, file);
    fwrite(registrars, sizeof(Registrar), registrar_count, file);
    fclose(file);
}

void load_registrars() {
    FILE *file = fopen(REGISTRAR_FILE, "rb");
    if (!file) {
        perror("Error opening registrar file");
        return;
    }
    fread(&registrar_count, sizeof(int), 1, file);
    fread(registrars, sizeof(Registrar), registrar_count, file);
    fclose(file);
}

void save_domains() {
    FILE *file = fopen(DOMAIN_FILE, "wb");
    if (!file) {
        perror("Error opening domain file");
        return;
    }
    fwrite(&domain_count, sizeof(int), 1, file);
    fwrite(domains, sizeof(Domain), domain_count, file);
    fclose(file);
}

void load_domains() {
    FILE *file = fopen(DOMAIN_FILE, "rb");
    if (!file) {
        perror("Error opening domain file");
        return;
    }
    fread(&domain_count, sizeof(int), 1, file);
    fread(domains, sizeof(Domain), domain_count, file);
    fclose(file);
}

void save_data() {
    save_tlds();
    save_registrars();
    save_domains();
}

void load_data() {
    load_tlds();
    load_registrars();
    load_domains();
}

void add_tld(const char *tld, const char *registry) {
    pthread_mutex_lock(&tld_mutex);

    // Check if the TLD already exists
    for (int i = 0; i < tld_count; i++) {
        if (strcmp(tlds[i].tld, tld) == 0) {
            printf("TLD '%s' already exists.\n", tld);
            pthread_mutex_unlock(&tld_mutex);
            return;
        }
    }

    if (tld_count < MAX_TLD_LEN) {
        strncpy(tlds[tld_count].tld, tld, MAX_TLD_LEN - 1);
        strncpy(tlds[tld_count].registry, registry, MAX_NAME_LEN - 1);
        tld_count++;
        save_tlds();
        printf("TLD '%s' added by registry '%s'.\n", tld, registry);
    } else {
        printf("TLD limit reached.\n");
    }

    pthread_mutex_unlock(&tld_mutex);
}

void list_tlds() {
    pthread_mutex_lock(&tld_mutex);

    for (int i = 0; i < tld_count; i++) {
        printf("TLD: %s, Registry: %s\n", tlds[i].tld, tlds[i].registry);
    }

    pthread_mutex_unlock(&tld_mutex);
}

void add_registrar(const char *name) {
    pthread_mutex_lock(&registrar_mutex);

    // Check if the registrar already exists
    for (int i = 0; i < registrar_count; i++) {
        if (strcmp(registrars[i].name, name) == 0) {
            printf("Registrar '%s' already exists.\n", name);
            pthread_mutex_unlock(&registrar_mutex);
            return;
        }
    }

    if (registrar_count < MAX_NAME_LEN) {
        strncpy(registrars[registrar_count].name, name, MAX_NAME_LEN - 1);
        registrar_count++;
        save_registrars();
        printf("Registrar '%s' added.\n", name);
    } else {
        printf("Registrar limit reached.\n");
    }

    pthread_mutex_unlock(&registrar_mutex);
}

void list_registrars() {
    pthread_mutex_lock(&registrar_mutex);

    for (int i = 0; i < registrar_count; i++) {
        printf("Registrar: %s\n", registrars[i].name);
    }

    pthread_mutex_unlock(&registrar_mutex);
}

void register_domain(const char *domain, const char *owner, const char *registrar, const char *tld, int years) {
    pthread_mutex_lock(&domain_mutex);

    // Check if the domain already exists
    for (int i = 0; i < domain_count; i++) {
        if (strcmp(domains[i].domain, domain) == 0) {
            printf("Domain '%s.%s' is already registered.\n", domain, tld);
            pthread_mutex_unlock(&domain_mutex);
            return;
        }
    }

    if (domain_count < MAX_DOMAIN_LEN) {
        for (int i = 0; i < tld_count; i++) {
            if (strcmp(tlds[i].tld, tld) == 0) {
                for (int j = 0; j < registrar_count; j++) {
                    if (strcmp(registrars[j].name, registrar) == 0) {
                        strncpy(domains[domain_count].domain, domain, MAX_DOMAIN_LEN - 1);
                        strncpy(domains[domain_count].owner, owner, MAX_NAME_LEN - 1);
                        strncpy(domains[domain_count].registrar, registrar, MAX_NAME_LEN - 1);
                        strncpy(domains[domain_count].status, "active", MAX_NAME_LEN - 1);
                        domains[domain_count].expiration = time(NULL) + (years * 365 * 24 * 60 * 60);
                        domain_count++;
                        save_domains();
                        printf("Domain '%s.%s' registered successfully to '%s'.\n", domain, tld, owner);
                        pthread_mutex_unlock(&domain_mutex);
                        return;
                    }
                }
                printf("Registrar '%s' not found.\n", registrar);
                pthread_mutex_unlock(&domain_mutex);
                return;
            }
        }
        printf("TLD '%s' not found.\n", tld);
    } else {
        printf("Domain limit reached.\n");
    }

    pthread_mutex_unlock(&domain_mutex);
}

void whois(const char *domain, const char *tld) {
    pthread_mutex_lock(&domain_mutex);

    for (int i = 0; i < domain_count; i++) {
        if (strcmp(domains[i].domain, domain) == 0) {
            printf("WHOIS for %s.%s:\n", domain, tld);
            printf("  Owner: %s\n", domains[i].owner);
            printf("  Status: %s\n", domains[i].status);
            printf("  Expiration: %s", ctime(&domains[i].expiration));
            printf("  Registrar: %s\n", domains[i].registrar);
            pthread_mutex_unlock(&domain_mutex);
            return;
        }
    }
    printf("Domain '%s.%s' not found.\n", domain, tld);

    pthread_mutex_unlock(&domain_mutex);
}

void transfer_domain(const char *domain, const char *tld, const char *new_owner) {
    pthread_mutex_lock(&domain_mutex);

    for (int i = 0; i < domain_count; i++) {
        if (strcmp(domains[i].domain, domain) == 0) {
            strncpy(domains[i].owner, new_owner, MAX_NAME_LEN - 1);
            save_domains();
            printf("Domain '%s.%s' ownership transferred to '%s'.\n", domain, tld, new_owner);
            pthread_mutex_unlock(&domain_mutex);
            return;
        }
    }
    printf("Domain '%s.%s' not found.\n", domain, tld);

    pthread_mutex_unlock(&domain_mutex);
}

void renew_domain(const char *domain, const char *tld, int years) {
    pthread_mutex_lock(&domain_mutex);

    for (int i = 0; i < domain_count; i++) {
        if (strcmp(domains[i].domain, domain) == 0) {
            domains[i].expiration += years * 365 * 24 * 60 * 60;
            save_domains();
            printf("Domain '%s.%s' renewed for %d more year(s).\n", domain, tld, years);
            pthread_mutex_unlock(&domain_mutex);
            return;
        }
    }
    printf("Domain '%s.%s' not found.\n", domain, tld);

    pthread_mutex_unlock(&domain_mutex);
}

void remove_domain(const char *domain, const char *tld) {
    pthread_mutex_lock(&domain_mutex);

    for (int i = 0; i < domain_count; i++) {
        if (strcmp(domains[i].domain, domain) == 0) {
            // Mark the domain as removed
            strncpy(domains[i].status, "removed", MAX_NAME_LEN - 1);
            save_domains();
            printf("Domain '%s.%s' removed.\n", domain, tld);
            pthread_mutex_unlock(&domain_mutex);
            return;
        }
    }
    printf("Domain '%s.%s' not found.\n", domain, tld);

    pthread_mutex_unlock(&domain_mutex);
}


int main() {
    // Initialize mutexes
    init_mutexes();

    // Load data
    load_data();

    // Example usage
    add_tld("com", "GlobalRegistry");
    add_registrar("RegistrarCo");
    register_domain("example", "Alice", "RegistrarCo", "com", 2);
    whois("example", "com");
    transfer_domain("example", "com", "Bob");
    renew_domain("example", "com", 3);
    remove_domain("example", "com");

    // Save data and clean up
    save_data();
    destroy_mutexes();

    return 0;
}
