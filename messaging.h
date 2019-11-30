#pragma once

#include <avr/pgmspace.h>

#include "strings.h"
#include "FlashStringHelper.h"

inline const char * readProgmem(const char * const * progmemPointer)
{
	return static_cast<const char *>(pgm_read_ptr(progmemPointer));
}

inline const char * getItemName(byte type, byte r, byte k)
{

  const byte index[13]={0,1,3,11,18,32,44,55,65,66,80,92,103};
  if(k==0){
  	return readProgmem(&nitem[index[type-1]+r]);
  } else {
  	return readProgmem(&nitem[index[type+4]+ttab[type-5][r]]);
  }
}

inline FlashStringHelper getPrintableItemName(byte type, byte r, byte k)
{
	return asFlashStringHelper(getItemName(type, r, k));
}

