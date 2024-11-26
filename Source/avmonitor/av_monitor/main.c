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

#include <ctype.h>
#include <avmonitor.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#define Trace(fmt, ...)                                 \
    do {                                                \
        fprintf(stdout, "<< " fmt "\n", ##__VA_ARGS__); \
        fflush(stdout);                                 \
    } while (0)

void ShowMenu()
{
    printf("Enter\n"
           "\tE : Send sample event\n"
           "\t? : Show help\n"
           );
}

int main()
{
    int16_t result = 0;
    avmonitor_event_t event;
    event.gstCaps = "SAMPLE CAPS";
    result = avmonitor_register_event(event);

    ShowMenu();

    int character;
    do {
        character = toupper(getc(stdin));

        switch (character) {
        case 'E': {
            avmonitor_event_t event;
            event.gstCaps = "SAMPLE CAPS";
            result = avmonitor_register_event(event);
            Trace("Register event result: %d", result);
        }
        case '?': {
            ShowMenu();
            break;
        }
        default:
            break;
        }
    } while (character != 'Q');

    Trace("Done");

    avmonitor_dispose();

    return 0;
}
