//
// Created by kamiyoru on 2021/5/11.
//

#ifndef SIMPLE_CPLUSPLUS_FORMATDISPATCHER_H
#define SIMPLE_CPLUSPLUS_FORMATDISPATCHER_H

#include "FormatDecls.h"

#include <type_traits>
#include <array>

namespace sim::format_internal {
struct FormatDispatcher {
  struct Base {
    virtual void format(ostream_t in, FormatAction &action) = 0;
  };

  template <typename T> class Impl : public Base {
    using item_underlying_type =
        std::remove_all_extents_t<std::remove_reference_t<std::decay_t<T>>>;
    T item;

  public:
    explicit Impl(T &&item) : item(std::forward<T>(item)) {}
    void format(ostream_t in, FormatAction &action) override {
      FormatAdaptor<item_underlying_type>::format(
          std::forward<T>(item), in, action);
    }
  };

  template <typename T> static Impl<T> create(T &&item) {
    return Impl<T>(std::forward<T>(item));
  }
};

template <typename TupleT, std::size_t... TupleIndex>
std::array<FormatDispatcher::Base *, std::tuple_size<TupleT>::value>
createRuntimeArgs(TupleT &tuple_ins, std::index_sequence<TupleIndex...>) {
  return {{&std::get<TupleIndex>(tuple_ins)...}};
}
} // namespace sim::format_internal

#endif // SIMPLE_CPLUSPLUS_FORMATDISPATCHER_H
