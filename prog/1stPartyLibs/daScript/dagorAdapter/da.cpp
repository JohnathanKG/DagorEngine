#include <fstream>
#include <osApiWrappers/dag_direct.h>
#include <perfMon/dag_cpuFreq.h>
#include <daScript/daScript.h>
#include <daScript/daScriptModule.h>
#include <daScript/simulate/fs_file_info.h>

void os_debug_break() {printf("unhandled exception?\n");_exit(1);}


das::smart_ptr<das::FileAccess> get_file_access( char * pak )
{
  if ( pak )
    return das::make_smart<das::FsFileAccess>(pak, das::make_smart<das::FsFileAccess>());
  return das::make_smart<das::FsFileAccess>();
}

void require_project_specific_modules()
{
  das::pull_all_auto_registered_modules();
}

void require_project_specific_debugger_modules() {}

namespace das
{
vector<void *> force_aot_stub() { return {}; }
} // namespace das
// Copyright (C) Gaijin Games KFT.  All rights reserved.