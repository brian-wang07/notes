#define FLAG 1
#if defined(FLAG)
int f(){return 1;}
#else
int f(){return 2;}
#endif
#undef FLAG
