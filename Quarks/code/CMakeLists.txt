﻿#
# CMakeList.txt : CMake project for Compilers, include source and define project specific logic here.
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     
# ECHO "    @@          ________          @@     
# ECHO "    @@      /%&@@@@@@*@@ @%\      @@     
# ECHO "    @@     % (@ %% &&@@  &@@@     @@     
# ECHO "    @@    @& @@/        \%%  @    @@     
# ECHO "    @@    @ @*@          @ @ @    @@     
# ECHO "    @@    & @ @          @|  @    @@     
# ECHO "    @@    @/ @@          @%  @    @@     
# ECHO "    @@    @* &@          @ @ @    @@     
# ECHO "    @@    @| %@          @@@*@    @@     
# ECHO "    @@    @|  @\ \&&&\  /@@@ @    @@     
# ECHO "    @@     @**@@@ &\&&&\ @* @/    @@     
# ECHO "    @@      \%@ %&&@@ \&&&\       @@     
# ECHO "    @@                  \&&&\     @@     
# ECHO "    @@         Q U A R K          @@     
# ECHO "    @@                            @@     
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[COMPILER SCRIPT .......................]"
# ECHO "                                         "

cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Compilers")

# Add source to this project's executable.
add_executable (Compilers 
	"Compilers.h"		"Compilers.c"
	"Step1Coder.h"		"Step1Coder.c"		"Main1Coder.c"
  "Step2Reader.h"    "Step2Reader.c"    "Main2Reader.c"
  "Step3Scanner.c"    "Step3Scanner.h"    "Main3Scanner.c"
  "Step4Parser.c"     "Step4Parser.h"     "Main4Parser.c"
 )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Compilers PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
 
