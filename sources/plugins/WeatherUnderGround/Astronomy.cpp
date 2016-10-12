#include "stdafx.h"
#include "Astronomy.h"
#include "ErrorAnswerHandler.h"
#include <shared/exception/Exception.hpp>

CAstronomy::CAstronomy(boost::shared_ptr<yApi::IYPluginApi> api,
                       IWUConfiguration& wuConfiguration,
                       const std::string& deviceName,
                       const std::string& prefix)
   : m_localisation(wuConfiguration.getLocalisation()),
     m_countryOrState(wuConfiguration.getCountryOrState()),
     m_deviceName(deviceName),
     m_moonCharacteristics(boost::make_shared<CMoon>(deviceName, prefix + "Moon")),
     m_isDesactivated(false)
{
   try
   {
      initializeVariables(api, wuConfiguration);
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error of Astronomy module :" << e.what() << std::endl;

      api->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError");
      m_isDesactivated = true;
   }
}

void CAstronomy::initializeVariables(boost::shared_ptr<yApi::IYPluginApi> api,
                                     IWUConfiguration& wuConfiguration)
{
   if (wuConfiguration.IsAstronomyEnabled())
   {
      m_localisation = wuConfiguration.getLocalisation();

      //read the country or State code
      m_countryOrState = wuConfiguration.getCountryOrState();

      // Clear the list
      m_keywords.clear();

      m_keywords.push_back(m_moonCharacteristics->getHistorizable());
      m_moonCharacteristics->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Load.getName(),
                                     shared::plugin::yPluginApi::CStandardCapacities::Load.getUnit());

      // Declare keywords
      std::string m_URL = "www.wunderground.com/";
      api->declareDevice(m_deviceName, m_URL, m_keywords);

      m_url.str("");
      m_url << "http://api.wunderground.com/api/" << wuConfiguration.getAPIKey() << "/astronomy/q/" << m_countryOrState << "/" << m_localisation << ".json";
   }
   else
      m_isDesactivated = true;
}

void CAstronomy::onUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                          IWUConfiguration& wuConfiguration)
{
   try
   {
      initializeVariables(api, wuConfiguration);
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error of Astronomy module :" << e.what() << std::endl;

      api->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError");
      m_isDesactivated = true;
   }
}

void CAstronomy::parse(boost::shared_ptr<yApi::IYPluginApi> api,
                       const IWUConfiguration& wuConfiguration,
                       const shared::CDataContainer dataToParse)
{
   if (!m_isDesactivated)
   {
      try
      {
         if (wuConfiguration.IsAstronomyEnabled())
         {
            m_moonCharacteristics->setParameters(dataToParse,
                                                 "moon_phase.percentIlluminated",
                                                 "moon_phase.ageOfMoon");
         }

         api->historizeData(m_deviceName, m_keywords);
      }
      catch (shared::exception::CException& e)
      {
         std::cout << e.what() << std::endl;
         throw e;
      }
   }
}

CAstronomy::~CAstronomy()
{}

std::string CAstronomy::getUrl() const
{
   return m_url.str();
}

bool CAstronomy::isDesactivated() const
{
   return m_isDesactivated;
}