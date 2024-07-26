extern int (*utl_logger) (const char *__restrict __format, ...);

#ifdef DEBUG
#define LOG_D(...)                                                             \
  utl_logger("DEBUG | %s::%s:%d:    ", __FILE__, __func__, __LINE__),          \
      utl_logger(__VA_ARGS__), utl_logger("\n")
#else
#define LOG_D(...)
#endif
#define LOG_W(...)                                                             \
  utl_logger("WARN  | %s::%s:%d:    ", __FILE__, __func__, __LINE__),          \
      utl_logger(__VA_ARGS__), utl_logger("\n")
#define LOG_E(...)                                                             \
  utl_logger("ERROR | %s::%s:%d:    ", __FILE__, __func__, __LINE__),          \
      utl_logger(__VA_ARGS__), utl_logger("\n")
