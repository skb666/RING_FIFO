# 环形缓冲队列

实现一个通用环形缓冲队列

+ 支持任意 POD 类型数据
+ 支持单个数据存取
+ 支持多个数据存取
+ 支持循环覆盖或非覆盖模式
+ 非覆盖模式下可以当作优先队列使用

为了兼容嵌入式，未实现锁机制，若需要则在此通用环形队列的基础上再根据需求自行封装一层接口。

## 使用方式

主要实现逻辑在 [ring_fifo.c](src/ring_fifo.c) 中，将该源文件与头文件加入项目编译，或者将其编译为[静态链接库](CMakeLists.txt)、动态链接库使用。

简单使用示例见 [example.c](example.c)

**编译运行 example**

```bash
cmake -S. -Bbuild
cmake --build build --target example -- -j${nproc}      # 16 位目标
cmake --build build --target example32 -- -j${nproc}    # 32 位目标
cmake --build build --target example64 -- -j${nproc}    # 64 位目标
cmake --build build --target all -- -j${nproc}          # 以上所有
```

## 外部接口

```c
/**
 * @brief  定义 RING_FIFO 变量
 * @param  Type 存放的数据类型
 * @param  BufName RING_FIFO 变量名称
 * @param  Size 最多可以存放的数据数量
 * @param  Cover 是否支持覆盖 (0 / 1)
 * @retval None
 */
#define ring_define(Type, BufName, Size, Cover)        \
    Type __##BufName##_data[Size];                     \
    RING_FIFO BufName = {                              \
        .buffer = __##BufName##_data,                  \
        .capacity = Size,                              \
        .element_size = sizeof(__##BufName##_data[0]), \
        .cover = Cover,                                \
        .head = 0,                                     \
        .tail = 0,                                     \
        .size = 0,                                     \
    }

/**
 * @brief  定义 RING_FIFO 静态变量
 * @param  Type 存放的数据类型
 * @param  BufName RING_FIFO 变量名称
 * @param  Size 最多可以存放的数据数量
 * @param  Cover 是否支持覆盖 (0 / 1)
 * @retval None
 */
#define ring_define_static(Type, BufName, Size, Cover) \
    static Type __##BufName##_data[Size];              \
    static RING_FIFO BufName = {                       \
        .buffer = __##BufName##_data,                  \
        .capacity = Size,                              \
        .element_size = sizeof(__##BufName##_data[0]), \
        .cover = Cover,                                \
        .head = 0,                                     \
        .tail = 0,                                     \
        .size = 0,                                     \
    }

/**
 * @brief  放入单个数据
 * @param  ring RING_FIFO 变量的地址
 * @param  element 待存入数据的地址
 * @retval 是否成功放入，失败时返回值小于 0（支持覆盖时，参数正确传入则永远返回0）
 */
int8_t ring_push(RING_FIFO *ring, const void *element);

/**
 * @brief  优先队列，二分插入（仅在非覆盖模式下可用）
 * @param  ring RING_FIFO 变量的地址
 * @param  element 待存入数据的地址
 * @param  compare 比较 element 的函数，返回值 [-1, 0, 1]
 * @retval 是否成功放入，失败时返回值小于 0
 */
int8_t ring_binsert(RING_FIFO *ring, const void *element, int (*compare)(const void *, const void *));

/**
 * @brief  取出单个数据
 * @param  ring RING_FIFO 变量的地址
 * @param  element 取出数据的存放地址
 * @retval 是否成功取出，失败时返回值小于 0
 */
int8_t ring_pop(RING_FIFO *ring, void *element);

/**
 * @brief  弹出单个数据
 * @param  ring RING_FIFO 变量的地址
 * @retval 是否成功弹出，失败时返回值小于 0
 */
int8_t ring_pop_unread(RING_FIFO *ring);

/**
 * @brief  获取下一个被取出数据的地址
 * @param  ring RING_FIFO 变量的地址
 * @retval 成功时返回数据地址，否则返回 NULL
 */
void *ring_peek(RING_FIFO *ring);

/**
 * @brief  放入多个数据
 * @param  ring RING_FIFO 变量的地址
 * @param  elements 待存入数据块的首地址
 * @param  num 待存入数据的个数
 * @retval 成功放入几个（支持覆盖时，参数正确传入则返回值与数据个数相等）
 */
NUM_TYPE ring_push_mult(RING_FIFO *ring, const void *elements, NUM_TYPE num);

/**
 * @brief  取出多个数据
 * @param  ring RING_FIFO 变量的地址
 * @param  elements 取出数据块的存放地址
 * @param  num 待取出数据的个数
 * @retval 成功取出几个
 */
NUM_TYPE ring_pop_mult(RING_FIFO *ring, void *elements, NUM_TYPE num);

/**
 * @brief  重置 RING_FIFO
 * @param  ring RING_FIFO 变量的地址
 * @retval None
 */
void ring_reset(RING_FIFO *ring);

/**
 * @brief  RING_FIFO 是否为空
 * @param  ring RING_FIFO 变量的地址
 * @retval 为空返回 1
 */
int8_t ring_is_empty(RING_FIFO *ring);

/**
 * @brief  RING_FIFO 是否已满
 * @param  ring RING_FIFO 变量的地址
 * @retval 已满返回 1
 */
int8_t ring_is_full(RING_FIFO *ring);

/**
 * @brief  获取 RING_FIFO 内实际数据数量
 * @param  ring RING_FIFO 变量的地址
 * @retval RING_FIFO 内实际数据数量
 */
NUM_TYPE ring_size(RING_FIFO *ring);

/**
 * @brief  打印 RING_FIFO 内部信息
 * @param  ring RING_FIFO 变量的地址
 * @retval None
 */
void print_ring(RING_FIFO *ring);
```