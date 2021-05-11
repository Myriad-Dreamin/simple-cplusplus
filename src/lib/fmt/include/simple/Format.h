//
// Created by kamiyoru on 2021/5/11.
//

#ifndef SIMPLE_CPLUSPLUS_FORMAT_H
#define SIMPLE_CPLUSPLUS_FORMAT_H

#include "FormatDispatcher.h"
#include "FormatAction.h"
#include "FormatAdaptor.h"
#include "FormatDecls.h"

#include <cassert>
#include <utility>
#include <vector>

namespace sim {
namespace format_internal {

template <typename... Args>
void format(ostream_t in, const char *fmt, Args &&... args) {
  std::vector<FormatAction> actions;
  if (!parseFmt(actions, fmt)) {
    return;
  }
  auto wrapped_args =
      std::make_tuple(FormatDispatcher::create(args)...);
  auto runtime_args_array =
      createRuntimeArgs(wrapped_args, std::index_sequence_for<Args...>());
  for (auto &action : actions) {
    if (action.idx < runtime_args_array.size()) {
      runtime_args_array[action.idx]->format(in, action);
    } else {
      writeImpl(in, action.origin);
    }
  }
}

} // namespace format_internal

template <typename... Args> void format(const char *fmt, Args &&... args) {
  format_internal::format(stdout, fmt, std::forward<Args>(args)...);
}

template <typename T> auto align(uint8_t a, T &&item) {
  return format_internal::Align::Arguments<T>(
      format_internal::Align::Style::Right, ' ', a, std::forward<T>(item));
}

template <typename T> auto red(T &&item) {
  return format_internal::Colorize::Arguments<T>(
      format_internal::Colorize::Color::Red, std::forward<T>(item));
}

template <typename T> auto yellow(T &&item) {
  return format_internal::Colorize::Arguments<T>(
      format_internal::Colorize::Color::Yellow, std::forward<T>(item));
}

} // namespace sim

#endif // SIMPLE_CPLUSPLUS_FORMAT_H
