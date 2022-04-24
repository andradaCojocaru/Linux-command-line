#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"
#include "utils.h"

linked_list_t*
ll_create(unsigned int data_size)
{
	linked_list_t * list = malloc (sizeof(linked_list_t));
	list->head = NULL;
	list->data_size = data_size;
	list->size = 0;
	return list;
}

void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
	unsigned int i;

	if (list == NULL)
		return;

	if (n < 0) {
		fprintf(stderr, "eroare");
	} else if (n == 0) {
		ll_node_t *new;
		new = malloc(sizeof(ll_node_t));
		DIE(new == NULL, "Eroare alocare memorie");
		new->data = malloc(sizeof(list->data_size));
		DIE(new->data == NULL, "Eroare alocare memorie");
		memcpy(new->data, new_data, list->data_size);
		new->next = list->head;
		list->head = new;
		list->size++;
	} else if (n >= list->size) {
		ll_node_t *new, *curr;
		new = malloc(sizeof(ll_node_t));
		DIE(new == NULL, "Eroare alocare memorie");
		new->data = malloc(sizeof(list->data_size));
		DIE(new->data == NULL, "Eroare alocare memorie");
		memcpy(new->data, new_data, list->data_size);
		new->next = NULL;
		curr = list->head;
		while (curr->next != NULL) {
			curr = curr->next;
		}
		curr->next = new;
		list->size++;
	} else {
		ll_node_t *new, *curr;
		new = malloc(sizeof(ll_node_t));
		DIE(new == NULL, "Eroare alocare memorie");
		new->data = malloc(sizeof(list->data_size));
		DIE(new->data == NULL, "Eroare alocare memorie");
		memcpy(new->data, new_data, list->data_size);
		new->next = NULL;
		curr = list->head;
		for (i = 0; i < n - 1; i++) {
			curr = curr->next;
		}
		curr->next = new;
		list->size++;
	}
}

ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
	unsigned int i;
	if (list == NULL || list->head == NULL)
		return NULL;

	if (n < 0) {
		fprintf(stderr, "eroare");
		return 0;
	} else if (n == 0) {
		ll_node_t *first;
		first = list->head;
		list->head = first->next;
		list->size--;
		free(first->data);
		free(first);
		return first;
	} else if (n >= list->size) {
		ll_node_t *last, *prev;
		last = list->head->next;
		prev = list->head;
		while (last->next != NULL) {
			prev = last;
			last = last->next;
		}
		prev->next = NULL;
		list->size--;
		free(last->data);
		free(last);
		return last;
	} else {
		ll_node_t *prev, *curr;
		curr = list->head->next;
		prev = list->head;
		for (i = 0; i < n - 1; i++) {
			prev = curr;
			curr = curr->next;
		}
		curr->next = prev->next;
		list->size--;
		free(prev->data);
		free(prev);
		return prev;
	}
}

unsigned int
ll_get_size(linked_list_t* list)
{
	return list -> size;
}

void
ll_free(linked_list_t** pp_list)
{
	ll_node_t *prev, *curr;
	curr = (*pp_list)->head->next;
	prev = (*pp_list)->head;

	while (curr != NULL) {
		free(prev->data);
		free(prev);
		prev = curr;
		curr = curr->next;	
	}

	free(*pp_list);
	(*pp_list)->head = NULL;
}

void
ll_print_int(linked_list_t* list)
{
	ll_node_t *node;
	if (list == NULL)
		return;
	node = list -> head;
	while (node != NULL) {
		printf("%d", (int)node->data);
		node = node->next;
	}

	printf("\n");
}

void
ll_print_string(linked_list_t* list)
{
	if (list == NULL)
		return;
	ll_node_t *node;
	node = list -> head;
	while (node != NULL) {
		printf("%s", node->data);
		node = node->next;
	}

	printf("\n");
}
