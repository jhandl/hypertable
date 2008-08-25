/**
 * Copyright (C) 2007 Luke Lu (Zvents, Inc.)
 *
 * This file is part of Hypertable.
 *
 * Hypertable is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License.
 *
 * Hypertable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef HYPERTABLE_VERSION_H
#define HYPERTABLE_VERSION_H

#include <cstdio>

// version macros for detecting header/library mismatch
#define HT_VERSION_MAJOR        0
#define HT_VERSION_MINOR        9
#define HT_VERSION_MICRO        0
#define HT_VERSION_PATCH        9
#define HT_VERSION              "Hypertable 0.9.0.9 (release tarball)"

namespace Hypertable {
  extern const int version_major;
  extern const int version_minor;
  extern const int version_micro;
  extern const int version_patch;

  extern const char *version();

  // must be inlined for version check
  inline void check_version() {
    if (version_major != HT_VERSION_MAJOR ||
        version_minor != HT_VERSION_MINOR ||
        version_micro != HT_VERSION_MICRO ||
        version_patch != HT_VERSION_PATCH) {
      std::fprintf(stderr, "Hypertable header/library version mismatch:\n"
                   " header: %s\nlibrary: %s\n", HT_VERSION, version());
      std::exit(1);
    }
  }
}

#endif // HYPERTABLE_VERSION_H
