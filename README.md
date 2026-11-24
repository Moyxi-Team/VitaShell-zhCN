# VitaShell 2.16 简体中文汉化版

[![Build VitaShell VPK](https://github.com/Moyxi-Team/VitaShell-zhCN/actions/workflows/build.yml/badge.svg)](https://github.com/Moyxi-Team/VitaShell-zhCN/actions/workflows/build.yml)

> [English README](README.en.md) ｜ 维护：Moyxi-Team ｜ 仓库：<https://github.com/Moyxi-Team/VitaShell-zhCN>

本项目基于 **theheroGAC 的非官方 VitaShell 2.16**（官方 2.02 早已停更）整理的**简体中文完整汉化版**，并提供 GitHub Actions 自动构建，直接产出可安装的 `VitaShell.vpk`。

**下载**：优先到 [Releases](https://github.com/Moyxi-Team/VitaShell-zhCN/releases) 拿现成 VPK；也可从 Actions 最新成功构建的 Artifacts 中取 `VitaShell-zhCN-vpk`。

---

## 一、这个版本做了什么

官方与常见分发包的简体中文翻译只覆盖部分界面，缺键会回落到英文；另有大量提示语直接写死在 C 代码里，压根不随语言切换。本版本做了三类改动：

| 类别 | 文件 | 说明 |
| --- | --- | --- |
| 语言表补全 | `l10n/chinese_s.txt`、`resources/chinese_s.txt` | 共 **234 条**语言键，全部为简体中文，无空项、无英文残留；专有名词采用「中文 + 原文」形式（如 `QR 码 (QR Code)`） |
| 补齐缺失键 | `language.h` | 新增上游原本没有的键：`FREE_SPACE`、`PROCESSING`、`BATTERY_INFO_MSG`、`LOW_BATTERY_WARNING_MSG`、`AUDIO_REPEAT_ON`、`AUDIO_REPEAT_OFF` |
| 去硬编码 | `main.c`、`io_process.c`、`settings.c`、`coredump.c` | 把写死在代码里的英文（剩余空间、处理中、电池信息、低电量警告、音频循环、QR 扫描提示、崩溃转储界面字段）改为走语言表 |

上游 2.16 的功能特性（完整触摸屏操作、OTG 支持修复、平滑滚动等）全部保留，未删减功能。

## 二、功能特性（来自上游 2.16）

- **完整触摸屏支持**：仅前置触摸面板，精确点选、拖动惯性滚动、双击直接打开、右侧双击呼出上下文菜单、设置中可开关触摸
- **滚动优化**：无抖动无回弹，可完整滚到列表首尾
- **OTG 支持修复**：任意 OTG 线缆可用（内核模块将 0x700 功率档降到 0x200）
- 文件管理器、VPK/ZIP 安装、内置 FTP、文本/十六进制编辑器、图片查看、音频播放、QR 扫描等原有功能

## 三、安装

1. **不要用 VitaShell 自己安装自己的 VPK**（会出现文件占用冲突）。请用 **molecularShell** 或 **ONEMenu** 安装。
2. 若 VitaShell 已损坏无法启动，也可以直接覆盖：把 VPK 里的 `eboot.bin` 复制到 `ux0:app/VITASHELL/eboot.bin`。
3. 安装完打开 VitaShell → **设置 → 语言** → 选 **Chinese (Simp)**，按提示重启即生效。

## 四、中文是怎么生效的（重要机制）

汉化**不是**靠 VPK 里附带一个语言文件，而是：

- 语言表 `resources/chinese_s.txt` 在编译期被编进 `eboot.bin`，作为内部默认资源（源码 `init.c` 的 `default_files` 表）。
- 每次启动时 `installDefaultFiles()` 遍历这张表：**目标文件不存在 → 从 eboot 解出写入**；**目标文件已存在且 replace=0 → 不覆盖**。

由此得出实际行为：

| 你的机器状态 | 装完 VPK 后 |
| --- | --- |
| 之前从未装过 VitaShell，或已删除 `ux0:/VitaShell/language/chinese_s.txt` | 启动即自动生成**最新内嵌中文语言表**，切到 Chinese (Simp) 就是完整汉化 |
| 之前装过原版 / 旧版汉化，`ux0:/VitaShell/language/chinese_s.txt` 已存在 | 该文件**不会被覆盖**，界面仍是旧译文；需手动删除该文件后重启（或直接用仓库里的 `l10n/chinese_s.txt` 覆盖） |

代码层的汉化（去硬编码部分）随 `eboot.bin` 生效，不需要额外文件。

## 五、常见问题：别人只安装 VPK 能体验到完整汉化吗

**分两种情况：**

- **可以**：对方设备上不存在 `ux0:/VitaShell/language/chinese_s.txt`（全新设备，或先手动删掉该文件）。此时只装 VPK、启动一次、再把语言切到 Chinese (Simp)，就是完整汉化 —— 因为最新语言表已内嵌在 eboot 里。
- **不可以**：对方之前装过 VitaShell，语言目录里已有旧的 `chinese_s.txt`（原版安装时就会生成）。按 replace=0 的规则不会覆盖，界面仍是旧译文。
  - 解决办法：装完 VPK 后删除 `ux0:/VitaShell/language/chinese_s.txt`，重启 VitaShell；或把本仓库的 `l10n/chinese_s.txt` 复制过去（UTF-8，不要带 BOM）。

所以对外分享时，**推荐随包附带 `chinese_s.txt` 并注明「装完后覆盖 ux0:/VitaShell/language/chinese_s.txt」**，这样对方无论什么初始状态都能得到完整汉化，不会出现「装了新版但文案没变」的困惑。

## 六、自行构建

### 方式一：GitHub Actions（推荐）

推送任意改动到 `master` 分支即自动触发；也可在 Actions 页面手动 `Run workflow`。

### 方式二：本地 Docker

```bash
docker run --rm -v "$PWD":/build -w /build vitasdk/vitasdk:latest /bin/bash -c '
  set -e
  for pkg in xz libarchive quirc taihen zstd; do vdpm "$pkg"; done
  cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
  cmake --build build -j"$(nproc)"
'
```

产物：`build/VitaShell.vpk`、`build/eboot.bin`。

**构建避坑**：如果遇到

```
error: too many arguments to function '(SceUID (*)(void))cur->old'; expected 0, have 3
```

这是三层原因叠加，本仓库均已处理：

1. **编译标准**：GCC 14 起默认 `-std=gnu23`，C23 规定 `f()` 等价于 `f(void)`；而 taihen 的 `TAI_CONTINUE` 宏用 `(type (*)())`（无参数表）转换函数指针，传参即被判「参数过多」。本仓库在 `CMakeLists.txt` 中显式加 `-std=gnu11`，并改用自带 `tai_compat.h` 的 `TAI_CONTINUE_ARG` 宏。
2. **依赖安装位置**：`vdpm` 装包与 `cmake` 编译必须在**同一次 `docker run` 内**完成。分两次 `--rm` 运行会让安装结果随容器销毁而丢失。
3. **libarchive 依赖 zstd**：新版 `libarchive` 内含 zstd 解压滤镜，需 `-lzstd`（由 `vdpm zstd` 提供）；该库多线程构建，还要补 `-lpthread`，否则报 `undefined reference to 'ZSTD_*'` 及 `sched_yield` / `pthread_*`。

### 方式三：本地 VitaSDK

安装 [VitaSDK](https://github.com/vitasdk) 后，用 `vdpm` 装齐 `xz libarchive quirc taihen zstd`，然后 `cmake -S . -B build && cmake --build build -j`。

## 七、目录说明

| 路径 | 内容 |
| --- | --- |
| `modules/kernel` | 内核模块（挂载重定向、OTG 功率、安全模式伪装） |
| `modules/patch` | 补丁模块 |
| `modules/usbdevice` | USB 设备模块 |
| `modules/user` | 用户态模块 |
| `l10n/` | 各语言文件，简体中文为 `chinese_s.txt`（源文件） |
| `resources/` | 编入 eboot 的默认资源，`chinese_s.txt` 需与 `l10n/` 内保持一致 |
| `.github/workflows/build.yml` | 自动构建流水线 |

## 八、已知问题

- 少数由第三方库直接输出的报错文本仍为英文（解压库、QR 库内部信息），这部分不走语言表
- PS TV 的 USB 挂载流程未做完整回归测试
- 触摸相关操作建议真机验证，模拟器表现可能不同

## 九、致谢

- **TheFloW** — VitaShell 原作者
- **theheroGAC** — 2.16 非官方版作者（触摸屏实现与多语言增强）
- **TheRealYoti** — 代码贡献，**isage** — QR 扫描修复
- 各语言译者与 Beta 测试者
- **VitaSDK** 团队与所有开源贡献者

## 十、许可与免责

本项目沿用上游 **GPLv3** 许可，详见 [LICENSE](LICENSE)。

这是非官方修改版，覆盖系统应用有风险，请先备份数据，自行承担使用后果。
