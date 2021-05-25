/*
 * ring_buffer.c
 *
 * Simple ring buffer implementation. 
 *
 * NOTE --- ALL documentation located in ring_buffer.h
 */ 

#include "ring_buffer.h"

#define modulo_incr(val, base) ((val + 1) % base)
#define modulo_decr(val, base) ((val + base - 1) % base)

AI void _incr_rb_next(ring_buffer *rb)
{
    uint8_t curr_idx = rb->next_index_to_fill;
    rb->next_index_to_fill = modulo_incr(curr_idx, DEFAULT_RING_BUF_SIZE);
    if (curr_idx > rb->next_index_to_fill) rb->wrapped |= 1;

    return;
}


AI bool rb_has_wrapped(ring_buffer *rb)
{
    return !!rb->wrapped;
}



AI uint8_t rb_get (
    uint8_t idx,
    ring_buffer *rb
)
{
    assert(idx < DEFAULT_RING_BUF_SIZE);
    return rb->buf[idx] ;
}


AI void rb_push (
    uint8_t val,
    ring_buffer *rb
)
{
    rb->buf[rb->next_index_to_fill] = val;
    _incr_rb_next(rb);
}


AI uint8_t rb_find_once (
    uint8_t val,
    ring_buffer *rb
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
	if (rb->buf[idx] == val) 
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
    ring_buffer *rb,
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
	if (rb->buf[idx] == val)
	    arr[local_num_occ++] = idx;
    
    
    /*
     * @arr is populated, set @num_occ, and we're done 
     */
    *num_occ = local_num_occ;


    return;
}


AI void rb_get_last_n_entries (
    ring_buffer *rb,
    uint8_t *arr,
    uint8_t N,
    uint8_t *num_fetched
)
{
    /*
     * Setup
     */ 
    if (!(rb->wrapped) && (rb->next_index_to_fill == 0)) return;

    printf("rb->next_index_to_fill: %u\n", rb->next_index_to_fill); 

    uint8_t idx = rb->next_index_to_fill;
    uint8_t local_num_fetched = 0;

    
    /*
     * Iterate
     */
    while (local_num_fetched < N)
    {
	idx = modulo_decr(idx, DEFAULT_RING_BUF_SIZE);
	arr[local_num_fetched] = rb_get(idx, rb);
	local_num_fetched++;
	if (true
	    && !rb->wrapped
	    && idx == 0) break;
	printf("idx: %u\n", idx);
    }


    /*
     * Set state
     */ 
    *num_fetched = local_num_fetched;


    return;
}


AI void rb_print (
    const char *prefix,
    ring_buffer *rb
)
{
    /*
     * Setup
     */ 
    uint8_t start = 
	(rb->wrapped) ? 
	(rb->next_index_to_fill) :
	(0) ;	

    uint8_t idx = start;


    /*
     * Iterate
     */ 
    printf("%s ", prefix);
    do {
	printf("%u ", rb_get(idx, rb));
	idx = modulo_incr(idx, DEFAULT_RING_BUF_SIZE);	
    } while (idx != start) ;


    printf("|\n");
    return;
}

