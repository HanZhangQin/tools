#include <stdio.h>
#include "../include/list.h"
#include "../include/common_type.h"

#define MAX_LIST_LENGTH 100
#define PTR_2_IDX(type, ptr, base) ((ptr != NULL) ? ((u32)((type *)(ptr) - (type *)(base))) : 0)

typedef struct
{
    singly_linked_list_entry_t node;
    u32 data;
} data_t;

u32 print_data_node(data_t *pData)
{
    printf ("%d %s", pData->data, (pData->node.next != NULL) ? "-> " : "\n" );
    return RET_SUCCESS;
}

void create_data_list(singly_linked_list_head_t *data_list, data_t *data_pool, u32 is_reverse)
{
    for (u32 i = 0; i < MAX_LIST_LENGTH; i++)
    {
        data_pool[i].data = i;
        if (!is_reverse)
            SinglyLinkedList_PushBack(data_list, &data_pool[i].node);
        else
            SinglyLinkedList_PushFront(data_list, &data_pool[i].node);
    }

    printf ("Inited %s SinglyLinkedList:\n", is_reverse ? "Reversed" : "");
    SinglyLinkedList_GoTrough(data_list, (singly_linked_list_callback_fn_t)print_data_node);
}

void random_del_node(singly_linked_list_head_t *data_list, data_t *data_pool)
{
    singly_linked_list_entry_t *pNode;

    while (!SinglyLinkedList_IsEmpty(data_list))
    {
        u32 node_index;
        do
        {
            node_index = gen_rand_u32(0, MAX_LIST_LENGTH);
            pNode = &data_pool[node_index].node;
        } while (RET_FAIL == SinglyLinkedList_SearchAndDelNode(data_list, pNode));

        printf ("Afetr Del List %d [Length: %d head: %d tail: %d]:\n", node_index, SinglyLinkedList_Length(data_list), PTR_2_IDX(data_t, data_list->head, data_pool), PTR_2_IDX(data_t, data_list->tail, data_pool));

        SinglyLinkedList_GoTrough(data_list, (singly_linked_list_callback_fn_t)print_data_node);

    }
    printf ("EMPTY_LIST Length: %d head: 0x%llx tail: 0x%llx\n", SinglyLinkedList_Length(data_list), (u64)data_list->head, (u64)data_list->tail);
}

u32 g_init_check_sum = 0;
u32 add_inital_check_sum(singly_linked_list_entry_t *pNode)
{
    g_init_check_sum += ((data_t *)pNode)->data;
    return g_init_check_sum;
}

u32 g_cur_check_sum = 0;
u32 add_current_check_sum(singly_linked_list_entry_t *pNode)
{
    g_cur_check_sum += ((data_t *)pNode)->data;
    return g_cur_check_sum;
}

u32 calc_cur_check_sum(singly_linked_list_head_t *list1, singly_linked_list_head_t *list2)
{
    g_cur_check_sum = 0;
    SinglyLinkedList_GoTrough(list1, add_current_check_sum);
    SinglyLinkedList_GoTrough(list2, add_current_check_sum);

    return g_cur_check_sum;
}

void random_insert_del_node(singly_linked_list_head_t *data_list, data_t *data_pool)
{
    singly_linked_list_head_t list_back;
    singly_linked_list_head_t *another_list = &list_back;
    SinglyLinkedList_Init(another_list);

    SinglyLinkedList_GoTrough(data_list, add_inital_check_sum);
    SinglyLinkedList_GoTrough(another_list, add_inital_check_sum);
    printf ("RAND INSERT DEL TEST INIT CHECKSUM %d\n", g_init_check_sum);
    u32 loop_count = 0;
    u64 cur_time = time(NULL);
    while (time(NULL) - cur_time < 300)
    {
        u32 node_index = gen_rand_u32(0, MAX_LIST_LENGTH);
        u32 is_push_front = gen_rand_u32(0, 2);
        singly_linked_list_entry_t *pNode = &data_pool[node_index].node;
        singly_linked_list_head_t *push_list = NULL;

        if (RET_SUCCESS == SinglyLinkedList_SearchAndDelNode(data_list, pNode))
        {
            push_list = another_list;
        }
        else if (RET_SUCCESS == SinglyLinkedList_SearchAndDelNode(another_list, pNode))
        {
            push_list = data_list;
        }
        else
        {
            printf ("ERR: node %d not in any list!\n", node_index);
            SinglyLinkedList_GoTrough(data_list, (singly_linked_list_callback_fn_t)print_data_node);
            SinglyLinkedList_GoTrough(another_list, (singly_linked_list_callback_fn_t)print_data_node);
            return;
        }

        if (is_push_front)
        {
            SinglyLinkedList_PushFront(push_list, pNode);
            if (push_list->head != pNode)
            {
                printf ("ERR: Push Front Fail!\n");
                SinglyLinkedList_GoTrough(data_list, (singly_linked_list_callback_fn_t)print_data_node);
                SinglyLinkedList_GoTrough(another_list, (singly_linked_list_callback_fn_t)print_data_node);
                return;
            }
        }
        else
        {
            SinglyLinkedList_PushBack(push_list, pNode);
            if (push_list->tail != pNode)
            {
                printf ("ERR: Push tail Fail!\n");
                SinglyLinkedList_GoTrough(data_list, (singly_linked_list_callback_fn_t)print_data_node);
                SinglyLinkedList_GoTrough(another_list, (singly_linked_list_callback_fn_t)print_data_node);
                return;
            }
        }
        calc_cur_check_sum(data_list, another_list);
        if (g_cur_check_sum != g_init_check_sum)
        {
            printf ("ERR: check sum uncorrect %d %d\n", g_cur_check_sum, g_init_check_sum);
            SinglyLinkedList_GoTrough(data_list, (singly_linked_list_callback_fn_t)print_data_node);
            SinglyLinkedList_GoTrough(another_list, (singly_linked_list_callback_fn_t)print_data_node);
            return;
        }
        loop_count++;
    }

    u32 node_count = 0;
    node_count = SinglyLinkedList_GoTrough(data_list, (singly_linked_list_callback_fn_t)print_data_node);
    node_count += SinglyLinkedList_GoTrough(another_list, (singly_linked_list_callback_fn_t)print_data_node);

    printf ("Test done loop cnt %d %d\n", loop_count, node_count);
}

u32 data_remove_condition(singly_linked_list_entry_t *data)
{
    return ((((data_t *)data)->data % 5) != 0) ? true : false;
}

void test_SinglyLinkedList_SearchAndDelNodeByCond(singly_linked_list_head_t *data_list, data_t *data_pool)
{
    create_data_list(data_list, data_pool, false);
    SinglyLinkedList_SearchAndDelNodeByCond(data_list, data_remove_condition);
    SinglyLinkedList_GoTrough(data_list, (singly_linked_list_callback_fn_t)print_data_node);
}

int main(void)
{
    data_t data_pool[MAX_LIST_LENGTH];
    singly_linked_list_head_t data_list;
    SinglyLinkedList_Init(&data_list);
 #if 0
    // create a list in order
    create_data_list(&data_list, data_pool, false);
    random_del_node(&data_list, data_pool);

    printf ("\n");
    // create a list in reverse order
    create_data_list(&data_list, data_pool, true);
    random_del_node(&data_list, data_pool);

    printf ("\n");
#endif
#if 0
    // create a list in order
    create_data_list(&data_list, data_pool, false);
    random_insert_del_node(&data_list, data_pool);
#endif
    test_SinglyLinkedList_SearchAndDelNodeByCond(&data_list, data_pool);
    return 0;
}
