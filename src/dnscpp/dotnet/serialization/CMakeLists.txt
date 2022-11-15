set(SRCS
    Array.cpp
    ArrayFiller8b.cpp
    ArrayItem.cpp
    ArrayOfObject.cpp
    ArrayOfPrimitive.cpp
    ArrayOfString.cpp
    Assembly.cpp
    Class.cpp
    End.cpp
    Field.cpp
    MemberReference.cpp
    Primitive.cpp
)

target_sources(dnscpp PRIVATE ${SRCS})
