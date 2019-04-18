#include <iostream>
#include "gflags/gflags.h"

DEFINE_bool(verbose, false, "Display program name before message");
DEFINE_string(message, "Hello world!", "Message to print");

static bool IsNonEmptyMessage(const char *flagname, const std::string &value)
{
  return value[0] != '\0';
}
DEFINE_validator(message, &IsNonEmptyMessage);

int main(int argc, char *argv[])
{
  gflags::SetUsageMessage("some usage message");
  gflags::SetVersionString("1.0.0");
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  if (FLAGS_verbose) std::cout << gflags::ProgramInvocationShortName() << ": ";
  std::cout << FLAGS_message << std::endl;
  gflags::ShutDownCommandLineFlags();
  return 0;
}
