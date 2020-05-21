#if (APPLE)
  #define FUNCTION_NAME(name) _##name
#else
  #define FUNCTION_NAME(name) name
#endif

.globl FUNCTION_NAME(AtomicStore)
.globl FUNCTION_NAME(AtomicExchange)


FUNCTION_NAME(AtomicStore):
    movq %rsi, (%rdi)
    retq

FUNCTION_NAME(AtomicExchange):
    movq %rsi, %rax
    xchg %rax, (%rdi)
    retq
