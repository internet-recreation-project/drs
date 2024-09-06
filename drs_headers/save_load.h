#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "drs.h"
#include <stdio.h>

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

#endif