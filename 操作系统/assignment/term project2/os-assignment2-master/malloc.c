#include <stdio.h>
#include <unistd.h>

#define align4(x) (((((x)-1) >> 2) << 2) + 4)
#define align8(x) (((((x)-1) >> 3) << 3) + 8)
#define BLOCK_SIZE 40 // data段不计入block_size，用sizeof计算有偏差
// #define BLOCK_SIZE sizeof(struct s_block)

void * base;

typedef struct s_block *t_block;
struct s_block {
    size_t size;
    t_block prev;
    t_block next;
    int free;
    // int padding; // 填充使返回的data是对齐的地址
    void *ptr;
    char data[1];
};

// 分裂多余的空间
void split_block(t_block b, size_t s){
    t_block new;
    new = (t_block)(b->data + s);
    new->size = b->size - s -BLOCK_SIZE;
    new->prev = b;
    new->next = b->next;
    new->free = 1;
    new->ptr = new->data;
    b->size = s;
    b->next = new;
}

// 找到可用的一块空间,first fit实现
t_block find_block(t_block *last, size_t size){
    t_block curr = base;
    while(curr && !(curr->free && curr->size >= size)){
        *last = curr;
        curr = curr->next;
    }
    // printf("find block: %p\n",curr);
    return curr;
}

// 通过sbrk扩展heap
t_block extend_heap(t_block last, size_t s){
    t_block b;
    b = sbrk(0);
    if(sbrk(BLOCK_SIZE + s) == (void *)-1)
        return NULL;
    b->size = s;
    b->prev = last;
    b->next = NULL;
    b->free = 0;
    b->ptr = b->data;
    if(last)
        last->next = b;
    return b;
}


void *myalloc(size_t size)
{
    t_block curr,prev;

    if(!size)
        return NULL;
    size_t s = align8(size);

    if(base){
        prev = base;
        curr = find_block(&prev, s);
        if(curr){
            if((curr->size) - s >= (BLOCK_SIZE) + 8)
                split_block(curr, s);
            curr->free = 0;
        }
        else{
            // 找不到可用的块，扩展heap
            curr = extend_heap(prev, s);
            if(!curr)
                return NULL;
        }
    }
    else{
        // first time
        curr = extend_heap(NULL, s);
        if(!curr)
            return NULL;
        base = curr;
    }
    return curr->data;
}

// 从给的data地址获得block地址
t_block get_block(void *p) {
    char *tmp;  
    tmp = p;
    return (p = tmp -= BLOCK_SIZE);
}

//确认地址合法
int valid_addr(void *p){
    if(base){
        if(p > base && p < sbrk(0)){
            return p == (get_block(p))->ptr;
        }
    }
    return 0;
}

t_block fusion(t_block b) {
    if (b->next && b->next->free) {
        b->size += BLOCK_SIZE + b->next->size;
        b->next = b->next->next;
        if(b->next)
            b->next->prev = b;
    }
    return b;
}

void myfree(void *ptr)
{
    t_block curr;
    if(valid_addr(ptr)){
        curr = get_block(ptr);
        curr->free = 1;
        if(curr->prev && curr->prev->free)
            curr = fusion(curr->prev);
        if(curr->next)
            fusion(curr);
        else{
            // free是heap顶的地址
            if(curr->prev)
                curr->prev->next = NULL;
            else
                base = NULL;
            brk(curr);
        }
    }
}


int main()
{
    void *ptr;
    printf("pid: %d\n", getpid());
    printf("heapStart: %p\n\n", sbrk(0));
    sleep(15);

    printf("------------alloc------------\n");
    ptr = myalloc(5000);
    printf("heapEnd: %p\n", sbrk(0));
    printf("alloc address: %p\n", ptr);
    printf("------------alloc------------\n\n");
    sleep(15);

    printf("------------free------------\n");
    myfree(ptr);
    printf("heapEnd: %p\n", sbrk(0));
    printf("------------free------------\n\n");
    sleep(15);

    printf("------------alloc------------\n");
    ptr = myalloc(30);
    printf("heapEnd: %p\n", sbrk(0));
    printf("------------alloc------------\n");
    sleep(15);

    // sleep(100);
}