#include <sys/statvfs.h>
#include <iostream>

int main()
{
    struct statvfs info {};

    // TODO：查询文件系统，把结果写入 info
    if (statvfs("/run/initramfs/rw", &info) != 0)
    {
        std::cerr << "[storageinfo] failed to read filesystem statistics"
                  << std::endl;
        return 1;
    }

    // TODO：把统计字段换算成字节并打印

    const auto totalBytes =
    static_cast<unsigned long long>(info.f_blocks) * info.f_frsize;

    const auto availableBytes =
    static_cast<unsigned long long>(info.f_bavail) * info.f_frsize;

    std::cout << "[storageinfo] total=" << totalBytes
          << " bytes available=" << availableBytes
          << " bytes"
          << std::endl;

    return 0;
}
