#include "fast_mem_equals.h"
#include <unordered_map>

int main()
{
    std::unordered_map<std::string, std::string, std::hash<std::string>, fast_string_equal_to> k;
    k["param_1"] = "value_1";
    k["param_2"] = "value_2";

    auto p1 = k["param_1"];
    printf("%s\n", p1.c_str());

    return 0;
}

