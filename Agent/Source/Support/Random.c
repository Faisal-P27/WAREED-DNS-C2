



#define	TYPE_0		0
#define	BREAK_0		8
#define	DEG_0		0
#define	SEP_0		0
/* x**7 + x**3 + 1.  */
#define	TYPE_1		1
#define	BREAK_1		32
#define	DEG_1		7
#define	SEP_1		3
/* x**15 + x + 1.  */
#define	TYPE_2		2
#define	BREAK_2		64
#define	DEG_2		15
#define	SEP_2		1
/* x**31 + x**3 + 1.  */
#define	TYPE_3		3
#define	BREAK_3		128
#define	DEG_3		31
#define	SEP_3		3
/* x**63 + x + 1.  */
#define	TYPE_4		4
#define	BREAK_4		256
#define	DEG_4		63
#define	SEP_4		1
#define	MAX_TYPES	5	/* Max number of types above.  */
#define	ULONG_MAX  ((unsigned long)(~0L))     /* 0xFFFFFFFF for 32-bits */
#define	LONG_MAX   ((long)(ULONG_MAX >> 1))   /* 0x7FFFFFFF for 32-bits*/
static int degrees[MAX_TYPES] = { DEG_0, DEG_1, DEG_2, DEG_3, DEG_4 };
static int seps[MAX_TYPES] = { SEP_0, SEP_1, SEP_2, SEP_3, SEP_4 };
static long int randtbl[DEG_3 + 1] =
  { TYPE_3,
      0x9a319039, 0x32d9c024, 0x9b663182, 0x5da1f342, 
      0xde3b81e0, 0xdf0a6fb5, 0xf103bc02, 0x48f340fb, 
      0x7449e56b, 0xbeb1dbb0, 0xab5c5918, 0x946554fd, 
      0x8c2e680f, 0xeb3d799f, 0xb11ee0b7, 0x2d436b86, 
      0xda672e2a, 0x1588ca88, 0xe369735d, 0x904f35f7, 
      0xd7158fd6, 0x6fa6f051, 0x616e6b96, 0xac94efdc, 
      0x36413f93, 0xc622c298, 0xf5a42ab8, 0x8a88d77b, 
      0xf5ad9d0e, 0x8999220b, 0x27fb47b9
    };

static long int *fptr = &randtbl[SEP_3 + 1];
static long int *rptr = &randtbl[1];
static long int *state = &randtbl[1];
static int rand_type = TYPE_3;
static int rand_deg = DEG_3;
static int rand_sep = SEP_3;
static long int *end_ptr = &randtbl[sizeof(randtbl) / sizeof(randtbl[0])];

long int Random (void)
{
  if (rand_type == TYPE_0)
    {
      state[0] = ((state[0] * 1103515245) + 12345) & LONG_MAX;
      return state[0];
    }
  else
    {
      long int i;
      *fptr += *rptr;
      /* Chucking least random bit.  */
      i = (*fptr >> 1) & LONG_MAX;
      ++fptr;
      if (fptr >= end_ptr)
	{
	  fptr = state;
	  ++rptr;
	}
      else
	{
	  ++rptr;
	  if (rptr >= end_ptr)
	    rptr = state;
	}
      return i;
    }
}


void Srandom (unsigned int x)
{
  state[0] = x;
  if (rand_type != TYPE_0)
    {
      register long int i;
      for (i = 1; i < rand_deg; ++i)
	state[i] = (1103515145 * state[i - 1]) + 12345;
      fptr = &state[rand_sep];
      rptr = &state[0];
      for (i = 0; i < 10 * rand_deg; ++i)
	Random();
    }
}