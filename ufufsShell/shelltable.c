#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shelltable.h"
//diferente do nosso trabalho, nesta hash o tamanho não será variável
typedef struct item {
  int key;
  void* data; 
} Item;

struct hash {
  int qnt, TABLE_SIZE, ITEM_SIZE;
  Item **itens;
};

int keyFold(int chave, int TABLE_SIZE);
int sondagemLinear(int pos, int i, int TABLE_SIZE);

Hash* criaHash(int TABLE_SIZE, int TAMANHO_TIPO) {
  Hash* ha = (Hash*) malloc(sizeof(Hash));
  if(ha == NULL) return NULL;
  int i;
  ha->TABLE_SIZE = TABLE_SIZE;
  ha->ITEM_SIZE = TAMANHO_TIPO;
  ha->itens = (Item**) malloc(TABLE_SIZE * sizeof(Item*));
  if(ha->itens == NULL) {
    free(ha);
    return NULL;
  }
  ha->qnt = 0;
  for(i = 0; i < TABLE_SIZE; i++) {
    ha->itens[i] = NULL;
  }
  return ha;
}

void liberaHash(Hash* ha) {
  if(ha == NULL) return;
  int i;
  for(i = 0; i < ha->TABLE_SIZE; i++) {
    if(ha->itens[i] != NULL) {
      free(ha->itens[i]->data); 
      free(ha->itens[i]);
    }
  }
  free(ha->itens);
  free(ha);
  return;
}

int insereHash(Hash* ha, int chave, void *dados) {
  if (ha == NULL || ha->qnt == ha->TABLE_SIZE)
    return 0;

  int i, newPos;
  int pos = keyFold(chave, ha->TABLE_SIZE);
  for(i = 0; i < ha->TABLE_SIZE; i++) {
    newPos = sondagemLinear(pos, i, ha->TABLE_SIZE);
    if(ha->itens[newPos] == NULL) {
      Item* item = (Item*) malloc(sizeof(Item));
      if(item == NULL) return 0;
      item->key = chave;
      item->data = (void *) malloc(ha->ITEM_SIZE);
      if(item->data == NULL) {
        free(item);
        return 0;
      }
      memcpy(item->data, dados, ha->ITEM_SIZE);
      ha->itens[newPos] = item;
      ha->qnt++;
      return 1;
    }
  }
  return 0;
}

int buscaHash(Hash* ha, int chave, void *dados) {
  if(ha == NULL) return 0;
  int i, newPos;
  int pos = keyFold(chave, ha->TABLE_SIZE);
  for(i = 0; i < ha->TABLE_SIZE; i++) {
    newPos = sondagemLinear(pos, i, ha->TABLE_SIZE);
    if(ha->itens[newPos] == NULL)
      return 0;
    if(ha->itens[newPos]->key == chave) {
      memcpy(dados, ha->itens[newPos]->data, ha->ITEM_SIZE);
      return 1;
    }
  }
  return 0;
}

int keyFold(int chave, int TABLE_SIZE) {
  int num_bits = 10;
  int p1 = chave >> num_bits;
  int p2 = chave & (TABLE_SIZE - 1);
  return p1 ^ p2;
}

int sondagemLinear(int pos, int i, int TABLE_SIZE) {
  return ((pos + i) & 0x7FFFFFFF) % TABLE_SIZE;
}

