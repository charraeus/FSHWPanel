/************************************************************************/
/*                                                                      */
/*                      Keyboard Interface 3 * 8                        */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */
/************************************************************************/
/* keyboard Interface:
 * 3 rows x 8 columns: P0.0 - P0.2 * P2
 * support multiple keys pressed simultaniously
 * if more than two keys pressed, then every key need a decoupling diode
 *
 * 0x00         = no key changed
 * 0x01 .. 0x18 = key released
 * 0x81 .. 0x98 = key pressed
 */

#pragma cd pl(9999)
#include <reg51.h>

#define KEY_PRESS_MASK  0x80
#define uchar unsigned char


uchar n_keys_pressed;				// 0, 1 or 2
static uchar idata valid_keys[3];		// last returned values
static uchar idata last_keys[3];		// t - 1


void InitKeyboard( void )
{
  uchar i;

  for( i = 0; i < 3; ++i ){
    valid_keys[i] = 0xFF;				// low active
    last_keys[i]  = 0xFF;
  }
  n_keys_pressed = 0;
}


uchar GetKey( void )		// call with debouncing time (10..200msec)
{
  uchar i, j;
  uchar mask;
  uchar key;
  uchar column;
  uchar key_nr = 0;
  uchar code ROW[] = { ~0x01, ~0x02, ~0x04 };		// low active

  for( i = 0; i < 3;  ++i ){

    P0 |= ~0x07;
    P0 &= ROW[i];					// set rows
    column = P2;					// read column

    if( column != valid_keys[i] && key_nr == 0 ){
        for( mask = 1, j = 0; j < 8; mask <<= 1, j++ ){

            key = column & mask;				// check bitwise

            if( key == (last_keys[i] & mask) ){		// debouncing

                if( key != (valid_keys[i] & mask) ){		// key not handled
                    valid_keys[i] = valid_keys[i] & ~mask | key;
                    key_nr = 8 * i + j + 1;			// 0x01 ... 0x18

                        if( key == 0 ){				// low active
                            n_keys_pressed++;
                            key_nr |= KEY_PRESS_MASK;			// 0x81 ... 0x98
                        }else{
                            n_keys_pressed--;
                        }
                break;
                }
            }
        }
    }
    last_keys[i] = column;				// for next debouncing
  }
  return key_nr;
}
