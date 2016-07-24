//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Authors: Christopher
//               adamix
// License: See LICENSE in root directory
//
//-----------------------------------------------------

#include "main.h"
#include "CScripts.h"

SQInteger sq_createVehicle(SQVM * v); // done
SQInteger sq_destroyVehicle(SQVM * v); // done
SQInteger sq_setVehicleHealth(SQVM * v); // done
SQInteger sq_setVehicleColor(SQVM * v); // done
SQInteger sq_getVehicleHealth(SQVM * v); //done
SQInteger sq_getVehicleColors(SQVM * v); //done
SQInteger sq_getVehicleTurnSpeed(SQVM * v); // done
SQInteger sq_getVehicleMoveSpeed(SQVM * v); // done
SQInteger sq_getVehiclePosition(SQVM * pVM); // done
SQInteger sq_setVehiclePosition(SQVM * pVM); // done
SQInteger sq_setVehicleTurnSpeed(SQVM * pVM); // done
SQInteger sq_setVehicleMoveSpeed(SQVM * pVM); // done

// Added by VC-Players team.
SQInteger sq_popVehicleTrunk(SQVM * pVM);// done