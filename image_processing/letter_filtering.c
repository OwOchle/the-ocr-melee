#include "objects_detection.h"
#include "utils/linked_list.h"
#include "utils/shapes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

linkedList* filter_shapes(linkedList* shapes){
    long pixel_sum = 0;
    long shape_count = 0;
    linkedList* filtered_shapes = list_create();
    Node *elm = shapes->head;
    while (elm->next != NULL)
    {
        if (elm->x != -42 && elm->y != -42)
        {
            shape_count++;
            ShapeBoundingBox* shape_box = get_shape_boundings(elm->shape);
            int width = shape_box->max_x - shape_box->min_x;
            int height = shape_box->max_y - shape_box->min_y;



            double ratio =  (double)((shape_box->max_y-shape_box->min_y) / (double)(shape_box->max_x-shape_box->min_x));
            double area = (shape_box->max_y-shape_box->min_y) * (shape_box->max_x-shape_box->min_x);

            long pixel_c =  1700;
            pixel_sum += pixel_c;
            if ((ratio >= 0.5 && ratio <= 2) && (pixel_c >= 1500 && pixel_c <= 4500)){
                list_append_shape(filtered_shapes, elm->shape);
                
            }
            
        }
        elm = elm->next;
    }

    printf("Mean pixel count : %li\n", pixel_sum/shape_count);

    return filtered_shapes;
}
