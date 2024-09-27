#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <ctype.h>
#define LINECHARNUM 6
#define MAX_SIZE 50000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct priorityQueue
{
    int heap[MAX_SIZE];
    int count;
} priorityQueue;

typedef struct Data
{
    char *patternString;
    int threadCount;
    char *inputFileName;
    char *outputFileName;
    int counter;
    priorityQueue root;
} Data;

void nodeChange(int *a, int *b);
void push(priorityQueue *root, int data);
int pop(priorityQueue *root);

void initData(Data *d, char *argv[]);
void makeThread(Data *d);
void writeData(Data *data);
void *findSequence(void *);
char *ltrim(char *s);
char *DeleteEnter(char s[]);

int main(int argc, char *argv[])
{
    Data data;
    if (argc != 5)
    {
        perror("필요한 환경 인자의 개수가 부족합니다.");
        exit(EXIT_FAILURE);
    }
    if (atoi(argv[2]) < 1)
    {
        perror("쓰레드의 개수는 1이상이여야 합니다.");
        exit(EXIT_FAILURE);
    }
    initData(&data, argv);
    makeThread(&data);
    writeData(&data);
    return 0;
}
void initData(Data *data, char *argv[])
{
    data->patternString = argv[1];
    data->threadCount = atoi(argv[2]);
    data->inputFileName = argv[3];
    data->outputFileName = argv[4];
    data->root.count = 0;
}
void makeThread(Data *data)
{
    Data d;
    d.threadCount = data->threadCount;
    d.patternString = data->patternString;
    d.inputFileName = data->inputFileName;
    d.outputFileName = data->outputFileName;
    d.counter = 1;
    d.root = data->root;
    d.root.count = data->root.count;

    pthread_t tid[data->threadCount];
    int status;
    void *thread_result;
    for (int i = 0; i < d.threadCount; i++)
    {
        status = pthread_create(&tid[i], NULL, findSequence, (void *)&d);
        if (status != 0)
        {
            perror("쓰레드 생성 오류");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < d.threadCount; i++)
    {
        status = pthread_join(tid[i], &thread_result);
        if (status != 0)
        {
            perror("쓰레드 join 오류");
            exit(EXIT_FAILURE);
        }
    }

    data->root = d.root;
}
void *findSequence(void *data)
{
    Data *d = (Data *)data;
    char firstLineNumber[LINECHARNUM];
    int fdRead = open(d->inputFileName, O_RDONLY);
    read(fdRead, firstLineNumber, LINECHARNUM);
    int totalLine = atoi(ltrim(firstLineNumber));

    lseek(fdRead, 6 * (totalLine), SEEK_SET);
    char remainBuf[LINECHARNUM];
    ssize_t charRemain = read(fdRead, remainBuf, LINECHARNUM);
    int totalChar = 6 * totalLine + charRemain;

    lseek(fdRead, 6, SEEK_SET);
    int perThreadNumber = (totalChar - LINECHARNUM) / d->threadCount;
    int perThreadLoopCount = (totalChar - LINECHARNUM) / d->threadCount;
    int perThreadNumberRemain = (totalChar - LINECHARNUM) % d->threadCount;
    int counting;
    if (d->counter < d->threadCount)
    {
        counting = ++(d->counter) - 1;
    }

    if ((counting + 1 == d->threadCount))
    {
        perThreadNumber += perThreadNumberRemain;
    }
    char perThreadChar[strlen(d->patternString) + strlen(d->patternString) / 6 + 2];
    char *bufp = perThreadChar;
    int match_Index;

    for (int i = 0; i < perThreadNumber; i++)
    {
        lseek(fdRead, i + LINECHARNUM + counting * perThreadLoopCount, SEEK_SET);
        read(fdRead, bufp, strlen(d->patternString) + strlen(d->patternString) / 5 + 1);

        if (d->patternString[0] == bufp[0])
        {
            bufp[strlen(d->patternString) + strlen(d->patternString) / 6 + 1] = '\0';
            bufp = DeleteEnter(bufp);
            bufp[strlen(d->patternString)] = '\0';
            
            int k = 0;
            for (match_Index = 1; match_Index < strlen(d->patternString); match_Index++)
            {
                if (d->patternString[match_Index] != bufp[match_Index])
                {
                    k = 1;
                    break;
                }
                if (match_Index == strlen(d->patternString) - 1)
                {
                    bufp[strlen(d->patternString)] = '\0';
                    break;
                }
            }
            if (!k)
            {
                int offSet = i + LINECHARNUM + counting * perThreadLoopCount - 6;
                int line = offSet / 6;
                int index = offSet - line;
                pthread_mutex_lock(&mutex);
                push(&d->root, index);
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    close(fdRead);
    return data;
}

void writeData(Data *data)
{
    int fdWrite = open(data->outputFileName, O_WRONLY | O_APPEND | O_TRUNC);
    while (data->root.count)
    {
        char buffer[LINECHARNUM];
        sprintf(buffer, "%d", pop(&data->root));
        write(fdWrite, buffer, strlen(buffer));
        if(data->root.count!=0){
            write(fdWrite, "\n", 1);
        }
    }
    close(fdWrite);
}
// 문자열 좌측 공백문자 삭제 함수
char *ltrim(char *s)
{
    char *begin;
    begin = s;

    while (*begin != '\0')
    {
        if (isspace(*begin))
            begin++;
        else
        {
            s = begin;
            break;
        }
    }

    return s;
}
char *DeleteEnter(char s[])
{
    char *str = s;
    int i, k = 0;

    for (i = 0; i < strlen(s); i++)
        if (s[i] != '\n')
            str[k++] = s[i];

    str[k] = '\0';
    return str;
}

void nodeChange(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void push(priorityQueue *root, int data)
{
    if (root->count >= MAX_SIZE)
        return;

    root->heap[root->count] = data;
    int now = root->count;

    int parent = (root->count - 1) / 2;
    while (now > 0 && root->heap[now] < root->heap[parent])
    {
        nodeChange(&root->heap[now], &root->heap[parent]);
        now = parent;
        parent = (parent - 1) / 2;
    }

    root->count++;
}

int pop(priorityQueue *root)
{
    if (root->count <= 0)
        return -9999;

    int res = root->heap[0];

    root->count--;

    root->heap[0] = root->heap[root->count];

    int now = 0, leftChild = 1, rightChild = 2;

    int target = now;

    while (leftChild < root->count)
    {
        if (root->heap[target] > root->heap[leftChild])
            target = leftChild;

        if (root->heap[target] > root->heap[rightChild] && rightChild < root->count)
            target = rightChild;

        if (target == now)
            break;
        else
        {
            nodeChange(&root->heap[now], &root->heap[target]);
            now = target;
            leftChild = now * 2 + 1;
            rightChild = now * 2 + 2;
        }
    }
    return res;
}