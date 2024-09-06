#ifndef TLD_H
#define TLD_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "drs.h"

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

void remove_tld(const char *tld) {
    pthread_mutex_lock(&tld_mutex);

    for (int i = 0; i < tld_count; i++) {
        if (strcmp(tlds[i].tld, tld) == 0) {
            // Remove the TLD by shifting all subsequent elements
            for (int j = i; j < tld_count - 1; j++) {
                tlds[j] = tlds[j + 1];
            }
            tld_count--;
            save_tlds();
            printf("TLD '%s' removed.\n", tld);
            pthread_mutex_unlock(&tld_mutex);
            return;
        }
    }
    printf("TLD '%s' not found.\n", tld);

    pthread_mutex_unlock(&tld_mutex);
}

#endif