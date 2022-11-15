set(SRCS
    Array.cpp
    ArrayItem.cpp
    ArrayOfObject.cpp
    ArrayOfPrimitive.cpp
    ArrayOfString.cpp
    Assembly.cpp
    Class.cpp
    Common.cpp
    End.cpp
    Field.cpp
    MemberReference.cpp
    Primitive.cpp
)

target_sources(dnscpp PRIVATE ${SRCS})
