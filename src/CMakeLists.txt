# 添加所有 .cpp 文件
aux_source_directory(. DIR_LIB_SRCS)

# 指定生成目标
add_executable(main ${DIR_LIB_SRCS})