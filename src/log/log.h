#ifndef LB_BASE_LOG_PCH_H
#define LB_BASE_LOG_PCH_H

#include "../pch.h"
#include <mmap/mmap_file.h>

#define otlog_xxx(xxx) BASE_LOG::Log(xxx, __FILE_NAME__, __LINE__, __FUNCTION__, true)
#define otlog_info otlog_xxx(BASE_LOG::Log::Level::kInfo)
#define otlog_fault otlog_xxx(BASE_LOG::Log::Level::kFault)

BEGIN_NAMESPACE_BASE_LOG

#if __i386__ || __x86_64__
#define RESUMABLE_ASSERT_DEBUG_BREAK() asm("int3")
#elif __arm__
#define RESUMABLE_ASSERT_DEBUG_BREAK()                    \
  asm("mov r0, %0    \n" /* PID                        */ \
      "mov r1, 0x11  \n" /* SIGSTOP                    */ \
      "mov r12, 0x25 \n" /* syscall kill               */ \
      "svc 0x80      \n" /* software interrupt         */ \
      "mov r0, r0    \n" /* nop to pause debugger here */ \
      ::"r"(getpid())                                     \
      : "r0", "r1", "r12", "memory")
#elif __arm64__
#define RESUMABLE_ASSERT_DEBUG_BREAK()                    \
  asm("mov x0, %x0   \n" /* PID                        */ \
      "mov x1, 0x11  \n" /* SIGSTOP                    */ \
      "mov x16, 0x25 \n" /* syscall kill               */ \
      "svc 0x80      \n" /* software interrupt         */ \
      "mov x0, x0    \n" /* nop to pause debugger here */ \
      ::"r"(getpid())                                     \
      : "x0", "x1", "x16", "memory")
#else
#error "Unsupported architecture."
#endif  // __i386__ || __x86_64__

class Log {
    
public:
    enum class Level {
        kInfo = 1,
        kFault,
        kError,
    };
    
    virtual ~Log();
    Log() = default;
    Log(Level level,
        const std::string& src,
        int32_t line,
        const std::string& func,
        bool is_print_console);
    
    template<typename T>
    Log& operator<<(const T& content) {
        oss_ << std::hex << content;
        return *this;
    }
    
private:
    void Print(Level level,
               const std::string& src,
               int32_t line,
               const std::string& func);
    
    std::string Format(const std::string& tag,
                       const std::string& src,
                       int32_t line,
                       const std::string& func,
                       const std::string& content);
    
    static void MmapPrint(const std::string& content);
    
private:
    std::ostringstream oss_;
    
    bool is_print_console_;
    Level level_;
    std::string src_;
    int32_t line_;
    std::string func_;
};

END_NAMESPACE_BASE_LOG

#endif /* LB_BASE_LOG_PCH_H */
