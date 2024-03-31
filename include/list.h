#pragma once

#include "std_type.h"

typedef struct singly_linked_list_entry_s
{ 
    struct singly_linked_list_entry_s *next;
} singly_linked_list_entry_t;

typedef struct 
{ 
    singly_linked_list_entry_t *head;
    singly_linked_list_entry_t *tail;
} singly_linked_list_head_t;

typedef u32 (*singly_linked_list_callback_fn_t)(singly_linked_list_entry_t *);

static inline void SinglyLinkedList_Init( singly_linked_list_head_t *obj)
{
    obj->head = obj->tail = NULL;
}

static inline u32 SinglyLinkedList_IsEmpty( singly_linked_list_head_t *obj)
{
    return (NULL == obj->head);
}

static inline void SinglyLinkedList_PushFront( singly_linked_list_head_t *obj, singly_linked_list_entry_t *node)
{
    if (SinglyLinkedList_IsEmpty(obj))
    {
        node->next = NULL;
        obj->head = node;
        obj->tail = node;
    }
    else 
    {
        node->next = obj->head;
        obj->head = node;       
    }
}

static inline void SinglyLinkedList_PushBack( singly_linked_list_head_t *obj, singly_linked_list_entry_t *node)
{
    node->next = NULL;
    if (SinglyLinkedList_IsEmpty(obj))
    {
        obj->head = node;
    }
    else 
    {
        obj->tail->next = node;
    }    
    obj->tail = node;
}

static inline singly_linked_list_entry_t *SinglyLinkedList_PeekHead( singly_linked_list_head_t *obj)
{
    return obj->head;
}

static inline singly_linked_list_entry_t *SinglyLinkedList_PeekTail( singly_linked_list_head_t *obj)
{
    return obj->tail;
}

static inline void SinglyLinkedList_SetTailIfEmpty( singly_linked_list_head_t *obj)
{
    if (SinglyLinkedList_IsEmpty(obj))
    {
        obj->tail = NULL;
    }
}

static inline singly_linked_list_entry_t *SinglyLinkedList_PopFront(singly_linked_list_head_t *obj)
{
    singly_linked_list_entry_t *node = obj->head;
    
    if (!SinglyLinkedList_IsEmpty(obj))
    {
        obj->head = obj->head->next;
        SinglyLinkedList_SetTailIfEmpty(obj);
    }

    return node;
}

static inline u32 SinglyLinkedList_SearchAndDelNode(singly_linked_list_head_t *obj, singly_linked_list_entry_t *node)
{
    singly_linked_list_entry_t **cur = &obj->head;
    while (*cur != NULL) 
    {
        if (*cur == node)
        {
            *cur = node->next;
            if (obj->tail == node)
            {
                obj->tail = SinglyLinkedList_IsEmpty(obj) ? NULL : (singly_linked_list_entry_t *)cur; 
            }            
            return RET_SUCCESS;
        }
        cur = &((*cur)->next);
    }

    return RET_FAIL;
}

static inline u32 SinglyLinkedList_GoTrough(singly_linked_list_head_t *obj, singly_linked_list_callback_fn_t callback)
{
    singly_linked_list_entry_t *pNode = obj->head;
    u32 cnt = 0;
    
    while (pNode != NULL)
    {        
        if(NULL != callback)              
        {
            callback(pNode);
        }        
        pNode = pNode->next;
        cnt++;
    }

    return cnt;
}

static inline u32 SinglyLinkedList_GoTroughAndDel(singly_linked_list_head_t *obj, singly_linked_list_callback_fn_t callback)
{
    u32 cnt = 0;
    while (!SinglyLinkedList_IsEmpty(obj))
    {
        singly_linked_list_entry_t *pNode = SinglyLinkedList_PopFront(obj);       
        if (NULL != callback) 
        {
            callback(pNode);
        }
        cnt++;
    }

    return cnt;
}

#define SinglyLinkedList_DelAllNodes(obj)   SinglyLinkedList_GoTroughAndDel((singly_linked_list_head_t *)(obj), NULL)
#define SinglyLinkedList_Length(obj)        SinglyLinkedList_GoTrough((singly_linked_list_head_t *)(obj), NULL)


typedef struct doubly_linked_list_entry_s
{ 
    struct doubly_linked_list_entry_s *next;
    struct doubly_linked_list_entry_s *prev;
} doubly_linked_list_entry_t;

typedef union
{
    doubly_linked_list_entry_t root;
    struct
    {
        doubly_linked_list_entry_t *head;
        doubly_linked_list_entry_t *tail;
    };
} doubly_linked_list_head_t;

typedef u32 (*doubly_linked_list_callback_fn_t)(doubly_linked_list_entry_t *);

static inline void DoublyLinkedList_Init(doubly_linked_list_head_t *obj)
{
    obj->head = obj->tail = &obj->root;
}

static inline u32 DoublyLinkedList_IsEmpty(doubly_linked_list_head_t * obj)
{
    return (obj->head == &obj->root);
}

static inline void DoublyLinkedList_InsertBefore(doubly_linked_list_entry_t *next, doubly_linked_list_entry_t *node)
{
    node->next = next;
    node->prev = next->prev;
    next->prev->next = node;
    next->prev = node;
}

static inline void DoublyLinkedList_InsertAfter(doubly_linked_list_entry_t *prev, doubly_linked_list_entry_t *node)
{
    node->prev = prev;
    node->next = prev->next;
    prev->next->prev = node;
    prev->next = node;
}

static inline void DoublyLinkedList_PushFront(doubly_linked_list_head_t *obj, doubly_linked_list_entry_t *node)
{
    DoublyLinkedList_InsertBefore(obj->head, node);
}

static inline void DoublyLinkedList_PushBack(doubly_linked_list_head_t *obj, doubly_linked_list_entry_t *node)
{
    DoublyLinkedList_InsertAfter(obj->tail, node);
}

static inline doubly_linked_list_entry_t *DoublyLinkedList_Del(doubly_linked_list_entry_t * node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

static inline doubly_linked_list_entry_t *DoublyLinkedList_PopFront(doubly_linked_list_head_t *obj)
{
    doubly_linked_list_entry_t *node = NULL;
    if (!DoublyLinkedList_IsEmpty(obj))
    {
        node = DoublyLinkedList_Del(obj->head);
    }
    
    return node;
}

static inline doubly_linked_list_entry_t *DoublyLinkedList_PopBack(doubly_linked_list_head_t *obj)
{
    doubly_linked_list_entry_t *node = NULL;
    if (!DoublyLinkedList_IsEmpty(obj))
    {
        node = DoublyLinkedList_Del(obj->tail);
    }
    
    return node;
}

static inline u32 DoublyLinkedList_GoTroughAndDel(doubly_linked_list_head_t *obj,  doubly_linked_list_callback_fn_t callback)
{
    u32 node_cnt = 0;
    while (!DoublyLinkedList_IsEmpty(obj))
    {
        doubly_linked_list_entry_t *pNode = DoublyLinkedList_PopFront(obj);
        if (NULL != callback)
        {
            callback(pNode);
        }
        node_cnt++;
    }
    return node_cnt;
}

static inline u32 DoublyLinkedList_GoTrough(doubly_linked_list_head_t *obj,  doubly_linked_list_callback_fn_t callback)
{
    u32 node_cnt = 0;
    doubly_linked_list_entry_t *pNode = obj->head;
    while (pNode != &obj->root)
    {        
        if (NULL != callback)
        {
            callback(pNode);
        }
        pNode = pNode->next;
        node_cnt++;
    }
    return node_cnt;
}

static inline void DoublyLinkedList_Reverse(doubly_linked_list_head_t *obj)
{
    doubly_linked_list_entry_t *cur = &obj->root;
    do
    {
        doubly_linked_list_entry_t *prev = cur->prev;
        doubly_linked_list_entry_t *next = cur->next;
        cur->next = prev;
        cur->prev = next;
        cur = cur->prev;

    } while (cur != &obj->root);
}

#define DoublyLinkedList_DelAllNodes(obj)   DoublyLinkedList_GoTroughAndDel((doubly_linked_list_head_t *)(obj), NULL)
#define DoublyLinkedList_Length(obj)	    DoublyLinkedList_GoTrough((doubly_linked_list_head_t *)obj, NULL)