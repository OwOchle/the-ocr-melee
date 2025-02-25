#include "objects_detection.h"
#include "utils/linked_list.h"
#include "utils/shapes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdbool.h>

#include "utils/verbose.h"


bool is_shape_in_main_grid(linkedList* shape, long avg_width, long avg_height, long avg_ratio, double avg_density, int width, int height, double ratio, double density, linkedList* shapes) {
    ShapeBoundingBox* shape_box = get_shape_boundings(shape);

    bool size_match = (width >= avg_width * 0.4 && width <= avg_width * 1.6) || (height >= avg_height * 0.4 && height <= avg_height * 1.3);
    bool ratio_match = (ratio >= avg_ratio * 0.5 && ratio <= avg_ratio * 1.8);
    bool density_match = (density >= avg_density*0.2);

    bool is_in_grid = false;
    Node* elm = shapes->head;
    while (elm->next != NULL) {
        if (elm->x != -42 && elm->y != -42) {
            ShapeBoundingBox* other_box = get_shape_boundings(elm->shape);
            int dx = abs(shape_box->min_x - other_box->min_x);
            int dy = abs(shape_box->min_y - other_box->min_y);

            if (dx <= width && dy <= height) {
                is_in_grid = true;
                break;
            }
        }
        elm = elm->next;
    }

    return size_match && is_in_grid;
}



linkedList* filter_shapes(linkedList* shapes){
    long pixel_sum = 0;
    long width_sum = 0;
    long height_sum = 0;
    double ratio_sum = 0;
    long shape_count = 0;
    long area_sum = 0;

    long avg_area = -1;
    long avg_pixels = -1;
    long avg_height = -1;
    long avg_width = -1;
    long avg_ratio = -1;
    double avg_density = -1;

    linkedList* filtered_shapes = list_create();
    Node *elm = shapes->head;
    while (elm->next != NULL)
    {
        if (elm->x != -42 && elm->y != -42)
        {
            shape_count++;
            ShapeBoundingBox* shape_box = get_shape_boundings(elm->shape);
            elm->shape_bounding_box = shape_box;

            int width = shape_box->max_x - shape_box->min_x;
            int height = shape_box->max_y - shape_box->min_y;

            double ratio =  (double)((shape_box->max_y-shape_box->min_y) / (double)(shape_box->max_x-shape_box->min_x));
            double area = (shape_box->max_y-shape_box->min_y) * (shape_box->max_x-shape_box->min_x);

            long pixel_c =  get_shape_pixel_count(elm->shape);

            pixel_sum += pixel_c;
            width_sum += width;
            height_sum += height;
            if (ratio > 0.00000 && ratio < 100.0){ // remove Nan and Inf
                ratio_sum += ratio;
            }
            area_sum += area;
        }
        elm = elm->next;
    }
    avg_pixels = pixel_sum/shape_count;
    avg_height = height_sum/shape_count;
    avg_width = width_sum/shape_count;
    avg_area = area_sum/shape_count;
    avg_density = (double)(area_sum/shape_count)/(double)(pixel_sum/shape_count);
    avg_ratio = ratio_sum/(double)shape_count;

    elm = shapes->head;
    while (elm->next != NULL)
    {
        if (elm->x != -42 && elm->y != -42)
        {   
            if (elm->shape_bounding_box == NULL){
                ShapeBoundingBox* shape_box = get_shape_boundings(elm->shape);
                elm->shape_bounding_box = shape_box;
            }            
            int width = elm->shape_bounding_box->max_x - elm->shape_bounding_box->min_x;
            int height = elm->shape_bounding_box->max_y - elm->shape_bounding_box->min_y;

            double ratio =  (double)((elm->shape_bounding_box->max_y-elm->shape_bounding_box->min_y) / (double)(elm->shape_bounding_box->max_x-elm->shape_bounding_box->min_x));
            double area = (elm->shape_bounding_box->max_y-elm->shape_bounding_box->min_y) * (elm->shape_bounding_box->max_x-elm->shape_bounding_box->min_x);

            long pixel_c =  get_shape_pixel_count(elm->shape);
            double density = (double)(area/pixel_c);
            if (is_shape_in_main_grid(elm->shape, avg_width, avg_height, avg_ratio, avg_density, width, height, ratio, density, shapes)){
                list_append_shape(filtered_shapes, elm->shape);
           }
           free(elm->shape_bounding_box);
        }
        elm = elm->next;
    }

    verbose_printf("Mean pixel count : %li\n", pixel_sum/shape_count);
    verbose_printf("Mean height_sum : %li\n", height_sum/shape_count);
    verbose_printf("Mean width_sum : %li\n", width_sum/shape_count);
    verbose_printf("Mean area_sum : %li\n", area_sum/shape_count);
    verbose_printf("Mean density : %f\n", (double)(area_sum/shape_count)/(double)(pixel_sum/shape_count));
    verbose_printf("Mean ratio_sum : %f\n", ratio_sum/(double)shape_count);


    return filtered_shapes;
}

