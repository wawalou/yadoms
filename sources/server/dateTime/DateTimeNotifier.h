#pragma once
#include <Poco/Util/Timer.h>

namespace dateTime {

   //-----------------------------------------------------------------------------
   /// \class   Class which start a date time notifier (send notification every minute)
   //-----------------------------------------------------------------------------
   class CDateTimeNotifier
   {
   public:
      //-----------------------------------------------------------------------------
      /// \brief		      Constructor
      //-----------------------------------------------------------------------------
      CDateTimeNotifier();

      //-----------------------------------------------------------------------------
      /// \brief		                     Destructor
      //-----------------------------------------------------------------------------
      virtual ~CDateTimeNotifier();

      //-----------------------------------------------------------------------------
      /// \brief		                     Start the date/time notification service
      //-----------------------------------------------------------------------------
      void start();

      //-----------------------------------------------------------------------------
      /// \brief		                     Stop the date/time notification service
      //-----------------------------------------------------------------------------
      void stop();
   private:
      //--------------------------------------------------------------
      /// \Brief		The datetime task
      //--------------------------------------------------------------
      Poco::Util::TimerTask::Ptr m_dateTimeTask;

      //--------------------------------------------------------------
      /// \Brief		The datetime timer
      //--------------------------------------------------------------
      Poco::Util::Timer m_dateTimeTimer;

   };

} //namespace dateTime