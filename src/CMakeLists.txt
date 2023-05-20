aux_source_directory(. SRC_LIB_SRCS)
aux_source_directory(./Tile TILE_LIB_SRCS)
aux_source_directory(./Win WIN_LIB_SRCS)
aux_source_directory(./Rules RULE_LIB_SRCS)

add_library(library ${TILE_LIB_SRCS} ${WIN_LIB_SRCS} ${RULE_LIB_SRCS})
add_executable(mahjong ${SRC_LIB_SRCS})

target_link_libraries(mahjong library)
