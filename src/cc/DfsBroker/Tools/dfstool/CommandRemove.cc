/**
 * Copyright (C) 2007 Doug Judd (Zvents, Inc.)
 * 
 * This file is part of Hypertable.
 * 
 * Hypertable is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or any later version.
 * 
 * Hypertable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "Common/Error.h"

#include "CommandRemove.h"

using namespace hypertable;

const char *CommandRemove::msUsage[] = {
  "remove <file>",
  "",
  "  This command sends a remove request for the DFS file <file>",
  "  to the DfsBroker.",
  (const char *)0
};


int CommandRemove::run() {
  if (mArgs.size() < 1) {
    cerr << "Error:  no filename supplied." << endl;
    return -1;
  }

  return mClient->Remove(mArgs[0].first);
}

