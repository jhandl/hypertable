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

#ifndef HYPERTABLE_SERVERLOCKFILEHANDLER_H
#define HYPERTABLE_SERVERLOCKFILEHANDLER_H

#include "AsyncComm/ApplicationQueue.h"

#include "Hyperspace/HandleCallback.h"


namespace Hypertable {

  class Master;

  /**
   *
   */
  class ServerLockFileHandler : public HandleCallback {
  public:
    ServerLockFileHandler(RangeServerStatePtr &rs_state, MasterPtr master, ApplicationQueuePtr &app_queue) : HandleCallback(EVENT_MASK_LOCK_ACQUIRED|EVENT_MASK_LOCK_RELEASED), m_state_ptr(rs_state), m_master_ptr(master), m_app_queue_ptr(app_queue) { return; }
    virtual void lock_acquired(uint32_t mode);
    virtual void lock_released();
    RangeServerStatePtr m_state_ptr;
    MasterPtr           m_master_ptr;
    ApplicationQueuePtr m_app_queue_ptr;
  };

}

#endif // HYPERTABLE_SERVERLOCKFILEHANDLER_H
