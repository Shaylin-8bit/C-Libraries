#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hash_map.h"

hash_map gen_map(int sze) {
  hash_map result = {sze>0 ? sze : 1, malloc(sze * sizeof(hash_node))};
  if (result.map == NULL) {
    fprintf(stderr, "Failed to allocate %lu bytes at %p\n", sze * sizeof(hash_node), result.map);
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < sze; i++) result.map[i].trk = 0;
  return result;
}

int hash_key(char* key, int sze) {
  unsigned long result = 0;
  for (int i = 0; i < strlen(key); i++) result += key[i];
  return result % sze;
}

float full(hash_map* map) {
  int full = 0;
  for (int i = 0; i < map->sze; i++) {
    full += map->map[i].trk;
  }
  return full/map->sze;
}

char ins_node(char* key, int val, hash_map* map) {
  if (full(map) > 0.75) {
    resize_map(map, 2);
  }
  int sze = map->sze;
  int index = hash_key(key, sze);
  int i = index;
  while (map->map[i].trk) {
    if (!strcmp(map->map[i].key, key)) break;
    i++;
    if (i == sze) i %= sze;
    if (i == index) return 1;
  }
  hash_node* node = &map->map[i];
  node->trk = 1;
  node->val = val;
  node->key = malloc(strlen(key)+1);
  strcpy(node->key, key);
  return 0; 
}

int grab_node(char* key, hash_map* map) {
  int sze = map->sze;
  int index = hash_key(key, sze);
  int i = index;
  while (strcmp(map->map[i].key, key)) {
    i++;
    if (i == sze) i %= sze;
    if (i == index) {
      fprintf(stderr, "Key error: \"%s\"\n", key);
      exit(EXIT_FAILURE);
    }
  }
  return map->map[i].val;
}

int del_node (char* key, hash_map* map) {
  int sze = map->sze;
  int index = hash_key(key, sze);
  int i = index;
  while (strcmp(map->map[i].key, key)) {
    i++;
    if (i == sze) i %= sze;
    if (i == index) return 1;
  }
  hash_node* node = &map->map[i];
  node->trk = 0;
  free(node->key);
  if (full(map) < 0.2) resize_map(map, 0.7);
  return 0;
}

void clean_hash(hash_map* map) {
  for (int i = 0; i < map->sze; i++) {
    hash_node* node = &map->map[i];
    if (node->trk) {
      free(node->key);
      node->trk = 0; 
    }
  }
  free(map->map);
}

void resize_map(hash_map* map, float factor) {
  int sze = map->sze;
  int new_sze = (int)sze*factor;
  hash_map new_map = gen_map(new_sze);
  for (int i = 0; i < sze && i < new_sze; i++) {
    hash_node* node = &map->map[i];
    if (node->trk) {
      ins_node(node->key, node->val, &new_map);
    }
  }
  clean_hash(map);
  *map = new_map;
}
