#include "foc_utils.h"




// function approximating the sine calculation by using fixed size array
// input angle is in radians
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328PB__) || defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega2560__) || defined(AVR_ATmega1280)
// ATMega implementation uses 65 entry LUT and interpolation, and ldexp() to scale float values
// LUT is 16 bit integers, scale is 2^15 to use as much of the 16 bit range as possible
// UNO: precision: RMS difference to sin() 0.00006479, speed: 71.5us including normalization
__attribute__((weak)) float _sin(float angle) {
  static uint16_t sine_array[65] = {0,804,1608,2411,3212,4011,4808,5602,6393,7180,7962,8740,9512,10279,11039,11793,12540,13279,14010,14733,15447,16151,16846,17531,18205,18868,19520,20160,20788,21403,22006,22595,23170,23732,24279,24812,25330,25833,26320,26791,27246,27684,28106,28511,28899,29269,29622,29957,30274,30572,30853,31114,31357,31581,31786,31972,32138,32286,32413,32522,32610,32679,32729,32758,32768};
  uint16_t i = (uint16_t)(ldexp(angle,16) / _2PI);
  int32_t a, b, frac = i & 0xff;
  i = (i >> 8) & 0xff;
  if (i < 64)
    a = sine_array[i], b = sine_array[i + 1];
  else if(i < 128)
    a = sine_array[128 - i], b = sine_array[127 - i];
  else if(i < 192)
    a = -sine_array[-128 + i], b = -sine_array[-127 + i];
  else
    a = -sine_array[256 - i], b = -sine_array[255 - i];
  return ldexp((a + (((b - a) * frac) >> 8)), -15);
}
#else
// On other (32 bit) MCUs we use a 257 entry LUT without interpolation, and avoid using ldexp()
// LUT is 16 bit integers, scale is 2^15 to use as much of the 16 bit range as possible
// STM32G474: precision: RMS difference to sin() 0.00125253, speed: 210ns including normalization
__attribute__((weak)) float _sin(float angle) {
  static uint16_t sine_array[257] = {0, 201, 402, 603, 804, 1005, 1206, 1407, 1608, 1809, 2009, 2210, 2411, 2611, 2811, 3012, 3212, 3412, 3612, 3812, 4011, 4211, 4410, 4609, 4808, 5007, 5205, 5404, 5602, 5800, 5998, 6195, 6393, 6590, 6787, 6983, 7180, 7376, 7571, 7767, 7962, 8157, 8351, 8546, 8740, 8933, 9127, 9319, 9512, 9704, 9896, 10088, 10279, 10469, 10660, 10850, 11039, 11228, 11417, 11605, 11793, 11980, 12167, 12354, 12540, 12725, 12910, 13095, 13279, 13463, 13646, 13828, 14010, 14192, 14373, 14553, 14733, 14912, 15091, 15269, 15447, 15624, 15800, 15976, 16151, 16326, 16500, 16673, 16846, 17018, 17190, 17361, 17531, 17700, 17869, 18037, 18205, 18372, 18538, 18703, 18868, 19032, 19195, 19358, 19520, 19681, 19841, 20001, 20160, 20318, 20475, 20632, 20788, 20943, 21097, 21251, 21403, 21555, 21706, 21856, 22006, 22154, 22302, 22449, 22595, 22740, 22884, 23028, 23170, 23312, 23453, 23593, 23732, 23870, 24008, 24144, 24279, 24414, 24548, 24680, 24812, 24943, 25073, 25202, 25330, 25457, 25583, 25708, 25833, 25956, 26078, 26199, 26320, 26439, 26557, 26674, 26791, 26906, 27020, 27133, 27246, 27357, 27467, 27576, 27684, 27791, 27897, 28002, 28106, 28209, 28311, 28411, 28511, 28610, 28707, 28803, 28899, 28993, 29086, 29178, 29269, 29359, 29448, 29535, 29622, 29707, 29792, 29875, 29957, 30038, 30118, 30196, 30274, 30350, 30425, 30499, 30572, 30644, 30715, 30784, 30853, 30920, 30986, 31050, 31114, 31177, 31238, 31298, 31357, 31415, 31471, 31527, 31581, 31634, 31686, 31737, 31786, 31834, 31881, 31927, 31972, 32015, 32058, 32099, 32138, 32177, 32214, 32251, 32286, 32319, 32352, 32383, 32413, 32442, 32470, 32496, 32522, 32546, 32568, 32590, 32610, 32629, 32647, 32664, 32679, 32693, 32706, 32718, 32729, 32738, 32746, 32753, 32758, 32762, 32766, 32767, 32768};
  uint32_t i = ((uint32_t)(angle * (256*8 /_2PI) + 1) >> 1) & 0x3ff;
  if (i < 256) {
    return (1/32768.0f)*sine_array[i];
  }
  else if(i < 512) {
    return (1/32768.0f)*sine_array[512 - i];
  }
  else if(i < 768) {
    return -(1/32768.0f)*sine_array[-512 + i];
  }
  else {
    return -(1/32768.0f)*sine_array[1024 - i];
  }
}
#endif



// function approximating cosine calculation by using fixed size array. default implementation is using sine function
__attribute__((weak)) float _cos(float a){
  float a_sin = a + _PI_2;
  return _sin(a_sin);
}


// normalizing radian angle to [0,2PI]
__attribute__((weak)) float _normalizeAngle(float angle){
  float a = fmod(angle, _2PI);
  return a >= 0 ? a : (a + _2PI);
}

// Electrical angle calculation
float _electricalAngle(float shaft_angle, int pole_pairs) {
  return (shaft_angle * pole_pairs);
}

// square root approximation function using
// https://reprap.org/forum/read.php?147,219210
// https://en.wikipedia.org/wiki/Fast_inverse_square_root
__attribute__((weak)) float _sqrtApprox(float number) {//low in fat
  // float x;
  // const float f = 1.5F; // better precision

  // x = number * 0.5F;
  float y = number;
  long i = * ( long * ) &y;
  i = 0x5f375a86 - ( i >> 1 );
  y = * ( float * ) &i;
  // y = y * ( f - ( x * y * y ) ); // better precision
  return number * y;
}
