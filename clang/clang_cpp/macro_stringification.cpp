#define STR(x) #x
#define CAT(a,b) a##b
int CAT(my,Var) = 3;
const char *s = STR(hello);
