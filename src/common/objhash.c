#include "objhash.h"

// Function to check if an ObjHash is valid (non-null)
// An ObjHash is considered valid if any of its fields are non-zero.
bool is_objhash(ObjHash obj_hash) {
    // A null ObjHash is defined as {0, 0, false}
    return (obj_hash.value != 0 || obj_hash.rotation_value != 0 || obj_hash.is_rotation_right != false);
}