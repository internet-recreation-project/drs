#ifndef REGISTRARS_H
#define REGISTRARS_H

#include "drs.h"
#include <string.h>
#include <pthread.h>
#include <stdio.h>

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

void remove_registrar(const char *name) {
    pthread_mutex_lock(&registrar_mutex);

    for (int i = 0; i < registrar_count; i++) {
        if (strcmp(registrars[i].name, name) == 0) {
            // Remove the registrar by shifting all subsequent elements
            for (int j = i; j < registrar_count - 1; j++) {
                registrars[j] = registrars[j + 1];
            }
            registrar_count--;
            save_registrars();
            printf("Registrar '%s' removed.\n", name);
            pthread_mutex_unlock(&registrar_mutex);
            return;
        }
    }
    printf("Registrar '%s' not found.\n", name);

    pthread_mutex_unlock(&registrar_mutex);
}

#endif