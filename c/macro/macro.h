#define lua_assert(x) ((void) 0)
#define cast(t, exp) ((t) (exp))
#define check_exp(c, e) (lua_assert(c), e)
#define lmod(s, size) (check_exp((size & (size - 1)) == 0, (cast(int, (s) & ((size) - 1)))))

