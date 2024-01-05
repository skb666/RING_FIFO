#include <stdio.h>
#include <string.h>

#include "ring_fifo.h"

typedef enum {
    KE_PRESS,
    KE_RELEASE,
    KE_LONG_PRESS,
    KE_LONG_RELEASE,
    KE_NONE,
} KEY_EVENT;

typedef enum {
    EV_KEY,
    EV_NONE,
} EVENT_TYPE;

typedef struct {
    int type;
    int sub_type;
} EVENT;

int main() {
    int res;
    EVENT *ptr;
    EVENT ev_o[4];
    EVENT ev[] = {
        {
            .type = EV_KEY,
            .sub_type = KE_PRESS,
        },
        {
            .type = EV_KEY,
            .sub_type = KE_RELEASE,
        },
        {
            .type = EV_KEY,
            .sub_type = KE_LONG_PRESS,
        },
        {
            .type = EV_KEY,
            .sub_type = KE_LONG_RELEASE,
        },
    };

    /* 定义一个 ring_fifo
     * 数据类型：EVENT
     * 变量名称：events_buffer
     * 对应类型数据的数量：3
     * 是否支持循环覆盖：是
     */
    ring_define_static(EVENT, events_buffer, 3, 1);
    printf("size: %zd\n", sizeof(events_buffer));

    /* 将单个数据放入 ring_fifo
     * ring_fifo 变量地址：&events_buffer
     * 待存入数据的地址：&ev[0]
     * 返回值：是否成功放入（支持覆盖时，参数正确传入则永远返回0）
     */
    printf("\nring_push\n");
    ring_push(&events_buffer, &ev[0]);
    print_ring(&events_buffer);

    /* 将多个数据放入 ring_fifo
     * ring_fifo 变量地址：&events_buffer
     * 待存入数组的起始地址：ev + 1
     * 待存入数组数据的个数：3
     * 返回值：成功放入几个（支持覆盖时，参数正确传入则返回值与数据个数相等）
     */
    printf("\nring_push_mult\n");
    res = ring_push_mult(&events_buffer, ev + 1, 3);
    printf("success: %d\n", res);
    print_ring(&events_buffer);

    /* 查看 ring_fifo 下一个被取出的数据
     * ring_fifo 变量地址：&events_buffer
     * 返回值：是否成功取出
     */
    printf("\nring_peek\n");
    ptr = ring_peek(&events_buffer);
    if (ptr) {
        printf("ev_o->type: %d\n", ptr->type);
        printf("ev_o->subtype: %d\n", ptr->sub_type);
    }
    print_ring(&events_buffer);

    /* 从 ring_fifo 取出单个数据
     * ring_fifo 变量地址：&events_buffer
     * 取出数据的存放地址：&ev_o[0]
     * 返回值：是否成功取出
     */
    printf("\nring_pop\n");
    res = ring_pop(&events_buffer, &ev_o[0]);
    if (res >= 0) {
        printf("ev_o->type: %d\n", ev_o[0].type);
        printf("ev_o->subtype: %d\n", ev_o[0].sub_type);
    }
    print_ring(&events_buffer);

    /* 遍历 ring_fifo 中存在的数据，不取出
     * ring_fifo 变量地址：&events_buffer
     * 当前数据地址：ptr
     * 返回值：ptr 传入 NULL 时返回数据首地址，有下一个数据时返回下一个数据地址，否则返回 NULL
     */
    printf("\nring_peek_next\n");
    ptr = NULL;
    do {
        ptr = ring_peek_next(&events_buffer, ptr);
        if (ptr) {
            printf("ev_o->type: %d\n", ptr->type);
            printf("ev_o->subtype: %d\n", ptr->sub_type);
        }
    } while (ptr);
    print_ring(&events_buffer);

    /* 从 ring_fifo 取出多个数据
     * ring_fifo 变量地址：&events_buffer
     * 取出数组的存放地址：ev_o
     * 待取出数据的个数：4
     * 返回值：成功取出几个
     */
    printf("\nring_pop_mult\n");
    res = ring_pop_mult(&events_buffer, ev_o, 4);
    printf("success: %d\n", res);
    for (int i = 0; i < res; ++i) {
        printf("ev_o->type: %d\n", ev_o[i].type);
        printf("ev_o->subtype: %d\n", ev_o[i].sub_type);
    }
    print_ring(&events_buffer);

    printf("\033[1;34m[EXAMLE: \033[1;31m%zdbit\033[1;34m]\033[0;32m OK!!!\033[0m\n", sizeof(NUM_TYPE) * 8);

    return 0;
}
