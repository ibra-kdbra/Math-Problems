#ifndef ALLOC_TRACKER_H
#define ALLOC_TRACKER_H

#include "stdlib.h"

typedef void dtor_t(void* subject);
struct Allocation {
    void* address = NULL;
    dtor_t* destructor = NULL;
    Allocation* next = NULL;
    Allocation* prev = NULL;
};

struct AllocTracker {
    Allocation* last_allocation = NULL;
};

/**
 * @brief Attack allocation to the given tracker.
 * 
 * @param tracker 
 * @param allocation 
 */
void attach_allocation(AllocTracker* tracker, Allocation* allocation);

/**
 * @brief Deallocate all tracked allocations.
 * 
 * @param tracker 
 */
void dealloc_all(AllocTracker* tracker);

/**
 * @brief Deallocate specified address.
 * 
 * @param tracker 
 * @param address 
 */
void dealloc_specific(AllocTracker* tracker, const void* address);

/**
 * @brief Deallocate stored allocation.
 * 
 * @param allocation 
 */
void dealloc(Allocation* allocation);

/**
 * @brief Free variable and set its value to NULL.
 * 
 * @param variable variable to free
 */
void free_variable(void** variable);

#define start_local_tracking() AllocTracker __local_alloc_tracker = {}

#define __construct_name_impl(name, line) __allocation##name##line
#define __construct_name(name) __construct_name_impl(name, __LINE__)

#define __construct_allocation(variable, destruct)      \
    Allocation __construct_name(variable) = {           \
        .address = &variable,                           \
        .destructor = (dtor_t*) destruct,               \
    }

#define track_allocation(variable, destructor)          \
    __construct_allocation(variable, destructor);       \
    attach_allocation(&__local_alloc_tracker,           \
        &__construct_name(variable))

#define free_all_allocations() dealloc_all(&__local_alloc_tracker)

#define deallocate(address) dealloc_specific(&__local_alloc_tracker, address)

/**
 * @brief Return value but clean all allocations first.
 * 
 */
#define return_clean(value) do { free_all_allocations(); return value; } while (0)

#endif