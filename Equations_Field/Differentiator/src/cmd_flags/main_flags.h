#define GET_WRAPPER(name) __wrapper_##name

{ {'O', "owl"}, { {}, 0, print_owl },
    "print 10 owls to the screen." },

{ {'I', ""}, { GET_WRAPPER(log_threshold), 1, edit_int },
    "set log threshold to the specified number.\n"
    "\tDoes not check if integer was specified." },

{ {'D', ""}, { GET_WRAPPER(differentiation_power), 1, edit_int },
    "set maximum equation differentiation power.\n"
    "\tDoes not check if integer was specified." },

{ {'S', ""}, { GET_WRAPPER(series_power), 1, edit_int },
    "set max power of X to build the series to.\n"
    "\tDoes not check if integer was specified." },

{ {'P', ""}, { GET_WRAPPER(series_point), 1, edit_double },
    "set point at which to build the series.\n"
    "\tDoes not check if double was specified." },
