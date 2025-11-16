add_library(header INTERFACE)
target_include_directories(header INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/dram_bf
    ${CMAKE_CURRENT_SOURCE_DIR}/dram_bbf
    ${CMAKE_CURRENT_SOURCE_DIR}/dram_ohbbf
    ${CMAKE_CURRENT_SOURCE_DIR}/rdma_bf
    ${CMAKE_CURRENT_SOURCE_DIR}/rdma_bbf
    ${CMAKE_CURRENT_SOURCE_DIR}/rdma_ohbbf
    # ${CMAKE_CURRENT_SOURCE_DIR}/temp_bf
)


add_library(murmur STATIC "common/murmur3.cpp")
target_link_libraries(murmur PUBLIC header)
target_compile_options(murmur PRIVATE -Ofast)

add_library(utils STATIC "common/utils.cpp")
target_link_libraries(utils PUBLIC header)
target_compile_options(utils PRIVATE -Ofast)


add_library(drambf STATIC "dram_bf/dram_bf.cpp")
target_link_libraries(drambf PUBLIC header murmur)
target_compile_options(drambf PRIVATE -Ofast)

add_library(drambbf STATIC "dram_bbf/dram_bbf.cpp")
target_link_libraries(drambbf PUBLIC header murmur)
target_compile_options(drambbf PRIVATE -Ofast)

add_library(dramohbbf STATIC "dram_ohbbf/dram_ohbbf.cpp")
target_link_libraries(dramohbbf PUBLIC header murmur)
target_compile_options(dramohbbf PRIVATE -Ofast)

# add_library(temp_wormhole_bf STATIC "temp_bf/bloomfilter.cpp")
# target_link_libraries(temp_wormhole_bf PUBLIC header murmur)
# target_compile_options(temp_wormhole_bf PRIVATE -Ofast)

if (TOGGLE_RDMA)

    add_library(rdmabf STATIC "rdma_bf/rdma_bf.cpp")
    target_link_libraries(rdmabf PUBLIC header murmur)
    target_compile_options(rdmabf PRIVATE -Ofast)

    add_library(rdmabbf STATIC "rdma_bbf/rdma_bbf.cpp")
    target_link_libraries(rdmabbf PUBLIC header murmur)
    target_compile_options(rdmabbf PRIVATE -Ofast)

    add_library(rdmaohbbf STATIC "rdma_ohbbf/rdma_ohbbf.cpp")
    target_link_libraries(rdmaohbbf PUBLIC header murmur)
    target_compile_options(rdmaohbbf PRIVATE -Ofast)

endif()