//Keep printf (for development telemetry) and keep malloc (at least initially), 
//the standard solution is adding a syscalls.c that provides _write and _sbrk.
//syscalls.c (newlib / GCC), UART-backed
//Src/rotor-fault-detection-c/syscalls.c. This version routes printf to a HAL UART handle.


// syscalls.c - newlib glue for bare-metal STM32
// Routes _write() to HAL UART, provides _sbrk() for malloc.

#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>

// Include the correct HAL header for STM32N6 series in the STM32 project.
#include "stm32n6xx_hal.h"

// Select the UART used for printf output.
// In CubeMX, enable a USART mapped to ST-LINK VCP (or any UART wired to a header),
// then ensure the handle name matches here.
extern UART_HandleTypeDef huart3;

int _close(int file) {
  (void)file;
  return -1;
}

int _fstat(int file, struct stat *st) {
  (void)file;
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file) {
  (void)file;
  return 1;
}

int _lseek(int file, int ptr, int dir) {
  (void)file;
  (void)ptr;
  (void)dir;
  return 0;
}

int _read(int file, char *ptr, int len) {
  (void)file;
  (void)ptr;
  (void)len;
  return 0;
}

int _write(int file, char *ptr, int len) {
  (void)file;
  if (len <= 0) return 0;

  // Blocking transmit. For higher throughput, switch to DMA or ITM/RTT.
  HAL_UART_Transmit(&huart3, (uint8_t *)ptr, (uint16_t)len, HAL_MAX_DELAY);
  return len;
}

int _kill(int pid, int sig) {
  (void)pid;
  (void)sig;
  errno = EINVAL;
  return -1;
}

int _getpid(void) {
  return 1;
}

// Heap implementation for malloc/free.
// CubeIDE/GCC typically defines these linker symbols.
extern uint8_t _end;     // end of .bss/.data
extern uint8_t _estack;  // top of stack

void *_sbrk(ptrdiff_t incr) {
  static uint8_t *heap_end;
  uint8_t *prev_heap_end;

  if (heap_end == NULL) heap_end = &_end;

  prev_heap_end = heap_end;

  // Simple stack collision check.
  if ((heap_end + incr) >= (&_estack)) {
    errno = ENOMEM;
    return (void *)-1;
  }

  heap_end += incr;
  return (void *)prev_heap_end;
}
