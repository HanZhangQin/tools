#ifndef LIST_H
#define LIST_H

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

#endif