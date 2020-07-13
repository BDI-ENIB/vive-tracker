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

#if !defined(POSITION_H)
#define POSITION_H

#include <cytypes.h>

// Position optimisation
#define POS_SERIALIZE_DEFAULT(pos) POS_SERIALIZE(pos, 11, 12, 9) 
#define POS_SERIALIZE(pos, x_bits, y_bits, a_bits) (uint32_t) ( ((((uint32_t) pos->x) & ((1 << (x_bits)) - 1)) << (y_bits + a_bits)) | ((((uint32_t) pos->y) & ((1 << (y_bits)) - 1)) << a_bits) | ((((uint32_t) pos->a) & ((1 << (a_bits)) - 1)) << 0) )

#define POS_UNSERIALIZE_DEFAULT(pos, pos_s) POS_UNSERIALIZE(pos, pos_s, 11, 12, 9)
#define POS_UNSERIALIZE(pos, pos_s, x_bits, y_bits, a_bits) pos->x = (double) (((pos_s->i) & (((1 << (x_bits)) - 1) << (y_bits + a_bits))) >> (y_bits + a_bits)); pos->y = (double) (((pos_s->i) & (((1 << (y_bits)) - 1) << a_bits)) >> a_bits); pos->a = (double) (((pos_s->i) & (((1 << (a_bits)) - 1) << 0)) >> 0)

// Position serialize
typedef union Position_serialized Position_serialized;
union Position_serialized { // Western Union
    uint8_t c[4];
    uint32_t i;
};

typedef union Float_serialized Float_serialized;
union Float_serialized { // Western Union
    uint8_t c[4];
    float f;
};

// Position2D structure : Represent a position on a plane
typedef struct Position2D Position2D;
struct Position2D {
    double x;
    double y;
    
    double a;
};

// Position structure : Represent a position in a 3D space
typedef struct Position3D Position3D;
struct Position3D {
    double x;
    double y;
    double z;
    
    double alpha;
    double beta;
    double gamma;
};

#endif

/* [] END OF FILE */
