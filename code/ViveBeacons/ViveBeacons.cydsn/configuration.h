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

#if !defined(CONFIGURATION_H)
#define CONFIGURATION_H

#include <cytypes.h>

// Lighthouse beacon axis names
#define J_AXIS 0
#define K_AXIS 1

#define SQRT_2 1.41421356237
#define SQRT_3_2 0.8660254038 //sqrt(3)/2 for trigo

// Sweeping axis
#define HORIZONTAL_AXIS K_AXIS
#define VERTICAL_AXIS J_AXIS

/*// Beacon coordinates (removed, now it's configured at startup)
#define BEACON_COORD_X ((left_side == 1) ? coord_x_left : coord_x_right)
#define BEACON_COORD_Y ((left_side == 1) ? coord_y_left : coord_y_right)
#define BEACON_COORD_Z ((left_side == 1) ? coord_z_left : coord_z_right)

#define BEACON_COORD_ALPHA 0
#define BEACON_COORD_BETA CY_M_PI/2
#define BEACON_COORD_GAMMA 0*/

// LEDs height
#define LED_COORD_HEIGHT 0

// Helps for tracker offsets table
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#endif 

/* [] END OF FILE */
