#include <stdlib.h>
#include <string.h>
#include "umap.h"
/**
umap_
     |- item_size 
     |- slots_size ; len(slots) = item_size * slots_size;
     |- slots ; slot, a slot obj, not ptr;
     \- ops 
umap will not alloc any space for slot in slots.
**/
void UMap_init(
   UMap* this, void* slots, uint32_t slots_size, uint32_t item_size, UMap_Ops* ops
){
    this->ops = ops;
    this->slots = slots;
    this->slots_size = slots_size;
    this->item_size = item_size;
}
void UMap_insert( UMap* this, void* item ){
    uint64_t index = this->ops->hash( item );
    index %= (this->slots_size);
    void* slot = &(this->slots[ index * (this->item_size) ] ); 
    this->ops->append( slot, item );
}
/* target will be removed from slot , but not be released */
void* UMap_remove( UMap* this, void* tar ){
    uint64_t index = this->ops->hash( tar );
    index %= (this->slots_size);

    void* slot = &(this->slots[ index * (this->item_size) ] );
    return this->ops->remove( slot, tar );
}
void* UMap_find( UMap* this, void* tar ){
    uint64_t index = this->ops->hash( tar );
    index %= this->slots_size;
    void* slot = &(this->slots[ index *(this->item_size) ] );
    return this->ops->find( slot, tar );
}
