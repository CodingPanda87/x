#include "x.h"

using namespace x;

Result divide(cI32& a, cI32& b) {
    if (b == 0) {
        return Result(1, "b is zero");
    }
    return Result::SetData(a / b);
}

str pack_info(Struct &user){
    return _fmt("\nauthor info:\nname \t= {}, \nage \t= {}, \ngit \t= {}",
                user.get<str>("name"), 
                user.get<i32>("age"), 
                user.get<str>("git"));
}

int main() {
    _prtln("******************************************");
    _prt("Hi , x-lib ! ");
    _prtln("^_^");
    _prtln("nice to meet you , {}", _build_time());
    _prtln("******************************************");

    u32 a = 198, b = 3;
    _prtln("a / b = {}", divide(a, b).dat1<i32>());
    b = 0;
    if(!divide(a, b).ok())
        _prtln("error: {}", divide(a, b).message());

    Struct user = {
        {"name", _s("codingPanda")},
        {"age", 30},
        {"git", _s("git@github.com:CodingPanda87/x.git")}
    };
    _prtln("{}\n", pack_info(user));
}
