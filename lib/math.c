/* math.c - Contains common math functions 
 */

#include <lib/math.h>

/**
 * Calculates the Log2(x) of a number x
 * @param  x Number to calculate Log2 of
 * @return   The Log2 of the input x
 *
 * Adapted for use in this source from the fastapprox library by Paul Mineiro
 * 	under the BSD-3 License (Copyright (C) 2012 Paul Mineiro)
 */
float logBase2 (float x) {
    union { float f; uint32_t i; } vx = { x };
    union { uint32_t i; float f; } mx = { (vx.i & 0x007FFFFF) | 0x3f000000 };
    float y = vx.i;
    y *= 1.1920928955078125e-7f;

    return y - 124.22551499f - 1.498030302f * mx.f - 1.72587999f / (0.3520887068f + mx.f);
}

/**
 * Calculates the log of a number x
 * @param  x Number to caculate log of
 * @return   The log of the input x
 * 
 * Adapted for use in this source from the fastapprox library by Paul Mineiro
 * 	under the BSD-3 License (Copyright (C) 2012 Paul Mineiro)
 */
float log (float x) {
    return 0.69314718f * logBase2 (x);
}

/**
 * Returns the ceiling of a value (ex. 3.2 -> 4)
 * @param  x Value to calculate ceiling of
 * @return   Ceiling of the input x
 */
int ceil(float x) {
	if(x<0)return (int)x;
	return ((int)x)+1;
}

/**
 * Returns the floor of a value (ex. 3.2 -> 3)
 * @param  x Value to calculate floor of
 * @return   Floor of the input x
 */
int floor(float x) {
	if (x >= 0.0f)
	{
		return ((int)x);
	}
	else
	{
		return ((int)x - 1);
	}
}
