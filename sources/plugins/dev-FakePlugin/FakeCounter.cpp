#include "stdafx.h"
#include "FakeCounter.h"

CFakeCounter::CFakeCounter(const std::string& deviceName)
   : m_deviceName(deviceName),
     m_incrementCount(boost::make_shared<yApi::historization::CCounter>("incrementCount",
                                                                        yApi::EKeywordAccessMode::kGet,
                                                                        yApi::historization::EMeasureType::kIncrement)),
     m_totalCount(boost::make_shared<yApi::historization::CCounter>("totalCounter")),
     m_historizers({m_incrementCount , m_totalCount}),
     m_dist(0, 15)
{
   m_totalCount->set(0);
}

CFakeCounter::~CFakeCounter()
{
}

void CFakeCounter::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   // Declare device and associated keywords (= values managed by this device)
   if (!api->deviceExists(m_deviceName))
      api->declareDevice(m_deviceName, getModel(), m_historizers);
}

void CFakeCounter::read()
{
   // Generate a random increment (0 to 15)
   m_incrementCount->set(m_dist(m_gen));

   // Generate a random increment (0 to 15)
   m_totalCount->set(m_totalCount->get() + m_dist(m_gen));
}

void CFakeCounter::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api)
      throw shared::exception::CException("api must be defined");

   api->historizeData(m_deviceName, m_historizers);
}

const std::string& CFakeCounter::getDeviceName() const
{
   return m_deviceName;
}

const std::string& CFakeCounter::getModel()
{
   static const std::string model("Fake counter");
   return model;
}

