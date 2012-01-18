
/*
 * nsfSnappy.c -- Snappy meets NSF
 *
 * Victor Guerra ( vguerra@gmail.com )
 * 20111128
 */

#include "nsfSnappy.h"


static int ArgumentParse(Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[],
                         NsfObject *obj, Tcl_Obj *procName,
                         Nsf_Param CONST *paramPtr, int nrParameters, int serial,
                         int doCheck, ParseContext *pc) {
  return Nsf_ArgumentParse(interp, objc, objv, (Nsf_Object *)obj,
			   procName, paramPtr, nrParameters, serial,
                           doCheck, (Nsf_ParseContext *)pc);
}

#include "nsfSnappyAPI.h"

/*
 * cmd compress snappyCompress {
 *   {-argName "input" -required 1 -type tclobj}
 * }
 */

int
NsfsnappyCompress(Tcl_Interp *interp, Tcl_Obj *input) {
  size_t max_compressed_length;
  int input_length,i;
  char *compressed;

  //char *raw_input = Tcl_GetUnicodeFromObj(input,&input_length);
  char *raw_input = Tcl_GetStringFromObj(input,&input_length);
  
  /*  for (i=0; i < input_length; i++) {
      printf("char: %c\n", *(raw_input + i));
    }*/
  
  max_compressed_length = snappy_max_compressed_length(input_length);
  compressed = (char *) malloc(max_compressed_length);
  if (snappy_compress((char *)raw_input, input_length, compressed, 
		      &max_compressed_length) != SNAPPY_OK) {
    return NsfPrintError(interp, "Error while compressing data");
  } 

  Tcl_SetObjResult(interp, Tcl_NewByteArrayObj((uint8_t*)compressed, max_compressed_length));
  //Tcl_SetObjResult(interp, Tcl_NewStringObj(compressed, max_compressed_length));
  //Tcl_SetResult(interp,compressed,TCL_VOLATILE);
  /*for (i=0; i<=max_compressed_length; i++) {
    printf("char: %d\n", *compressed + i);
    }*/
  free(compressed);
  return TCL_OK;
}

/* cmd uncompress snappyUncompress { 
 *   {-argName "query" -required 1 -type tclobj} 
 * } 
 */

int 
NsfsnappyDecompress(Tcl_Interp *interp, Tcl_Obj *input) {
  size_t max_uncompressed_length;
  int input_length;
  unsigned char *compressed_input = Tcl_GetByteArrayFromObj(input,&input_length);

  if (snappy_uncompressed_length(compressed_input, input_length, 
				 &max_uncompressed_length) != SNAPPY_OK) {
    /* we have to report an error right here */
    return NsfPrintError(interp, "Error when parsing compressed data");
  }
  char *output = (char *)malloc(max_uncompressed_length);
  if (snappy_uncompress(compressed_input, input_length, output, 
			&max_uncompressed_length) != SNAPPY_OK) {
    /* we set the variable on the interpreter */
    return NsfPrintError(interp, "Error while uncompressing data");
  }
  Tcl_SetObjResult(interp, Tcl_NewStringObj(output, max_uncompressed_length));
  //Tcl_SetObjResult(interp, Tcl_NewByteArrayObj((uint8_t*)output, max_uncompressed_length));
  //Tcl_SetObjResult(interp, Tcl_NewUnicodeObj(output, max_uncompressed_length));

  free(output);
  return TCL_OK;
}

/* cmd validate Nsfsnappy_Validate { 
 *   {-argName "query" -required 1} 
 * } 
 */

int 
NsfsnappyValidate(Tcl_Interp *interp, Tcl_Obj *input) {
  int input_length;
  int resultValue = 0;
  
  //unsigned char *compressed = Tcl_GetUnicodeFromObj(input,&input_length);
  unsigned char *compressed = Tcl_GetByteArrayFromObj(input,&input_length);

  if (snappy_validate_compressed_buffer(compressed, input_length) == SNAPPY_OK) { 
    resultValue = 1; 
  } 
  Tcl_SetObjResult(interp, Tcl_NewIntObj(resultValue));
  return TCL_OK;
}


void
Nsfsnappy_Exit(ClientData clientData) {
  fprintf(stderr, "Snappy exits\n");
}

int
Nsfsnappy_Init(Tcl_Interp *interp) {
  int i;

#ifdef USE_TCL_STUBS
  if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL) {
    return TCL_ERROR;
  }

# ifdef USE_NSF_STUBS
  if (Nsf_InitStubs(interp, "1.1", 0) == NULL) {
    return TCL_ERROR;
  }
# endif
#else
  if (Tcl_PkgRequire(interp, "Tcl", TCL_VERSION, 0) == NULL) {
    return TCL_ERROR;
  }
#endif
  Tcl_PkgProvide(interp, "nsf::snappy", PACKAGE_VERSION);

#ifdef PACKAGE_REQUIRE_FROM_SLAVE_INTERP_WORKS_NOW
  if (Tcl_PkgRequire(interp, "nsf", XOTCLVERSION, 0) == NULL) {
    return TCL_ERROR;
  }
#endif

  Tcl_CreateExitHandler(Nsfsnappy_Exit, interp);

  /* create all method commands (will use the namespaces above) */
  for (i=0; i < nr_elements(method_definitions)-1; i++) {
    Tcl_CreateObjCommand(interp, method_definitions[i].methodName, method_definitions[i].proc, 0, 0);
  }

  Tcl_SetIntObj(Tcl_GetObjResult(interp), 1);
  return TCL_OK;
}

int
Nsfsnappy_SafeInit(interp)
     Tcl_Interp *interp;
{
  return Nsfsnappy_Init(interp);
}
