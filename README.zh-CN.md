# VitaShell 2.16 简体中文汉化版

> [English README](README.md)

本项目是基于 **theheroGAC 的非官方 VitaShell 2.16**（官方 2.02 已停止更新）整理的**简体中文完整汉化版**，并提供 GitHub Actions 自动构建，直接产出可安装的 `VitaShell.vpk`。

维护：Moyxi-Team ｜ 仓库地址：<https://github.com/Moyxi-Team/VitaShell-zhCN>

---

## 一、这个版本做了什么

VitaShell 官方与常见分发包的简体中文翻译只覆盖部分界面，缺键会回落到英文，另有大量提示语直接写死在 C 代码里，无法随语言切换。本版本做了三类改动：

| 类别 | 文件 | 说明 |
| --- | --- | --- |
| 语言表补全 | `l10n/chinese_s.txt` | 共 **235 条**语言键，全部为简体中文，无空项、无英文残留键 |
| 补齐缺失键 | `language.h` | 新增上游原本没有的键：`FREE_SPACE`、`PROCESSING`、`BATTERY_INFO_MSG`、`LOW_BATTERY_WARNING_MSG`、`AUDIO_REPEAT_ON`、`AUDIO_REPEAT_OFF` |
| 去硬编码 | `main.c`、`io_process.c`、`settings.c` | 把写死在代码中的英文提示（剩余空间、处理中、电池信息、低电量警告、音频循环等）改为走语言表，随系统语言切换 |

对照原生 2.16 的功能特性（触摸屏操作、OTG 支持修复等）全部保留，未删减功能。

## 二、功能特性（来自上游 2.16）

- **完整触摸屏支持**：仅前置触摸面板，精确点选、拖动惯性滚动、双击直接打开、右侧双击呼出上下文菜单、设置中可开关触摸
- **滚动优化**：无抖动无回弹，可完整滚到列表首尾
- **OTG 支持修复**：任意 OTG 线缆可用（内核模块将 0x700 功率档降到 0x200）
- 文件管理器、VPK/ZIP 安装、内置 FTP、文本/十六进制编辑器、图片查看、音频播放、QR 扫描等原有功能

## 三、安装与使用

### 获取 VPK

两种途径，任选其一：

1. **GitHub Actions 产物**：仓库 Actions 页面 → 最新一次成功构建 → Artifacts 里下载 `VitaShell-zhCN-vpk`
2. **Releases**：如已发布，直接在 Releases 下载

### 安装注意（重要）

- **不要用 VitaShell 自己安装自己的 VPK**，会出现文件占用冲突。请用 **molecularShell** 或 **ONEMenu** 安装。
- 或者直接覆盖：把新的 `eboot.bin` 复制到 `ux0:app/VITASHELL/eboot.bin`。
- **覆盖安装会冲掉 `ux0:VitaShell/language/` 下已有的语言文件**，安装完请重新放入 `chinese_s.txt`。

### 让中文生效

1. 把 `l10n/chinese_s.txt` 放到掌机 `ux0:VitaShell/language/chinese_s.txt`（UTF-8 编码，不要带 BOM）
2. 打开 VitaShell → 设置 → 语言，选择简体中文（`chinese_s`）
3. 系统语言本身是简体中文时，VitaShell 会自动加载该文件

## 四、自行构建

### 方式一：GitHub Actions（推荐）

推送任意改动到 `master` 分支即自动触发；也可以在 Actions 页面手动 `Run workflow`。

### 方式二：本地 Docker

```bash
docker run --rm -v "$PWD":/build -w /build vitasdk/vitasdk:latest /bin/bash -c '
  set -e
  for pkg in xz libarchive quirc taihen; do vdpm "$pkg"; done
  cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
  cmake --build build -j"$(nproc)"
'
```

产物：`build/VitaShell.vpk`、`build/eboot.bin`。

**构建避坑**：`vdpm` 安装依赖与 `cmake` 编译必须放在**同一个容器进程内**执行。如果分两次 `docker run --rm`，安装的包会随容器销毁而丢失，编译会退回到镜像自带的旧版 `taihen.h`，报错：

```
error: too many arguments to function '(SceUID (*)(void))cur->old'
```

原因是旧版 `TAI_CONTINUE` 宏展开成无参形式 `(type(*)(void))`，新版展开成 `(type(*)())`，允许传参。装上新版 taihen 即消除。

### 方式三：本地 VitaSDK

安装 [VitaSDK](https://github.com/vitasdk) 后，用 `vdpm` 装齐 `xz libarchive quirc taihen`，然后 `cmake -S . -B build && cmake --build build -j`。

## 五、目录说明

| 路径 | 内容 |
| --- | --- |
| `modules/kernel` | 内核模块（挂载重定向、OTG 功率、安全模式伪装） |
| `modules/patch` | 补丁模块 |
| `modules/usbdevice` | USB 设备模块 |
| `modules/user` | 用户态模块 |
| `l10n/` | 各语言文件，简体中文为 `chinese_s.txt` |
| `resources/` | 主题、图标、字体等资源 |
| `.github/workflows/build.yml` | 自动构建流水线 |

## 六、已知问题

- 少数由第三方库直接输出的报错文本仍为英文（如解压库、QR 库内部信息），这部分不走语言表
- PS TV 的 USB 挂载流程未做完整回归测试
- 触摸相关操作建议在真机验证，模拟器表现可能不同

## 七、致谢

- **TheFloW** — VitaShell 原作者
- **theheroGAC** — 2.16 非官方版作者（触摸屏实现与多语言增强）
- **TheRealYoti** — 代码贡献，**isage** — QR 扫描修复
- 各语言译者与 Beta 测试者
- **VitaSDK** 团队与所有开源贡献者

## 八、许可与免责

本项目沿用上游 **GPLv3** 许可，详见 [LICENSE](LICENSE)。

这是非官方修改版，刷机与覆盖系统应用均有风险，请先备份数据，自行承担使用后果。
