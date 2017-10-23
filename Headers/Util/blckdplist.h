#ifndef BLCKDPLIST_H
#define BLCKDPLIST_H

#include "../main.h"

void instance_bp_list(int bp_id, int bp_amount);

int add_to_bp_list(int bp_id, int pid);

void del_from_bp_list(int bp_id, int pid);

int get_bp_at(int bp_id, int position);

#endif // BLCKDPLIST_H