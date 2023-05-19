aux_source_directory(. SRC_LIB_SRCS) # 添加所有 .cpp 文件
aux_source_directory(./Tile TILE_LIB_SRCS)
aux_source_directory(./Win WIN_LIB_SRCS)

add_library(tile ${TILE_LIB_SRCS})
add_library(win ${WIN_LIB_SRCS})

add_executable(mahjong ${SRC_LIB_SRCS}) # 指定生成目标

target_link_libraries(mahjong tile win)
