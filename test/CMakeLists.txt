include(GoogleTest)

aux_source_directory(. TEST_LIB_SRCS) # 添加所有 .cpp 文件

add_executable(MyTests ${TEST_LIB_SRCS}) # 指定生成目标

target_link_libraries(MyTests ${GTEST_BOTH_LIBRARIES})

add_test(Test MyTests)