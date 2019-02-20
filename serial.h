#ifndef SERIAL_H
#define	SERIAL_H

#include <xc.h> // include processor files - each processor file is guarded.


#ifdef	__cplusplus
extern "C" {
  #endif /* __cplusplus */

  void UARTSendString(char* c);
  void UARTNewLine(void);
  void interrupt_enable(void);
  void initialize_TX(void);
  void initialize_RX(void);
  void UARTSendChar(char c);
  char UARTRecieveChar(void);

  // TODO If C++ is being used, regular C code needs function names to have C
  // linkage so the functions can be used by the c code.

  #ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */
