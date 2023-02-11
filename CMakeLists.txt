cmake_minimum_required(VERSION 3.14)

project (parsepacket)

add_executable(parsepacket 
    parsepacket.cxx
)
target_compile_features(parsepacket PRIVATE cxx_std_17)