#include "stdafx.h"
#include "Rfy.h"
#include <shared/plugin/yPluginApi/historization/Curtain.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CRfy::CRfy(boost::shared_ptr<yApi::IYPluginApi> api,
              const std::string& command,
              const shared::CDataContainer& deviceDetails)
      : m_state(boost::make_shared<yApi::historization::CCurtain>("state"))
   {
      m_state->setCommand(command);

      m_subType = deviceDetails.get<unsigned char>("subType");
      m_id = deviceDetails.get<unsigned int>("id");
      m_unitCode = deviceDetails.get<unsigned char>("unitCode");

      Init(api);
   }

   CRfy::CRfy(boost::shared_ptr<yApi::IYPluginApi> api,
              unsigned int subType,
              const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_state(boost::make_shared<yApi::historization::CCurtain>("state"))
   {
      m_state->set(yApi::historization::ECurtainCommand::kStop);

      m_subType = static_cast<unsigned char>(subType);
      switch (m_subType)
      {
      case sTypeRFY:
      case sTypeRFYext:
      case sTypeASA:
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }

      m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");
      m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");

      Init(api);
   }

   CRfy::CRfy(boost::shared_ptr<yApi::IYPluginApi> api,
              const RBUF& rbuf,
              size_t rbufSize)
      : m_subType(0),
        m_unitCode(0),
        m_id(0),
        m_state(boost::make_shared<yApi::historization::CCurtain>("state"))
   {
      // Should not be called (transmitter-only device)
      BOOST_ASSERT_MSG(false, "Constructing CRfy object from received buffer is not possible, CRfy is transmitter-only device");
   }

   CRfy::~CRfy()
   {
   }

   void CRfy::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Build device description
      buildDeviceModel();
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeRFY);
         details.set("subType", m_subType);
         details.set("id", m_id);
         details.set("unitCode", m_unitCode);
         api->declareDevice(m_deviceName, m_deviceModel, m_state, details);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CRfy::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.RFY);

      rbuf.RFY.packetlength = ENCODE_PACKET_LENGTH(RFY);
      rbuf.RFY.packettype = pTypeRFY;
      rbuf.RFY.subtype = m_subType;
      rbuf.RFY.seqnbr = seqNumberProvider->next();
      rbuf.RFY.id1 = static_cast<unsigned char>((m_id & 0xFF0000) >> 16);
      rbuf.RFY.id2 = static_cast<unsigned char>((m_id & 0xFF00) >> 8);
      rbuf.RFY.id3 = static_cast<unsigned char>(m_id & 0xFF);
      rbuf.RFY.unitcode = m_unitCode;
      rbuf.RFY.cmnd = toProtocolState(*m_state);
      rbuf.RFY.rfu1 = 0;
      rbuf.RFY.rfu2 = 0;
      rbuf.RFY.rfu3 = 0;
      rbuf.RFY.rssi = 0;
      rbuf.LIGHTING1.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(RFY));
   }

   void CRfy::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      // Nothing to historize (transmitter-only device)
   }

   const std::string& CRfy::getDeviceName() const
   {
      return m_deviceName;
   }

   void CRfy::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << m_deviceModel <<
         "." << static_cast<unsigned int>(m_id) <<
         "." << static_cast<unsigned int>(m_unitCode);
      m_deviceName = ssdeviceName.str();
   }

   void CRfy::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeRFY: ssModel << "Somfy";
         break;
      case sTypeRFYext: ssModel << "SomfyExt";
         break;
      case sTypeASA: ssModel << "ASA";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }

   unsigned char CRfy::toProtocolState(const yApi::historization::CCurtain& curtainState)
   {
      switch (curtainState.get()())
      {
      case yApi::historization::ECurtainCommand::kOpenValue: return rfy_sUp;
      case yApi::historization::ECurtainCommand::kCloseValue: return rfy_sDown;
      case yApi::historization::ECurtainCommand::kStopValue: return rfy_sStop;
      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(curtainState.get()()));
      }
   }
} // namespace rfxcomMessages


