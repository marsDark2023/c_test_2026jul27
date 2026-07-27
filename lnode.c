#include <stdlib.h>
#include "lnode.h"

void ListNode_init( 
    ListNode* this, uint32_t equal(void*,void*)
){
    this->nodes = 0;
    this->end = 0;
    this->equal = equal;
}
void ListNode_append( ListNode* this, Node* item ){
    item->next = 0;
    Node* end = this->end;
    if( end ){
        end->next = item;
        end->next->prev = end;
        this->end = end->next;
    }else
    {
        this->nodes = item;
        this->nodes->prev = 0;
        this->end = this->nodes;
    }
}
Node* ListNode_find( ListNode* this, Node* tar){
    if( !(this->nodes) )
        return 0;
    Node* node = this->nodes ;
    Node* end = this->end ;
    uint32_t (*equal)(void*, void* ) = this->equal;
    do{ 
       if( equal( node, tar ) )
            return node;
       node = node->next;
    } while( node!= 0  );
    return 0;
}
/*
looking for tar and remove it 
*/
Node* ListNode_remove( ListNode* this, Node* tar ){
    if( !this->nodes )
        return 0;
    uint32_t (*equal) (void*, void*) = this->equal;
    Node* curr = this->nodes;
    while( curr!=0 ){
        /* found */
        if( equal( curr, tar ) ){
            LISTNODE_REMOVE( this, curr ); 
            return curr;
        } 
        /* found end */
        curr = curr->next;
    }
    return 0;
}

