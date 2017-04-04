/**
   This file is part of libavrc++.

   Copyright (c) 2014, netWorms, All rights reserved.

   libavrc++ is  free software: you can  redistribute it and/or modify  it under
   the terms of the  GNU Lesser General Public License as  published by the Free
   Software Foundation, either version 3 of the License, or (at your option) any
   later version.

   libavrc++ is distributed in the hope that  it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR  PURPOSE.  See the  GNU Lesser  General Public License  for more
   details.

   You should  have received  a copy  of the GNU  Lesser General  Public License
   along with libavrc++.  If not, see <http://www.gnu.org/licenses/>.
*/

/* -------------------------------------------------------------------------- */
#include "common/common.hh"
/* -------------------------------------------------------------------------- */

#ifndef __USI_HH__
#define __USI_HH__

namespace usi {

typedef usicr::bit<7> usisie;
typedef usicr::bit<6> usioie;
typedef usicr::bits4, 0x3> usiwm;
typedef usicr::bit<2, 0x3> usics;
typedef usicr::bit<1> usiclk;
typedef usicr::bit<0> usitc;

}  // usi

#endif /* __USI_HH__ */
