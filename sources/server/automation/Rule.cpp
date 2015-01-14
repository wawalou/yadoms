#include "stdafx.h"
#include "Rule.h"
#include "condition/ConditionRoot.h"
#include "ActionList.h"

namespace automation
{

CRule::CRule(const database::entities::CRule& ruleData,
   boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver,
   boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
   condition::IConditionFactory& conditionFactory,
   boost::shared_ptr<action::script::IRunnerFactory> scriptRunnerFactory)
   :m_name(ruleData.Name()),
   m_id(ruleData.Id()),
   m_condition(new condition::CConditionRoot(ruleData.Triggers(), conditionFactory)),
   m_actions(new CActionList(ruleData.Actions(), pluginGateway, scriptRunnerFactory)),
   m_notificationObserver(notificationObserver)
{
   m_condition->registerToNotificationCenter(m_notificationObserver);
}

CRule::~CRule()
{
   m_condition->unregisterFromNotificationCenter(m_notificationObserver);
   stop();
}

void CRule::start()
{
   m_thread.reset(new CRuleThread(m_name, *this));
   m_thread->start();
}

void CRule::stop()
{
   m_actions->stopPending();
   m_thread->stop();
   m_thread.reset();
}

void CRule::doWork()
{
   try
   {
      while (true)
      {
         m_condition->wait();
         m_actions->doAll();
      }
   }
   catch (boost::thread_interrupted&)
   {
   }
}

} // namespace automation	
	
	