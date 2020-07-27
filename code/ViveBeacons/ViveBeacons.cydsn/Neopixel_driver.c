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

#include "Neopixel_driver.h"

Neopixel_driver* Neopixel_driver_create() {
    Neopixel_driver* neopixel_driver = (Neopixel_driver *) malloc(1*sizeof(Neopixel_driver));
    
    neopixel_driver->frame_buffer = NULL;
    neopixel_driver->nb_led = 0;
    
    return neopixel_driver;
}

void Neopixel_driver_init(Neopixel_driver *neopixel_driver, uint8_t nb_led) {
    neopixel_driver->frame_buffer = NeoPixel_1_Start(nb_led, NULL, 12);
    neopixel_driver->nb_led = nb_led;
}

void Neopixel_driver_clear(Neopixel_driver *neopixel_driver) {
    memset(neopixel_driver->frame_buffer, 0, 3*(neopixel_driver->nb_led));
}

void Neopixel_driver_show(Neopixel_driver *neopixel_driver) {
    (void) neopixel_driver; // suppress warning
    NeoPixel_1_Update();
}

void Neopixel_driver_set_pixel_color(Neopixel_driver *neopixel_driver, uint8_t led_id, uint8_t R, uint8_t G, uint8_t B) {
    // GRB buffer, soooo ....
    neopixel_driver->frame_buffer[0+3*led_id] = G;
    neopixel_driver->frame_buffer[1+3*led_id] = R;
    neopixel_driver->frame_buffer[2+3*led_id] = B;
}