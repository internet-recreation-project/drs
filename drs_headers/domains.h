#ifndef DOMAINS_H
#define DOMAINS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "drs.h"

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
            if (years > 1) {
                printf("Domain '%s.%s' renewed for %d more years.\n", domain, tld, years);
            } else {
                printf("Domain '%s.%s' renewed for %d more year.\n", domain, tld, years);
            }
            pthread_mutex_unlock(&domain_mutex);
            return;
        }
    }
    printf("Domain '%s.%s' not found.\n", domain, tld);

    pthread_mutex_unlock(&domain_mutex);
}

void list_domains() {
    pthread_mutex_lock(&domain_mutex);

    for (int i = 0; i < domain_count; i++) {
        printf("Domain: %s, Owner: %s, Registrar: %s, Status: %s, Expiration: %s\n", domains[i].domain, domains[i].owner, domains[i].registrar, domains[i].status, ctime(&domains[i].expiration));
    }

    pthread_mutex_unlock(&domain_mutex);
}

void remove_domain(const char *domain, const char *tld) {
    pthread_mutex_lock(&domain_mutex);

    for (int i = 0; i < domain_count; i++) {
        if (strcmp(domains[i].domain, domain) == 0) {
            // Remove the domain by shifting all subsequent elements
            for (int j = i; j < domain_count - 1; j++) {
                domains[j] = domains[j + 1];
            }
            domain_count--;
            save_domains();
            printf("Domain '%s.%s' removed.\n", domain, tld);
            pthread_mutex_unlock(&domain_mutex);
            return;
        }
    }
    printf("Domain '%s.%s' not found.\n", domain, tld);

    pthread_mutex_unlock(&domain_mutex);
}

#endif