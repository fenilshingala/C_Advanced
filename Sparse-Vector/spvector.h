#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H
#include <stdio.h> /* printf */

/* opaque pointer since definition of Node is hidden from the client we need to
 * provide some way to use Node struct, and here it is -- typedef.  At this
 * point Node is NOT KNOWN to the compiler (which makes Node an INCOMPLETE
 * TYPE, but declaring a pointer to an incomplete type is allowed by C (and
 * C++).
 *
 * see http://en.wikipedia.org/wiki/Opaque_pointer
 *
 * we also need a special type ConstElementNode_handle:
 * notice that:
 * even though "ElementNode*" is same as "ElementNode_handle"
 * "const ElementNode*" and "const ElementNode_handle"
 * are DIFFERENT:
 * const ElementNode* p; is modifiable pointer to constant data
 * while 
 * const ElementNode_handle p; is constant pointer to modifiable data
 */
typedef struct ElementNode *       ElementNode_handle;
typedef struct ElementNode const * ConstElementNode_handle;
typedef struct RowNode*            RowNode_handle;
typedef struct RowNode const *     ConstRowNode_handle;

/*===================*
 * element functions *
 *===================*/

/* print elements as a vector, adding 0's for non-existing positions
 * assume the length of the vector = dim
 * doesn't check if the list contains positions beyond dim */
void printf_elements( ConstElementNode_handle, char *, int); 

/* 
 * print all elements in the list with positions */
void print_elements(ConstElementNode_handle); /* no zeros */

/* insert an element into a list 
 * list is ordered using pos
 * if position pos is already occupied, the value of the node
 * should be updated with val
 * if val=0, then the element should be deleted
 * return 0 if operation is succesfull 
 *        1 if malloc failed */
int insert_element(ElementNode_handle *,int,int);

/* 
 * delete an element at position pos if it exists */
void delete_element( ElementNode_handle *,int );

/*
 * get the value at the given position,
 * p_e is the head pointer 
 */
int  get( ConstElementNode_handle, int );

/* 
 * scalar product of 2 lists.
 * */
int scalar_product( ConstElementNode_handle, ConstElementNode_handle );

/* 
 * adds 2 lists as vectors, returns a new list */
ElementNode_handle add( ConstElementNode_handle,ConstElementNode_handle );

/* 
 * deallocate a list */
void free_elements( ElementNode_handle );

/*===============* 
 * row functions * 
 *===============*/

/* prints rows as a matrix, adding 0's for non-existing positions
 * (both for non-existing rows and elements)
 * assume the number of rows = dim
 * assume the length of the vector = dim
 * doesn't check if the list contains positions beyond dim */
void printf_rows( ConstRowNode_handle, char *, int );

/* 
 * prints all non-zero elements of the list of rows */
void print_rows( ConstRowNode_handle );

/* 
 * inserts a list of elements at position pos.
 * If a row at position pos already exists, do not insert, return 2
 * If the list p_e is empty, do nothing (don't insert a NULL pointer into p_r)
 * returns 1 if malloc fails
 * returns 2 if position pos already exists
 * returns 0 otherwise */
int insert_row( RowNode_handle *,int,ElementNode_handle );

/* inserts a new element at pos_col 
 * in the row corresponding to position pos_row
 * (pos_row'th row, pos_col'th column) 
 * uses the same rules as insert_element */
int insert_element2( RowNode_handle *,int,int,int );

/* returns a pointer to a row at position pos if it exists, 
 * NULL otherwise */
RowNode_handle find_row( RowNode_handle *,int );

/* transposes a list of rows:
 * it's the matrix transposition if you view rows as a matrix
 * anyways - if an element in the original RowNode list 
 * is on row i at position j, then in the transposed
 * it is in the row j at position i. 
 * Notice that it's possible to get empty rows in the transposed
 * matrix (for example the following matrix
 * [ 1 0 4 ]     [ 1 2 3 ]
 * [ 2 0 5 ] ->  [ 0 0 0 ]
 * [ 3 0 6 ]     [ 4 5 6 ]
 * for full credit the transposed matrix should contain only 2 
 * rows (positions 0 and 2) 
 * Returns a new list of rows. Input list p_r should not be modified.*/
RowNode_handle transpose( ConstRowNode_handle );

/* multiply two lists of RowNodes as matricies.
 * Algorithm: 
 * 1) transpose the second list.
 * 2) Now [i,j]'th element of the product matrix
 * is the scalar product of i'th row of the first
 * and the j'th row of the transposed. 
 * Returns a new list of rows. Input lists p_r1, p_r2 should not be modified. */
RowNode_handle mult( ConstRowNode_handle, ConstRowNode_handle );

/* 
 * deallocate a list of RowNodes */
void free_rows( RowNode_handle );

/* 
 * Calculates the determinant of the list of RowNodes viewed as a matrix.
 * Dimension is required! 
 * extra credit, discuss algorithm with me first */
int  determinant( ConstRowNode_handle, int );

#endif
