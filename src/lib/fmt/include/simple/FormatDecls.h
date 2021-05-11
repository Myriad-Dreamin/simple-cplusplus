//
// Created by kamiyoru on 2021/5/11.
//

#ifndef SIMPLE_CPLUSPLUS_FORMATDECLS_H
#define SIMPLE_CPLUSPLUS_FORMATDECLS_H

#include "FormatAction.h"

#include <cstdio>
#include <type_traits>

namespace sim::format_internal {

using ostream_t = FILE *;

static inline void writeImpl(ostream_t in, std::string_view str) {
  for (auto &s : str) {
    fputc(s, in);
  }
}

struct FormatAction;

template <typename T, typename Require=void> class FormatAdaptor {};

template<typename T>
using format_adaptor_method_t = void (*)(const T&value, ostream_t in, FormatAction &action);

template <typename T, T> struct is_match_type;

#define assert_has_method_cls_name assert_has_format_adaptor_method
#define assert_has_method_target_cls_t(T) FormatAdaptor<T>
#define assert_has_method_method_t(T) format_adaptor_method_t<T>
#define assert_has_method_method_name format
#include <simple/Internal/ClassMethodTrait1.inc>

} // namespace sim::format_internal

#endif // SIMPLE_CPLUSPLUS_FORMATDECLS_H
