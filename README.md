# Implementation of string equality based on SSE2 intrinsics
In most cases when strings are very different strcmp() works fast enough, but when strings have the same prefix or equals
it works much longer.

```
bool fast_mem_equals(const uint8_t* pStr1, const uint8_t *pStr2, std::size_t count);
bool fast_is_equals(const std::string& s1, const std::string& s2);
```
Uses SSE intrinsics to test string equality. When strings are different there is no any benifit,
but when strings are equal it works twice faster.

## When it useful ?
In most case we compare different strings, but when we work with std::unordered_map<> most part of time strings are equal,
because good implementation of hash<> should put different strings to different buckets.

```
struct fast_string_equal_to
{
    inline bool operator()(const std::string& _Left, const std::string& _Right) const
    {
        return fast_is_equals(_Left, _Right);
    }
};
```
Fast comparator for std::unordered_map<>.

## Example
```
    std::unordered_map<std::string, std::string, std::hash<std::string>, fast_string_equal_to> k;
    k["param_1"] = "value_1";
    k["param_2"] = "value_2";

    auto p1 = k["param_1"];
    printf("%s\n", p1.c_str());
```
