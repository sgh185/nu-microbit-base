/*
 * utils.c
 *
 * Utilities implementation 
 */ 
#include "utils.h"


/*
 * ---------- Calculation Utilities ----------
 *
 * NOTE --- There is suspicious casting. Ignore 
 */ 
AI uint32_t __sum_bytes (
    uint8_t *values,
    uint8_t num_values
)
{
    /*
     * TOP
     *
     * Sum @values up for @num_values entries
     */ 
    uint32_t sum = 0;
    for (uint8_t i = 0 ; i < num_values ; i++)
	sum += ((uint32_t) values[i]);


    return sum;
}


AI uint32_t __sum_words (
    uint32_t *values,
    uint32_t num_values
)
{
    /*
     * TOP
     *
     * Sum @values up for @num_values entries
     */ 
    uint32_t sum = 0;
    for (uint32_t i = 0 ; i < num_values ; i++)
	sum += values[i];


    return sum;
}


AI float __average_bytes (
    uint8_t *values,
    uint8_t num_values
) 
{
    /*
     * TOP
     *
     * Calculate average for @num_values in @values 
     */ 
    uint32_t sum = __sum_bytes(values, num_values);
    float average = ((float) sum / (float) num_values);	
    return average;
}


AI float __lsr_slope_bytes (
    uint8_t *x_values,
    uint8_t *y_values,
    uint8_t N 
)
{
    /*
     * TOP
     *
     * Compute the slope of the least-squares regression
     * line for the @N points specified in @x_values, @y_values
     */ 

    /*
     * Compute Σx, Σy 
     */
    uint32_t sum_x = __sum_bytes(x_values, N); 
    uint32_t sum_y = __sum_bytes(y_values, N); 


    /*
     * Compute Σ(x^2) 
     */
    uint32_t x_sq_values[N];
    for (uint8_t i = 0 ; i < N ; i++)
    {
	uint32_t x = ((uint32_t) x_values[i]);
	x_sq_values[i] = x * x;
    }

    uint32_t sum_x_sq = __sum_words((uint32_t *) &x_sq_values, (uint32_t) N); 


    /*
     * Compute Σ(xy) 
     */
    uint32_t xy_values[N];
    for (uint32_t i = 0 ; i < N ; i++)
    {
	uint32_t x = ((uint32_t) x_values[i]);
	uint32_t y = ((uint32_t) y_values[i]);
	xy_values[i] = x * y;
    }

    uint32_t sum_xy = __sum_words((uint32_t *) &xy_values, (uint32_t) N); 


    /*
     * Compute m:
     *
     * [(N * Σ(xy)) - ((Σx) * Σ(y))]
     * -----------------------------
     *    [(N * Σ(x^2)) - (Σx)^2]
     *
     * Casting isn't an issue, the math will work with these bounds.
     *
     * The casting is suspicious though
     */ 
    int32_t numerator = ((int32_t) N * (int32_t) sum_xy) - ((int32_t) sum_x * (int32_t) sum_y);
    int32_t denominator = ((int32_t) N * (int32_t) sum_x_sq) - ((int32_t) sum_x * (int32_t) sum_x);
    float m_slope = ((float) numerator / (float) denominator);


    return m_slope;
}




