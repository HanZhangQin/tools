#include <stdio.h>
#include "../include/list.h"
#include "../include/common_type.h"

#define MAX_LIST_LENGTH 20
#define PTR_2_IDX(type, ptr, base) ((ptr != NULL) ? ((u32)((type *)(ptr) - (type *)(base))) : 0)

typedef struct
{
    doubly_linked_list_entry_t node;
    u32 data;
} data_t;

void create_doubly_linked_list(doubly_linked_list_head_t *list, data_t *data_pool, u32 is_reverse)
{
    for (u32 i = 0; i < MAX_LIST_LENGTH; i++)
    {
        data_pool[i].data = i;
        if (is_reverse)
        {
            DoublyLinkedList_PushFront(list, &data_pool[i].node);
        }
        else
        {
            DoublyLinkedList_PushBack(list, &data_pool[i].node);
        }
    }
}

void print_doubly_linked_list(doubly_linked_list_head_t *list, u32 is_reverse)
{ 
    u32 node_cnt = 0;
    if (!is_reverse)
    {
        doubly_linked_list_entry_t *node = list->head;
        while (node != &list->root)
        {
            node_cnt++;
            printf ("%d ", ((data_t *)node)->data);
            node = node->next;
        }
    }
    else 
    {
        doubly_linked_list_entry_t *node = list->tail;
        while (node != &list->root)
        {
            node_cnt++;
            printf ("%d ", ((data_t *)node)->data);
            node = node->prev;
        }
    }
    
    if (!node_cnt)
    {
        printf ("EMPTY list\n");
    }

    printf ("\n");
    printf ("NODE:%d\n", node_cnt);
}

int main(void)
{
    doubly_linked_list_head_t data_list;
    DoublyLinkedList_Init(&data_list);
    
    data_t data_pool[MAX_LIST_LENGTH];
    
    create_doubly_linked_list(&data_list, data_pool, false);
    printf ("Init Doubly linked list:\n");
    print_doubly_linked_list(&data_list, false);
#if 0
    printf ("head %d, tail %d\n", PTR_2_IDX(data_t, data_list.head, data_pool), PTR_2_IDX(data_t, data_list.tail, data_pool));
    for (u32 i = 0; i < MAX_LIST_LENGTH; i++)
    {
        printf ("next %d, prev %d\n", PTR_2_IDX(data_t, data_pool[i].node.next, data_pool), PTR_2_IDX(data_t, data_pool[i].node.prev, data_pool));
    }

    print_doubly_linked_list(&data_list, true);
    
    DoublyLinkedList_DelAllNodes(&data_list);

    create_doubly_linked_list(&data_list, data_pool, true);
    print_doubly_linked_list(&data_list, false);
    print_doubly_linked_list(&data_list, true);
#endif    
    printf ("Reversed Doubly linked list:\n");
    DoublyLinkedList_Reverse(&data_list);
    print_doubly_linked_list(&data_list, false);

    printf ("Del Doubly linked list:\n");
    DoublyLinkedList_DelAllNodes(&data_list);
    print_doubly_linked_list(&data_list, false);
#if 0    
    printf ("head %d, tail %d\n", PTR_2_IDX(data_t, data_list.head, data_pool), PTR_2_IDX(data_t, data_list.tail, data_pool));
    for (u32 i = 0; i < MAX_LIST_LENGTH; i++)
    {
        printf ("next %d, prev %d\n", PTR_2_IDX(data_t, data_pool[i].node.next, data_pool), PTR_2_IDX(data_t, data_pool[i].node.prev, data_pool));
    }
#endif    
    

    return 0;
}
