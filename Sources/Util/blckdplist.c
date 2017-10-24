
#include "../../Headers/Util/blckdplist.h"

void instance_bp_list(int bp_id, int bp_amount){

    int empty_val = -1;

    /* Obtener puntero a lista de blocked processes */
    void * bp_address;
    bp_address = shmat(bp_id, NULL, 0);

    /* Almacenar la cantidad de celdas vacias */
    memcpy(bp_address, &bp_amount, sizeof(int));
    bp_address += sizeof(int);

    memset(bp_address, -1, sizeof(int) * (MAX_BLOCKED_P+1));

    /* Liberar referencia */
    shmdt(bp_address);
}

int add_to_bp_list(int bp_id, int pid){

    /* Obtener puntero a lista de blocked processes */
    void * bp_address = shmat(bp_id, NULL, 0);

    /* Consultar la cantidad de espacios de bp libres */
    int bp_left;
    memcpy(&bp_left, bp_address, sizeof(int));

    if (bp_left == 0)
        return -1;

    bp_left -= 1;

    /* Disminuir la cantidad de espacios libres */
    memcpy(bp_address, &bp_left, sizeof(int));

    /* Adelantar el puntero */
    bp_address += sizeof(int);

    for (int i = 0; i < MAX_BLOCKED_P; i++){

        int current_val;
        memcpy(&current_val, bp_address, sizeof(int));

        if (current_val == -1) {
            memcpy(bp_address, &pid, sizeof(int));
            break;
        }
    }

    /* Liberar referencia */
    shmdt(bp_address);

    return 1;
}

void del_from_bp_list(int bp_id, int pid){

    /* Obtener puntero a lista de blocked processes */
    void * bp_address = shmat(bp_id, NULL, 0);

    /* Consultar la cantidad de espacios de bp libres */
    int bp_left;
    memcpy(&bp_left, bp_address, sizeof(int));

    bp_left += 1;

    /* Aumentar la cantidad de espacios libres */
    memcpy(bp_address, &bp_left, sizeof(int));

    /* Adelantar el puntero */
    bp_address += sizeof(int);

    for (int i = 0; i < MAX_BLOCKED_P; i++){

        int current_val;
        memcpy(&current_val, bp_address, sizeof(int));

        if (current_val == pid) {
            int empty = -1;
            memcpy(bp_address, &empty, sizeof(int));
            break;
        }
    }

    /* Liberar referencia */
    shmdt(bp_address);
}

int get_bp_at(int bp_id, int position){

    /* Obtener puntero a lista de blocked processes */
    void * bp_address;
    bp_address = shmat(bp_id, NULL, 0);

    /* Los procesos bloqueados empiezan desde 1 */
    if (position < 1)
        position = 1;

    /* Adelantar el puntero hasta el proceso */
    bp_address += sizeof(int);
    bp_address += sizeof(int)*(position-1);

    int ret_val;
    memcpy(&ret_val, bp_address, sizeof(int));

    /* Liberar referencia */
    shmdt(bp_address);

    return ret_val;
}