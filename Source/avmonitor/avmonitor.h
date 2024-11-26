/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2020 Metrological
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#pragma once

#include <stdbool.h>
#include <stdint.h>

#undef EXTERNAL
#if defined(WIN32) || defined(_WINDOWS) || defined (__CYGWIN__) || defined(_WIN64)
#ifdef AVMONITOR_EXPORTS
#define EXTERNAL __declspec(dllexport)
#else
#define EXTERNAL __declspec(dllimport)
#pragma comment(lib, "avmonitor.lib")
#endif
#else
#define EXTERNAL __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum avmonitor_event_type {
    UNKNOWN = 0,
    CAPTURE_START = 1,
    STATE_CHANGE = 2,
    RESOLUTION_CHANGE = 3,
    SINK_STATS = 4,
    Z_ORDER_CHANGE = 5,
    EOS = 6,
    CAPS = 7,
    CAPTURE_STOP = 8
} avmonitor_event_type_t;


typedef struct avmonitor_event {
    uint64_t timeStamp;
    uint64_t id;
    avmonitor_event_type_t type;
    int stateChange; //gst using default enum type
    int16_t windowWidth;
    int16_t windowHeight;
    int8_t visible;
    int8_t zorder;
    int64_t startPTS;
    int64_t firstPTS;
    int64_t currentPTS;
    int64_t position;
    int64_t frameRenderCount;
    int64_t frameDropCount;
    int8_t fps;
    int8_t fpsMean;
    const char* gstCaps;
} avmonitor_event_t;


EXTERNAL uint32_t avmonitor_register_event(const avmonitor_event_t event);

/**
 * @brief Close the cached open connection if it exists.
 *
 */
EXTERNAL void avmonitor_dispose();

#ifdef __cplusplus
} // extern "C"
#endif
