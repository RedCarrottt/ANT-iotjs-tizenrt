/* Copyright 2016-2020 Gyeonghwan Hong, Eunsoo Park, Sungkyunkwan University
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

#ifndef JMEM_HEAP_SEGMENTED_TRANSLATION_H
#define JMEM_HEAP_SEGMENTED_TRANSLATION_H

#include "jrt.h"

extern uint8_t *sidx_to_addr(uint32_t sidx);
extern uint32_t addr_to_saddr_and_sidx(uint8_t *addr, uint8_t **saddr_out);

#endif /* !defined(JMEM_HEAP_SEGMENTED_TRANSLATION_H) */