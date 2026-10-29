project('storagewatcher', 'cpp', version: '0.1.0',
        default_options: ['cpp_std=c++23'])

sdbusplus_dep = dependency('sdbusplus')

# 这里只构建并安装守护进程；纯逻辑测试使用宿主机 g++ 单独运行。
executable(
    'storagewatcher',
    'main.cpp',
    dependencies: [sdbusplus_dep],
    install: true,
)
