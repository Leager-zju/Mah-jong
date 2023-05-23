aux_source_directory(. SRC_LIB_SRCS)
aux_source_directory(./Common COMMON_LIB_SRCS)
aux_source_directory(./Tile TILE_LIB_SRCS)
aux_source_directory(./Yakus YAKU_LIB_SRCS)


add_library(library ${COMMON_LIB_SRCS} ${COMMON_LIB_SRCS} ${TILE_LIB_SRCS} ${YAKU_LIB_SRCS})
add_executable(mahjong ${SRC_LIB_SRCS})

target_link_libraries(mahjong library)
