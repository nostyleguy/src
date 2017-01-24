// ======================================================================
//
// ByteOrder.h
// copyright 1998 Bootprint Entertainment
// copyright 2001 Sony Online Entertainment
//
// ======================================================================

#ifndef BYTE_ORDER_H
#define BYTE_ORDER_H

// ======================================================================

ulong  __cdecl ntohl(ulong  netLong);
uint16_t __cdecl ntohs(uint16_t netShort);

ulong  __cdecl htonl(ulong  hostLong);
uint16_t __cdecl htons(uint16_t hostShort);

// ======================================================================

#endif
