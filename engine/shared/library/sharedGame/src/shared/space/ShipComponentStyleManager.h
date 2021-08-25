//======================================================================
//
// ShipComponentStyleManager.h
// copyright (c) 2004 Sony Online Entertainment
//
//======================================================================

#ifndef INCLUDED_ShipComponentStyleManager_H
#define INCLUDED_ShipComponentStyleManager_H

//======================================================================

class ShipComponentStyleManager
{
public:
	static void install();
	static void remove();
	static int getDefaultStyleForComponent(uint32 componentCrc);
};

//======================================================================

#endif
