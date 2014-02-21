#pragma once

#include "entities/Hardware.h"

namespace server { 
namespace database { 

   class IHardwareRequester
   {
   public:
      //--------------------------------------------------------------
      /// \brief           Add new hardware plugin instance
      /// \param [in] newHardware   New hardware informations
      /// \return          harwareId generated by the database
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual int addHardware(boost::shared_ptr<entities::CHardware> newHardware) = 0;

      //--------------------------------------------------------------
      /// \brief           Get hardware informations
      /// \param [in] hardwareId   Hardware plugin instance Id
      /// \throw           CInvalidParameterException if hardwareId is unknown
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CHardware> getHardware(int hardwareId) = 0;

      //--------------------------------------------------------------
      /// \brief           List all hardware plugin instances
      /// \param [in] evenDeleted   if true, returns deleted hardwares too
      /// \return          List of registered plugin instances
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CHardware> > getHardwares(bool evenDeleted = false) = 0;

      //--------------------------------------------------------------
      /// \brief           Update hardware informations
      /// \param [in] hardwareId   Hardware plugin instance Id
      /// \param [in] newConfiguration   New configuration
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateHardwareConfiguration(int hardwareId, const std::string& newConfiguration) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove hardware plugin instance
      /// \param [in] hardwareId   Hardware plugin instance Id
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeHardware(int hardwareId) = 0;

      //--------------------------------------------------------------
      /// \brief           Enable/disable hardware plugin instance
      /// \param [in] hardwareId   Hardware plugin instance Id
      /// \param [in] enable       true to flag as enable, false to flag as disable
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void enableInstance(int hardwareId, bool enable) = 0;

      //--------------------------------------------------------------
      /// \brief           Disable all instance of a plugin
      /// \param [in] pluginName    plugin to disable
      //--------------------------------------------------------------
      virtual void disableAllPluginInstance(const std::string& pluginName) = 0;

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IHardwareRequester()
      {
      }
   };
} //namespace server 
} //namespace database 
