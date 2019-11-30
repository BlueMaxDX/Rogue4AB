#include <avr/pgmspace.h>
#include "strings.h"
#include "messaging.h"

void setActiveMessage(byte id)
{
  activeMessage = asFlashStringHelper(pgm_read_ptr(&messages[id]));
}

void itmToGitm(byte type, byte r, byte k)
{
  activeMessage = asFlashStringHelper(getItemName(type, r, k));
}
