#include <partie2.h>
#include <stdio.h>
#include <string.h>

void creer_reseau(switche *switches, station *stations, int *nb_switches, int *nb_stations,char *nomf)
{
    printf("debut creer_reseau\n");
    FILE *f = fopen(nomf, "r"); 
    printf("fopen ok, f=%p\n", f);
    if(f != NULL){
        char ligne[256];
        fgets(ligne,256,f);
        printf("ligne lue: %s", ligne);
        printf("ligne avant strtok: '%s'\n", ligne);
        int nb_machine = atoi(strtok(ligne," "));
        printf("nb_machine: %d\n", nb_machine);
        char *tok = strtok(NULL, " ");
        printf("tok: %s\n", tok);
        int nb_liens = atoi(tok);
        printf("nb_liens: %d\n", nb_liens);
        int indice[nb_machine];
        int indice_local[nb_machine];
        for(int i= 0; i<nb_machine;i++)
        {
            printf("iteration %d\n", i);
            fgets(ligne,256,f);
            printf("ligne machine: %s", ligne);
            printf("type: %d\n", atoi(ligne));
            if(atoi(ligne) == 2)
            {
                switche s;
                s.nbliens = 0;
                s.liens = NULL;
                char *save_ligne;
                char *save_mac;
                strtok_r(ligne, ";", &save_ligne);
                char *p = strtok_r(NULL, ";", &save_ligne);
                int pi = strtol(strtok_r(p, ":", &save_mac), NULL, 16);
                s.mac[0] = pi;
                for(int j = 1;j<6;j++)
                {
                    pi = strtol(strtok_r(NULL, ":", &save_mac), NULL, 16);
                    s.mac[j] = pi;
                }
                s.nb_ports = atoi(strtok_r(NULL, ";", &save_ligne));
                s.prio = atoi(strtok_r(NULL, ";", &save_ligne));
                indice[i] = 2;
                indice_local[i] = *nb_switches;
                switches[*nb_switches] = s;
                (*nb_switches)++;
            }
            else
            {
                station s;
                char *save_ligne;
                char *save_mac;
                strtok_r(ligne, ";", &save_ligne);
                char *p = strtok_r(NULL, ";", &save_ligne);
                int pi = strtol(strtok_r(p, ":", &save_mac), NULL, 16);
                s.mac[0] = pi;
                for(int j = 1;j<6;j++)
                {
                    pi = strtol(strtok_r(NULL, ":", &save_mac), NULL, 16);
                    s.mac[j] = pi;
                }
                char *m = strtok_r(NULL, ";", &save_ligne);
                uint32_t result = atoi(strtok_r(m, ".", &save_mac))<<24 | atoi(strtok_r(NULL, ".", &save_mac))<<16 | atoi(strtok_r(NULL, ".", &save_mac))<<8 | atoi(strtok_r(NULL, ".", &save_mac));
                s.ip = result;
                indice[i] = 1;
                indice_local[i] = *nb_stations;
                stations[*nb_stations] = s;
                (*nb_stations)++;
            }
        }
        for(int i =0; i<nb_liens;i++)
        {
            fgets(ligne,256,f);
            int mach1 = atoi(strtok(ligne,";"));
            int mach2 = atoi(strtok(NULL,";"));
            lien cable;
            cable.cout = atoi(strtok(NULL,";"));
            if(indice[mach1] == 2)
            {
                memcpy(cable.mac1,switches[indice_local[mach1]].mac,6*sizeof(uint8_t));
                if(indice[mach2] == 2)
                {
                    memcpy(cable.mac2,switches[indice_local[mach2]].mac,6*sizeof(uint8_t));
                    switches[indice_local[mach2]].liens = realloc(switches[indice_local[mach2]].liens, (switches[indice_local[mach2]].nbliens +1) * sizeof(lien));
                    switches[indice_local[mach2]].liens[switches[indice_local[mach2]].nbliens] =cable; 
                    switches[indice_local[mach2]].nbliens++;
                    switches[indice_local[mach1]].liens = realloc(switches[indice_local[mach1]].liens, (switches[indice_local[mach1]].nbliens +1) * sizeof(lien));
                    switches[indice_local[mach1]].liens[switches[indice_local[mach1]].nbliens] =cable; 
                    switches[indice_local[mach1]].nbliens++;
                }
                else
                {
                    memcpy(cable.mac2,stations[indice_local[mach2]].mac,6*sizeof(uint8_t));
                    switches[indice_local[mach1]].liens = realloc(switches[indice_local[mach1]].liens, (switches[indice_local[mach1]].nbliens +1) * sizeof(lien));
                    switches[indice_local[mach1]].liens[switches[indice_local[mach1]].nbliens] =cable; 
                    switches[indice_local[mach1]].nbliens++;
                }
                
            }
            else
            {
                memcpy(cable.mac1,stations[indice_local[mach1]].mac,6*sizeof(uint8_t));
                memcpy(cable.mac2,switches[indice_local[mach2]].mac,6*sizeof(uint8_t));
                switches[indice_local[mach2]].liens = realloc(switches[indice_local[mach2]].liens, (switches[indice_local[mach2]].nbliens +1) * sizeof(lien));
                switches[indice_local[mach2]].liens[switches[indice_local[mach2]].nbliens] =cable; 
                switches[indice_local[mach2]].nbliens++;
            }
            
        }
    }
}



void commuter(switche *switches , int nb_switches,switche *s,trame *t,int port_src)
{
  bool estdans = false;
  for(int i =0; i<s->nb_entre;i++)
  {
      if(compare_mac(s->table[i].mac , t->src))
      {
          estdans=true;
      }
  }
  if(!estdans)
  {
    entre_table enter;
    memcpy(enter.mac ,t->src,6*sizeof(uint8_t));
    enter.port = port_src;
    s->table = realloc(s->table,(s->nb_entre+1)*sizeof(entre_table));
    s->table[s->nb_entre] = enter;
    s->nb_entre++;
    
  }
  estdans = false;
  int port_dst = -1;
  for(int i =0; i<s->nb_entre;i++)
  {
      if(compare_mac(s->table[i].mac , t->dst))
      {
          estdans=true;
          port_dst = s->table[i].port;
      }
  }
  if(estdans)
  {
      bool trouve = false;
      for(int i = 0; i < nb_switches && !trouve; i++) {
          if(compare_mac(switches[i].mac, s->liens[port_dst].mac2)) {
          commuter(switches, nb_switches, &switches[i], t, port_dst);
          trouve = true;
     }
  }
}
else {
    for(int i = 0; i < s->nbliens; i++) {
        if(i != port_src) {
            bool trouve = false;
            for(int j = 0; j < nb_switches && !trouve; j++) {
                if(compare_mac(switches[j].mac, s->liens[i].mac2)) {
                    commuter(switches, nb_switches, &switches[j], t, i);
                    trouve = true;
                }
              }
          }
        }
      }
  
}

void affiche_reseau(switche *switches, int nb_switches, station *stations, int nb_stations)
{
    printf("=== SWITCHES ===\n");
    for(int i = 0; i < nb_switches; i++)
    {
        printf("Switch %d : ", i);
        affiche_switch(&switches[i]);
        printf("\n");
    }
    printf("=== STATIONS ===\n");
    for(int i = 0; i < nb_stations; i++)
    {
        printf("Station %d : ", i);
        affiche_station(&stations[i]);
        printf("\n");
    }
}

