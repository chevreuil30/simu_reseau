#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct station
{
  uint8_t mac[6];
  uint32_t ip; 
}station;


typedef struct entre_table
{
  uint8_t mac[6];
  int port;
}entre_table;

typedef struct lien
{
    uint8_t mac1[6];
    uint8_t mac2[6];
    int cout;
}lien;


typedef struct switche
{
  uint8_t mac[6];
  int nb_ports;
  int prio;
  entre_table *table;
  int nb_entre;
  lien *liens;
  int nbliens;
    
}switche;

typedef struct trame {
    uint8_t préambule[7];
    uint8_t sfd;
    uint8_t dst[6];
    uint8_t src[6];
    uint16_t type;
    uint8_t* data;
    uint16_t data_size;
    uint8_t fcs[4];
} trame;




void affiche_mac(uint8_t *mac);

void affiche_ip(uint32_t ip);


void afficher_table_commutation(switche *s);

void affiche_switch(switche *s);

void affiche_station(station *s);

void affiche_trame(trame *t);

bool compare_mac(uint8_t *mac1,uint8_t *mac2);
