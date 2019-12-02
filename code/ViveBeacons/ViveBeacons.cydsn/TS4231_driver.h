/* ============================================================================
 *
 *  Copyright (C) 2019 - Evan Roué
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * ============================================================================
*/

#if !defined(TS4231_DRIVER_H)
#define TS4231_DRIVER_H

#include <cytypes.h>
#include <stdbool.h>
#include "project.h"

// Defines
#define BUS_DRIVE_DELAY 1 // in microseconds
#define BUS_CHECK_DELAY 500 // in microseconds
#define SLEEP_RECOVERY 100 // in microseconds

#define DEFAULT_CONFIGURATION_VALUE 0x392B
    
#define MAX_COUNTER_VALUE 65535
#define TIMEOUT_LENGTH 1000

// Enums
typedef enum State State;
enum State {S0, SLEEP, WATCH, S3};

typedef enum Status Status;
enum Status {UNKNOWN, BUS_FAIL, VERIFY_FAIL, WATCH_FAIL, CONFIG_PASS};

// Attributes
typedef struct TS4231_driver TS4231_driver;
struct TS4231_driver {
    uint32_t enveloppe_pin;
    uint32_t data_pin;
    State state;
};

// Public methods
TS4231_driver* TS4231_driver_create(uint32_t enveloppe_pin, uint32_t data_pin);
bool TS4231_driver_init(TS4231_driver* ts4231_driver);
bool TS4231_driver_go_to_watch(TS4231_driver* ts4231_driver);
bool TS4231_driver_go_to_sleep(TS4231_driver* ts4231_driver);
void TS4231_driver_delete(TS4231_driver* ts4231_driver);

// Private methods
bool TS4231_driver_wait_for_light(TS4231_driver* ts4231_driver);
Status TS4231_driver_configure(TS4231_driver* ts4231_driver, uint16_t configuration_value);
State TS4231_driver_check_bus(TS4231_driver* ts4231_driver);
void TS4231_driver_write_configuration(TS4231_driver* ts4231_driver, uint16_t configuration_value);
uint16_t TS4231_driver_read_configuration(TS4231_driver* ts4231_driver);

#endif

/* [] END OF FILE */
