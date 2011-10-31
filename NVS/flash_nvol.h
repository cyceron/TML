/**************************************************************************
MODULE:       FLASH_NVOL
CONTAINS:     Storage of non-volatile variables in Flash memory
DEVELOPED BY: Embedded Systems Academy, Inc. 2010
              www.esacademy.com
COPYRIGHT:    NXP Semiconductors, 2010. All rights reserved.
VERSION:      1.10
***************************************************************************/ 

#ifndef _FLASHNVOL_
#define _FLASHNVOL_

// Data Types
#define UNSIGNED8 unsigned char
#define UNSIGNED16 unsigned short
#define UNSIGNED32 unsigned int
#define INTEGER8 char
#define INTEGER16 short
#define INTEGER32 int
#define BOOL int

#define TRUE 1
#define FALSE 0

/**************************************************************************
DOES:    Initializes access to non-volatile memory
RETURNS: TRUE for success, FALSE for error
**************************************************************************/
BOOL NVOL_Init
  (
  void
  );

/**************************************************************************
DOES:    Sets the value of a variable
RETURNS: TRUE for success, FALSE for error
**************************************************************************/
BOOL NVOL_SetVariable
  (
  UNSIGNED16 Id,				                           // id for variable
  UNSIGNED8 *Value,										   // variable data
  UNSIGNED16 Size										   // size of data in bytes
  );

/**************************************************************************
DOES:    Gets the value of a variable
RETURNS: TRUE for success, FALSE for error
**************************************************************************/
BOOL NVOL_GetVariable
  (
  UNSIGNED16 Id,						                   // id of variable
  UNSIGNED8 *Value,										   // location to store variable data
  UNSIGNED16 Size										   // size of variable in bytes
  );

#endif
