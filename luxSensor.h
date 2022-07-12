enum led{
    plus = 1,
    minus = 0
};

typedef struct ret{
    int op_symbol;
    long arr[3];
    int limit;
}ret_t;

void *measureLux(void* arg);