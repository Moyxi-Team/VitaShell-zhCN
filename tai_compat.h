#ifndef TAI_COMPAT_H
#define TAI_COMPAT_H

#include <taihen.h>

/*
 * 涓嶄緷璧?taihen 澶存枃浠堕噷 TAI_CONTINUE 鐨勫叿浣撳啓娉曪紝鑷灞曞紑 hook 閾俱€? *
 * 鑳屾櫙锛? *   - 鍚勭増鏈?taihen.h 閲?TAI_CONTINUE 鐨勬寚閽堣浆鎹㈠啓娉曚笉鍚岋紝鏈夌殑鍐欐垚 (type (*)(void))锛? *     鍦?GCC 14 榛樿鐨?-std=gnu23 涓嬩細鎶?"too many arguments to function"銆? *   - tai_hook_ref_t 鏄?uintptr_t锛堟暣鏁帮級锛岀洿鎺ュ己杞垚缁撴瀯浣撴寚閽堜細瑙﹀彂
 *     -Wint-conversion锛圙CC 14 璧峰湪 C 璇█涓涓洪敊璇級銆? *   鍥犳杩欓噷鐢?__builtin_memcpy 鍦ㄦ暣鏁颁笌鎸囬拡涔嬮棿鎼瓧鑺傦紝閬垮厤鏁村瀷/鎸囬拡杞崲鍛婅銆? *
 * 鐢ㄦ硶涓?TAI_CONTINUE 涓€鑷达細TAI_CONTINUE_ARG(杩斿洖绫诲瀷, hook_ref, 鍙傛暟...)
 */
#define TAI_CONTINUE_ARG(type, hook, ...)                          \
  ({                                                               \
    tai_hook_ref_t ref_ = (hook);                                   \
    struct _tai_hook_user *cur_ = 0, *next_ = 0;                    \
    __builtin_memcpy(&cur_, &ref_, sizeof(cur_));                   \
    __builtin_memcpy(&next_, &cur_->next, sizeof(next_));            \
    (next_ == 0) ? ((type (*)())cur_->old)(__VA_ARGS__)             \
                 : ((type (*)())next_->func)(__VA_ARGS__);          \
  })

#endif /* TAI_COMPAT_H */
