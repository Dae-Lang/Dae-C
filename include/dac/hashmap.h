#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>

#include "dac/string.h"
#include "dac/vector.h"

typedef struct {
  Vector* itens;
  size_t itemSize;
  size_t capacity;
} HashMap;

typedef struct {
  String key;
  void* value;
  void* next;
} HashItem;

HashMap* HashMap_New(size_t);

unsigned int HashMap_Hash(HashMap*, String);

void* HashMap_Get(HashMap*, String);

void HashMap_Put(HashMap*, String, void*);

#endif