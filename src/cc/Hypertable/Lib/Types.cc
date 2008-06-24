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

#include "Common/Compat.h"
#include <cstring>
#include <iostream>

#include "AsyncComm/CommBuf.h"
#include "Common/Serialization.h"

#include "Types.h"

using namespace std;
using namespace Hypertable;
using namespace Serialization;

size_t TableIdentifier::encoded_length() const {
  return 8 + encoded_length_vstr(name);
}

void TableIdentifier::encode(uint8_t **bufp) const {
  encode_vstr(bufp, name);
  encode_i32(bufp, id);
  encode_i32(bufp, generation);
}

void TableIdentifier::decode(const uint8_t **bufp, size_t *remainp) {
  HT_TRY("decoding table identitier",
    name = decode_vstr(bufp, remainp);
    id = decode_i32(bufp, remainp);
    generation = decode_i32(bufp, remainp));
}

size_t RangeSpec::encoded_length() const {
  return encoded_length_vstr(start_row) + encoded_length_vstr(end_row);
}

void RangeSpec::encode(uint8_t **bufp) const {
  encode_vstr(bufp, start_row);
  encode_vstr(bufp, end_row);
}

void RangeSpec::decode(const uint8_t **bufp, size_t *remainp) {
  HT_TRY("decoding range spec",
    start_row = decode_vstr(bufp, remainp);
    end_row = decode_vstr(bufp, remainp));
}


ostream &Hypertable::operator<<(ostream &os, const TableIdentifier &tid) {
  os <<"{TableIdentifier: name='"<< tid.name <<"' id='" << tid.id
     <<"' generation='"<< tid.generation <<"'}";
  return os;
}

ostream &Hypertable::operator<<(ostream &os, const RangeSpec &range) {
  os <<"{RangeSpec:"; 

  if (range.start_row == 0)
    os <<" start=[NULL]";
  else
    os <<" start='"<< range.start_row <<"'";

  if (range.end_row == 0)
    os <<" end=[NULL]";
  else
    os <<" end='"<< range.end_row <<"'";

  os <<'}';

  return os;
}


std::ostream &Hypertable::operator<<(std::ostream &os, const ScanSpec &scanSpec) {
  os << "RowLimit    = " << scanSpec.row_limit << endl;
  os << "MaxVersions = " << scanSpec.max_versions << endl;
  os << "Columns     = ";
  for (std::vector<const char *>::const_iterator iter = scanSpec.columns.begin(); iter != scanSpec.columns.end(); iter++)
    os << *iter << " ";
  os << endl;
  if (scanSpec.start_row)
    os << "StartRow  = " << scanSpec.start_row << endl;
  else
    os << "StartRow  = " << endl;
  os << "StartRowInclusive = " << scanSpec.start_row_inclusive << endl;
  if (scanSpec.end_row)
    os << "EndRow    = " << scanSpec.end_row << endl;
  else
    os << "EndRow    = " << endl;
  os << "EndRowInclusive = " << scanSpec.end_row_inclusive << endl;
  os << "MinTime     = " << scanSpec.interval.first << endl;
  os << "MaxTime     = " << scanSpec.interval.second << endl;
  return os;
}

