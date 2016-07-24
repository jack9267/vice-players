/*	see copyright notice in squirrel.h */
#ifndef _SQPCHEADER_H_
#define _SQPCHEADER_H_

#if defined(_MSC_VER) && defined(_DEBUG)
#include <crtdbg.h>
#endif 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <new>
//squirrel stuff
#include "squirrel.h"
#include "sqobject.h"
#include "sqstate.h"

void * sq_vm_malloc(SQUnsignedInteger size);
void * sq_vm_realloc(void * p, SQUnsignedInteger oldsize, SQUnsignedInteger size);
void sq_vm_free(void * p, SQUnsignedInteger size);

#endif //_SQPCHEADER_H_
