//======================================================================
//
// ShipComponentDataBooster.cpp
// copyright (c) 2004 Sony Online Entertainment
//
//======================================================================

#include "serverGame/FirstServerGame.h"
#include "serverGame/ShipComponentDataBooster.h"
#include "sharedGame/ShipComponentStyleManager.h"
#include "serverGame/ShipObject.h"
#include "sharedFoundation/DynamicVariableList.h"
#include "sharedGame/SharedObjectAttributes.h"
#include "sharedGame/ShipComponentDescriptor.h"
#include "UnicodeUtils.h"

#include "sharedLog/Log.h"

//======================================================================

namespace ShipComponentDataBoosterNamespace
{
	namespace Objvars
	{
		std::string const boosterEnergyCurrent          = "ship_comp.booster.energy_current";
		std::string const boosterEnergyMaximum          = "ship_comp.booster.energy_maximum";
		std::string const boosterEnergyRechargeRate     = "ship_comp.booster.energy_recharge_rate";
		std::string const boosterEnergyConsumptionRate  = "ship_comp.booster.energy_consumption_rate";
		std::string const boosterAcceleration           = "ship_comp.booster.acceleration";
		std::string const boosterSpeedMaximum           = "ship_comp.booster.speed_maximum";
		std::string const componentStyle                = "ship_comp.component_style";
	}
}

using namespace ShipComponentDataBoosterNamespace;

//----------------------------------------------------------------------

ShipComponentDataBooster::ShipComponentDataBooster (ShipComponentDescriptor const & shipComponentDescriptor) :
ServerShipComponentData         (shipComponentDescriptor),
m_boosterEnergyCurrent          (100.0f),
m_boosterEnergyMaximum          (100.0f),
m_boosterEnergyRechargeRate     (20.0f),
m_boosterEnergyConsumptionRate  (20.0f),
m_boosterAcceleration           (5.0f),
m_boosterSpeedMaximum           (10.0f),
m_style(1)
{
    m_style = ShipComponentStyleManager::getDefaultStyleForComponent(m_descriptor->getCrc());
}

//----------------------------------------------------------------------

ShipComponentDataBooster::~ShipComponentDataBooster ()
{
}

//----------------------------------------------------------------------

bool ShipComponentDataBooster::readDataFromShip      (int chassisSlot, ShipObject const & ship)
{
	if (!ServerShipComponentData::readDataFromShip (chassisSlot, ship))
		return false;

	m_boosterEnergyCurrent          = ship.getBoosterEnergyCurrent ();
	m_boosterEnergyMaximum          = ship.getBoosterEnergyMaximum ();
	m_boosterEnergyRechargeRate     = ship.getBoosterEnergyRechargeRate ();
	m_boosterEnergyConsumptionRate  = ship.getBoosterEnergyConsumptionRate ();
	m_boosterAcceleration           = ship.getBoosterAcceleration ();
	m_boosterSpeedMaximum           = ship.getBoosterSpeedMaximum ();
	m_style                         = ship.getComponentStyle(chassisSlot);

	return true;
}

//----------------------------------------------------------------------

void ShipComponentDataBooster::writeDataToShip       (int chassisSlot, ShipObject & ship) const
{
	ServerShipComponentData::writeDataToShip (chassisSlot, ship);

	IGNORE_RETURN(ship.setBoosterEnergyMaximum         (m_boosterEnergyMaximum));
	IGNORE_RETURN(ship.setBoosterEnergyCurrent         (m_boosterEnergyCurrent));
	IGNORE_RETURN(ship.setBoosterEnergyRechargeRate    (m_boosterEnergyRechargeRate));
	IGNORE_RETURN(ship.setBoosterEnergyConsumptionRate (m_boosterEnergyConsumptionRate));
	IGNORE_RETURN(ship.setBoosterAcceleration          (m_boosterAcceleration));
	IGNORE_RETURN(ship.setBoosterSpeedMaximum          (m_boosterSpeedMaximum));
	IGNORE_RETURN(ship.setComponentStyle               (chassisSlot, m_style));
}

//----------------------------------------------------------------------

bool ShipComponentDataBooster::readDataFromComponent (TangibleObject const & component)
{
	if (!ServerShipComponentData::readDataFromComponent (component))
		return false;

	DynamicVariableList const &  objvars = component.getObjVars ();

	if (!objvars.getItem (Objvars::boosterEnergyCurrent, m_boosterEnergyCurrent))
		WARNING (true, ("ShipComponentDataBooster [%s] has no boosterEnergyCurrent [%s]", component.getNetworkId ().getValueString ().c_str (), Objvars::boosterEnergyCurrent.c_str ()));

	if (!objvars.getItem (Objvars::boosterEnergyMaximum, m_boosterEnergyMaximum))
		WARNING (true, ("ShipComponentDataBooster [%s] has no boosterEnergyMaximum [%s]", component.getNetworkId ().getValueString ().c_str (), Objvars::boosterEnergyMaximum.c_str ()));

	if (!objvars.getItem (Objvars::boosterEnergyRechargeRate, m_boosterEnergyRechargeRate))
		WARNING (true, ("ShipComponentDataBooster [%s] has no sboosterEnergyRechargeRate [%s]", component.getNetworkId ().getValueString ().c_str (), Objvars::boosterEnergyRechargeRate.c_str ()));

	if (!objvars.getItem (Objvars::boosterEnergyConsumptionRate, m_boosterEnergyConsumptionRate))
		WARNING (true, ("ShipComponentDataBooster [%s] has no boosterEnergyConsumptionRate [%s]", component.getNetworkId ().getValueString ().c_str (), Objvars::boosterEnergyConsumptionRate.c_str ()));

	if (!objvars.getItem (Objvars::boosterAcceleration, m_boosterAcceleration))
		WARNING (true, ("ShipComponentDataBooster [%s] has no boosterAcceleration [%s]", component.getNetworkId ().getValueString ().c_str (), Objvars::boosterAcceleration.c_str ()));

	if (!objvars.getItem (Objvars::boosterSpeedMaximum, m_boosterSpeedMaximum))
		WARNING (true, ("ShipComponentDataBooster [%s] has no boosterSpeedMaximum [%s]", component.getNetworkId ().getValueString ().c_str (), Objvars::boosterSpeedMaximum.c_str ()));

	if (!objvars.getItem(Objvars::componentStyle, m_style))
	{
		WARNING (true, ("ShipComponentDataBooster Name: [%s], ID [%s] has no componentStyle [%s]", Unicode::wideToNarrow(component.getObjectName()).c_str(), component.getNetworkId().getValueString().c_str(), Objvars::componentStyle.c_str()));
	}

	return true;
}

//----------------------------------------------------------------------

void ShipComponentDataBooster::writeDataToComponent  (TangibleObject & component) const
{
	ServerShipComponentData::writeDataToComponent (component);

	IGNORE_RETURN(component.setObjVarItem (Objvars::boosterEnergyCurrent,         m_boosterEnergyCurrent));
	IGNORE_RETURN(component.setObjVarItem (Objvars::boosterEnergyMaximum,         m_boosterEnergyMaximum));
	IGNORE_RETURN(component.setObjVarItem (Objvars::boosterEnergyRechargeRate,    m_boosterEnergyRechargeRate));
	IGNORE_RETURN(component.setObjVarItem (Objvars::boosterEnergyConsumptionRate, m_boosterEnergyConsumptionRate));
	IGNORE_RETURN(component.setObjVarItem (Objvars::boosterAcceleration,          m_boosterAcceleration));
	IGNORE_RETURN(component.setObjVarItem (Objvars::boosterSpeedMaximum,          m_boosterSpeedMaximum));
	IGNORE_RETURN(component.setObjVarItem (Objvars::componentStyle,               m_style));
}


//----------------------------------------------------------------------

void ShipComponentDataBooster::printDebugString      (Unicode::String & result, Unicode::String const & padding) const
{
	ServerShipComponentData::printDebugString (result, padding);
	
	char buf [1024];
	const size_t buf_size = sizeof (buf);
	
	std::string const & nPad = Unicode::wideToNarrow (padding) + "   ";
	
	snprintf (buf, buf_size,
		"%sboosterEnergyCurrent: %f\n"
		"%sboosterEnergyMaximum: %f\n"
		"%sboosterEnergyRechargeRate: %f\n"
		"%sboosterEnergyConsumptionRate: %f\n"
		"%sboosterAcceleration: %f\n"
		"%sboosterSpeedMaximum: %f\n"
		"%sStyle:        %d\n",
		nPad.c_str (), m_boosterEnergyCurrent,
		nPad.c_str (), m_boosterEnergyMaximum,
		nPad.c_str (), m_boosterEnergyRechargeRate,
		nPad.c_str (), m_boosterEnergyConsumptionRate,
		nPad.c_str (), m_boosterAcceleration,
		nPad.c_str (), m_boosterSpeedMaximum,
		nPad.c_str(), m_style);
	
	result += Unicode::narrowToWide (buf);
}

//----------------------------------------------------------------------

void ShipComponentDataBooster::getAttributes(std::vector<std::pair<std::string, Unicode::String> > & data) const
{
	ServerShipComponentData::getAttributes(data);

	Unicode::String attrib;
	static char buffer[128];
	static const size_t buffer_size = sizeof (buffer);

	snprintf(buffer, buffer_size, "%.1f", m_boosterEnergyCurrent);
	attrib = Unicode::narrowToWide(buffer);
	attrib += cm_slash;
	snprintf(buffer, buffer_size, "%.1f", m_boosterEnergyMaximum);
	attrib += Unicode::narrowToWide(buffer);
	data.push_back(std::make_pair(cm_shipComponentCategory + SharedObjectAttributes::ship_component_booster_energy, attrib));

	snprintf(buffer, buffer_size, "%.1f", m_boosterEnergyRechargeRate);
	attrib = Unicode::narrowToWide(buffer);
	data.push_back(std::make_pair(cm_shipComponentCategory + SharedObjectAttributes::ship_component_booster_energy_recharge_rate, attrib));

	snprintf(buffer, buffer_size, "%.1f", m_boosterEnergyConsumptionRate);
	attrib = Unicode::narrowToWide(buffer);
	data.push_back(std::make_pair(cm_shipComponentCategory + SharedObjectAttributes::ship_component_booster_energy_consumption_rate, attrib));
	
	snprintf(buffer, buffer_size, "%.1f", m_boosterAcceleration);
	attrib = Unicode::narrowToWide(buffer);
	data.push_back(std::make_pair(cm_shipComponentCategory + SharedObjectAttributes::ship_component_booster_acceleration, attrib));

	snprintf(buffer, buffer_size, "%.1f", m_boosterSpeedMaximum);
	attrib = Unicode::narrowToWide(buffer);
	data.push_back(std::make_pair(cm_shipComponentCategory + SharedObjectAttributes::ship_component_booster_speed_maximum, attrib));

	snprintf(buffer, buffer_size, "%d", m_style);
	attrib = Unicode::narrowToWide(buffer);
	data.push_back(std::make_pair(cm_shipComponentVisualsCategory + SharedObjectAttributes::ship_component_style, attrib));
}

//======================================================================
