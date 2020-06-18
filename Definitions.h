#pragma once

/**
*  GRID SIZE
*/
#define REALMZ_GRID_SIZE 32
#define TEXTURE_ATLAS_MAX_WIDTH 20
#define IMGOBJ_MAX_IMGS 4
#define UI_THING_OBJ_LABELS_MAX_CHAR 10
#define MAP_COUNT_LEVELS 8

namespace def{
    /**
     * ENUM IMG SIZES.
     */
    enum class IMG_SIZE : int {
        IMG_SIZE_32X32, /**< enum IMG_SIZE_32X32  */
        IMG_SIZE_32X64, /**< enum IMG_SIZE_32X64  */
        IMG_SIZE_64X32, /**< enum IMG_SIZE_64X32  */
        IMG_SIZE_64X64, /**< enum IMG_SIZE_64X64  */
    };

    enum ReturnMsg {
        FILE_OK,
        FILE_NOT_SELECTED,
        IMAGE_EMPTY,
        NOT_ENOUGH_MEMORY,
        PIXELBUF_INVALID_SIZE,
        ARRAY_OUT_OF_RANGE

    };
}