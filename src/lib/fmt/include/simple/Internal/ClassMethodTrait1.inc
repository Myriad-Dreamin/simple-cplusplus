
#ifndef assert_has_method_cls_name
#error "must define assert_has_method_cls_name"
#endif

#ifndef assert_has_method_target_cls_t
#error "must define assert_has_method_target_cls_t"
#endif

#ifndef assert_has_method_method_t
#error "must define assert_has_method_method_t"
#endif

#ifndef assert_has_method_method_name
#error "must define assert_has_method_method_name"
#endif

template <class T> struct assert_has_method_cls_name {
  using decayed_t = std::decay_t<T>;
  using method_t = assert_has_method_method_t(decayed_t);
  using cls_t = assert_has_method_target_cls_t(decayed_t);
private:
  template <typename Impl> static uint8_t test(is_match_type<method_t, &Impl::assert_has_method_method_name> *) { return uint8_t{}; }
  template <typename Impl> static uint16_t test(...) { return uint16_t{}; }
public:
  static const bool value = (sizeof(test<cls_t>(nullptr)) == sizeof(uint8_t));
};

#undef assert_has_method_cls_name
#undef assert_has_method_target_cls_t
#undef assert_has_method_method_t
#undef assert_has_method_method_name
