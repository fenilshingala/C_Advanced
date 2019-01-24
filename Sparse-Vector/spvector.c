#include <stdlib.h>
#include <stdio.h>
#include "spvector.h"

/* definition of Node is hidden from the client (driver) so that client CANNOT
 * use INTERNALS of the Node.  this is poor-man encapsulations - hiding
 * implementation details.  The main reason for encapsulations -- if I ever
 * decide to change Node struct no client code will break. Example: I decide to
 * change "left" to "Left", all I do is change map.h and map.c, no client code
 * is effected, since clients were FORCED to use getleft(...) function, rather
 * than field name "left".  Also see typedef in map.h
 */
struct ElementNode {
  int    data;
  int    pos;
  struct ElementNode* next;
};

struct RowNode {
  int pos;
  struct ElementNode* elements;
  struct RowNode* next;
};

typedef struct ElementNode ElementNode;
typedef struct RowNode RowNode;

/* PROTOTYPE */
ElementNode_handle getnext(ElementNode_handle g_n);
void setnext(ElementNode_handle s_n, ElementNode_handle next);

ElementNode_handle getnext(ElementNode_handle g_n) {
	return g_n->next;
}
void setnext(ElementNode_handle s_n, ElementNode_handle next) {
	s_n->next = next;
}

void free_elements(ElementNode_handle f_h) {
	ElementNode_handle current = f_h;
	ElementNode_handle prev = f_h;

	while (f_h) {
		current = getnext(prev);
		free(prev);
		prev = current;
		f_h = current;
	}
}

/* insert an element into a list
 * list is ordered using pos
 * if position pos is already occupied, the value of the node
 * should be updated with val
 * if val=0, then the element should be deleted
 * return 0 if operation is succesfull
 *        1 if malloc failed */
int insert_element(ElementNode_handle *i_e, int pos, int data) {

    ElementNode_handle current = *i_e;

	if (current == NULL || current->pos > pos) {
		ElementNode_handle temp;

		if (data == 0) { return 1; }

		temp = (ElementNode_handle)malloc(sizeof(ElementNode));
		if(!temp) return 1;
		temp->pos = pos;
		temp->data = data;

		setnext(temp, current);
		*i_e = temp;
	}
	else {
		ElementNode_handle temp;

		current = *i_e;

		temp = (ElementNode_handle)malloc(sizeof(ElementNode));
		if(!temp) return 1;
		temp->pos = pos;
		temp->data = data;

		if (current->pos == pos) {
			if(data!=0){ current->data = data; }
			else{delete_element(i_e, pos);}
			return 0;
		}

		while (getnext(current) != NULL && getnext(current)->pos <= temp->pos) {
			current = getnext(current);
		}

		if (current->pos == pos) {
			if (data == 0) {
				delete_element(i_e, pos);
				return 0;
			}
			else {
				current->data = data;
			}
		}
		else {
			if (data == 0) { return 1; }
			setnext(temp, getnext(current));
			setnext(current, temp);
		}
	}
	return 0;
}

/* delete an element at position pos if it exists */
void delete_element(ElementNode_handle * d_e, int pos) {
	ElementNode_handle current = *d_e;
	ElementNode_handle prev = current;

	/* IF NO ELEMENTS IN *d_e return */
	if (current == NULL) { return; }
	
	while (current) {
		if(current->pos == pos){
			if (prev == current) {
				*d_e = getnext(current);
				free(current);
				return;
			}
			else {
				setnext(prev, getnext(current));
				free(current);
				return;
			}
		}
		else {
			prev = current;
			current = getnext(current);
		}
	}
}

/* 
 * scalar product of 2 lists.
 * */
int scalar_product(ConstElementNode_handle c_h1, ConstElementNode_handle c_h2) {

	int result = 0;

	while (c_h1 && c_h2) {
		if (c_h1->pos == c_h2->pos) {
			result += (c_h1->data * c_h2->data);
			c_h1 = c_h1->next;
			c_h2 = c_h2->next;
		}
		else if (c_h1->pos > c_h2->pos) {
			c_h2 = c_h2->next;
		}
		else if (c_h1->pos < c_h2->pos) {
			c_h1 = c_h1->next;
		}
	}

	return result;
}

ElementNode_handle add(ConstElementNode_handle ls1, ConstElementNode_handle ls2) {
	int result = 0;
	ElementNode_handle temp = NULL;

	while (ls1 && ls2) {
		
		if (ls1->pos == ls2->pos) {
			result = ls1->data + ls2->data;
			insert_element(&temp, ls1->pos, result);
			ls1 = ls1->next;
			ls2 = ls2->next;
		}

		else if (ls1->pos < ls2->pos) {
			result = ls1->data;
			insert_element(&temp, ls1->pos, result);
			ls1 = ls1->next;
		}

		else if (ls1->pos > ls2->pos) {
			result = ls2->data;
			insert_element(&temp, ls2->pos, result);
			ls2 = ls2->next;
		}

	}
	while (ls1) {
		insert_element(&temp, ls1->pos, ls1->data);
		ls1 = ls1->next;
	}
	while (ls2) {
		insert_element(&temp, ls2->pos, ls2->data);
		ls2 = ls2->next;
	}

	return temp;
}

int  get(ConstElementNode_handle p_e, int pos) {
	if (!(p_e)) { return 0; }
	while ( (p_e) && p_e->pos != pos ) {
		p_e = p_e->next;
	}
	if(p_e)	return p_e->data;
	return 0;
}


/* ROW FUNCTIONS -------------------------------------------------------------------------------------------- */
int insert_row(RowNode_handle * p_r, int pos, ElementNode_handle p_e) {
	RowNode_handle current = *p_r;

	if (p_e == NULL) return 0;

	if (current == NULL || current->pos > pos) {
		RowNode_handle temp;

		temp = (RowNode_handle)malloc(sizeof(RowNode));
		if(!temp) return 1;
		temp->pos = pos;
		temp->elements = p_e;
		temp->next = NULL;

		temp->next = current;
		*p_r = temp;
	}
	else {
		RowNode_handle temp;

		current = *p_r;

		temp = (RowNode_handle)malloc(sizeof(RowNode));
		if(!temp) return 1;
		temp->pos = pos;
		temp->elements = p_e;

		while (current->next != NULL && current->next->pos <= temp->pos) {
			current = current->next;
		}

		if (current->pos == pos) {
			return 1; // ROW ALREADY EXIST
		}
		else {
			temp->next  = current->next;
			current->next = temp;
		}
	}

	return 0;
}

void free_rows(RowNode_handle r_h) {
	RowNode_handle current = r_h;
	RowNode_handle prev = r_h;

	while (r_h) {
		current = prev->next;
		free(prev);
		prev = current;
		r_h = current;
	}
}

int insert_element2(RowNode_handle *r_h, int row_pos, int col_pos, int data) {

	RowNode_handle current = *r_h;
	ElementNode_handle current_el;
	RowNode_handle prev = *r_h;

	current_el = NULL;
	if (current == NULL || current->pos > row_pos) {
		insert_element(&current_el, col_pos, data);
		if (current_el != NULL) {
			insert_row(&current, row_pos, current_el);
			*r_h = current;
		}
		return 0;
	}
	else {
		while (current->next != NULL && current->pos < row_pos) {
			prev = current;
			current = current->next;
		}
		if (current->pos == row_pos) {
			insert_element(&current->elements, col_pos, data);
			
			if (current->elements == NULL) {
				if (prev == current) {
					*r_h = current->next;
					free(current);
					return 0;
				}
				prev->next = current->next;
				free(current);
				return 0;
			}
		}
		else {
			if (data == 0) { return 1; }
			insert_element(&current_el, col_pos, data);
			if (current_el) { insert_row(&prev, row_pos, current_el); return 0; }
			return 1;
		}
	}
	return 0;
}

RowNode_handle transpose(ConstRowNode_handle tr) {
	RowNode_handle result = NULL;
	ElementNode_handle row;

	while (tr) {
		row = tr->elements;
		while (row) {
			insert_element2(&result, row->pos, tr->pos, row->data);
			row = row->next;
		}
		tr = tr->next;
	}
	return result;
}

RowNode_handle mult(ConstRowNode_handle r1, ConstRowNode_handle r2) {
	RowNode_handle t_r2;
	RowNode_handle transp_r2 = transpose(r2);
	RowNode_handle result = NULL;

	t_r2 = transp_r2;

	while (r1) {
		while (t_r2) {
			insert_element2(&result, r1->pos, t_r2->pos, scalar_product(r1->elements, t_r2->elements));
			t_r2 = t_r2->next;
		}
		t_r2 = transp_r2;
		r1 = r1->next;
	}

	return result;
}


/*print functions*/
void printf_elements(ConstElementNode_handle p_e, char *fmt, int dim) {
	int i,last_pos=-1;
	while (p_e) {
		for (i=last_pos+1;i<p_e->pos;++i) { printf(fmt,0); }
		printf(fmt,p_e->data);
		last_pos=p_e->pos;
		p_e = p_e->next;
	}
	for (i=last_pos+1;i<dim;++i) { printf(fmt,0); }
}

void print_elements(ConstElementNode_handle p_e) {
	while (p_e) {
		printf("%i at pos %i, ",p_e->data,p_e->pos);
		p_e = p_e->next;
	}
}

void print_rows(ConstRowNode_handle p_r) {
	while (p_r) {
		printf("row %i: ", p_r->pos);
		print_elements(p_r->elements);
		printf("\n");
		p_r = p_r->next;
	}
}

void printf_rows(ConstRowNode_handle p_r, char *fmt, int dim) {
	int i,j,last_pos=-1;
	while (p_r) {
		for (i=last_pos+1;i<p_r->pos;++i) { /* output row of 0's */
			for (j=0;j<dim;++j) printf(fmt,0); 
			printf("\n");
		}
		printf_elements(p_r->elements,fmt,dim);
		printf("\n");
		last_pos=p_r->pos;
		p_r = p_r->next;
	}
	for (i=last_pos+1;i<dim;++i) { /* output row of 0's */
		for (j=0;j<dim;++j) printf(fmt,0); 
		printf("\n");
	}
}


int  determinant(ConstRowNode_handle p_r,int dim) {
	/* this is a meaningless code to get rid of "unsed argument" warnings in 
	 * Borland an MS */
	int result=1;
	if (dim==0) ++result;
	if (p_r)    --result;
	return result;
}
