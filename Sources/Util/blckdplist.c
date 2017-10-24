
#include "../../Headers/Util/blckdplist.h"

void instance_bp_list(int bp_id, int bp_amount){

    /* Obtener puntero a lista de blocked processes */
    int * bp_address = shmat(bp_id, NULL, 0);

    bp_address[0] = bp_amount;

    for(int i = 1; i < MAX_BLOCKED_P+1; i++)
        bp_address[i] = -1;

}

int add_to_bp_list(int bp_id, int pid){

    /* Obtener puntero a lista de blocked processes */
    int * bp_address = shmat(bp_id, NULL, 0);

    /* Primer lugar donde encuentra un -1 */
    for(int i = 1; i < MAX_BLOCKED_P + 1; i++){
        if(bp_address[i] == -1){
            bp_address[i] = pid;
            bp_address[0] -= 1;
            return 1;
        }
    }

    return -1;

}

int del_from_bp_list(int bp_id, int pid){

    /* Obtener puntero a lista de blocked processes */
    int * bp_address = shmat(bp_id, NULL, 0);

    /* Luegar donde encuentra el pid */
    for(int i = 1; i < MAX_BLOCKED_P + 1; i++){
        if(bp_address[i] == pid){
            bp_address[i] = -1;
            bp_address[0] += 1;
            return 1;
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