#include "DefaultExtensionMessageFactory.h"
#include "Peer.h"
#include "MockBtContext.h"
#include "PeerMessageUtil.h"
#include "HandshakeExtensionMessage.h"
#include "UTPexExtensionMessage.h"
#include "Exception.h"
#include "BtRegistry.h"
#include "BtRuntime.h"
#include <cppunit/extensions/HelperMacros.h>

class DefaultExtensionMessageFactoryTest:public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(DefaultExtensionMessageFactoryTest);
  CPPUNIT_TEST(testCreateMessage_unknown);
  CPPUNIT_TEST(testCreateMessage_Handshake);
  CPPUNIT_TEST(testCreateMessage_UTPex);
  CPPUNIT_TEST_SUITE_END();
private:
  MockBtContextHandle _btContext;
  PeerHandle _peer;
public:
  DefaultExtensionMessageFactoryTest():_btContext(0), _peer(0) {}

  void setUp()
  {
    BtRegistry::unregisterAll();
    MockBtContextHandle btContext = new MockBtContext();
    unsigned char infohash[20];
    memset(infohash, 0, sizeof(infohash));
    btContext->setInfoHash(infohash);
    _btContext = btContext;

    BtRuntimeHandle btRuntime = new BtRuntime();
    BtRegistry::registerBtRuntime(_btContext->getInfoHashAsString(),
				  btRuntime);

    _peer = new Peer("192.168.0.1", 6969, 16*1024, 256*1024);
    _peer->setExtension("ut_pex", 1);
  }

  void tearDown()
  {
    BtRegistry::unregisterAll();
  }

  void testCreateMessage_unknown();
  void testCreateMessage_Handshake();
  void testCreateMessage_UTPex();
};


CPPUNIT_TEST_SUITE_REGISTRATION(DefaultExtensionMessageFactoryTest);

void DefaultExtensionMessageFactoryTest::testCreateMessage_unknown()
{
  DefaultExtensionMessageFactory factory;
  factory.setBtContext(_btContext);
  factory.setPeer(_peer);
  _peer->setExtension("foo", 255);

  char id[1] = { 255 };

  string data = string(&id[0], &id[1]);
  try {
    factory.createMessage(data.c_str(), data.size());
    CPPUNIT_FAIL("exception must be thrown.");
  } catch(Exception* e) {
    cerr << *e << endl;
    delete e;
  }
}

void DefaultExtensionMessageFactoryTest::testCreateMessage_Handshake()
{
  DefaultExtensionMessageFactory factory;
  factory.setBtContext(_btContext);
  factory.setPeer(_peer);

  char id[1] = { 0 };

  string data = string(&id[0], &id[1])+"d1:v5:aria2e";
  HandshakeExtensionMessageHandle m = factory.createMessage(data.c_str(), data.size());
  CPPUNIT_ASSERT_EQUAL(string("aria2"), m->getClientVersion());
}

void DefaultExtensionMessageFactoryTest::testCreateMessage_UTPex()
{
  DefaultExtensionMessageFactory factory;
  factory.setBtContext(_btContext);
  factory.setPeer(_peer);
  
  char c1[6];
  char c2[6];
  char c3[6];
  char c4[6];
  PeerMessageUtil::createcompact(c1, "192.168.0.1", 6881);
  PeerMessageUtil::createcompact(c2, "10.1.1.2", 9999);
  PeerMessageUtil::createcompact(c3, "192.168.0.2", 6882);
  PeerMessageUtil::createcompact(c4, "10.1.1.3",10000);

  char id[1] = { BT_RUNTIME(_btContext)->getExtensionMessageID("ut_pex") };

  string data = string(&id[0], &id[1])+"d5:added12:"+
    string(&c1[0], &c1[6])+string(&c2[0], &c2[6])+
    "7:added.f2:207:dropped12:"+
    string(&c3[0], &c3[6])+string(&c4[0], &c4[6])+
    "e";

  UTPexExtensionMessageHandle m = factory.createMessage(data.c_str(), data.size());
  CPPUNIT_ASSERT_EQUAL(BT_RUNTIME(_btContext)->getExtensionMessageID("ut_pex"),
		       m->getExtensionMessageID());
}
