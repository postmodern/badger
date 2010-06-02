/*
 * Badger - A fear-less, robust and portable systems management solution.
 *
 * Copyright (c) 2010 Hal Brodigan (postmodern.mod3 at gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#ifndef _BADGER_CONFIG_H_
#define _BADGER_CONFIG_H_

#define BADGER_VERSION "${LIBRARY_VERSION}"

#cmakedefine DEBUG

// badger encoders
#cmakedefine ENCODERS_XOR
#cmakedefine ENCODERS_AES

// badger services
#cmakedefine SERVICES_SYS
#cmakedefine SERVICES_FS
#cmakedefine SERVICES_FFI

#endif
