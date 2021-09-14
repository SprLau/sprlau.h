#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>
#include<ctype.h>

#define Max(a,b)    a > b ? a : b
#define Min(a,b)    a < b ? a : b
#define MaxSize     1000
//qsort(a, 6, sizeof(*a), cmp);

typedef long long int LLI;

typedef struct T {
    int val;
    int depth;
    struct T* lc;
    struct T* rc;
    struct T* pr;
} TREE;

typedef struct chain {
    int value;
    struct chain* nxt;
    struct chain* pre;
} CHAIN;

typedef struct queue {
    int Queue[MaxSize];
    int frt;
    int rear;
} QUEUE;

typedef struct stack {
    int Stack[MaxSize];
    int cur;
} STACK;

void InsertSort(int *arr, int length) {
    for (int i = 1; i < length; i++) {
        int k = i - 1, cur = arr[i];
        while (arr[k] > cur && k > -1) {
            arr[k + 1] = arr[k];
            --k;
        }
        arr[k + 1] = cur;
    }
}

void ModifyRoot(int val, TREE* rt) {
    rt->val = val;
    rt->lc = rt->pr = rt->rc = NULL;
}

TREE* WhereTo(int val, TREE* now) {
    if (val > now->val) {
        if (now->rc) return WhereTo(val, now->rc);
        else return now;
    }
    else {
        if (now->lc) return WhereTo(val, now->lc);
        else return now;
    }

}

void AddTwig(int val, TREE* rt) {
    TREE* tem = WhereTo(val, rt);
    if (val > tem->val) {
        tem->rc = (TREE*)malloc(sizeof(TREE));
        tem->rc->pr = tem;
        tem->rc->val = val;
        tem->rc->lc = tem->rc->rc = NULL;
    }
    else {
        tem->lc = (TREE*)malloc(sizeof(TREE));
        tem->lc->pr = tem;
        tem->lc->val = val;
        tem->lc->lc = tem->lc->rc = NULL;
    }
}

void CreateTree(TREE* rt) {
    int value[1000];
    int all; printf("How Many: ");
    scanf("%d", &all);
    for (int i = 0; i < all; i++) {
        scanf("%d", &value[i]);
        if (i == 0) {
            ModifyRoot(value[i], rt);
            continue;
        }
        else AddTwig(value[i], rt);
    }
}

TREE* SearchTwig(int val, TREE* now) {
    if (now->val == val) return now;
    if (val > now->val) {
        if (now->rc) return SearchTwig(val, now->rc);
        else {
            puts("NULL!");
            return NULL;
        }
    }
    else {
        if (now->lc) return SearchTwig(val, now->lc);
        else {
            puts("NULL!");
            return NULL;
        }
    }
}

void DeleteTwig(TREE* bt, int key)
{
    TREE* L, * LL;
    TREE* p = bt;
    TREE* parent = bt;
    int child = 0;
    if (!bt)
        return;
    while (p) {
        if (p->val == key) {
            if (!p->lc && !p->rc) {
                if (p == bt)
                    free(p);
                else if (child == 0) {
                    parent->lc = NULL;
                    free(p);
                }
                else {
                    parent->rc = NULL;
                    free(p);
                }
            }

            else if (!p->lc) {
                if (child == 0)
                    parent->lc = p->rc;
                else
                    parent->rc = p->rc;
                free(p);
            }

            else if (!p->rc) {
                if (child == 0)
                    parent->lc = p->lc;
                else
                    parent->rc = p->lc;
                free(p);
            }

            else {
                LL = p;
                L = p->rc;
                if (L->lc) {
                    LL = L;
                    L = L->lc;
                    p->val = L->val;
                    LL->lc = L->lc;
                    for (; L->lc; L = L->lc);
                    L->lc = p->lc;
                    p->lc = NULL;
                }
                else {
                    p->val = L->val;
                    LL->rc = L->rc;
                }
            }
            p = NULL;
        }

        else if (key < p->val) {
            child = 0;
            parent = p;
            p = p->lc;
        }

        else {
            child = 1;
            parent = p;
            p = p->rc;
        }
    }
}

typedef struct LOPQ {
    TREE* numQ[MaxSize];
    int front;
    int rear;
} LOPQUEUE;

LOPQUEUE LOPqueue;

void IniLOPQueue() {
    LOPqueue.front = 0;
    LOPqueue.rear = 0;
}

void PushLOPQueue(TREE* rt) {
    LOPqueue.numQ[++LOPqueue.rear] = rt;
}

TREE* PopLOPQueue() {
    return LOPqueue.numQ[++LOPqueue.front];
}

int EmptyLOPQueue() {
    return LOPqueue.rear == LOPqueue.front;
}

void LayerOrderPrint(TREE* rt) {
    TREE* tem;
    PushLOPQueue(rt);
    while (!EmptyLOPQueue()) {
        tem = PopLOPQueue();
        printf("%d ", tem->val);
        if (tem->lc)
            PushLOPQueue(tem->lc);
        if (tem->rc)
            PushLOPQueue(tem->rc);
    }
    puts("");
}

void PreOrderPrint(TREE* now) /*root-l-r*/ {
    if (!now) return;
    printf("%d ", now->val);
    PreOrderPrint(now->lc);
    PreOrderPrint(now->rc);
}

void MidOrderPrint(TREE* now) /*l-root-r*/ {
    if (!now) return;
    MidOrderPrint(now->lc);
    printf("%d ", now->val);
    MidOrderPrint(now->rc);
}

void PosOrderPrint(TREE* now) /*l-r-root*/ {
    if (!now) return;
    PosOrderPrint(now->lc);
    PosOrderPrint(now->rc);
    printf("%d ", now->val);
}

CHAIN* PointAtNode(int seq, CHAIN* first) {
    CHAIN* nnow = first;
    for (int i = 1; i < seq; i++) {
        nnow = nnow->nxt;
    }
    return nnow;
}

void CreateFirstNode(int val, CHAIN* first) {
    first->pre = first->nxt = NULL;
    first->value = val;
}

void AppendNode(int val, CHAIN* first) {
    while (first->nxt) first = first->nxt;
    first->nxt = (CHAIN*)malloc(sizeof(CHAIN));
    first->nxt->pre = first;
    first->nxt->nxt = NULL;
    first->nxt->value = val;
}

void DeleteTail(CHAIN* first) {
    while (first->nxt) first = first->nxt;
    first->pre->nxt = NULL;
    free(first);
    first = NULL;
    return;
}

int ChainLength(CHAIN* first) {
    CHAIN* start = first;
    int len = 0;
    while (start) {
        ++len;
        start = start->nxt;
        if (start == first) break;
    }
    return len;
}

CHAIN* SearchNode(int val, CHAIN* first) {
    int length = ChainLength(first);
    int count = 0;
    while (first) {
        count++;
        if (count > 2 * length) break;
        if (first->value == val) {
            return first;
        }
        first = first->nxt;
    }
    return NULL;
}

void DeleteNode(int val, CHAIN* first) {
    CHAIN* tobe = SearchNode(val, first);
    tobe->pre->nxt = tobe->nxt;
    tobe->nxt->pre = tobe->pre;
    free(tobe); tobe = NULL;
}

void PrintChain(CHAIN* first, int jumpLine) {
    while (first) {
        printf("%d%c", first->value, first->nxt ? ' ' : '\0');
        first = first->nxt;
    }
    if (jumpLine) putchar('\n');
    return;
}

void DestroyChain(CHAIN* first) {
    if (!first) return;
    DestroyChain(first->nxt);
    free(first);
    return;
}

void ReverseChain(CHAIN* first) {
    CHAIN* last = first;
    int length = 1;
    while (last->nxt) {
        if (last->nxt == first) break;
        last = last->nxt;
        length++;
    }

    CHAIN* temFirst = first;
    CHAIN* temLast = last;

    for (int i = 0; i < length / 2; i++) {
        int temVal = temFirst->value;
        temFirst->value = temLast->value;
        temLast->value = temVal;
        temFirst = temFirst->nxt;
        temLast = temLast->pre;
    }
}

void CreateChain(CHAIN* first) {
    int length; printf("How Many: ");
    scanf("%d", &length);
    for (int i = 0; i < length; i++) {
        if (i == 0) {
            int t;
            scanf("%d", &t);
            CreateFirstNode(t, first);
        }
        else {
            int t;
            scanf("%d", &t);
            AppendNode(t, first);
        }
    }
}

void MakeChainLoop(CHAIN* first) {
    CHAIN* tem = first;
    while (tem->nxt) tem = tem->nxt;
    tem->nxt = first;
    first->pre = tem;
}

int cmpStr(const void* a, const void* b) {
    return strcmp((char*)a, (char*)b);
}

int cmpNum(const void* a, const void* b) {
    return ((*(int*)a) - (*(int*)b));
}

int cmpCha(const void* a, const void* b) {
    return ((int)(*(char*)a) - (int)(*(char*)b));
}

void IniQueue(QUEUE* a) {
    memset(a->Queue, 0, sizeof(a->Queue));
    a->frt = 0;
    a->rear = 0;
}

void PushQueue(QUEUE* a, int add) {
    a->Queue[++a->rear] = add;
}

int EmptyQueue(QUEUE* a) {
    return a->frt == a->rear;
}

int PopQueue(QUEUE* a) {
    if (EmptyQueue(a)) {
        puts("ERROR!");
        return 0;
    }
    return a->Queue[++a->frt];
}

void IniStack(STACK* a) {
    memset(a->Stack, 0, sizeof(a->Stack));
    a->cur = 0;
}

void PushStack(STACK* a, int add) {
    a->Stack[++a->cur] = add;
}

int EmptyStack(STACK* a) {
    return a->cur == 0;
}

int PopStack(STACK* a) {
    if (EmptyStack(a)) {
        puts("ERROR!");
        return 0;
    }
    return a->Stack[a->cur--];
}

int Combination(int up, int down) {
    int tem = 1;
    for (int i = 0; i < up; i++) {
        tem *= (down - i);
    }
    int tem2 = 1;
    for (int i = 1; i <= up; i++) {
        tem2 *= i;
    }
    return tem / tem2;
}

typedef struct StackB {
    char base[1024];
    int fur;
} STACKBASE;

STACKBASE* StackBase;

void IniStackBase() {
    StackBase = (STACKBASE*)malloc(sizeof(STACKBASE));
    StackBase->fur = 0;
}

void PushStackBase(int push) {
    StackBase->base[StackBase->fur] = push;
    StackBase->fur++;
}

char PopStackBase() {
    StackBase->fur--;
    char tem = StackBase->base[StackBase->fur];
    return tem;
}

char ITOA(int n) {
    return (char)('0' + n);
}

int ATOI(char a) {
    if (a >= '0' && a <= '9')
        return (int)(a - '0');
    else if (a >= 'a' && a <= 'f')
        return (int)(10 + (a - 'a'));
    else if (a >= 'A' && a <= 'F')
        return (int)(10 + (a - 'A'));
    else return -1;
}

void DecToBin(int num) {
    IniStackBase();
    while (num) {
        PushStackBase(ITOA(num % 2));
        num /= 2;
    }
    while (StackBase->fur != 0) {
        printf("%c", PopStackBase());
    }
}

void DecToHex(int num) {
    IniStackBase();
    while (num) {
        if (num % 16 > 9) {
            PushStackBase((char)(num % 16 - 10 + 'A'));
        }
        else
            PushStackBase(ITOA(num % 16));
        num /= 16;
    }
    while (StackBase->fur != 0) {
        printf("%c", PopStackBase());
    }
}

char* DecToBin_Return(int num) {
    if (num == 0) {
        static char res[65];
        memset(res, '\0', sizeof(res));
        strcpy(res, "0");
        return res;
    }
    IniStackBase();
    while (num) {
        PushStackBase(ITOA(num % 2));
        num /= 2;
    }
    static char res[65];
    memset(res, '\0', sizeof(res));
    int cur = 0;
    while (StackBase->fur != 0) {
        res[cur++] = PopStackBase();
    }
    return res;
}

char* DecToOct_Return(int num) {
    if (num == 0) {
        static char res[65];
        memset(res, '\0', sizeof(res));
        strcpy(res, "0");
        return res;
    }
    IniStackBase();
    while (num) {
        PushStackBase(ITOA(num % 8));
        num /= 8;
    }
    static char res[65];
    memset(res, '\0', sizeof(res));
    int cur = 0;
    while (StackBase->fur != 0) {
        res[cur++] = PopStackBase();
    }
    return res;
}

void BinToDec(const char bin[]) {
    int dec = 0, len = strlen(bin);
    for (int i = 0; i < len; i++) {
        dec += ((int)bin[i] - (int)'0') * (1 << (len - i - 1));
    }
    printf("%d", dec);
}

int BinToDec_Return(const char bin[]) {
    int dec = 0, len = strlen(bin);
    for (int i = 0; i < len; i++) {
        dec += ((int)bin[i] - (int)'0') * (1 << (len - i - 1));
    }
    return dec;
}

int HexToDec(const char hex[]) {
    int trans[500] = { 0 };
    for (int i = (int)'A'; i <= (int)'Z'; i++) {
        trans[i] = 10 + i - (int)'A';
    }
    for (int i = (int)'0'; i <= (int)'9'; i++) {
        trans[i] = i - (int)'0';
    }
    int dec = 0, len = strlen(hex);
    for (int i = 0; i < len; i++) {
        char tem = hex[i];
        if ('a' <= hex[i] && hex[i] <= 'f') tem += 'A' - 'a';
        dec += trans[(int)tem] * (1 << 4 * (len - 1 - i));
    }
    //printf("%d", dec);
    return dec;
}

void Trim_n(char str[]) {
    if (str[strlen(str) - 1] == '\n') str[strlen(str) - 1] = '\0';
}

void ExtractTextFile(FILE* in, char tar[10000][100]) {
    for (int i = 0; fgets(tar[i], 100, in); i++);
}

void NumberBits(char num_Format_b1234_or_B1234[]) {
    char num[1000];
    strcpy(num, num_Format_b1234_or_B1234);
    int len = strlen(num);
    int origin = 0;
    switch (num[0]) {
    case 'b':    case 'B':
        for (int i = 1; i < len; i++)   origin += (int)(ATOI(num[i]) * pow(2.0, (double)len - i - 1));
        break;
    case 'd':    case 'D':
        for (int i = 1; i < len; i++)   origin += (int)(ATOI(num[i]) * pow(10.0, (double)len - i - 1));
        break;
    case 'o':    case 'O':
        for (int i = 1; i < len; i++)   origin += (int)(ATOI(num[i]) * pow(8.0, (double)len - i - 1));
        break;
    case 'h':    case 'H':
        for (int i = 1; i < len; i++)   origin += (int)(ATOI(num[i]) * pow(16.0, (double)len - i - 1));
        break;
    }
    char bin[65];
    strcpy(bin, DecToBin_Return(origin));
    printf("Binary: %s\nBitWidth: %ld\n", bin, strlen(bin));
}

int FunctionTime = 0;
int resultHamilton = 0;
int JudgeHamilton(int n, int origin, int now, int index[]) {
    FunctionTime++;
    if (now == origin && FunctionTime > 1) {
        int flagErr = 0;
        for (int i = 1; i <= n; i++) {
            if (index[i] == 0)
                flagErr = 1;
        }
        if (flagErr == 0) {
            return 1;
        }
        else return 0;
    }

    if (index[now] == 1)
        return 0;
    else index[now] = 1;

    for (int i = 1; i <= n; i++) {
        if (i == now) continue;
        else {
            if (index[10 * now + i] == 1) {
                resultHamilton = resultHamilton || JudgeHamilton(n, origin, i, index);
            }
        }
    }

    index[now] = 0;

    return resultHamilton;
}

LLI Square(LLI number) {
    return number * number;
}
int count = 0;
LLI FastPower(LLI base, LLI power) {
    printf("%d\n", count);

    count++;
    if (power == 1)
        return base;
    else
        return power & 1 ? Square(FastPower(base, power >> 1)) * base : Square(FastPower(base, power >> 1));
}

int appear[10], list[10];

void AllArray(int n, int now) {
    if (now > n) {
        for (int ii = 1; ii <= n; ii++) {
            printf("%d ", list[ii]);
        }
        puts("");
        return;
    }
    for (int i = 1; i <= n; i++) {
        if (!appear[i]) {
            appear[i] = 1;
            list[now] = i;
            AllArray(n, now + 1);
            appear[i] = 0;
        }
    }
}

void Hanio(int big, int sml, char A, char B, char C) {
    if (big == sml) {
        printf("move %d from %c to %c\n", big, A, C);
        return;
    }
    else {
        Hanio(big - 1, sml, A, C, B);
        printf("move %d from %c to %c\n", big, A, C);
        Hanio(big - 1, sml, B, A, C);
    }
}

int Fibonacci(int seq) {
    return seq == 0 ? 0 : (seq == 1 ? 1 : (Fibonacci(seq - 1) + Fibonacci(seq - 2)));
}

void CodeTrans() {
    printf("Code: ");
    char code[100];
    memset(code, '\0', sizeof(code));
    fgets(code, 100, stdin);
    Trim_n(code);
    printf("Which type is this code:    1.True 2.Ones-Complement 3.Complement        Type: ");
    int ori_type;
    scanf("%d", &ori_type);
    printf("Select Type:                1.True 2.Ones-Complement 3.Complement        Type: ");
    int tar_type;
    scanf("%d", &tar_type);
    printf("Result: ");
    switch (ori_type) {
    case 1: {
        switch (tar_type) {
        case 1: {
            break;
        }
        case 2: {
            if (code[0] == '1') {
                for (int i = 1; i < strlen(code); i++) {
                    code[i] = code[i] == '1' ? '0' : '1';
                }
            }
            break;
        }
        case 3: {
            if (code[0] == '1') {
                for (int i = 1; i < strlen(code); i++) {
                    code[i] = code[i] == '1' ? '0' : '1';
                }
                int cur = strlen(code) - 1;
                code[cur] += 1;
                while (code[cur] == '2') {
                    code[cur] = '0';
                    cur -= 1;
                    code[cur] += 1;
                }
            }
            break;
        }
        }
        break;
    }
    case 2: {
        switch (tar_type) {
        case 1: {
            if (code[0] == '1') {
                for (int i = 1; i < strlen(code); i++) {
                    code[i] = code[i] == '1' ? '0' : '1';
                }
            }
            break;
        }
        case 2: {
            break;
        }
        case 3: {
            if (code[0] == '1') {
                int cur = strlen(code) - 1;
                code[cur] += 1;
                while (code[cur] == '2') {
                    code[cur] = '0';
                    cur -= 1;
                    code[cur] += 1;
                }
            }
            break;
        }
        }
        break;
    }
    case 3: {
        switch (tar_type) {
        case 1: {
            if (code[0] == '1') {
                int cur = strlen(code) - 1;
                code[cur] -= 1;
                while (code[cur] == '0' - 1) {
                    code[cur] = '1';
                    cur -= 1;
                    code[cur] -= 1;
                }
                for (int i = 1; i < strlen(code); i++) {
                    code[i] = code[i] == '1' ? '0' : '1';
                }

            }
            break;
        }
        case 2: {
            if (code[0] == '1') {
                int cur = strlen(code) - 1;
                code[cur] -= 1;
                while (code[cur] == '0' - 1) {
                    code[cur] = '1';
                    cur -= 1;
                    code[cur] -= 1;
                }
            }
            break;
        }
        case 3: {
            break;
        }
        }
        break;
    }
    }
    puts(code);
}