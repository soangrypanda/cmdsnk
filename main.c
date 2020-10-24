#include "twocoord.h"
#include "twomap.h"
#include "map.h"

int main(void)
{
//    int i = 10;
//    int *ip = &i;


    void *a = new_o(object);
    print_o(a);
    void *b = new_o(twocoord, 4, 6);
    print_o(b);

    void *map = new_o(twomap, map1, max_objs1, w1, h1, max_w1, max_h1, no1);

    printf("object size - %ld\n", get_size_o(a));
    printf("twocoord size - %ld\n", get_size_o(b));
    printf("twomap size - %ld\n", get_size_o(map));

    printf("twocoord x is %d\n", get_x(b));
    printf("twocoord y is %d\n", get_y(b));
    set_x(b, 10);
    set_y(b, 13);
    printf("twocoord x is %d\n", get_x(b));
    printf("twocoord y is %d\n", get_y(b));
    
    void *c = new_o(twocoord, 10, 13); 
    printf("compare result - %d\n", compare_o(b, c));

    print_o(map);

    delete_o(&a);
    delete_o(&b);
    delete_o(&c);
    delete_o(&map);
}
