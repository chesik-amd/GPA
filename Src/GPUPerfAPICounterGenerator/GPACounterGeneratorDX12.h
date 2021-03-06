//==============================================================================
// Copyright (c) 2015-2017 Advanced Micro Devices, Inc. All rights reserved.
/// \author AMD Developer Tools Team
/// \file
/// \brief  Class for DX12 counter generation
//==============================================================================

#ifndef _GPA_COUNTER_GENERATOR_DX12_H_
#define _GPA_COUNTER_GENERATOR_DX12_H_

#include "GPACounterGeneratorDX12Base.h"

/// The DX12-specific counter generator
class GPA_CounterGeneratorDX12 : public GPA_CounterGeneratorDX12Base
{
public:
    /// Constructor
    GPA_CounterGeneratorDX12();

    /// Destructor
    virtual ~GPA_CounterGeneratorDX12();

protected:

    /// \copydoc GPA_CounterGeneratorBase::GeneratePublicCounters
    virtual GPA_Status GeneratePublicCounters(
        GDT_HW_GENERATION desiredGeneration,
        GDT_HW_ASIC_TYPE asicType,
        gpa_uint8 generateAsicSpecificCounters,
        GPA_PublicCounters* pPublicCounters) override;

    /// \copydoc GPA_CounterGeneratorBase::GenerateHardwareCounters
    virtual GPA_Status GenerateHardwareCounters(
        GDT_HW_GENERATION desiredGeneration,
        GDT_HW_ASIC_TYPE asicType,
        gpa_uint8 generateAsicSpecificCounters,
        GPA_HardwareCounters* pHardwareCounters) override;

private:

    /// Helper function to indicates whether the specified HW generation represents an AMD GPU
    /// \param generation the hw generation to check
    /// \return true if the specified HW generation represents an AMD GPU
    static bool IsAMDGPU(GDT_HW_GENERATION generation);

    /// Logic inside this function is based on the AmdExtGpuBlock enum in AmdExtGpaInterface in DXCP driver.
    /// The driver gives each block an ID, but ignores the instance. GPA treats each instance as a different
    /// block, so we need to translate.
    /// \param generation the generation whose block id needs to be calculated
    /// \param pGroup the group for which the block id needs to be calculated
    /// \return the block id according to the driver
    static UINT CalculateBlockIdDX12(GDT_HW_GENERATION generation, GPA_CounterGroupDesc* pGroup);

    /// Generates internal counters
    /// \param pHardwareCounters the hardware counters to generate
    /// \param generation the generation for which counters need to be generated
    /// \return true on success
    static bool GenerateInternalCounters(GPA_HardwareCounters* pHardwareCounters, GDT_HW_GENERATION generation);

    /// Copy constructor - private override to prevent usage
    GPA_CounterGeneratorDX12(const GPA_CounterGeneratorDX12Base&) = delete;

    /// Move constructor - private override to prevent usage
    GPA_CounterGeneratorDX12(GPA_CounterGeneratorDX12Base&&) = delete;

    /// Copy operator - private override to prevent usage
    /// \return reference to object
    GPA_CounterGeneratorDX12& operator=(const GPA_CounterGeneratorDX12&) = delete;

    /// Move operator - private override to prevent usage
    /// \return reference to object
    GPA_CounterGeneratorDX12& operator=(GPA_CounterGeneratorDX12&&) = delete;
};

#endif // _GPA_COUNTER_GENERATOR_DX12_H_

