#pragma once
#include <stdint.h>
typedef struct __operators{
    void* (*iterator) (void* slot, void* node); 
    void (*append) (void* slot, void* node);
    void* (*find) (void* slot, void* node); 
    void* (*remove) (void* slot, void* node); 
    uint64_t (*hash) (void* node); 
} UMap_Ops;

typedef struct __umap{
    void* slots;
    uint32_t slots_size;
    uint32_t item_size;
    UMap_Ops* ops;
} UMap;

void UMap_init( UMap* this, void* slots, uint32_t slots_size, uint32_t item_size, UMap_Ops* ops );
void UMap_insert( UMap* this, void* item );
void* UMap_remove( UMap* this, void* tar );
void* UMap_find( UMap* this, void* tar );
