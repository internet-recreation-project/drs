#ifndef WHOIS_H
#define WHOIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "domains.h"

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

#endif