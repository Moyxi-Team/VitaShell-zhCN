#ifndef TAI_COMPAT_H
#define TAI_COMPAT_H

#include <taihen.h>

/*
 * 不依赖 taihen 头文件里 TAI_CONTINUE 的具体写法，自行展开 hook 链。
 *
 * 背景：
 *   - 各版本 taihen.h 里 TAI_CONTINUE 的指针转换写法不同，有的写成 (type (*)(void))，
 *     在 GCC 14 默认的 -std=gnu23 下会报 "too many arguments to function"。
 *   - tai_hook_ref_t 是 uintptr_t（整数），直接强转成结构体指针会触发
 *     -Wint-conversion（GCC 14 起在 C 语言中视为错误）。
 *   因此这里用 __builtin_memcpy 在整数与指针之间搬字节，避免整型/指针转换告警。
 *
 * 用法与 TAI_CONTINUE 一致：TAI_CONTINUE_ARG(返回类型, hook_ref, 参数...)
 */
#define TAI_CONTINUE_ARG(type, hook, ...)                          \
  ({                                                               \
    tai_hook_ref_t ref_ = (hook);                                   \
    struct _tai_hook_user *cur_ = 0, *next_ = 0;                    \
    __builtin_memcpy(&cur_, &ref_, sizeof(cur_));                   \
    next_ = cur_->next;                                             \
    (next_ == 0) ? ((type (*)())cur_->old)(__VA_ARGS__)             \
                 : ((type (*)())next_->func)(__VA_ARGS__);          \
  })

#endif /* TAI_COMPAT_H */
