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

#if !defined(POSITION_FINDER_H)
#define POSITION_FINDER_H

#include <cytypes.h>
#include <stdbool.h>
#include "project.h"
#include "VIVE_sensors.h"
#include "position.h"

// Attributes
typedef struct Position_finder Position_finder;
struct Position_finder {
    Position3D* beacon_position;
    Position2D* current_position;
    Position2D* previous_position;
    VIVE_sensors_data* vive_sensors_data_v;
    VIVE_sensors_data* vive_sensors_data_h;
    double tracker_led_offset[8][2];
    double led_height;
    uint16_t computation_time;
};

// Public methods
Position_finder* Position_finder_create();
void Position_finder_init(Position_finder *position_finder, Position3D* beacon_position, double led_height, double tracker_led_offset[8][2]);
void Position_finder_find_position(Position_finder *position_finder, VIVE_sensors_data* vive_sensors_data);

// Utils
double Position_finder_normalize_angle(double angle);
int Position_finder_compare(void const *a, void const *b); // For median filter

#endif

/* [] END OF FILE */
