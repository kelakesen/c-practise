#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// ======================= 错误处理 =======================
void die(const char *message)
{
    if (errno) {
        perror(message);
    } else {
        fprintf(stderr, "ERROR: %s\n", message);
    }
    exit(1);
}

// ======================= 栈结构体定义 =======================
typedef struct {
    int *items;      // 指向堆内存数组的指针
    int top;         // 栈顶索引（-1 表示空栈）
    int capacity;    // 当前总容量
} Stack;

// ======================= 栈操作函数 =======================

// 创建栈（初始容量默认为 4）
Stack *Stack_create(int initial_capacity)
{
    if (initial_capacity <= 0) initial_capacity = 4;

    Stack *s = malloc(sizeof(Stack));
    if (!s) die("Failed to allocate stack structure.");

    s->items = malloc(sizeof(int) * initial_capacity);
    if (!s->items) {
        free(s);
        die("Failed to allocate stack items.");
    }

    s->top = -1;
    s->capacity = initial_capacity;
    return s;
}

// 销毁栈（释放所有堆内存）
void Stack_destroy(Stack *s)
{
    if (s) {
        if (s->items) free(s->items);
        free(s);
    }
}

// 检查栈是否为空
int Stack_is_empty(Stack *s)
{
    return s->top == -1;
}

// 检查栈是否已满（内部使用，用于扩容判断）
static int Stack_is_full(Stack *s)
{
    return s->top + 1 >= s->capacity;
}

// 内部扩容函数（容量翻倍）
static void Stack_resize(Stack *s)
{
    int new_capacity = s->capacity * 2;
    int *new_items = realloc(s->items, sizeof(int) * new_capacity);
    if (!new_items) die("Failed to resize stack.");

    s->items = new_items;
    s->capacity = new_capacity;
    printf("[Info] Stack resized to capacity: %d\n", new_capacity); // 调试提示
}

// 压入元素（入栈）
void Stack_push(Stack *s, int value)
{
    if (!s) die("Stack is NULL.");

    if (Stack_is_full(s)) {
        Stack_resize(s);
    }

    s->items[++s->top] = value;
}

// 弹出元素（出栈）
int Stack_pop(Stack *s)
{
    if (!s) die("Stack is NULL.");
    if (Stack_is_empty(s)) die("Stack underflow (cannot pop from empty stack).");

    return s->items[s->top--];
}

// 查看栈顶元素（不出栈）
int Stack_peek(Stack *s)
{
    if (!s) die("Stack is NULL.");
    if (Stack_is_empty(s)) die("Stack is empty (cannot peek).");

    return s->items[s->top];
}

// 获取当前栈中元素个数
int Stack_size(Stack *s)
{
    return s->top + 1;
}

// 打印栈中所有元素（从栈底到栈顶）
void Stack_print(Stack *s)
{
    if (!s || Stack_is_empty(s)) {
        printf("Stack: [empty]\n");
        return;
    }

    printf("Stack (bottom -> top): ");
    for (int i = 0; i <= s->top; i++) {
        printf("%d ", s->items[i]);
    }
    printf("\n");
}

// ======================= 测试主函数 =======================
int main(int argc, char *argv[])
{
    printf("=== Testing Stack Implementation ===\n");

    // 1. 创建栈（初始容量 2，以便快速触发扩容演示）
    Stack *s = Stack_create(2);

    // 2. 压入 5 个元素（触发至少一次扩容）
    printf("\nPushing 10, 20, 30, 40, 50...\n");
    Stack_push(s, 10);
    Stack_push(s, 20);
    Stack_push(s, 30);
    Stack_push(s, 40);
    Stack_push(s, 50);
    Stack_print(s);  // 预期：10 20 30 40 50

    // 3. 查看栈顶
    printf("\nPeek top: %d\n", Stack_peek(s)); // 预期：50

    // 4. 弹出两个元素
    printf("\nPopping: %d\n", Stack_pop(s));   // 预期：50
    printf("Popping: %d\n", Stack_pop(s));     // 预期：40
    Stack_print(s);                            // 预期：10 20 30

    // 5. 获取当前大小
    printf("\nCurrent size: %d\n", Stack_size(s)); // 预期：3

    // 6. 继续弹完
    printf("\nPopping remaining...\n");
    while (!Stack_is_empty(s)) {
        printf("Pop: %d\n", Stack_pop(s));
    }
    Stack_print(s); // 预期：empty

    // 7. 测试错误情况（尝试从空栈弹出，会触发 die 终止程序）
    // 取消下面注释即可测试错误处理
    // printf("\nTrying to pop from empty stack:\n");
    // Stack_pop(s);

    // 8. 清理资源
    Stack_destroy(s);
    printf("\nStack destroyed. Program exit successfully.\n");

    return 0;
}
