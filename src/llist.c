/*
 * llist.c is part of libllist.
 * 
 * libllist is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * libllist is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with libllist.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * llist.c 
 *
 * AUTHOR: Noah Harvey
 *
 * VERSION: v0.0.1
 *
 * DESCRIPTION: c file for llist.h
 */

#include "llist.h"
#include <stdlib.h>

Node* nodenew()
{
	Node* newnode = (Node*)malloc(sizeof(Node));
	VALIDPNTR(newnode, NULL);

	newnode->data = newnode->prev = newnode->next = NULL;

	return newnode;
}

LList* llnew()
{
	LList* newlist = (LList*)malloc(sizeof(LList));
	VALIDPNTR(newlist, NULL);

	newlist->head = NULL;
	newlist->tail = NULL;
	newlist->length = 0;

	return newlist;
}

int lldestroy(LList* list)
{
	VALIDPNTR(list,1);
	Node* crnt = list->head;
	while(crnt)	
	{
		Node* temp = crnt;
		crnt = crnt->next;
		free(temp);
	}
	free(list);
	return 0;
}

LList* llappend(LList* list, void* value)
{
	return llinsert(list, value, list->length);
}

LList* llinssort(LList* list, void* value)
{
	VALIDPNTR(list,NULL);

	int index = 0;	
	size_t getind(void* data)
	{
		if(*((int*)data) > *((int*)value)) return 1;
		index++;
		return 0;
	}	

	llapply(list, &getind);

	return llinsert(list, value, index);
}

LList* llinsert(LList* list, void* value, int index)
{
	//error check
	VALIDPNTR(list,NULL);
	if(index < 0 || index > list->length) return NULL;

	//create new node
	Node* newnode; 
	newnode = nodenew();
	VALIDPNTR(newnode,NULL);
	newnode->data = value;

	//insert node into list
	Node* prevnode = llget(list, index-1);
	Node* nextnode = llget(list, index);
	newnode->prev = prevnode;
	newnode->next = nextnode;

	if(prevnode) prevnode->next = newnode;
	if(nextnode) nextnode->prev = newnode;

	if(index == 0)
		list->head = newnode;
	if(index >= list->length)
		list->tail = newnode;
	
	list->length++;
	return list;
}

LList* llremove(LList* list, size_t index)
{
	Node* node;
	VALIDPNTR((node = llget(list,index)),NULL);
	node->prev->next = node->next;
	free(node);
	return list;
}

LList*  llset(LList* list, void* value, int index)
{
	VALIDPNTR(list,NULL);
	Node* temp = llget(list,index);
	VALIDPNTR(temp,NULL);
	
	temp->data = value;

	return list;
}

Node* llget(LList* list, int index)
{
	VALIDPNTR(list,NULL);
	if(index < list->length && index >= 0)
	{
		int i;
		Node* temp = list->head;
		for(i = 0; i < list->length; i++)
		{
			if(i == index) return temp;
			temp = temp->next;
		}
	}
	return NULL;
}

void* llgetvalue(LList* list, int index)
{
	Node* tmp = llget(list,index);
	VALIDPNTR(tmp,NULL);
	return tmp->data;
}

LList* llapply(LList* list, size_t (*func)(void* data))
{
	VALIDPNTR(list,NULL);
	VALIDPNTR(func,NULL);

	Node* curr = list->head;
	while(curr)
	{	
		if((*func)(curr->data) > 0) break;
		curr=curr->next;
	}

	return list;
}

LList* llmap(LList* list, void* (*func)(void* data))
{
	VALIDPNTR(list,NULL);
	VALIDPNTR(func,NULL);
	
	LList* newlst = llnew();
	VALIDPNTR(newlst, NULL);

	Node* curr = list->head;
	while(curr)
	{
 		llappend(newlst, (*func)(curr->data));
		curr = curr->next;	
	}	

	return newlst;
}

LList* llfilter(LList* list, char (*func)(void* data))
{
	VALIDPNTR(list,NULL);
	VALIDPNTR(func,NULL);
	
	LList* newlst = llnew();
	VALIDPNTR(newlst,NULL);

	Node* curr = list->head;
	while(curr)
	{
		if((*func)(curr->data)) llappend(newlst, curr->data);
		curr = curr->next;	
	}	

	return newlst;
}
