#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

void insertMap(HashMap * map , char * key, void * value) {
    long pos = hash(key, map->capacity);
    
    while (map->buckets[pos] != NULL && map->buckets[pos]->key != NULL) {
        if (is_equal(map->buckets[pos]->key, key)) return; 
        pos = (pos + 1) % map->capacity;
        map->current = pos;
    }

    Pair * new = createPair(key, value);
    map->buckets[pos] = new;
    map->size++;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    size_t new_capacity = map->capacity * 2;
    Pair ** new_buckets = NULL;
    new_buckets = (Pair **)malloc(new_capacity * sizeof(Pair *));
    for (size_t i = 0; i < new_capacity; i++) {
        new_buckets[i] = NULL;
    }
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->buckets[i] != NULL) {
            long pos = hash(map->buckets[i]->key, new_capacity);
            while (new_buckets[pos] != NULL) {
                pos = (pos + 1) % new_capacity;
            }
            new_buckets[pos] = map->buckets[i];
        }
    }
    free(map->buckets);
    map->buckets = new_buckets;
    map->capacity = new_capacity;
}

HashMap * createMap(long capacity) {
    HashMap * map = (HashMap *)malloc(sizeof(HashMap));
    map->buckets = (Pair **)malloc(capacity * sizeof(Pair *));
    for (size_t i = 0; i < capacity; i++) {
        map->buckets[i] = NULL;
    }
    map->capacity = capacity;
    map->size = 0;
    map->current = -1;
    return map;
}

void eraseMap(HashMap * map, char * key) {
    size_t pos = hash(key, map->capacity);

    while (map->buckets[pos] != NULL) {
        if (is_equal(map->buckets[pos]->key, key)) {
            map->buckets[pos]->key = NULL;
            map->size--;
            return;
        }
        pos = (pos + 1) % map->capacity; 
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
    size_t pos = hash(key, map->capacity);
    while (map->buckets[pos] != NULL) {
        if (is_equal(map->buckets[pos]->key, key)) {
            map->current = pos;
            return map->buckets[pos];
        }
        pos = (pos + 1) % map->capacity;
    }
    return NULL;
}


Pair * firstMap(HashMap * map) {
    if (map == NULL || map->buckets == NULL) return NULL;
       
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->buckets[i] != NULL) {
            map->current = i;
            return map->buckets[i];  
        }
    }
    map->current = -1; 
    return NULL;
}

Pair * nextMap(HashMap * map) {
    if (map == NULL || map->buckets == NULL || map->current == -1) return NULL;

    for (size_t i = map->current + 1; i < map->capacity; i++) {
        if (map->buckets[i] != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    map->current = -1; 
    return NULL;
}
