#pragma once
#include "lnode.h"
#include "umap.h"

/* slots */
#define SLOTS_SIZE  4096

/*target value*/
typedef struct{ 
    uint32_t chinese; 
    uint32_t math; 
    uint32_t english; 
} Subject;

typedef struct{ char* name;  Subject val; } NameVal;

typedef struct __nvmap{
    ListNode listnodes[ SLOTS_SIZE ];
    UMap umap;
    ListNode list; /* recode all nodes in umap; */
    Node* iterator_ptr;
} Nvmap;

void Nvmap_init( Nvmap* this );
uint32_t Nvmap_insert( Nvmap* this, NameVal* tar );
uint32_t Nvmap_remove( Nvmap* this, NameVal* tar );
NameVal* Nvmap_find( Nvmap* this, NameVal* tar );
uint32_t Nvmap_modify( Nvmap* this, NameVal* tar );
NameVal* Nvmap_iterator( Nvmap* this );
void Nvmap_free( Nvmap* this ); 
