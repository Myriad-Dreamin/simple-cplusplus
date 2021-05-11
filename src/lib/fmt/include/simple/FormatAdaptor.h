//
// Created by kamiyoru on 2021/5/11.
//

#ifndef SIMPLE_CPLUSPLUS_FORMATADAPTOR_H
#define SIMPLE_CPLUSPLUS_FORMATADAPTOR_H

#include "FormatDecls.h"
#include <cassert>
#include <windows.h>

namespace sim::format_internal {

#define declareFormatImpl(T)                                                   \
  void performFormatActionWithItemImpl(                                        \
      ostream_t in, const FormatAction &action, T item)
declareFormatImpl(int64_t);
declareFormatImpl(uint64_t);
#undef declareFormatImpl

template <typename T>
struct FormatAdaptor<T, std::enable_if_t<std::is_integral_v<T>>> {

  using item_underlying_type =
      std::remove_all_extents_t<std::remove_reference_t<T>>;

  using Args = T;

  static void format(const Args &args, ostream_t in, FormatAction &action) {
    if constexpr (std::is_unsigned_v<item_underlying_type>) {
      performFormatActionWithItemImpl(in, action, uint64_t(args));
    } else if constexpr (std::is_signed_v<item_underlying_type>) {
      performFormatActionWithItemImpl(in, action, int64_t(args));
    } else {
      static_assert(("format action not implemented", false));
    }
  }
};

struct Align {
  enum Style {
    None,
    Left,
    Center,
    Right,
  };

  struct Base {};
  template <typename A> struct Arguments : public Align::Base {
    const A &item;
    union Opt {
      struct {
        uint16_t not_used : 14;
        uint16_t style : 2;
        uint8_t ch;
        uint8_t align;
      };
      uint32_t opts;
    } opts;
    explicit Arguments(Style style, uint8_t ch, uint8_t align, const A &item)
        : opts{}, item(item) {
      opts.align = align;
      opts.ch = ch;
      opts.style = style;
    }
  };
};

template <template <typename> class inferring, typename A>
struct FormatAdaptor<

    inferring<A>,
    std::enable_if_t<std::is_base_of_v<Align::Base, inferring<A>>>> {
  using Args = Align::Arguments<A>; // argument::Align<T>;

  static void format(const Args &args, ostream_t in, FormatAction &action) {
    assert(action.type == FormatActionType::Format);
    action.opts = args.opts.opts | ((Args::Opt *)(&action.opts))->not_used;
    FormatDispatcher::create(args.item).format(in, action);
  }
};

struct Colorize {
  enum Color {
    None,
    Red,
    Yellow,
  };

  struct Base {};
  template <typename A> struct Arguments : public Colorize::Base {
    const A &item;
    uint32_t color;
    explicit Arguments(uint32_t color, const A &item)
        : color(color), item(item) {}
  };
};

template <template <typename> class inferring, typename A>
struct FormatAdaptor<
    inferring<A>,
    std::enable_if_t<std::is_base_of_v<Colorize::Base, inferring<A>>>> {
  using Args = Colorize::Arguments<A>; // argument::Align<T>;

  static void format(const Args &args, ostream_t in, FormatAction &action) {
    // simple
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (args.color) {
    case Colorize::Color::Red:
      SetConsoleTextAttribute(hConsole, 0x0c);
      break;
    case Colorize::Color::Yellow:
      SetConsoleTextAttribute(hConsole, 0x0e);
      break;
    default:
      break;
    }

    FormatDispatcher::create(args.item).format(in, action);
    SetConsoleTextAttribute(hConsole, 0x07);
  }
};

} // namespace sim::format_internal

#endif // SIMPLE_CPLUSPLUS_FORMATADAPTOR_H
