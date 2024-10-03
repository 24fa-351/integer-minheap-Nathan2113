
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "heap.h"

#define KEY_NOT_PRESENT -1

heap_t *heap_create(int capacity) {
    
    // Allocating memory to heap
    heap_t* heap = (heap_t*)malloc(sizeof(heap));

    // Checking if memory allocation was successful
    if(heap == NULL) {
        printf("Memory error\n");
        return NULL;
    }

    // Set the values to size and capacity
    heap->size = 0;
    heap->capacity = capacity;

    // Allocating memory to array
    heap->data = (heap_node_t*)malloc(capacity * sizeof(int)); // THIS MIGHT NEED TO CHANGE

    // Checking if memory allocation was successful
    if(heap->data == NULL) {
        printf("Memory error\n");
        return NULL;
    }

    return heap;
}

void heap_free(heap_t *heap) {
    if(heap == NULL) {
        return;
    }

    free(heap->data);
    free(heap);
}

unsigned int heap_size(heap_t *heap) { return heap->size; }

unsigned int heap_parent(unsigned int index) {
    if(index == 0) {
        return 0;
    }

    return floor((index - 1) / 2);
}

unsigned int heap_left_child(unsigned int index) { return index * 2 + 1; }

unsigned int heap_right_child(unsigned int index) { return index * 2 + 2;}

unsigned int heap_level(unsigned int index) {
    int level = 0;

    while(index > 0) {
        index = (index - 1) / 2; // Move to the parent
        level++;
    }

    return level;

    // return ((int)(floor(log2(index + 1))) % 2) == 0;
}

void heap_print(heap_t *heap) {
    for (int ix = 0; ix < heap_size(heap); ix++) {
        printf("%3d - %3d : " HEAP_KEY_FORMAT "\n", heap_level(ix), ix,
               heap->data[ix].key);
    }
    printf("\n");
}

void heap_swap(heap_t *heap, int index1, int index2) {
    heap_key_t temp_key;
    heap_value_t temp_value;

    temp_key = heap->data[index1].key;
    temp_value = heap->data[index1].value;

    heap->data[index1].key = heap->data[index2].key;
    heap->data[index1].value = heap->data[index2].value;

    heap->data[index2].key = temp_key;
    heap->data[index2].value = temp_value;
}

void heap_bubble_up(heap_t *heap, int index) {
    if(heap == NULL || index <= 0) {
        return;
    }

    int parent_index = heap_parent(index);

    // While the current node is less than its parent
    while(index > 0 && heap->data[index].key < heap->data[parent_index].key) {
        // Swap the current node with its parent
        heap_node_t temp = heap->data[index];
        heap->data[index] = heap->data[parent_index];
        heap->data[parent_index] = temp;

        // Move up to the parent's index
        index = parent_index;
        parent_index = heap_parent(index);
    }
}

void heap_bubble_down(heap_t *heap, int index) {
    if(heap == NULL || index < 0 || index >= heap->size) {
        return;
    }

    int smallest = index;
    int left_index = heap_left_child(index);
    int right_index = heap_right_child(index);

    // Check if left child exists and is smaller than current node
    if(left_index < heap->size && heap->data[left_index].key < heap->data[smallest].key) {
        smallest = left_index;
    }

    // Check if right child exists and is smaller than current node
    if(right_index < heap->size && heap->data[right_index].key < heap->data[smallest].key) {
        smallest = right_index;
    }


    // If the smallest is not the current node, swap and continue bubbling down
    if(smallest != index) {
        // Swap current node with smallest child
        heap_node_t temp = heap->data[index];
        heap->data[index] = heap->data[smallest];
        heap->data[smallest] = temp;

        // Recursively bubble down the affected subtree
        heap_bubble_down(heap, smallest);
    }
}

void heap_insert(heap_t *heap, heap_key_t key, heap_value_t data) {
    if (heap_size(heap) == heap->capacity) {
        return;
    }

    heap->data[heap_size(heap)].key = key;
    heap->data[heap_size(heap)].value = data;
    heap->size++;

    heap_bubble_up(heap, heap_size(heap) - 1);
}


heap_value_t heap_remove_min(heap_t *heap) {
    if (heap_size(heap) == 0) {
        return (heap_value_t){.as_int = KEY_NOT_PRESENT};
    }

    heap_value_t min = heap->data[0].value;

    heap->size--;

    // move last element to the root
    heap->data[0] = heap->data[heap_size(heap)];

    // then bubble it down to its correct position
    heap_bubble_down(heap, 0);

    return min;
}
