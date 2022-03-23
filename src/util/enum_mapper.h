//
// Created by saajan on 3/16/22.
//

#ifndef NOCAP_CLION_ENUM_MAPPER_H
#define NOCAP_CLION_ENUM_MAPPER_H

#include <string_view>
#include <unordered_map>

using namespace std;

constexpr size_t SPLIT(char const *str, char sep)
{
    std::size_t ret{1u};

    while (*str)
        if (sep == *str++)
            ++ret;

    return ret;
}

constexpr size_t LENGTH(string_view a)
{
    return a.size() + 1;
}

template <size_t N, size_t M, typename ENUMTYPE, const ENUMTYPE... enums>
struct ENUM_MAPPER
{
    constexpr ENUM_MAPPER(const char (&enumstr)[N]) : emap(), smap()
    {
        size_t start = 0, end = 0;

        str = enumstr;
        const size_t count = sizeof...(enums);

        array<ENUMTYPE, count> named_enums = {enums...};

        for (size_t i = 0; i < N && end != string_view::npos; i++)
        {
            end = str.find_first_of(',', start);
            emap[i].first = named_enums[i];
            emap[i].second = string_view(str.substr(start, end - start));

            smap[i].first = emap[i].second;
            smap[i].second = emap[i].first;

            start = end + 2;
        }
    };
    string_view str;
    pair<ENUMTYPE, string_view> emap[M];
    pair<string_view, ENUMTYPE> smap[M];
};

#define MAPPED_SUBSET(ENUMTYPE, ...) ENUM_MAPPER<LENGTH(#__VA_ARGS__), SPLIT(#__VA_ARGS__, ','), ENUMTYPE, __VA_ARGS__>(#__VA_ARGS__);

#define MAPPED_ENUM(ENUMTYPE, ...) \
  enum ENUMTYPE                    \
  {                                \
    __VA_ARGS__,                   \
    ENUMTYPE##_COUNT               \
  };                               \
  constexpr ENUM_MAPPER ENUMTYPE##_MAP = ENUM_MAPPER<LENGTH(#__VA_ARGS__), SPLIT(#__VA_ARGS__, ','), ENUMTYPE, __VA_ARGS__>(#__VA_ARGS__);

#endif //NOCAP_CLION_ENUM_MAPPER_H
