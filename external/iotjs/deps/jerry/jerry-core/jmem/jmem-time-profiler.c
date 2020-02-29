/* Copyright 2014-2016 Samsung Electronics Co., Ltd.
 * Copyright 2016 University of Szeged.
 * Copyright 2016-2019 Gyeonghwan Hong, Eunsoo Park, Sungkyunkwan University
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <sys/time.h>

#include "jmem-time-profiler.h"

#define UNUSED(x) (void)(x)

/* Time profiling */
void profile_init_times(void) {
#if defined(JMEM_SEGMENTED_HEAP) && defined(JMEM_PROFILE_TIME)
  JERRY_CONTEXT(compression_time).tv_sec = 0;
  JERRY_CONTEXT(compression_time).tv_usec = 0;
#endif
}

void profile_print_times(void) {
#if defined(JMEM_SEGMENTED_HEAP) && defined(JMEM_PROFILE_TIME)
  FILE *fp = stdout;
#ifdef JMEM_PROFILE_TIME_FILENAME
  fp = fopen(JMEM_PROFILE_TIME_FILENAME, "a");
#endif

  fprintf(fp, "Category, Alloc, Free, Compression, Decompression, GC\n");
  fprintf(fp, "Count, %u, %u, %u, %u, %u\n", JERRY_CONTEXT(alloc_count),
          JERRY_CONTEXT(free_count), JERRY_CONTEXT(compression_count),
          JERRY_CONTEXT(decompression_count), JERRY_CONTEXT(gc_count));
  fprintf(fp, "Time(Sec), %ld.ld, %ld.ld, %ld.ld, %ld.ld, %ld.ld\n",
          JERRY_CONTEXT(alloc_time).tv_sec, JERRY_CONTEXT(alloc_time).tv_usec,
          JERRY_CONTEXT(free_time).tv_sec, JERRY_CONTEXT(free_time).tv_usec,
          JERRY_CONTEXT(compression_time).tv_sec,
          JERRY_CONTEXT(compression_time).tv_usec,
          JERRY_CONTEXT(decompression_time).tv_sec,
          JERRY_CONTEXT(decompression_time).tv_usec,
          JERRY_CONTEXT(gc_time).tv_sec, JERRY_CONTEXT(gc_time).tv_usec);

#ifdef JMEM_PROFILE_TIME_FILENAME
  fflush(fp);
  fclose(fp);
#endif
#endif
}

void _check_watch(struct timeval *timer);
void _stop_watch(struct timeval *timer, struct timeval *t);

void _check_watch(struct timeval *timer) {
  gettimeofday(timer, NULL);
}
void _stop_watch(struct timeval *timer, struct timeval *t) {
  struct timeval curr_time;

  gettimeofday(&curr_time, NULL);

  curr_time.tv_usec -= timer->tv_usec;
  if (curr_time.tv_usec < 0) {
    curr_time.tv_usec += 1000000;
    curr_time.tv_sec -= 1;
  }
  curr_time.tv_sec -= timer->tv_sec;
  JERRY_ASSERT(curr_time.tv_sec >= 0 && curr_time.tv_usec >= 0);

  t->tv_sec += curr_time.tv_sec;
  t->tv_usec += curr_time.tv_usec;

  if (t->tv_usec > 1000000) {
    t->tv_sec += 1;
    t->tv_usec -= 1000000;
  }
}

void __attr_always_inline___ profile_alloc_start(void) {
#if defined(JMEM_SEGMENTED_HEAP) && defined(JMEM_PROFILE_TIME)
  JERRY_CONTEXT(alloc_count)++;
  _check_watch(&g_timeval_alloc);
#endif
}
void __attr_always_inline___ profile_alloc_end(void) {
#if defined(JMEM_SEGMENTED_HEAP) && defined(JMEM_PROFILE_TIME)
  _stop_watch(&g_timeval_alloc, &JERRY_CONTEXT(alloc_time));
#endif
}

void __attr_always_inline___ profile_free_start(void) {
#if defined(JMEM_SEGMENTED_HEAP) && defined(JMEM_PROFILE_TIME)
  JERRY_CONTEXT(free_count)++;
  _check_watch(&g_timeval_free);
#endif
}
void __attr_always_inline___ profile_free_end(void) {
#ifdef JMEM_PROFILE_TIME
  _stop_watch(&g_timeval_free, &JERRY_CONTEXT(free_time));
#endif
}

void __attr_always_inline___ profile_compression_start(void) {
#ifdef JMEM_PROFILE_TIME
  JERRY_CONTEXT(compression_count)++;
  _check_watch(&g_timeval_compression);
#endif
}
inline void __attr_always_inline___ profile_compression_end(void) {
#ifdef JMEM_PROFILE_TIME
  _stop_watch(&g_timeval_compression, &JERRY_CONTEXT(compression_time));
#endif
}

inline void __attr_always_inline___ profile_decompression_start(void) {
#ifdef JMEM_PROFILE_TIME
  JERRY_CONTEXT(decompression_count)++;
  _check_watch(&g_timeval_decompression);
#endif
}
inline void __attr_always_inline___ profile_decompression_end(void) {
#ifdef JMEM_PROFILE_TIME
  _stop_watch(&g_timeval_decompression, &JERRY_CONTEXT(decompression_time));
#endif
}

inline void __attr_always_inline___ profile_gc_start(void) {
#ifdef JMEM_PROFILE_TIME
  JERRY_CONTEXT(gc_count)++;
  _check_watch(&g_timeval_gc);
#endif
}
inline void __attr_always_inline___ profile_gc_end(void) {
#ifdef JMEM_PROFILE_TIME
  _stop_watch(&g_timeval_gc, &JERRY_CONTEXT(gc_time));
#endif
}