#define ALLOC_TRACKER_CPP

#include "alloc_tracker.h"

AllocTracker GlobalTracker = {};

#include <errno.h>

void attach_allocation(AllocTracker* tracker, Allocation* allocation) {
    if (tracker->last_allocation) {
        tracker->last_allocation->next = allocation;
        allocation->prev = tracker->last_allocation;
    }
    tracker->last_allocation = allocation;
}

void dealloc_all(AllocTracker* tracker) {
    while (tracker->last_allocation) {
        dealloc(tracker->last_allocation);
        tracker->last_allocation = tracker->last_allocation->prev;
    }
}

void dealloc_specific(AllocTracker* tracker, const void* address) {
    while (tracker->last_allocation->prev && tracker->last_allocation->address != address) {
        tracker->last_allocation = tracker->last_allocation->prev;
    }
    if (tracker->last_allocation->address == address) {
        dealloc(tracker->last_allocation);
        if (tracker->last_allocation->prev)
            tracker->last_allocation->prev->next = tracker->last_allocation->next;
        if (tracker->last_allocation->next)
            tracker->last_allocation->next->prev = tracker->last_allocation->prev;
    }
    while (tracker->last_allocation->next) {
        tracker->last_allocation = tracker->last_allocation->next;
    }
}

void dealloc(Allocation* allocation) {
    if (allocation->address) {
        allocation->destructor(allocation->address);
        allocation->address = NULL;
    }
}

void free_variable(void** variable) {
    free(*variable);
    *variable = NULL;
}