#pragma once
#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/state_grammar.hpp>
#include <boost/msm/back/state_machine.hpp>

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/Log.h>
#include "deviceStateCommonDeclaration.hpp"


// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace msm = boost::msm;
using namespace boost::msm::front::euml;

//--------------------------------------------------------------
/// \brief	D�claration de l'�tat initialization
/// \note   
//--------------------------------------------------------------

BOOST_MSM_EUML_ACTION(Initialization_Entry)
{
   template <class Evt, class Fsm, class State>
   void operator()(Evt const& evt, Fsm& stateMachine, State&)
   {
      auto api = evt.get_attribute(m_pluginApi);
      auto configuration = Initialization.get_attribute(m_configuration);
      auto factory = evt.get_attribute(m_factory);

      stateMachine.process_event(EvtInitialized(api, factory));
   }

   template <class Evt, class Fsm, class State>
   void exception_caught(Evt const& evt, Fsm&, std::exception&)
   {
      auto api = evt.get_attribute(m_pluginApi);
   }
};

BOOST_MSM_EUML_STATE((Initialization_Entry, 
                      no_action,
                      attributes_ << m_configuration,
                      configure_ << no_configure_),
                     Initialization);