#define OPSTR       "Test and set Lock"
volatile int lock_var = 0;
#define INC(g)      {                                                                           \
                        while(InterlockedExchange(&lock_var, 1) == 1);                          \
                    }
#define RELEASE()   lock_var = 0



#define OPSTR       "Test, Test and Set Lock"
volatile int lock_var = 0;
#define ACCUIRE()   {                                                                           \
                        do {                                                                    \
                            while(lock_var == 1) {                                              \
                                _mm_pause();                                                    \
                            }                                                                   \
                        } while(InterlockedCompareExchange(&lock_var, 1, 0) == 1);              \
                    }
#define RELEASE()   lock_var = 0





#define OPSTR       "Bakery Lock"
int ncpus = getNumberOfCPUs();
int* choosing = new int [ncpus * 2];
int* number = new int [ncpus * 2];
#define INC(g)      {                                                                           \
                        choosing[thread] = 1;                                                   \
                        _mm_mfence();                                                           \
                        int max = 0;                                                            \
                        for (int i = 0; i < (ncpus * 2); i++) {                                 \
                            if (number[i] > max) {                                              \
                                max = number[i];                                                \
                            }                                                                   \
                                                                                                \
                        }                                                                       \
                        number[thread] = max + 1;                                               \
                        _mm_mfence();                                                           \
                        choosing[thread] = 0;                                                   \
                                                                                                \
                        for (int j = 0; j < (ncpus * 2); j++) {                                 \
                            while (choosing[j]);                                                \
                            while ((number[j] != 0) && ((number[j] < number[thread]) ||         \
                                ((number[j] == number[thread]) && (j < thread))));              \
                        }                                                                       \
                        (*g)++;                                                                 \
                        number[thread] = 0;                                                     \
                    }

#define INITBAK() {                                                                             \
    for(int a = 0;a < (getNumberOfCPUs() * 2);a++) {                                            \
        choosing[a] = 0;                                                                        \
        number[a] = 0;                                                                          \
    }                                                                                           \
}
