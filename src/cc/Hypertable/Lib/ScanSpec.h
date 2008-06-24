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

#ifndef HYPERTABLE_SCANSPEC_H
#define HYPERTABLE_SCANSPEC_H

#include <boost/noncopyable.hpp>

#include <vector>

namespace Hypertable {

  /**
   * Represents a row interval.  c-string data members are not managed
   * so caller must handle deallocation.
   */
  class RowInterval {
  public:
    RowInterval();
    RowInterval(const uint8_t **bufp, size_t *remainp) { decode(bufp, remainp); }
    RowInterval(const RowInterval &other) {
      start = new char[strlen(other.start)+1];
      strcpy(start,other.start);
      end = new char[strlen(other.end)+1];
      strcpy(end,other.end);
      start_inclusive = other.start_inclusive;
      end_inclusive = other.end_inclusive;
    }

    bool operator<(const RowInterval &other) {
      if (start < other.start) return true;
      if (start > other.start) return false;
      if (start_inclusive < other.start_inclusive) return true;
      if (start_inclusive > other.start_inclusive) return false;
      if (end < other.end) return true;
      if (end > other.end) return false;
      if (end_inclusive < other.end_inclusive) return true;
      return false;
    }

    bool operator==(const RowInterval &other ) {
      return strcmp(start, other.start) == 0
        && strcmp(end, other.end) == 0
        && start_inclusive == other.start_inclusive
        && end_inclusive == other.end_inclusive;
    }

    size_t encoded_length() const;
    void encode(uint8_t **bufp) const;
    void decode(const uint8_t **bufp, size_t *remainp);

    const char *start;
    bool start_inclusive;
    const char *end;
    bool end_inclusive;
  };


  /**
   * Represents a cell interval.  c-string data members are not managed
   * so caller must handle deallocation.
   */
  class CellInterval {
  public:
    CellInterval();
    CellInterval(const uint8_t **bufp, size_t *remainp) { decode(bufp, remainp); }
    CellInterval(const CellInterval &other) {
      start_row = new char[strlen(other.start_row)+1];
      strcpy(start_row,other.start_row);
      start_column = new char[strlen(other.start_column)+1];
      strcpy(start_column,other.start_column);
      end_row = new char[strlen(other.end_row)+1];
      strcpy(end_row,other.end_row);
      end_column = new char[strlen(other.end_column)+1];
      strcpy(end_column,other.end_column);
      start_inclusive = other.start_inclusive;
      end_inclusive = other.end_inclusive;
    }

    bool operator<(const CellInterval &other) {
      if (start_row < other.start_row) return true;
      if (start_row > other.start_row) return false;
      if (start_column < other.start_column) return true;
      if (start_column > other.start_column) return false;
      if (start_inclusive < other.start_inclusive) return true;
      if (start_inclusive > other.start_inclusive) return false;
      if (end_row < other.end_row) return true;
      if (end_row > other.end_row) return false;
      if (end_column < other.end_column) return true;
      if (end_column > other.end_column) return false;
      if (end_inclusive < other.end_inclusive) return true;
      return false;
    }

    bool operator==(const CellInterval &other ) {
      return strcmp(start_row, other.start_row) == 0
        && strcmp(start_column, other.start_column) == 0
        && strcmp(end_row, other.end_row) == 0
        && strcmp(end_column, other.end_column) == 0
        && start_inclusive == other.start_inclusive
        && end_inclusive == other.end_inclusive;
    }

    size_t encoded_length() const;
    void encode(uint8_t **bufp) const;
    void decode(const uint8_t **bufp, size_t *remainp);

    const char *start_row;
    const char *start_column;
    bool start_inclusive;
    const char *end_row;
    const char *end_column;
    bool end_inclusive;
  };
  


  /**
   * Represents a scan predicate.
   */
  class ScanSpec {
  public:
    ScanSpec();
    ScanSpec(const uint8_t **bufp, size_t *remainp) { decode(bufp, remainp); }
    ScanSpec(const ScanSpec &other) { deep_copy(other); }
    ScanSpec& operator=(const ScanSpec &other) { deep_copy(other); return *this; }

    size_t encoded_length() const;
    void encode(uint8_t **bufp) const;
    void decode(const uint8_t **bufp, size_t *remainp);

    void clear() {
      row_limit = 0;
      max_versions = 0;
      columns.clear();
      row_intervals.clear();
      cell_intervals.clear();
      time_interval.first = time_interval.second = 0;
      return_deletes = 0;
    }

    void base_copy(ScanSpec &other) {
      other.row_limit = row_limit;
      other.max_versions = max_versions;
      other.columns = columns;
      other.time_interval = time_interval;
      other.return_deletes = return_deletes;
      other.row_intervals.clear();
      other.cell_intervals.clear();
    }

    void deep_copy(const ScanSpec &other) {
      row_limit = other.row_limit;
      max_versions = other.max_versions;
      columns = other.columns;
      for (int i = 0; i < other.row_intervals.size(); i++) {
        RowInterval row_int = other.row_intervals.at(i);
        row_intervals.push_back(row_int);
      }
      for (int i = 0; i < other.cell_intervals.size(); i++) {
        CellInterval cell_int = other.cell_intervals.at(i);
        cell_intervals.push_back(cell_int);
      }
      time_interval = other.time_interval;
      return_deletes = other.return_deletes;
    }

    bool operator <(const ScanSpec &other) {
      if (row_intervals < other.row_intervals) return true;
      if (row_intervals > other.row_intervals) return false;
      if (cell_intervals < other.cell_intervals) return true;
      if (cell_intervals > other.cell_intervals) return false;
      if (columns < other.columns) return true;
      if (columns > other.columns) return false;
      if (return_deletes < other.return_deletes) return true;
      if (return_deletes > other.return_deletes) return false;
      if (time_interval < other.time_interval) return true;
      if (time_interval > other.time_interval) return false;
      if (row_limit < other.row_limit) return true;
      if (row_limit > other.row_limit) return false;
      if (max_versions < other.max_versions) return true;
      return false;
    }

    bool operator==(const ScanSpec &other ) {
      return row_intervals == other.row_intervals
	&& cell_intervals == other.cell_intervals
	&& time_interval == other.time_interval
	&& columns == other.columns
        && return_deletes == other.return_deletes
        && row_limit == other.row_limit
        && max_versions == other.max_versions;
    }

    int32_t row_limit;
    uint32_t max_versions;
    std::vector<const char *> columns;
    std::vector<RowInterval> row_intervals;
    std::vector<CellInterval> cell_intervals;
    std::pair<int64_t,int64_t> time_interval;
    bool return_deletes;
  };

  /**
   * Helper class for building a ScanSpec.  This class manages the allocation
   * of all string members.
   */
  class ScanSpecBuilder : boost::noncopyable {
  public:
    /**
     * Sets the maximum number of rows to return in the scan.
     *
     * @param n row limit
     */
    void set_row_limit(int32_t n) { m_scan_spec.row_limit = n; }

    /**
     * Sets the maximum number of revisions of each cell to return in the scan.
     *
     * @param n maximum revisions
     */
    void set_max_versions(uint32_t n) { m_scan_spec.max_versions = n; }

    /**
     * Adds a column family to be returned by the scan.
     *
     * @param str column family name
     */
    void add_column(const String &str) {
      m_strings.push_back(str);
      m_scan_spec.columns.push_back(m_strings.back().c_str());
    }

    /**
     * Adds a row to be returned in the scan
     *
     * @param str row key
     */
    void add_row(const String &str) {
      RowInterval ri;
      m_strings.push_back(str);
      ri.start = ri.end = m_strings.back().c_str();
      ri.start_inclusive = ri.end_inclusive = true;
      m_scan_spec.row_intervals.push_back(ri);
    }

    /**
     * Adds a row interval to be returned in the scan.
     *
     * @param start start row
     * @param start_inclusive true if interval should include start row
     * @param end end row
     * @param end_inclusive true if interval should include end row
     */
    void add_row_interval(const String &start, bool start_inclusive,
			  const String &end, bool end_inclusive) {
      RowInterval ri;
      m_strings.push_back(start);
      ri.start = m_strings.back().c_str();
      ri.start_inclusive = start_inclusive;
      m_strings.push_back(end);
      ri.end = m_strings.back().c_str();
      ri.end_inclusive = end_inclusive;
      m_scan_spec.row_intervals.push_back(ri);      
    }

    /**
     * Adds a cell to be returned in the scan
     *
     * @param str row key
     */
    void add_cell(const String &row, const String &column) {
      CellInterval ci;
      m_strings.push_back(row);
      ci.start_row = ci.end_row = m_strings.back().c_str();
      m_strings.push_back(column);
      ci.start_column = ci.end_column = m_strings.back().c_str();
      ci.start_inclusive = ci.end_inclusive = true;
      m_scan_spec.cell_intervals.push_back(ci);
    }

    /**
     * Adds a cell interval to be returned in the scan.
     *
     * @param start_row start row
     * @param start_column start column
     * @param start_inclusive true if interval should include start row
     * @param end_row end row
     * @param end_column end column
     * @param end_inclusive true if interval should include end row
     */
    void add_cell_interval(const String &start_row, const String &start_column, bool start_inclusive,
			   const String &end_row, const String &end_column, bool end_inclusive) {
      CellInterval ci;
      m_strings.push_back(start_row);
      ci.start_row = m_strings.back().c_str();
      m_strings.push_back(start_column);
      ci.start_column = m_strings.back().c_str();
      ci.start_inclusive = start_inclusive;
      m_strings.push_back(end_row);
      ci.end_row = m_strings.back().c_str();
      m_strings.push_back(end_column);
      ci.end_column = m_strings.back().c_str();
      ci.end_inclusive = end_inclusive;
      m_scan_spec.cell_intervals.push_back(ci);
    }

    /**
     * Sets the time interval of the scan.  Time values represent number of
     * nanoseconds from 1970-01-00 00:00:00.000000000.
     *
     * @param start start time in nanoseconds
     * @param end end time in nanoseconds
     */
    void set_time_interval(int64_t start, int64_t end) {
      m_scan_spec.time_interval.first = start;
      m_scan_spec.time_interval.second = end;
    }

    /**
     * Internal use only.
     */
    void set_return_deletes(bool val) {
      m_scan_spec.return_deletes = val;
    }

    /**
     * Clears the state.
     */
    void clear() {
      m_scan_spec.clear();
      m_strings.clear();
    }

    /**
     * Returns the built ScanSpec object
     *
     * @return reference to built ScanSpec object
     */
    ScanSpec &get() { return m_scan_spec; }
    
  private:
    std::vector<String> m_strings;
    ScanSpec m_scan_spec;
  };

  extern const int64_t BEGINNING_OF_TIME;
  extern const int64_t END_OF_TIME;

  std::ostream &operator<<(std::ostream &os, const RowInterval &);

  std::ostream &operator<<(std::ostream &os, const CellInterval &);

  std::ostream &operator<<(std::ostream &os, const ScanSpec &);


} // namespace Hypertable


#endif // HYPERTABLE_SCANSPEC_H
