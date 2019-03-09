/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "configuration.h"
#include "Position_finder.h"

/*
    --------------
    Public methods
    --------------
*/

/*
    ---------------------------------------------------------------------------
    Name : Create
    Description : Create an Position finder "object".
    ---------------------------------------------------------------------------
*/
    
Position_finder* Position_finder_create() {
    Position_finder* position_finder = (Position_finder*) malloc(1*sizeof(Position_finder));
    
    position_finder->current_position = (Position2D*) malloc(1*sizeof(Position2D));
    position_finder->previous_position = (Position2D*) malloc(1*sizeof(Position2D));
    
    return position_finder;
}

/*
    ---------------------------------------------------------------------------
    Name : Init
    Description : Init a position finder object. It set the beacon position,
    the height of the LEDs relative to the ground of the table, and the leds
    coordinates relative to the center of the tracker (the point on the tracker
    that you want to track)
    ---------------------------------------------------------------------------
*/

void Position_finder_init(Position_finder *position_finder, Position3D* beacon_position, double led_height, double tracker_led_offset[8][2]) {
    position_finder->vive_sensors_data_v = NULL;
    position_finder->vive_sensors_data_h = NULL;
    
    position_finder->beacon_position = beacon_position;
    
    position_finder->led_height = led_height;
    
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 2; j++)
            position_finder->tracker_led_offset[i][j] = tracker_led_offset[i][j];
}

/*
    ---------------------------------------------------------------------------
    Name : Find position
    Description : This function is the core of "Position finder". First of all,
    the function computes the LEDs absolute coordinates with the angles
    provided by the VIVE_sensors. The "object" keeps the last vertical and
    horizontal LEDs angles to be able to compute a position at each VIVE cycle.
    Then, the algorithm works in two steps :
        - Find the heading of the tracker. It takes every pair of LEDs that 
    responded and calculates the difference of angle between the theorical
    heading between those two points and the real one. Then perform a median
    filter to get rid of aberrant values.
        - Find the X,Y coordinates of the tracker. It takes every LEDs
    positions and get the average point, then it recalculates all positions relatives
    to this point. For each LED that responded it calculate the static angle between
    the center-to-average-point vector and the LED-to-average-point vector.
    Using rotation matrix and static lenght of the vector it then calculate
    the center-to-average-point vector, witch allow to get back the center coords.
    A median filter is is perfomed to get rid of aberrant values.
    ----------------------------------------------------------------------------
*/

void Position_finder_find_position(Position_finder *position_finder, VIVE_sensors_data* vive_sensors_data) {
    Position2D* led_positions[8];
    Position2D* position = position_finder->current_position;
    
    // Those pointers will received the address of a temporary table in the memory,
    // reallocated when there's a new value (to keep the memory footprint minimal)
    // in order to do an median filter if multiples LEDs responded.
    double *heading_values = NULL;
    double *x_values = NULL;
    double *y_values = NULL;
    
    for(int i = 0; i < 8; i++) {
        led_positions[i] = NULL;
    }

    // --- Update angle data --- 
    if(vive_sensors_data->axis == VERTICAL_AXIS) {
        free(position_finder->vive_sensors_data_v); // free old data
        position_finder->vive_sensors_data_v = vive_sensors_data;
    }
    else if(vive_sensors_data->axis == HORIZONTAL_AXIS) {
        free(position_finder->vive_sensors_data_h); // free old data
        position_finder->vive_sensors_data_h = vive_sensors_data;
    }
    
    // If one of those structures pointers are NULL, that means that we have
    // not enough correct data to compute a position
    /*if(position_finder->vive_sensors_data_h == NULL || position_finder->vive_sensors_data_v == NULL)
        return;
    */
    // --- Process LED angles : compute the absolute LEDs coordinates with the angles (trigo power !) --- 
    for(int i = 0; i < 8; i++) {
        if(isnan(position_finder->vive_sensors_data_h->angles[i]) || isnan(position_finder->vive_sensors_data_v->angles[i]))
            continue;
        
        double v_angle = position_finder->beacon_position->alpha+position_finder->vive_sensors_data_v->angles[i];
        double h_angle = position_finder->beacon_position->beta-(position_finder->vive_sensors_data_h->angles[i]);
        double l = (position_finder->beacon_position->z - position_finder->led_height)*tan(CY_M_PI/2-fabs(v_angle));
        
        Position2D* current_led_position = (Position2D*) malloc(1*sizeof(Position2D));
        current_led_position->x = position_finder->beacon_position->x + cos(h_angle)*l;
        current_led_position->y = position_finder->beacon_position->y + sin(h_angle)*l;
        led_positions[i] = current_led_position;
    }
    
    /* --- Compute heading --- */
    double sum = 0;
    int nb_pair = 0;
    
    // For each pairs of LEDs
    for(int i = 0; i < 8; i+=2) {
        for(int j = i+2; j < 8; j+=2) {
            if(led_positions[i] == NULL || led_positions[j] == NULL) // If they responded
                continue; // If not, continue to the next pair
            
            // And if their coordinates are valid
            if(isnan(led_positions[i]->x) || isnan(led_positions[i]->y) || isnan(led_positions[j]->x) || isnan(led_positions[j]->y)) 
                continue; // If not, continue to the next pair
            
            // Compute a possible heading for each pair of LEDs : possible_heading = (applied_angle - theorical_angle)
            double applied_delta_x = led_positions[j]->x - led_positions[i]->x;
            double applied_delta_y = led_positions[j]->y - led_positions[i]->y;
            double applied_angle = atan2(applied_delta_y, applied_delta_x);
            
            double theorical_delta_x = position_finder->tracker_led_offset[j][X_AXIS] - position_finder->tracker_led_offset[i][X_AXIS];
            double theorical_delta_y = position_finder->tracker_led_offset[j][Y_AXIS] - position_finder->tracker_led_offset[i][Y_AXIS];           
            double theorical_angle = atan2(theorical_delta_y, theorical_delta_x);
            
            nb_pair++;
            
            // Add to heading_values table
            heading_values = (double*) realloc(heading_values, nb_pair*sizeof(double));
            heading_values[nb_pair-1] = Position_finder_normalize_angle(applied_angle - theorical_angle);
        }
    }
    
    if(nb_pair > 1) { // If more than 1 pair of LEDs responded, then do a MEDIAN FILTER
        // -- Median filter --
        qsort(heading_values, nb_pair, sizeof(double), Position_finder_compare); // sort
        
        if((nb_pair % 2) == 0) // If length of heading_values is even, compute the average between the 2 values in the center
            position->a = (heading_values[(int) floor(nb_pair/2.0)] + heading_values[(int) ceil(nb_pair/2.0)]) / 2;
        else // Otherwise, take the center value
            position->a = heading_values[nb_pair/2];
    } else if(nb_pair == 1) { // If only 1 pair of LEDs responded, then we can only trust it
        position->a = heading_values[0];
    } else // ... Worse case, nothing good
        position->a = NAN;
    
    // --- Compute X,Y position ---
    
    //set-up : changing calculs origin, making list of working leds, working on offsets
    double medium_x = 0;
    double medium_y = 0;
    int nb_leds = 0;
    int valid_leds[8] = {0};
    for(int i = 0; i < 8; i++) { // For each LEDs
        if(isnan(led_positions[i]->x) || isnan(led_positions[i]->y)){ continue; } //if the led is valid
        medium_x += led_positions[i]->x;
        medium_y += led_positions[i]->y;
        valid_leds[i] = 1; //remember for later
        nb_leds++;
    }
    
    medium_x /= nb_leds; //medium point
    medium_y /= nb_leds;
    
    for(int i = 0; i < 8; i++) {
        if (valid_leds[i]) { //if the leds are valid
            led_positions[i]->x -= medium_x;    //recenter them around the medium point
            led_positions[i]->y -= medium_y;
        }
    }
    
    double cst_medium_x = 0;
    double cst_medium_y = 0;
    for(int i = 0; i <8; i++) {
        if (valid_leds[i]) { //if the leds are valid
            cst_medium_x = position_finder->tracker_led_offset[i][X_AXIS];
            cst_medium_y = position_finder->tracker_led_offset[i][Y_AXIS];
        }
    }
    
    cst_medium_x /= nb_leds;  //medium point in offset coords
    cst_medium_y /= nb_leds;
    
    const double medium_module = sqrt(cst_medium_x*cst_medium_x + cst_medium_y*cst_medium_y); 
    
    //allocation of the table to recieve results, note that nno reallocation is necessary here because we already now how much leds there is
    x_values = (double*) malloc(nb_leds*sizeof(double));
    
    //calcul part
    for (int i =0; i< 8; i++) {
        if (valid_leds[i]) {
            //TODO: setup circular coords for offset leds positions
            
            //setup calculs
            const double point_module = sqrt(position_finder->tracker_led_offset[i][X_AXIS]*position_finder->tracker_led_offset[i][X_AXIS]
                                             +position_finder->tracker_led_offset[i][Y_AXIS]*position_finder->tracker_led_offset[i][Y_AXIS]);
            const double offset_factor = medium_module/point_module;
            
            const double point_angle = atan2(position_finder->tracker_led_offset[i][Y_AXIS],position_finder->tracker_led_offset[i][X_AXIS]);
            const double medium_point_angle = CY_M_PI - point_angle;
            
            const double cos_medium_point_angle = cos(medium_point_angle);
            const double sin_medium_point_angle = sin(medium_point_angle);
            
            //final calcul: this is where the magic appends
            const double center_point_x = medium_x + cos_medium_point_angle * led_positions[i]->x + sin_medium_point_angle * led_positions[i]->y; 
            const double center_point_y = medium_x - sin_medium_point_angle * led_positions[i]->x - cos_medium_point_angle * led_positions[i]->y;
            
            //saving the results
            x_values[i] = center_point_x;
            y_values[i] = center_point_y;
        }
    }    
        
    if(nb_leds > 1) { // If more than 1 LED responded, then do a MEDIAN FILTER
        // -- Median filter --
        qsort(x_values, nb_leds, sizeof(double), Position_finder_compare); // sort
        qsort(y_values, nb_leds, sizeof(double), Position_finder_compare);
            
        if((nb_leds % 2) == 0) { // If length of heading_values is even, compute the average between the 2 values in the center
            position->x = (x_values[(int) floor(nb_leds/2.0)] + heading_values[(int) ceil(nb_leds/2.0)]) / 2;
            position->y = (y_values[(int) floor(nb_leds/2.0)] + heading_values[(int) ceil(nb_leds/2.0)]) / 2;
        } else { // Otherwise, take the center value
            position->x = x_values[nb_leds/2];
            position->y = y_values[nb_leds/2];
        }
    } else if(nb_pair == 1) { // If only 1 pair of LEDs responded, then we can only trust it
            position->x = x_values[0];
            position->y = y_values[0];
        } else { // ... Worse case, nothing good
            position->x = NAN;
            position->y = NAN;
        }
    
    // --- Free dynamic allocated temporary data ---
    for(int i = 0; i < 8; i++) {
        Position2D* current_led_position = led_positions[i];
        free(current_led_position);
    }
    
    free(heading_values);
    free(x_values);
    free(y_values);
}

/*
    ---------------------------------------------------------------------------
    Name : Normalize angle
    Description : Normalize angles (add or substract enough 2*PI to get angles
    between -PI and PI)
    ---------------------------------------------------------------------------
*/

double Position_finder_normalize_angle(double angle) {
    while(angle < -CY_M_PI)
        angle += 2*CY_M_PI;
    
    while(angle > CY_M_PI)
        angle -= 2*CY_M_PI;
    
    return angle;
}

/*
    ---------------------------------------------------------------------------
    Name : Compare
    Description : Comparison function for qsort(). Here is a simple function to
    compare numbers.
    ---------------------------------------------------------------------------
*/

int Position_finder_compare(void const *a, void const *b)
{
    int ret = 0;
    double const *pa = a;
    double const *pb = b;
    double diff = *pa - *pb;
    
    if (diff > 0)
        ret = 1;
    else if (diff < 0)
        ret = -1;
    else
        ret = 0;

    return ret;
}
/* [] END OF FILE */
