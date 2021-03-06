#
# Copyright (C) 2008 Doug Judd (Zvents, Inc.)
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; version 2 of
# the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.
#

set(SRC_DIR "${HYPERTABLE_SOURCE_DIR}/src/cc/Tools/dfsclient")
set(DST_DIR "${HYPERTABLE_BINARY_DIR}/src/cc/Tools/dfsclient")


set(dfsclient_SRCS
CommandCopyFromLocal.cc
CommandCopyToLocal.cc
CommandLength.cc
CommandMkdirs.cc
CommandRemove.cc
CommandRmdir.cc
CommandShutdown.cc
CommandExists.cc
)

# dfclient lib
add_library(HyperDfsCmds ${dfsclient_SRCS})
target_link_libraries(HyperDfsCmds HyperDfsBroker)

# dfsclient - command interpreter
add_executable(dfsclient dfsclient.cc)
target_link_libraries(dfsclient HyperDfsCmds)

# dfsTest
add_executable(dfsTest dfsTest.cc dfsTestThreadFunction.cc)
target_link_libraries(dfsTest HyperDfsCmds)

configure_file(${SRC_DIR}/dfsTest.golden ${DST_DIR}/dfsTest.golden COPYONLY)

add_test(HyperDfsBroker dfsTest)

install(TARGETS HyperDfsCmds dfsclient
        RUNTIME DESTINATION ${VERSION}/bin
        LIBRARY DESTINATION ${VERSION}/lib
        ARCHIVE DESTINATION ${VERSION}/lib)

