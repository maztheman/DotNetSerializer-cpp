SET(SRCS 
    DotNetAssembly.cpp
    DotNetClass.cpp
    DotNetField.cpp
    DotNetFileReader.cpp
    MemberReferenceField.cpp
    UserClassField.cpp #should rename thijs to DotNetUserClassField
)

target_sources(dnscpp PRIVATE ${SRCS})