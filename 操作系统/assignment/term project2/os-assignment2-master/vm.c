/*
(1) 本小题不要求实现页置换（Page Replacement），TLB用简单的FIFO策略。30分。
(2)	实现LRU的TLB，8分。
(3)	实现基于LRU的Page Replacement，8分。
(4)	代码可读性，4分。
(5)	使用FIFO和LRU分别运行vm（TLB和页置换统一策略），打印比较Page-fault
rate和TLB hit rate，给出运行的截屏。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PAGE_TABLE_SIZE 256
#define PAGE_SIZE 256
#define TLB_SIZE 16
#define NUM_FRAME 128
#define FRAME_SIZE 256
#define PAGE 0
#define FRAME 1
// 因为Page Replace的时候没有对TLB表进行更新，这里方法不统一的话会有bug
#define TLBMethod "LRU"             // "FIFO" or "LRU"
#define PageReplacementMethod "LRU" // "FIFO" or "LRU"

#define BUFFER_SIZE 20

int TLB[TLB_SIZE][2];
time_t TLBCounter[TLB_SIZE];
int PageTable[PAGE_TABLE_SIZE];
time_t Counter[NUM_FRAME];
char address[BUFFER_SIZE];
char physicalMemory[NUM_FRAME][FRAME_SIZE];

int PageFaults = 0;
int TLBHits = 0;
int availableFrameNumber = 0;
int tlbInsertPosition = 0;
int pageReplacePosition = 0;
FILE *backing_store;

int findMinimun(time_t counter[], int size)
{
    int i;
    int minUsedTime = counter[0];
    int minIndex = 0;
    for (i = 0; i < size; i++)
    {
        if (counter[i] < minUsedTime)
        {
            minUsedTime = counter[i];
            minIndex = i;
        }
    }
    return minIndex;
}

int getPageFromTLB(int pageNumber)
{
    int i;
    for (i = 0; i < TLB_SIZE; i++)
    {
        if (TLB[i][PAGE] == pageNumber)
        {
            int frameNumber = TLB[i][FRAME];
            TLBHits++;
            return frameNumber;
        }
    }
    return -1; //没有找到返回-1
}

int getPageFromPageTable(int pageNumber)
{
    int frameNumber = PageTable[pageNumber];
    return frameNumber;
}

int TLBcheck(int pageNumber)
{
    int i;
    for (i = 0; i < TLB_SIZE; i++)
    {
        if (TLB[i][PAGE] == pageNumber)
        {
            return i;
        }
    }
    return -1;
}

int TLBUpdate(int pageNumber, int frameNumber)
{
    int i;
    int TLBposition = TLBcheck(pageNumber);
    if (TLBMethod == "FIFO")
    {
        if (TLBposition != -1)
        {
            return 0;
        }
        else
        {
            TLB[tlbInsertPosition][PAGE] = pageNumber;
            TLB[tlbInsertPosition][FRAME] = frameNumber;
            tlbInsertPosition = (tlbInsertPosition + 1) % TLB_SIZE;
        }
    }
    else if (TLBMethod == "LRU")
    {
        //每次引用时更新Counter
        if (TLBposition != -1)
        {
            TLBCounter[TLBposition] = time(NULL);
            // 基于stack的实现
            // if(tlbInsertPosition == TLB_SIZE - 1){
            //     for(i = TLBposition; i < TLB_SIZE-1; i++){
            //         TLB[i][PAGE] = TLB[i+1][PAGE];
            //         TLB[i][FRAME] = TLB[i+1][FRAME];
            //     }
            //     TLB[TLB_SIZE-1][PAGE] = pageNumber;
            //     TLB[TLB_SIZE-1][FRAME] = frameNumber;
            // }
            // else{
            //     for(i = TLBposition; i < tlbInsertPosition - 1; i++){
            //         TLB[i][PAGE] = TLB[i+1][PAGE];
            //         TLB[i][FRAME] = TLB[i+1][FRAME];
            //     }
            //     TLB[tlbInsertPosition -1][PAGE] = pageNumber;
            //     TLB[tlbInsertPosition-1][FRAME] = frameNumber;
            // }
            return 0;
        }
        else
        {
            if (tlbInsertPosition == TLB_SIZE - 1)
            {
                int replaceTLB = findMinimun(TLBCounter, TLB_SIZE);
                // int replaceTLB = 0;
                TLB[replaceTLB][PAGE] = pageNumber;
                TLB[replaceTLB][FRAME] = frameNumber;
            }
            else
            {
                TLB[tlbInsertPosition][PAGE] = pageNumber;
                TLB[tlbInsertPosition][FRAME] = frameNumber;
                TLBCounter[tlbInsertPosition] = time(NULL);
                tlbInsertPosition++;
            }
        }
    }
}

int getPageFromStore(int pageNumber)
{
    int i;

    fseek(backing_store, pageNumber * FRAME_SIZE, SEEK_SET);

    PageFaults++;

    if (availableFrameNumber == NUM_FRAME)
    { //表满
        if (PageReplacementMethod == "FIFO")
        {
            fread(physicalMemory[pageReplacePosition], sizeof(char), FRAME_SIZE, backing_store);

            // 更新PageTable
            for (i = 0; i < PAGE_TABLE_SIZE; i++)
            {
                if (PageTable[i] == pageReplacePosition)
                {
                    PageTable[i] = -1;
                }
            }
            PageTable[pageNumber] = pageReplacePosition;

            int frameNumber = pageReplacePosition;
            pageReplacePosition = (pageReplacePosition + 1) % NUM_FRAME;

            return frameNumber;
        }
        else if (PageReplacementMethod == "LRU")
        {
            int LRU = findMinimun(Counter, NUM_FRAME);
            for (i = 0; i < PAGE_TABLE_SIZE; i++)
            {
                if (PageTable[i] == LRU)
                {
                    PageTable[i] = -1;
                }
            }
            
            fread(physicalMemory[LRU], sizeof(char), FRAME_SIZE, backing_store);

            PageTable[pageNumber] = LRU;

            return LRU;
        }
    }
    else
    { //表没满直接放
        fread(physicalMemory[availableFrameNumber], sizeof(char), FRAME_SIZE, backing_store);

        PageTable[pageNumber] = availableFrameNumber;
        availableFrameNumber++;

        return availableFrameNumber - 1;
    }
}

void getPage(int la)
{
    int pageNumber = ((la >> 8) & 0xFF);
    int offset = (la & 0xFF);

    int frameNumber = getPageFromTLB(pageNumber);

    // TLB Miss
    if (frameNumber == -1)
    {
        frameNumber = getPageFromPageTable(pageNumber);
    }

    // PageFaults
    if (frameNumber == -1)
    {
        frameNumber = getPageFromStore(pageNumber);
    }

    TLBUpdate(pageNumber, frameNumber);
    Counter[frameNumber] = time(NULL);

    int value = physicalMemory[frameNumber][offset];
    printf("%d\n", value);
    // printf("Virtual address: %d Physical address: %d Value: %d\n", la,
    // frameNumber * FRAME_SIZE + offset, value);
}

int main(int argc, char *argv[])
{
    FILE *address_file = fopen(argv[2], "r");
    backing_store = fopen(argv[1], "rb");

    memset(PageTable, -1, sizeof(PageTable));
    memset(TLB, -1, sizeof(TLB));

    int addressNumber = 0;

    while (fgets(address, BUFFER_SIZE, address_file) != NULL)
    {
        int la = atoi(address);
        getPage(la);
        addressNumber++;
    }

    printf("Page Faults = %d\n", PageFaults);
    printf("TLB Hits = %d\n", TLBHits);
    printf("Page-fault rate = %f\n", PageFaults / (double)addressNumber);
    printf("TLB hit rate = %f\n", TLBHits / (double)addressNumber);

    fclose(address_file);
    fclose(backing_store);
}