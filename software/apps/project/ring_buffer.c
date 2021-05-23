/*
 * ring_buffer.c
 *
 * Simple ring buffer implementation. 
 *
 * NOTE --- ALL documentation located in ring_buffer.h
 */ 

#include "ring_buffer.h"

AI void _incr_rb_next(ring_buffer rb)
{
    rb.next_index_to_fill = ((rb.next_index_to_fill + 1) % DEFAULT_RING_BUF_SIZE);
    return;
}


AI uint8_t rb_get (
    uint8_t idx,
    ring_buffer rb
)
{
    assert(idx < DEFAULT_RING_BUF_SIZE);
    return rb.buf[idx] ;
}


AI void rb_push (
    uint8_t val,
    ring_buffer rb
)
{
    rb.buf[rb.next_index_to_fill] = val;
    _incr_rb_next(rb);
}


AI uint8_t rb_find_once (
    uint8_t val,
    ring_buffer rb
)
{
    /*
     * Setup
     */
    uint8_t idx;
    
    
    /*
     * Iterate
     */ 
    for (idx = 0 ; idx < DEFAULT_RING_BUF_SIZE ; idx++) 
	if (rb.buf[idx] == val) 
	    break;
    
    
    /*
     * If @val not found, set return to -1
     */
    if (idx == DEFAULT_RING_BUF_SIZE) idx = -1; 
    
	
    /*
     * Return the index
     */
    return idx;
}


AI void rb_find_all (
    uint8_t val,
    ring_buffer rb,
    uint8_t *arr,
    uint8_t *num_occ
) 
{
    /*
     * Setup
     */
    uint8_t idx, local_num_occ = 0;
    
    
    /*
     * Iterate
     */
    for (idx = 0 ; idx < DEFAULT_RING_BUF_SIZE ; idx++)
	if (rb.buf[idx] == val)
	    arr[local_num_occ++] = idx;
    
    
    /*
     * @arr is populated, set @num_occ, and we're done 
     */
    *num_occ = local_num_occ;


    return;
}


