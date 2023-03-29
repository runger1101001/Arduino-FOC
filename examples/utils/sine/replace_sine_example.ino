
/**
 * You can replace the sine function of SimpleFOC with your own version.
 * 
 * To do so, simply define a function called _sin() in your sketch.
 * 
 * You can also replace _cos(), but by default _cos() will call your _sin() function with an offset.
 * 
 * When you replace the _sin() function, the default implementation and its lookup table will not be linked and
 * will not take up any space in your sketch.
 */


#include <SimpleFOC.h>



// here is a version of _sin() that is 25% faster than the default SimpleFOC _sin() on Arduino UNO (ATMega).
// unfortunately, it is not as accurate, and the lookup table is twice the size.
// by defining it in your sketch like this, it will be used instead of the default one.

uint16_t lookup_table[129] = {0, 402, 804, 1206, 1608, 2009, 2411, 2811, 3212, 3612, 4011, 4410, 4808, 5205, 5602, 5998, 6393, 6787, 7180, 7571, 7962, 8351, 8740, 9127, 9512, 9896, 10279, 10660, 11039, 11417, 11793, 12167, 12540, 12910, 13279, 13646, 14010, 14373, 14733, 15091, 15447, 15800, 16151, 16500, 16846, 17190, 17531, 17869, 18205, 18538, 18868, 19195, 19520, 19841, 20160, 20475, 20788, 21097, 21403, 21706, 22006, 22302, 22595, 22884, 23170, 23453, 23732, 24008, 24279, 24548, 24812, 25073, 25330, 25583, 25833, 26078, 26320, 26557, 26791, 27020, 27246, 27467, 27684, 27897, 28106, 28311, 28511, 28707, 28899, 29086, 29269, 29448, 29622, 29792, 29957, 30118, 30274, 30425, 30572, 30715, 30853, 30986, 31114, 31238, 31357, 31471, 31581, 31686, 31786, 31881, 31972, 32058, 32138, 32214, 32286, 32352, 32413, 32470, 32522, 32568, 32610, 32647, 32679, 32706, 32729, 32746, 32758, 32766, 32768};

float _sin(float a) {
  unsigned int i = ((unsigned int)(a * (128*8 /_2PI) + 1) >> 1) & 0x1ff;
  if (i < 128) {
    return ldexp(lookup_table[i],-15);
  }
  else if(i < 256) {
    return ldexp(lookup_table[256 - i],-15);
  }
  else if(i < 384) {
    return -ldexp(lookup_table[-256 + i],-15);
  }
  else {
    return -ldexp(lookup_table[512 - i],-15);
  }
}





void setup() {
  // your setup code
}


void loop() {
  // your main loop code
}
