/*
 *  alloc.c  --  wrappers around malloc() / realloc() / free()
 *
 *  Copyright (C) 1999-2000,2016 by Massimiliano Ghilardi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */
#ifndef _TWIN_ALLOC_H
#define _TWIN_ALLOC_H

#include <Tw/datatypes.h>

extern udat ErrNo;
extern CONST char *ErrStr;
byte Error(udat ErrNo);

/* memory allocation. these function call Error(NOMEMORY) on failure */

void *AllocMem(size_t Size);              /* wrapper for malloc() */
void *ReAllocMem(void *Mem, size_t Size); /* wrapper for realloc() */
#define FreeMem free

void *AllocMem0(size_t ElementSize, size_t Count); /* wrapper for calloc() */
void *ReAllocMem0(void *Mem, size_t ElementSize, size_t OldCount,
                  size_t NewCount); /* wrapper for realloc() + memset() */

/* INLINE/define stuff: */

#define CopyMem(From, To, Size) memcpy(To, From, Size)
#define MoveMem(From, To, Size) memmove(To, From, Size)

void *CloneMem(CONST void *From, uldat Size);
char *CloneStr(CONST char *s);
char *CloneStrL(CONST char *s, uldat len);
char **CloneStrList(char **s);
trune *CloneStr2TRune(CONST char *s, uldat len);

#endif /* _TWIN_ALLOC_H */
