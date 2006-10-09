

#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "config.h"
#include "texture_system.h"
#include "material_system.h"
#include "light_system.h"
#include "model_loader.h"
#include "resmgr.h"
#include "camera.h"
#include "hoe_model.h"
#include "hoe_stream.h"
#include <hoe_math.h>
#include "sound.h"
#include "camera.h"
#include "states.h"
#include "hoe_time.h"
#include "hoe2d.h"
#include "hoe_map.h"
#include "hoe_input.h"
#include "hoe_info.h"
#include "cursor.h"
#include "hoe_font.h"
#include "physics.h"
#include "hoe.h"
#include "hoe_tokens.h"

IHoeSystem * HOEAPI Hoe3D::GetSystem(HOESYSTEMS sys)
{
	switch (sys)
	{
	case HOE_SYS_REF:
		return GetRef();
	case HOE_SYS_MATERIAL:
		return GetMaterialSystem();
	case HOE_SYS_INFO:
		return GetInfo();
	case HOE_SYS_INPUT:
		return (IsInputLoaded()) ? GetInput():NULL;
	case HOE_SYS_RESMGR:
		return GetResMgr();
	default:
		Con_Print("error get system %d",sys);
		return NULL;
	};

} 



 
