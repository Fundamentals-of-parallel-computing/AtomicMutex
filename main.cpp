#include <counter.h>

int main() {
    Counter(10000).atomic().print();
    Counter(10000).mutex().print();
    Counter(10000).nosync().print();
    return 0;
}
