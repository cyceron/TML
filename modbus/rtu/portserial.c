/*
  * FreeModbus Libary: LPC214X Port
  * Copyright (C) 2007 Tiago Prado Lone <tiago@maxwellbohr.com.br>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2007/04/24 23:15:18 wolti Exp $
 */

#include <LPC214X.h>
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Modbus Driver-------------------------------------*/
#define STR_RS 	28
#define SET_TXD() FIO0PIN |= (1<<STR_RS)
#define CLR_TXD() FIO0PIN &=~(1<<STR_RS)
/* ----------------------- static functions ---------------------------------*/

static void
sio_irq( void ) __attribute__ ((interrupt("IRQ")));
     static void     prvvUARTTxReadyISR( void );
     static void     prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/
     void            vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    if( xRxEnable )
    {
		CLR_TXD();
        U0IER |= 0x01;
    }
    else
    {
		SET_TXD();
        U0IER &= ~0x01;
    }
    if( xTxEnable )
    {
		SET_TXD();
        U0IER |= 0x02;
        prvvUARTTxReadyISR(  );
    }
    else
    {
		CLR_TXD();
        U0IER &= ~0x02;
    }
}

void
vMBPortClose( void )
{
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    BOOL            bInitialized = TRUE;
    USHORT          cfg = 0;
    ULONG           reload = ( ( PCLK / ulBaudRate ) / 16UL );
    volatile char   dummy;
	FIO0DIR |= (1<<STR_RS);
	SET_TXD();
    ( void )ucPORT;
    /* Configure UART1 Pins */
    PINSEL0 = 0x00000005;       /* Enable RxD1 and TxD1 */

    switch ( ucDataBits )
    {
    case 5:
        break;

    case 6:
        cfg |= 0x00000001;
        break;

    case 7:
        cfg |= 0x00000002;
        break;

    case 8:
        cfg |= 0x00000003;
        break;

    default:
        bInitialized = FALSE;
    }

    switch ( eParity )
    {
    case MB_PAR_NONE:
        break;

    case MB_PAR_ODD:
        cfg |= 0x00000008;
        break;

    case MB_PAR_EVEN:
        cfg |= 0x00000018;
        break;
    }

    if( bInitialized )
    {
        U0LCR = cfg;            /* Configure Data Bits and Parity */
        U0IER = 0;              /* Disable UART1 Interrupts */

        U0LCR |= 0x80;          /* Set DLAB */
        U0DLL = reload;         /* Set Baud     */
        U0DLM = reload >> 8;    /* Set Baud */
        U0LCR &= ~0x80;         /* Clear DLAB */

        /* Configure UART1 Interrupt */
        VICVectAddr0 = ( unsigned long )sio_irq;
        VICVectCntl0 = 0x20 | 6;
        VICIntEnable = 1 << 6;  /* Enable UART1 Interrupt */

        dummy = U0IIR;          /* Required to Get Interrupts Started */
    }

    return bInitialized;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    U0THR = ucByte;

    /* Wait till U0THR and U0TSR are both empty */
    while( !( U0LSR & 0x20 ) )
    {
    }

    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    while( !( U0LSR & 0x01 ) )
    {
    }

    /* Receive Byte */
    *pucByte = U0RBR;

    return TRUE;
}


void
sio_irq( void )
{
    volatile char   dummy;
    volatile char   IIR;

    while( ( ( IIR = U0IIR ) & 0x01 ) == 0 )
    {
        switch ( IIR & 0x0E )
        {
        case 0x06:             /* Receive Line Status */
            dummy = U0LSR;      /* Just clear the interrupt source */
            break;

        case 0x04:             /* Receive Data Available */
        case 0x0C:             /* Character Time-Out */
			{ 
            prvvUARTRxISR(  );
            break;
			}

        case 0x02:             /* THRE Interrupt */
		{
            prvvUARTTxReadyISR(  );
            break;
		}

        case 0x00:             /* Modem Interrupt */
            dummy = U0MSR;      /* Just clear the interrupt source */
            break;

        default:
            break;
        }
    }

    VICVectAddr = 0xFF;         /* Acknowledge Interrupt */
}


/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void
prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void
prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}
