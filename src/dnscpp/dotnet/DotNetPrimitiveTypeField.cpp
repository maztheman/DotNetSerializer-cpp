#include "DotNetPrimitiveTypeField.h"

std::string DotNetPrimitiveTypeField::do_ToString() const
{
    std::stringstream ss;
    std::string s;

    std::visit([&ss](auto&& arg) -> auto {
        using ValueT = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<ValueT, CStringVector>)
        {
            for(auto& s : arg) {
                ss << s << ",";
            }
        }
        else if constexpr (std::is_same_v<ValueT, CInt32Vector>)
        {
            for(auto& s : arg) {
                ss << s << ",";
            }
        }
        else if constexpr (std::is_same_v<ValueT, nulltype>)
        {
            
        }
        else
        {
            ss << arg;
        }
    }, m_Value);

    s = ss.str();
    return s;
}