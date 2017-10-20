#include "../../Headers/Util/random.h"

int get_random_int(int min, int max) {
    int random_int = rand () % (max-min+1) + min;
    return random_int;
}
