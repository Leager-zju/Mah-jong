aux_source_directory(. DIR_LIB_SRCS) # 添加所有 .cpp 文件
add_executable(main ${DIR_LIB_SRCS}) # 指定生成目标