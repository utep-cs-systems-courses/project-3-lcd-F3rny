#include "buzzer.h"
#include "sound_effect.h"

void collision()
{
  buzzer_set_period(4357); // sound
  __delay_cycles(200000);   // duration
  buzzer_set_period(0);   // turn off
}

void lost()
{
  buzzer_set_period(8849); // sound
  __delay_cycles(200000);   // duration
  buzzer_set_period(0);   // turn off
}
