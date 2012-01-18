/*
 * nsfSnappy.c -- Snappy meets NSF
 *
 * Victor Guerra ( vguerra@gmail.com )
 * 20111128
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <nsf.h>
#include <snappy-c.h>

/*********************************************************************** 
 * The following definitions should not be here, but they are included
 * to get compilation going for the time being.
 ***********************************************************************/

#if defined(__GNUC__) && __GNUC__ > 2
/* Use gcc branch prediction hint to minimize cost of e.g. DTrace
 * ENABLED checks.
 */
#  define unlikely(x) (__builtin_expect((x), 0))
#  define likely(x) (__builtin_expect((x), 1))
#else
#  define unlikely(x) (x)
#  define likely(x) (x)
#endif

typedef void *NsfObject;

#define PARSE_CONTEXT_PREALLOC 20
typedef struct {
  ClientData *clientData;
  int status;
  Tcl_Obj **objv;
  Tcl_Obj **full_objv;
  int *flags;
  ClientData clientData_static[PARSE_CONTEXT_PREALLOC];
  Tcl_Obj *objv_static[PARSE_CONTEXT_PREALLOC+1];
  int flags_static[PARSE_CONTEXT_PREALLOC+1];
  int lastobjc;
  int objc;
  int varArgs;
  NsfObject *object;
} ParseContext;

#define NSF_ARG_REQUIRED        0x000001

#define nr_elements(arr)  ((int) (sizeof(arr) / sizeof(arr[0])))
#define ObjStr(obj) (obj)->bytes ? (obj)->bytes : Tcl_GetString(obj)
#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif


#if defined(HAVE_STDINT_H)
# define HAVE_INTPTR_T 
# define HAVE_UINTPTR_T 
#endif

#if !defined(INT2PTR) && !defined(PTR2INT)
#   if defined(HAVE_INTPTR_T) || defined(intptr_t)
#       define INT2PTR(p) ((void *)(intptr_t)(p))
#       define PTR2INT(p) ((int)(intptr_t)(p))
#   else
#       define INT2PTR(p) ((void *)(p))
#       define PTR2INT(p) ((int)(p))
#   endif
#endif
#if !defined(UINT2PTR) && !defined(PTR2UINT)
#   if defined(HAVE_UINTPTR_T) || defined(uintptr_t)
#       define UINT2PTR(p) ((void *)(uintptr_t)(p))
#       define PTR2UINT(p) ((unsigned int)(uintptr_t)(p))
#   else
#       define UINT2PTR(p) ((void *)(p))
#       define PTR2UINT(p) ((unsigned int)(p))
#   endif
#endif

static int NsfsnappyCompress(Tcl_Interp *interp, Tcl_Obj *input);
static int NsfsnappyDecompress(Tcl_Interp *interp, Tcl_Obj *input);
static int NsfsnappyValidate(Tcl_Interp *interp, Tcl_Obj *input);
EXTERN int Nsfsnappy_Init(Tcl_Interp *interp);
EXTERN int Nsfsnappy_SafeInit(Tcl_Interp *interp);
static void Nsfsnappy_Exit(ClientData clientData);
