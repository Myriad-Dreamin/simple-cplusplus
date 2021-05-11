
#include <simple/Format.h>
#include <simple/FormatAction.h>

#include <string>

namespace sim::format_internal {
union Opt {
  struct {
    uint16_t not_used : 14;
    uint16_t style : 2;
    uint8_t ch;
    uint8_t align;
  };
  uint32_t opts;
};

void writeImplAligned(
    ostream_t in, uint8_t style, uint8_t ch, uint8_t align,
    std::string_view res) {
  switch (style) {
  case 0:
    writeImpl(in, res);
    break;
  case 1:
    writeImpl(in, res);
    while (res.size() < align) {
      fputc(ch, in);
      align--;
    }
  case 2:
    if (align > res.size()) {
      auto rh = (align + res.size() + 1) >> 1u;
      while (rh < align) {
        fputc(ch, in);
        align--;
      }
      writeImpl(in, res);
      while (res.size() < align) {
        fputc(ch, in);
        align--;
      }
    } else {
      writeImpl(in, res);
    }
    break;
  case 3:
    while (res.size() < align) {
      fputc(ch, in);
      align--;
    }
    writeImpl(in, res);
    break;
  default:
    break;
  }
}

void performFormatActionWithItemImpl(
    ostream_t in, const FormatAction &action, uint64_t item) {
  auto opt = (Opt *)(&action.opts);
  writeImplAligned(in, opt->style, opt->ch, opt->align, std::to_string(item));
}

void performFormatActionWithItemImpl(
    ostream_t in, const FormatAction &action, int64_t item) {
  auto opt = (Opt *)(&action.opts);
  writeImplAligned(in, opt->style, opt->ch, opt->align, std::to_string(item));
}

uint16_t view_to_idx(std::string_view s, uint16_t &default_idx) {
  uint16_t res = 0;
  if (s.empty()) {
    return default_idx++;
  }
  for (auto x : s) {
    res = res * 10 + x - '0';
  }
  return res;
}

bool parseFmt(FmtContainer &container, std::string_view fmt) {
  int const_part = 0, j = 0;
  uint16_t default_idx = 0;

  auto emplaceLiteralAction = [&](int i) {
    if (const_part < i) {
      container.emplace_back(
          createLiteralAction(fmt.substr(const_part, i - const_part)));
      const_part = i;
    }
  };

  for (int i = 0; i < fmt.size(); i++) {
    if (fmt[i] == '{') {
      emplaceLiteralAction(i);
      i++;
      for (j = i; j < fmt.size() && fmt[j] != '}'; j++) {
      }
      if (j == fmt.size()) {
        return false;
      }
      container.emplace_back(
          createFormatAction(view_to_idx(fmt.substr(i, j - i), default_idx)));
      i = j;
      const_part = i + 1;
    }
  }
  emplaceLiteralAction(fmt.size());

  return true;
}

} // namespace sim::format_internal
