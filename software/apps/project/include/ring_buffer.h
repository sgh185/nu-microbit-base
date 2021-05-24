/*
 * ring_buffer.h
 *
 * Simple ring buffer implementation for byte values
 */ 
#pragma once

#include "system_headers.h"


/*
 * ---------- Setup, definitions ----------
 */ 
#define DEFAULT_RING_BUF_SIZE 64

typedef struct {

    /*
     * Buffer 
     */ 
    uint8_t buf[DEFAULT_RING_BUF_SIZE] ;

    /*
     * State
     */ 
    uint8_t next_index_to_fill, wrapped ;


} ring_buffer ;


/*
 * ---------- Operations ----------
 */ 

/*
 * NOTE --- This only supports the operations needed
 * in the protocol/implementation of the system
 */

/*
 * _incr_rb_next
*
 * Increments the ring buffer's .next_index_to_fill field
 * Also sets .wrapped if the .next_index_to_fill wraps 
 * around to the beginning of the buffer
 *
 * @rb
 * - Ring buffer to push to
 * - typeof(@rb) = ring_buffer (NOT ring_buffer *) 
 */ 
extern void _incr_rb_next(ring_buffer rb) ;


/*
 * rb_has_wrapped
 *
 * Checks if the ring buffer has wrapped around 
 * to the beginning of .buf at any point by 
 * checking .wrapped field
 *
 * @rb
 * - Ring buffer to push to
 * - typeof(@rb) = ring_buffer (NOT ring_buffer *) 
 */ 
extern bool rb_has_wrapped(ring_buffer rb) ;


/*
 * rb_get
 *
 * Gets the value stored at a particular index. Performs
 * no bounds checking.
 *
 * @idx:
 * - Index of value to fetch
 * - typeof(@idx) = uint8_t
 *
 * @rb
 * - Ring buffer to work with
 * - typeof(@rb) = ring_buffer (NOT ring_buffer *) 
 */ 
extern uint8_t rb_get (
    uint8_t idx,
    ring_buffer rb
) ;


/*
 * rb_push
 *
 * Pushes a value to the ring buffer's next index to fill
 *
 * @val:
 * - Value to push
 * - typeof(@val) = uint8_t
 *
 * @rb
 * - Ring buffer to push to
 * - typeof(@rb) = ring_buffer (NOT ring_buffer *) 
 */ 
extern void rb_push (
    uint8_t val,
    ring_buffer rb
) ;


/*
 * rb_find_once
 *
 * Finds a value and returns the index of its first 
 * occurrence, return -1 otherwise 
 *
 * @val:
 * - Value to find
 * - typeof(@val) = uint8_t
 *
 * @rb
 * - Ring buffer to look in
 * - typeof(@rb) = ring_buffer (NOT ring_buffer *) 
 */ 
extern uint8_t rb_find_once (
    uint8_t val,
    ring_buffer rb
) ;


/*
 * rb_find_all
 *
 * Finds a value and fills out an array of all 
 * matching indices and returns the number of 
 * occurrences total
 *
 * @val:
 * - Value to find 
 * - typeof(@val) = uint8_t
 *
 * @rb
 * - Ring buffer to look in
 * - typeof(@rb) = ring_buffer (NOT ring_buffer *) 
 *
 * @arr
 * - Array to populate with matching indices
 * - typeof(@arr) = uint8_t * (size = 16 entries)
 *
 * @num_occ
 * - Variable to set with number of indices found
 * - typeof(@num_occ) = uint8_t * (ptr to variable) 
 */ 
extern void rb_find_all (
    uint8_t val,
    ring_buffer rb,
    uint8_t *arr,
    uint8_t *num_occ
) ;



/*
 * rb_get_last_n_entries 
 *
 * Get the last "n" entries of the ring buffer
 * and store them into an array passed into
 * this function
 *
 * @rb
 * - Ring buffer to look in
 * - typeof(@rb) = ring_buffer (NOT ring_buffer *) 
 *
 * @arr
 * - Array to populate with latest entries 
 * - typeof(@arr) = uint8_t * 
 *
 * @N
 * - Maximum number of entries to fetch
 * - typeof(@N) = uint8_t
 *
 * @num_fetched
 * - Variable to set with latest entries fetched 
 * - typeof(@num_fetched) = uint8_t * (ptr to variable) 
 */ 
extern void rb_get_last_n_entries (
    ring_buffer rb,
    uint8_t *arr,
    uint8_t N,
    uint8_t *num_fetched
) ;


/*
 * rb_print
 *
 * Prints the ring buffer's contents
 *
 * @prefix
 * - Printing will start with "@prefix: " in rb_print
 * - typeof(@rb) = const char *
 *
 * @rb
 * - Ring buffer to look in
 * - typeof(@rb) = ring_buffer (NOT ring_buffer *) 
 */ 
extern void rb_print (
    const char *prefix,
    ring_buffer rb
) ;

