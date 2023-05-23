//
//  MmapFile.cc
//  SDKCommon
//
//  Created by li zhixuan on 2023/3/16.
//

#include "log.h"

BEGIN_NAMESPACE_BASE_LOG

static const char* log_levels[] {
    nullptr, "info", "fault", "error", nullptr,
};

Log::~Log() {
    Print(level_, src_, line_, func_);
}

Log::Log(Level level,
         const std::string& src,
         int32_t line,
         const std::string& func,
         bool is_print_console)
:level_(level),
src_(src),
line_(line),
func_(func),
is_print_console_(is_print_console)
{
//    mmap_file_.Create("");
}

void Log::Print(Level level,
                const std::string& src,
                int32_t line,
                const std::string& func) {
    std::string content = Format(log_levels[static_cast<int32_t>(level)], src, line, func, oss_.str());
//#if DEBUG
    if (is_print_console_) {
        printf("%s", content.c_str());
    }
    if (level == Level::kFault) {
//        RESUMABLE_ASSERT_DEBUG_BREAK();
    }
//#endif
    mmap_file_.Wirte(content, content.size());
}

std::string Log::Format(const std::string& tag,
                   const std::string& src,
                   int32_t line,
                   const std::string& func,
                   const std::string& content) {
    std::string all_log = tag + "|" + src + "|" + std::to_string(line) + "|" + func + "|" + content + "\n";
    return all_log;
}

END_NAMESPACE_BASE_LOG
