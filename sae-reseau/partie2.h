#pragma once

#include <structure.h>

#include <stdlib.h>
#include <stdbool.h>

void creer_reseau(switche *switches, station *stations, int *nb_switches, int *nb_stations,char *nomf);

void commuter(switche *switches , int nb_switches,switche *s,trame *t,int port_src);
