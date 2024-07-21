#define ASSIGN_IF_ZERO(val, def)                                               \
  do {                                                                         \
    ((val) = ((val) == 0) ? (def) : (val))                                     \
  } while (0)
