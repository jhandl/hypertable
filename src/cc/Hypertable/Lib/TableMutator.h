/**
 * Copyright (C) 2008 Doug Judd (Zvents, Inc.)
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

#ifndef HYPERTABLE_TABLEMUTATOR_H
#define HYPERTABLE_TABLEMUTATOR_H

#include "AsyncComm/ConnectionManager.h"

#include "Common/Properties.h"
#include "Common/StringExt.h"
#include "Common/ReferenceCount.h"

#include "Cell.h"
#include "KeySpec.h"
#include "TableMutatorScatterBuffer.h"
#include "RangeLocator.h"
#include "RangeServerClient.h"
#include "Schema.h"
#include "Types.h"

namespace Hypertable {

  class TableMutator : public ReferenceCount {

  public:
    TableMutator(PropertiesPtr &props_ptr, Comm *comm, TableIdentifierT *table_identifier, SchemaPtr &schema_ptr, RangeLocatorPtr &range_locator_ptr);
    virtual ~TableMutator() { return; }

    void set(uint64_t timestamp, KeySpec &key, const void *value, uint32_t value_len);
    void set_delete(uint64_t timestamp, KeySpec &key);
    void flush();

    uint64_t memory_used();

    uint64_t get_resend_count() { return m_resends; }

  private:

    void wait_for_previous_buffer();

    void sanity_check_key(KeySpec &key);

    PropertiesPtr        m_props_ptr;
    Comm                *m_comm;
    SchemaPtr            m_schema_ptr;
    RangeLocatorPtr      m_range_locator_ptr;
    std::string          m_table_name;
    TableIdentifierT     m_table_identifier;
    uint64_t             m_memory_used;
    uint64_t             m_max_memory;
    TableMutatorScatterBufferPtr  m_buffer_ptr;
    TableMutatorScatterBufferPtr  m_prev_buffer_ptr;
    uint64_t             m_resends;
  };
  typedef boost::intrusive_ptr<TableMutator> TableMutatorPtr;



}

#endif // HYPERTABLE_TABLEMUTATOR_H