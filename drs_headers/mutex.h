#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>
#include "drs.h"

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

#endif