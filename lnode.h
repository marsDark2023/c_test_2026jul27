#pragma once 
#include <stdint.h>
/* remove curr from this:ListNode */
#define LISTNODE_REMOVE(this, curr) \
            do{ \
                 Node* prev = curr->prev; \
                 Node* next = curr->next; \
                 if(prev) \
                     prev->next = next; \
                 else \
                     this->nodes = next; \
                 if(next) \
                     next->prev = prev; \
                 else \
                     this->end = prev; \
            }while(0)

typedef struct __node {
    void* val;
    struct __node* next;
    struct __node* prev;
} Node;

typedef struct __listnode{
    Node* nodes;
    Node* end;
    
    uint32_t (*equal)(void*, void* ); /* how to compare */
} ListNode;

void ListNode_init( ListNode* this, uint32_t equal(void*,void*) );
void ListNode_append( ListNode* this, Node* item );
Node* ListNode_find( ListNode* this, Node* tar );
Node* ListNode_remove( ListNode* this, Node* tar );
