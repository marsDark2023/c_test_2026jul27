/**
dynamic alloc used;
nvmap_
      |- lnode
      |---------umap.slots--/
      \- umap   <----------/
**/

#include <stdlib.h>
#include <string.h>
#include "nvmap.h"


/* used by list */
uint32_t  list_equal (void* a, void* b){
    NameVal* a_val = (NameVal*)( ((Node*)a) -> val );
    NameVal* b_val = (NameVal*)( ((Node*)b) -> val );
    return strcmp( a_val->name, b_val->name ) == 0 ? 1 : 0 ;
}
/*used by umap */
void* umap_iterator( void* list, void* curr ){
    if(!curr)
        return ((ListNode*)list)->nodes;
    return ((Node*)curr)->next;
}
void umap_append( void* list, void* node ){
    ListNode_append( ((ListNode*) list), ((Node*) node) );
}
void* umap_find( void* list, void* node ){
    Node* res = ListNode_find( (ListNode*)list, (Node*)node );
    return (void*)res;
}
void* umap_remove( void* list, void* node ){
    Node* ret = ListNode_remove( (ListNode*)list, (Node*)node );
    return (void*)ret;
}
uint64_t umap_hash( void* node ){
    char* name = ( (NameVal*)(((Node*)node)->val) )->name ;
    uint64_t ret = 131;
    for (uint32_t i = 0 ; name[i]; ++i)
        ret = ret * 31 + name[i] ;
    return ret;
}
UMap_Ops ops = {
     umap_iterator, umap_append, umap_find, umap_remove, umap_hash
};
/**/


void Nvmap_init( Nvmap* this ){
    ListNode template = {0,0, list_equal };
    ListNode* listnodes = this->listnodes; /* slots */
    for (uint32_t i = 0 ; i<SLOTS_SIZE; ++i)
        listnodes[ i ] = template;
    memset( &(this->umap), 0, sizeof(UMap) );
    memset( &(this->list), 0, sizeof(ListNode) );
    this->iterator_ptr = 0;
    UMap_init( 
        &(this->umap), 
        (void*)listnodes, 
        SLOTS_SIZE, 
        sizeof(ListNode),
        &ops
        );
}

/* dynamic alloc Node
space_
      |- target node
      |- record node
      |- target string
      \- target name-val tuple
*/

/**
insert target; tar->name and tar->val;
**/
uint32_t Nvmap_insert( Nvmap* this, NameVal* tar ){
    uint32_t name_len = strlen(tar->name) + 1;
    uint32_t space_size = sizeof(Node) * 2 + name_len
        + sizeof(NameVal) ;
    void* space = malloc( space_size );
    if(!space)
        return -1;
    Node* tar_n = (Node*) space;
    Node* record = (Node*)( tar_n + sizeof(Node) );
    char* tar_name = (char*)( record + (sizeof(Node)) );
    NameVal* tar_nv = (NameVal*)( tar_name + name_len );

    strcpy( tar_name, tar->name );
    
    memcpy( tar_nv, tar, sizeof(NameVal) );
    tar_nv->name = tar_name;

    tar_n->val = (void*)tar_nv;

    record->val = space; /* insert to nvmap->list */

    UMap_insert( &(this->umap), (void*)tar_n );
    ListNode_append( &(this->list), record );
    return 0;
}
/**
only tar->name be required;
**/
uint32_t Nvmap_remove( Nvmap* this, NameVal* tar ){
   Node n0;
   n0.val = (void*) tar;
   Node* found = UMap_remove( &(this->umap), &n0 );
   if(!found)
        return -1;
    Node* record = found + sizeof(Node) ; /* was alloced with found; -->space */
    LISTNODE_REMOVE( (&(this->list)), record );
    free( found );
    return 0;
}
/**
only tar->name be required;
**/
NameVal* Nvmap_find( Nvmap* this, NameVal* tar){
    Node n0;
    n0.val = (void*) tar;
    Node* found = UMap_find( &(this->umap), &n0 );
    if(!found)
        return 0;
    return found->val;
}
/**
find target with tar->name , and set its value to tar->val;
**/
uint32_t Nvmap_modify( Nvmap* this, NameVal* tar ){
    Node n0;
    n0.val = (void*) tar;
    Node* found = UMap_find( &(this->umap), &n0 );
    if(!found)
        return -1;
    NameVal* nv_p = (NameVal*)(found->val);
    memcpy( &(nv_p->val), &(tar->val), sizeof(Subject) );
    return 0;
}
NameVal* Nvmap_iterator( Nvmap* this ){
    if( !(this->iterator_ptr) )
        this->iterator_ptr = this->list.nodes;
    else
        this->iterator_ptr = this->iterator_ptr->next;
    NameVal* ret = this->iterator_ptr ? (NameVal*)( ((Node*)( this->iterator_ptr->val ))->val ) : 0;
    return ret ;            
}

void Nvmap_free( Nvmap* this ){
   Node* ptr = this->list.nodes ;
   while(ptr){
        Node* next = ptr->next;        
        free( ptr->val );
        ptr = next;
   }
   Nvmap_init( this );
}
