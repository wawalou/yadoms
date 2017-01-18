#include "stdafx.h"
#include "ScriptLogger.h"
#include <shared/StringExtension.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FileChannel.h>
#include <Poco/SplitterChannel.h>
#include <Poco/AutoPtr.h>
#include <shared/Log.h>


CScriptLogger::CScriptLogger(boost::shared_ptr<yApi::IYInterpreterApi> api,
                             int scriptInstanceId,
                             const std::string& interpreterLoggerName,
                             const boost::filesystem::path& scriptLogPath)
   : m_api(api),
     m_scriptInstanceId(scriptInstanceId),
     m_logger(Poco::Logger::get(interpreterLoggerName + ".Rule." + std::to_string(scriptInstanceId)))
{
   Poco::AutoPtr<Poco::PatternFormatter> patternFormatter(new Poco::PatternFormatter);
   Poco::AutoPtr<Poco::FormattingChannel> formattingFileChannel;
   Poco::AutoPtr<Poco::FileChannel> fileChannel(new Poco::FileChannel());

   Poco::AutoPtr<Poco::SplitterChannel> splitterChannel(new Poco::SplitterChannel);

   patternFormatter->setProperty("pattern", "%H:%M:%S : [%p] : %t");
   patternFormatter->setProperty("times", "local"); //use local datetime

   if (!boost::filesystem::exists(scriptLogPath.parent_path().string()))
      if (!boost::filesystem::create_directories(scriptLogPath.parent_path().string()))
         throw std::exception((boost::format("Cannot create directory %1%") % scriptLogPath.parent_path()).str().c_str());

   fileChannel->setProperty("path", scriptLogPath.string());
   fileChannel->setProperty("rotation", "daily");
   fileChannel->setProperty("archive", "timestamp");
   fileChannel->setProperty("compress", "true");
   fileChannel->setProperty("purgeCount", "7");
   formattingFileChannel.assign(new Poco::FormattingChannel(patternFormatter,
                                                            fileChannel));

   splitterChannel->addChannel(formattingFileChannel);
   splitterChannel->addChannel(Poco::Logger::get(interpreterLoggerName).getChannel());

   m_logger.setChannel(splitterChannel);
   m_logger.setLevel(shared::CLog::logger().getLevel());
}

CScriptLogger::~CScriptLogger()
{
}

void CScriptLogger::init()
{
   m_msgInformation.setPriority(Poco::Message::PRIO_INFORMATION);
   m_msgInformation.setThread(m_logger.name());

   m_msgError.setPriority(Poco::Message::PRIO_ERROR);
   m_msgError.setThread(m_logger.name());
}

void CScriptLogger::information(const std::string& line)
{
   m_msgInformation.setText(line);
   m_logger.log(m_msgInformation);

   //TODO virer + dépendances
   //m_api->onScriptLog(m_scriptInstanceId,
   //                   false,
   //                   line);
}

void CScriptLogger::error(const std::string& line)
{
   m_msgError.setText(line);
   m_logger.log(m_msgError);

   //TODO virer + dépendances
   //m_api->onScriptLog(m_scriptInstanceId,
   //                   true,
   //                   line);
}
