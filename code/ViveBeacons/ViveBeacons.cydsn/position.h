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
