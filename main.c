#include <allocator.hpp>

int main() {
    void* x = malloc(100);
    void* y = calloc(10, 10);
    x = realloc(x, 500);
    free(x);
    free(y);
    return 0;
}
