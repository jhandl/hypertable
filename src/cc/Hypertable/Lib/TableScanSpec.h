/** -*- c++ -*-
 * Copyright (C) 2008 Doug Judd (Zvents, Inc.)
 *
 * This file is part of Hypertable.
 *
 * Hypertable is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2 of the
 * License.
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

#ifndef HYPERTABLE_TYPES_H
#define HYPERTABLE_TYPES_H

#include <utility>
#include <vector>

#include "Common/ByteString.h"
#include "Common/String.h"

namespace Hypertable {

  /** Identifies a table and a scan spec */
  class TableScanSpec {
  public:
    TableScanSpec(const TableIdentifier &table_id, const ScanSpec &scan_spec) : m_table_id(table_id), m_scan_spec(scan_spec) { return; }
    bool operator<(const TableScanSpec &other) {
      if (m_table_id < other.m_table_id) return true;
      if (m_table_id == other.m_table_id
        && m_scan_spec < other.m_scan_spec) return true;
      return false;
    }
    bool operator==(const TableScanSpec &other) {
      return (m_table_id == other.m_table_id && m_scan_spec == other.m_scan_spec);
    }
  private:
    TableIdentifier m_table_id;
    ScanSpec m_scan_spec;
  };

} // Hypertable namespace

#endif // HYPERTABLE_REQUEST_H
