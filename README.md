# moebius-openbmc-virtlab

为 QEMU 虚拟 Romulus 实例逐步构建 BMC 固件扩展的长期项目。

## 目录结构

```
meta-moebius/   自定义 Yocto 层——所有扩展（bb/bbappend、配置、服务）都在这里
scripts/        构建与部署辅助脚本
docs/           项目笔记与开发记录（官方文档另见 ~/myopenbmc/docs）
```

## 把 meta-moebius 挂进现有构建

在现有 romulus 构建目录的 `conf/bblayers.conf` 中追加一行：

```
BBLAYERS ?= " \
  ...
  /home/moebius/myopenbmc/moebius-openbmc-virtlab/meta-moebius \
  "
```

## 构建与运行（参考流程）

```bash
cd ~/openbmc-workspace/openbmc
source oe-init-build-env build/romulus
bitbake obmc-phosphor-image
runqemu romulus nographic
```

## 开发流程

见 [docs/](docs/) 与官方文档 [development/](https://github.com/openbmc/docs/tree/master/development)。
