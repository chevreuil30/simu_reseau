#include <structure.h>
#include <stdio.h>

void affiche_mac(uint8_t *mac)
{
    printf("adresse mac : %02x:%02x:%02x:%02x:%02x:%02x",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
}

void affiche_ip(uint32_t ip)
{
    printf("%d.%d.%d.%d", (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
}

void afficher_table_commutation(switche *s)
{
    for(int i = 0; i<s->nb_entre;i++)
    {
        affiche_mac(s->table[i].mac);
        printf("\nsur le port %d \n",s->table[i].port);
    }
}


void affiche_switch(switche *s)
{
    affiche_mac(s->mac);
    printf(" \n nombre de ports : %d \n priorité : %d \n table de commutation : \n " ,s->nb_ports,s->prio);
    afficher_table_commutation(s);
    printf("\n nombre d'entrée dans la table : %d \n",s->nb_entre);
}

void affiche_station(station *s)
{
    printf("adresse mac : ");
    affiche_mac(s->mac);
    printf("\n adresse ip : ");
    affiche_ip(s->ip);
}


void affiche_trame(trame *t)
{
    printf("SFD : %d\n adresse destination : ",t->sfd); 
    affiche_mac(t->dst);
    printf("\n adresse source : ");
    affiche_mac(t->src);
    printf("\n type : %02x%02x\n donnée (donnée en brut rien ne sert de les afficher \n FCS : juste du technique ca ne sert a rien de l'afficher" ,t->type >>  8 & 0xFF,t->type & 0xFF);
}


bool compare_mac(uint8_t *mac1,uint8_t *mac2)
{
    return(memcmp(mac1,mac2,6) == 0);
}



