/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : RNG1.c
**     Project     : RF_Remote_Code
**     Processor   : MKL25Z64VLH4
**     Component   : RingBuffer
**     Version     : Component 01.039, Driver 01.00, CPU db: 3.00.000
**     Repository  : My Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-12-30, 18:32, # CodeGen: 61
**     Abstract    :
**         This component implements a ring buffer for different integer data type.
**     Settings    :
**          Component name                                 : RNG1
**          Reentrant                                      : yes
**          Critical Section                               : CS1
**          Buffer Size                                    : 50
**          Element Size                                   : 1 Byte
**     Contents    :
**         Clear           - void RNG1_Clear(void);
**         Put             - uint8_t RNG1_Put(RNG1_ElementType elem);
**         Get             - uint8_t RNG1_Get(RNG1_ElementType *elemP);
**         Peek            - uint8_t RNG1_Peek(RNG1_BufSizeType index, RNG1_ElementType *elemP);
**         NofElements     - RNG1_BufSizeType RNG1_NofElements(void);
**         NofFreeElements - RNG1_BufSizeType RNG1_NofFreeElements(void);
**         Deinit          - void RNG1_Deinit(void);
**         Init            - void RNG1_Init(void);
**
**     License   :  Open Source (LGPL)
**     Copyright : (c) Copyright Erich Styger, 2014, all rights reserved.
**     Web: http://www.mcuoneclipse.com
**     This an open source software of an embedded component for Processor Expert.
**     This is a free software and is opened for education,  research  and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/
/*!
** @file RNG1.c
** @version 01.00
** @brief
**         This component implements a ring buffer for different integer data type.
*/         
/*!
**  @addtogroup RNG1_module RNG1 module documentation
**  @{
*/         

/* MODULE RNG1. */

#include "RNG1.h"

#if RNG1_IS_REENTRANT
  #define RNG1_DEFINE_CRITICAL() CS1_CriticalVariable()
  #define RNG1_ENTER_CRITICAL()  CS1_EnterCritical()
  #define RNG1_EXIT_CRITICAL()   CS1_ExitCritical()
#else
  #define RNG1_DEFINE_CRITICAL() /* nothing */
  #define RNG1_ENTER_CRITICAL()  /* nothing */
  #define RNG1_EXIT_CRITICAL()   /* nothing */
#endif
static RNG1_ElementType RNG1_buffer[RNG1_BUF_SIZE]; /* ring buffer */
static RNG1_BufSizeType RNG1_inIdx;  /* input index */
static RNG1_BufSizeType RNG1_outIdx; /* output index */
static RNG1_BufSizeType RNG1_inSize; /* size data in buffer */
/*
** ===================================================================
**     Method      :  RNG1_Put (component RingBuffer)
**     Description :
**         Puts a new element into the buffer
**     Parameters  :
**         NAME            - DESCRIPTION
**         elem            - New element to be put into the buffer
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t RNG1_Put(RNG1_ElementType elem)
{
  uint8_t res = ERR_OK;
  RNG1_DEFINE_CRITICAL();

  RNG1_ENTER_CRITICAL();
  if (RNG1_inSize==RNG1_BUF_SIZE) {
    res = ERR_TXFULL;
  } else {
    RNG1_buffer[RNG1_inIdx] = elem;
    RNG1_inIdx++;
    if (RNG1_inIdx==RNG1_BUF_SIZE) {
      RNG1_inIdx = 0;
    }
    RNG1_inSize++;
  }
  RNG1_EXIT_CRITICAL();
  return res;
}

/*
** ===================================================================
**     Method      :  RNG1_Get (component RingBuffer)
**     Description :
**         Removes an element from the buffer
**     Parameters  :
**         NAME            - DESCRIPTION
**       * elemP           - Pointer to where to store the received
**                           element
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t RNG1_Get(RNG1_ElementType *elemP)
{
  uint8_t res = ERR_OK;
  RNG1_DEFINE_CRITICAL();

  RNG1_ENTER_CRITICAL();
  if (RNG1_inSize==0) {
    res = ERR_RXEMPTY;
  } else {
    *elemP = RNG1_buffer[RNG1_outIdx];
    RNG1_inSize--;
    RNG1_outIdx++;
    if (RNG1_outIdx==RNG1_BUF_SIZE) {
      RNG1_outIdx = 0;
    }
  }
  RNG1_EXIT_CRITICAL();
  return res;
}

/*
** ===================================================================
**     Method      :  RNG1_NofElements (component RingBuffer)
**     Description :
**         Returns the actual number of elements in the buffer.
**     Parameters  : None
**     Returns     :
**         ---             - Number of elements in the buffer.
** ===================================================================
*/
RNG1_BufSizeType RNG1_NofElements(void)
{
  return RNG1_inSize;
}

/*
** ===================================================================
**     Method      :  RNG1_NofFreeElements (component RingBuffer)
**     Description :
**         Returns the actual number of free elements/space in the
**         buffer.
**     Parameters  : None
**     Returns     :
**         ---             - Number of elements in the buffer.
** ===================================================================
*/
RNG1_BufSizeType RNG1_NofFreeElements(void)
{
  return (RNG1_BufSizeType)(RNG1_BUF_SIZE-RNG1_inSize);
}

/*
** ===================================================================
**     Method      :  RNG1_Init (component RingBuffer)
**     Description :
**         Initializes the data structure
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void RNG1_Init(void)
{
  RNG1_inIdx = 0;
  RNG1_outIdx = 0;
  RNG1_inSize = 0;
}

/*
** ===================================================================
**     Method      :  RNG1_Clear (component RingBuffer)
**     Description :
**         Clear (empty) the ring buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void RNG1_Clear(void)
{
  RNG1_DEFINE_CRITICAL();

  RNG1_ENTER_CRITICAL();
  RNG1_Init();
  RNG1_EXIT_CRITICAL();
}

/*
** ===================================================================
**     Method      :  RNG1_Peek (component RingBuffer)
**     Description :
**         Returns an element of the buffer without removiing it.
**     Parameters  :
**         NAME            - DESCRIPTION
**         index           - Index of element. 0 peeks the top
**                           element, 1 the next, and so on.
**       * elemP           - Pointer to where to store the received
**                           element
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t RNG1_Peek(RNG1_BufSizeType index, RNG1_ElementType *elemP)
{
  uint8_t res = ERR_OK;
  int idx; /* index inside ring buffer */
  RNG1_DEFINE_CRITICAL();

  RNG1_ENTER_CRITICAL();
  if (index>=RNG1_BUF_SIZE) {
    res = ERR_OVERFLOW; /* asking for an element outside of ring buffer size */
  } else if (index<RNG1_inSize) {
    idx = (RNG1_outIdx+index)%RNG1_BUF_SIZE;
    *elemP = RNG1_buffer[idx];
  } else { /* asking for an element which does not exist */
    res = ERR_RXEMPTY;
  }
  RNG1_EXIT_CRITICAL();
  return res;
}

/*
** ===================================================================
**     Method      :  RNG1_Deinit (component RingBuffer)
**     Description :
**         Driver de-initialization
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/**
void RNG1_Deinit(void)
{
  ** Function is implemented as macro in the header file
}
*/
/* END RNG1. */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
