#pragma once
#include <shared/process/IApplicationStopHandler.h>

namespace shared
{
   namespace process
   {
      //-----------------------------
      ///\brief   Mac application stop handler
      //-----------------------------  
      class YADOMS_SHARED_EXPORT CApplicationStopHandler : IApplicationStopHandler
      {
      public:
         explicit CApplicationStopHandler(bool isRunningAsService);

         virtual ~CApplicationStopHandler();

         // IApplicationStopHandler implementation
         void setApplicationStopHandler(boost::function<bool()> onStopRequestedFct) override;
         // [END] IApplicationStopHandler implementation

      private:
         static boost::function<bool()> m_onStopRequestedFct;

         static void stopHandler(int signal);
      };
   }
} // namespace shared::process
