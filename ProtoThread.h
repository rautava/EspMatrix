#define THREAD_INTERVAL(__interval)                                              \
    {                                                                            \
        static unsigned long __prevMillis{millis() - (unsigned long)__interval}; \
        const unsigned long __millis{millis()};                                  \
        if (__millis - __prevMillis < __interval)                                \
        {                                                                        \
            return;                                                              \
        }                                                                        \
        __prevMillis = __millis;                                                 \
    }
