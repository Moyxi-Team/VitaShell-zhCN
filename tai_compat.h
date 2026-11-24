#ifndef TAI_COMPAT_H
#define TAI_COMPAT_H

#include <taihen.h>

/*
 * taihen 的 TAI_CONTINUE 在不同版本里展开方式不同：
 *   旧版头文件：((type (*)(void))cur->old)(__VA_ARGS__)  —— 函数指针带 (void) 参数表，
 *               传入实参时 GCC 直接报 "too many arguments to function"
 *   新版头文件：((type (*)())cur->old)(__VA_ARGS__)      —— 无参数表，允许任意实参
 *
 * 部分 VitaSDK 镜像与 vdpm 源里的 taihen 仍是旧版（vdpm 会提示 up to date 而不升级），
 * 会让 modules/kernel 与 modules/usbdevice 编译失败。
 * 这里不依赖头文件里的宏写法，直接自行展开 hook 链，新旧 SDK 都能编译通过。
 */
#define TAI_CONTINUE_ARG(type, hook, ...)                              \
  ({                                                                   \
    struct _tai_hook_user *cur_ = (struct _tai_hook_user *)(hook);     \
    struct _tai_hook_user *next_ = cur_->next;                         \
    (next_ == NULL) ? ((type (*)())cur_->old)(__VA_ARGS__)             \
                    : ((type (*)())next_->func)(__VA_ARGS__);          \
  })

#endif /* TAI_COMPAT_H */
