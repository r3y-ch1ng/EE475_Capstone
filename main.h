#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
  #endif /* __cplusplus */

  int address = 0;
  void read_SRAM(int address);
  void write_SRAM(void);
  void measureTemp(void);

  // TODO If C++ is being used, regular C code needs function names to have C
  // linkage so the functions can be used by the c code.

  #ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */
