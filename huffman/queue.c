#include "queue.h"
//functions to work with the queue. used in building a tree 
queue *pop(queue *temp, tree **result){
	*result = temp->value;
	queue *res = temp->next;
	free(temp);
	return res;
}

queue *push(queue *temp, tree *val){
	if (temp == NULL || temp->value->frequency > val->frequency){
		queue *new = malloc(sizeof(queue));
		new->next = temp;
		new->value = val;
		return new;
	}
	queue *current = temp;
	while ((current->next != NULL) && (val->frequency > current->next->value->frequency))
		current = current->next;
	queue *new = malloc(sizeof(queue));
	new->next = current->next;
	current->next = new;
	new->value = val;
	return temp;
}