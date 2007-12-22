/* <!-- copyright */
/*
 * aria2 - The high speed download utility
 *
 * Copyright (C) 2006 Tatsuhiro Tsujikawa
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */
/* copyright --> */
#ifndef _D_HANDSHAKE_EXTENSION_MESSAGE_H_
#define _D_HANDSHAKE_EXTENSION_MESSAGE_H_

#include "ExtensionMessage.h"
#include "BtConstants.h"

class BtContext;
typedef SharedHandle<BtContext> BtContextHandle;
class Peer;
typedef SharedHandle<Peer> PeerHandle;
class HandshakeExtensionMessage;
typedef SharedHandle<HandshakeExtensionMessage> HandshakeExtensionMessageHandle;
class Logger;

class HandshakeExtensionMessage:public ExtensionMessage {
private:
  string _clientVersion;

  uint16_t _tcpPort;

  map<string, uint8_t> _extensions;

  BtContextHandle _btContext;

  PeerHandle _peer;

  const Logger* _logger;

public:
  HandshakeExtensionMessage();

  virtual ~HandshakeExtensionMessage();

  virtual string getBencodedData();

  virtual uint8_t getExtensionMessageID()
  {
    return 0;
  }
  
  virtual const string& getExtensionName() const
  {
    return EXTENSION_NAME;
  }

  static const string EXTENSION_NAME;

  virtual string toString() const;

  virtual void doReceivedAction();

  void setClientVersion(const string& version)
  {
    _clientVersion = version;
  }

  const string& getClientVersion() const
  {
    return _clientVersion;
  }

  void setTCPPort(uint16_t port)
  {
    _tcpPort = port;
  }

  uint16_t getTCPPort() const
  {
    return _tcpPort;
  }

  void setExtension(const string& name, uint8_t id)
  {
    _extensions[name] = id;
  }

  void setExtensions(const Extensions& extensions)
  {
    _extensions = extensions;
  }

  uint8_t getExtensionMessageID(const string& name) const;

  void setPeer(const PeerHandle& peer);

  void setBtContext(const BtContextHandle& btContext);

  static HandshakeExtensionMessageHandle create(const char* data,
						size_t dataLength);

};

typedef SharedHandle<HandshakeExtensionMessage> HandshakeExtensionMessageHandle;
#endif // _D_HANDSHAKE_EXTENSION_MESSAGE_H_
