#include <twocoord.h>

int main(void)
{
    int i = 10;
    int *ip = &i;


    void *a = new_o(object);
    print_o(a);
    void *b = new_o(twocoord, 4, 6);
    print_o(b);

    printf("object size - %ld\n", get_size_o(a));
    printf("twocoord size - %ld\n", get_size_o(b));

    printf("twocoord x is %d\n", get_x(b));
    printf("twocoord y is %d\n", get_y(b));
    set_x(b, 10);
    set_y(b, 13);
    printf("twocoord x is %d\n", get_x(b));
    printf("twocoord y is %d\n", get_y(b));
    
    void *c = new_o(twocoord, 10, 13); 
    printf("compare result - %d\n", compare_o(b, c));
    delete_o(&a);
    delete_o(&b);
    delete_o(&c);
}