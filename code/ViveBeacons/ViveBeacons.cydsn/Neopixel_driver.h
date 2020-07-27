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

#if !defined(NEOPIXEL_DRIVER_H)
#define NEOPIXEL_DRIVER_H

#include "project.h"

typedef struct Neopixel_driver Neopixel_driver;
struct Neopixel_driver {
    volatile uint8_t *frame_buffer;
    uint8_t nb_led;
};

// Public methods
Neopixel_driver* Neopixel_driver_create();
void Neopixel_driver_init(Neopixel_driver *neopixel_driver, uint8_t nb_led);

void Neopixel_driver_clear(Neopixel_driver *neopixel_driver);
void Neopixel_driver_show(Neopixel_driver *neopixel_driver);
void Neopixel_driver_set_pixel_color(Neopixel_driver *neopixel_driver, uint8_t led_id, uint8_t R, uint8_t G, uint8_t B);

// Private methods

#endif

/* [] END OF FILE */