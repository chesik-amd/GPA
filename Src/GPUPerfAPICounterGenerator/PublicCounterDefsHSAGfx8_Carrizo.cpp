//==============================================================================
// Copyright (c) 2010-2018 Advanced Micro Devices, Inc. All rights reserved.
/// \author AMD Developer Tools Team
/// \file
/// \brief PublicCounterDefinitions for HSAGFX8_CARRIZO
//==============================================================================

#include "PublicCounterDefsHSAGfx8_Carrizo.h"

// *** Note, this is an auto-generated file. Do not edit. Execute PublicCounterCompiler to rebuild.

#include "GPAInternalCountersGfx8_Carrizo.h"

namespace HSAGfx8_Carrizo
{

bool UpdateAsicSpecificCounters(GDT_HW_GENERATION desiredGeneration, GDT_HW_ASIC_TYPE asicType, GPA_PublicCounters& p)
{
    UNREFERENCED_PARAMETER(desiredGeneration);
    UNREFERENCED_PARAMETER(p); // Unreferenced if there are no ASIC specific block instance registers

    if (!CounterGfx8_Carrizo::MatchAsic(asicType))
    {
        return false;
    }

    CounterGfx8_Carrizo::OverrideBlockInstanceCounters(asicType);

    return true;

}

} // HSAGfx8_Carrizo

