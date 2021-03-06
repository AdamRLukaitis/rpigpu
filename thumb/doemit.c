//-------------------------------------------------------------------------
// Assembly Tools
//
// This section contains macros/functions to provide simple assembler
// style functionality.  Main is executed to generate a VideoCore IV
// executable image to replace bootcode.bin.
//
//-------------------------------------------------------------------------
// this is derived from this tool
// https://github.com/hermanhermitage/videocoreiv/blob/master/tinyasm/asm.c
//-------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

static void assemble(void);

static unsigned int __pc__, __resized__, __pass__;
static FILE *       __target__;
static char *       __target_filename__ = "a.bin";

static int quit(char *message, int exitcode) {
  puts(message); exit((unsigned)exitcode); return 0;
}

int main(int argc, char *argv) {
  for(;;) {
    __pc__ = __resized__ = 0;
    assemble();
    __target__ && fclose(__target__) && quit("Assembly Failed: target file failed to close.", 1);
    __target__ && __resized__ && quit("Assembly Failed: binary changed sized during output pass.", 1);
    __target__ && quit("Assembly Succeeded.", 0);
    __resized__ && printf("Pass %d completed, but another pass is required.\n", ++__pass__);
    if (!__resized__) {
      printf("Pass %d completed, starting output pass.\n", ++__pass__);
      __target__ = fopen(__target_filename__, "wb");
    }
  }
}

// Target generation
#define emit(x)      do { if(__target__) fputc(x, __target__); __pc__++; } while(0)
//#define SHOWEMIT
#undef SHOWEMIT
#ifdef SHOWEMIT
#define emit1(x)     do { unsigned _ = x; printf("0x%04X\n",x); emit(_&0xff); emit((_>>8)&0xff); }  while(0)
#define emit2(x,y)   do { emit1(x); emit1(y); printf("\n"); } while(0)
#define emit3(x,y,z) do { emit1(x); emit1(y); emit1(z); printf("\n"); } while(0)
#else
#define emit1(x)     do { unsigned _ = x; emit(_&0xff); emit((_>>8)&0xff); }  while(0)
#define emit2(x,y)   do { emit1(x); emit1(y); } while(0)
#define emit3(x,y,z) do { emit1(x); emit1(y); emit1(z); } while(0)
#endif
#define fillb(size, value) do { int i; for(i=0; i<size; i+=2) emit1(value|(value<<8)); } while(0)
#define fillval(size, value) do { int i; for(i=0; i<size; i+=2) emit1(value); } while(0)
#define string(x)    do { char string[] = x; int i; for(i=0; i<sizeof string; i++) emit(string[i]); } while(0)

// Directives
#define equ(name, value)  unsigned int name = value
#define declare(name)     static int name
#define label(name)       do { __resized__ |= (name != __pc__); name = __pc__; } while(0)

// Sections

// Instructions

// Registers
int r0 = 0, r1 = 1, r2 = 2, r3 = 3, r4 = 4, r5 = 5, r6 = 6, r7 = 7,
    r8 = 8, r9 = 9, r10 = 10, r11 = 11, r12 = 12, r13 = 13, r14 = 14, r15 = 15,
    r16 = 16, r17 = 17, r18 = 18, r19 = 19, r20 = 20, r21 = 21, r22 = 22, r23 = 23,
    r24 = 24, r25 = 25, r26 = 26, r27 = 27, r28 = 28, r29 = 29, r30 = 30, r31 = 31,
    sp = 25, lr = 26, sr = 30, pc = 31
    ;

#define START(filename) void assemble(void) { __target_filename__ = filename;
#define END   }

