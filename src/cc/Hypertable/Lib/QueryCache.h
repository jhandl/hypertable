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

#ifndef HYPERTABLE_QUERY_CACHE_H
#define HYPERTABLE_QUERY_CACHE_H

#include "Hypertable/Lib/ScanSpec.h"
#include "Hypertable/Lib/Types.h"
#include "Common/lru_cache.h"
#include "Common/md5.h"




namespace Hypertable {

  /** Identifies a table and a scan spec */
  class QueryCacheKey {
  public:
    QueryCacheKey() {};
    QueryCacheKey(const TableIdentifier &table_id, const ScanSpec &scan_spec) {
      md5_context md5;
      memset(&md5, 0, sizeof(md5));
      md5_starts(&md5);
      table_id.md5_update(&md5);
      scan_spec.md5_update(&md5);
      md5_finish(&md5, m_hash);
    }
    friend bool operator<(const QueryCacheKey &self, const QueryCacheKey &other) {
      return (memcmp(self.m_hash, other.m_hash, 16) < 0);
    }
    friend bool operator==(const QueryCacheKey &self, const QueryCacheKey &other) {
      return (memcmp(self.m_hash, other.m_hash, 16) == 0);
    }
  private:
    unsigned char m_hash[16];
  };

  /** Holds the data stored in the cache */
  class QueryCacheData {
  public:
    QueryCacheData() {};
    QueryCacheData(const ScanSpec &scan_spec, const DynamicBuffer &buffer) {
      m_scan_spec = scan_spec;
      m_buffer.set(buffer.base, buffer.size);
    }
    bool matches(const ScanSpec &scan_spec) { 
      return (m_scan_spec == scan_spec && m_scan_spec.row_limit >= scan_spec.row_limit); 
    }
    DynamicBuffer& getBuffer() { return m_buffer; }
  private:
    ScanSpec m_scan_spec;
    DynamicBuffer m_buffer;
  };

  /** A cache for Hypertable queries */
  class QueryCache {
  public:
    QueryCache() : m_cache(1000) {};
    QueryCache(int size) : m_cache(size) {};

    /** Add a buffer to the cache */
    void add(const TableIdentifier &table_id, const ScanSpec &scan_spec, const DynamicBuffer &buffer) {
      const uint8_t *mod_end;
      ByteString row_key;
      // Add the data to the cache
      QueryCacheKey key(table_id, scan_spec);
      QueryCacheData data(scan_spec, buffer);
      m_cache.insert(key, data);
      // Add the cache key to the reverse lookup map for each data row
      mod_end = buffer.base + buffer.size;
      row_key.ptr = buffer.base;
      while (row_key.ptr < mod_end) {
        if (*row_key.ptr == 0) break;
        m_row_to_cache[row_key.str()] = key;
        row_key.next(); // skip key
        row_key.next(); // skip value
      }
    }

    /** get the cached buffer */
    bool get(const TableIdentifier &table_id, const ScanSpec &scan_spec, DynamicBuffer &dbuf) {
      QueryCacheKey key(table_id, scan_spec);
      QueryCacheData data = m_cache.fetch(key);
      if (data.matches(scan_spec)) {
        DynamicBuffer data_buf = data.getBuffer();
        dbuf.set(data_buf.base, data_buf.size);
        return true;
      }
      return false;
    }

    /** Invalidate the cache for the provided data row */
    void remove(const TableIdentifier *table_id, const char *row_key) {
      QueryCacheKey key = m_row_to_cache[row_key];
      m_row_to_cache.erase(row_key);
      m_cache.remove(key);
    }

  private:
    LRUCache<QueryCacheKey, QueryCacheData> m_cache;
    hash_map<const char *, QueryCacheKey> m_row_to_cache;
  };

} // Hypertable namespace

#endif // HYPERTABLE_QUERY_CACHE_H
