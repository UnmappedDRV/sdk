#pragma once

#include "driver.h"
#include <stddef.h>

//list utils

typedef struct list_node {
	struct list_node *prev;
	struct list_node *next;
} list_node_t;

typedef struct list {
	struct list_node *first;
	struct list_node *last;
} list_t;

static inline void *udrv_list_append(list_t *list, void *data) {
	if (!data) return NULL;
	list_node_t *node = (list_node_t *)data;
	if (list->last) {
		list->last->next = node;
	} else {
		list->first = node;
	}

	node->prev = list->last;
	node->next = NULL;
	list->last = node;

	return data;
}

static inline void *udrv_list_remove(list_t *list, void *data) {
	if (!data) return NULL;
	list_node_t *node = (list_node_t *)data;
	if (node->prev) {
		node->prev->next = node->next;
	} else {
		list->first = node->next;
	}

	if (node->next) {
		node->next->prev = node->prev;
	} else {
		list->last = node->prev;
	}

	return data;
}

static inline void *udrv_list_peek_first(list_t *list){
	return list->first;
}

static inline void *udrv_list_peek_last(list_t *list){
	return list->last;
}

static inline void *udrv_list_pop_first(list_t *list){
	return udrv_list_remove(list, list->first);
}

static inline void *udrv_list_pop_last(list_t *list){
	return udrv_list_remove(list, list->last);
}

#define foreach(type,name,list) for(type name = (void*)(list).first;\
		name;\
		name = (void*)((list_node_t*)name)->next)
