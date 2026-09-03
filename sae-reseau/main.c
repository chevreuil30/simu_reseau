#include <partie2.h>
#include <string.h>
#include <stdio.h>

int main() {
    switche switches[100];
    station stations[100];
    int nb_switches = 0;
    int nb_stations = 0;
    
    creer_reseau(switches, stations, &nb_switches, &nb_stations,"bin/config1.txt");
    
    trame t;
    memcpy(t.src, switches[0].mac, 6);
    memcpy(t.dst, stations[0].mac, 6);
    t.type = 0x0800;
    t.data = NULL;
    t.data_size = 0;
    
    commuter(switches, nb_switches, &switches[0], &t, 0);
    
    
    affiche_reseau(switches,nb_switches,stations,nb_stations);  
    
    return 0;
}
