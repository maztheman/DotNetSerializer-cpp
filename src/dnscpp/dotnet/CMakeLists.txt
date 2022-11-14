SET(SRCS 
    DotNetAssembly.cpp
    DotNetClass.cpp
    DotNetField.cpp
    DotNetFileReader.cpp
    DotNetPrimitiveTypeField.cpp
    FindDotNetClass.cpp
    FindDotNetAssembly.cpp
    MemberReferenceField.cpp
    UserClassField.cpp #should rename thijs to DotNetUserClassField
)

add_subdirectory(serialization)

target_sources(dnscpp PRIVATE ${SRCS})