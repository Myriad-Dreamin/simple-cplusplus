//
// Created by kamiyoru on 2021/5/11.
//

#ifndef SIMPLE_CPLUSPLUS_FORMATACTION_H
#define SIMPLE_CPLUSPLUS_FORMATACTION_H

#include <vector>
#include <cstdint>
#include <string_view>

namespace sim::format_internal {

struct FormatAction;

using FmtContainer = std::vector<FormatAction>;
bool parseFmt(FmtContainer &container, std::string_view fmt);

namespace FormatActionType {
enum {
  Unknown,
  Literal,
  Format,
};
}

struct FormatAction {
  std::string_view origin;
  uint32_t opts;
  uint16_t type;
  uint16_t idx;

  static constexpr auto fixed = std::numeric_limits<decltype(idx)>::max();

  FormatAction(std::string_view origin, uint16_t type, uint16_t idx)
      : origin(origin), type(type), idx(idx), opts{} {}
};

static inline FormatAction createLiteralAction(std::string_view origin) {
  return FormatAction(origin, FormatActionType::Literal, FormatAction::fixed);
}

static inline FormatAction createFormatAction(uint16_t idx) {
  return FormatAction("", FormatActionType::Format, idx);
}

} // namespace sim::format_internal

#endif // SIMPLE_CPLUSPLUS_FORMATACTION_H
