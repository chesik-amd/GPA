//==============================================================================
// Copyright (c) 2017 Advanced Micro Devices, Inc. All rights reserved.
/// \author AMD Developer Tools Team
/// \file
/// \brief  Class for common DX12 counter generation
//==============================================================================

#ifndef _GPA_COUNTER_GENERATOR_DX12_BASE_H_
#define _GPA_COUNTER_GENERATOR_DX12_BASE_H_

#include "GPACounterGeneratorBase.h"

/// The DX12 common counter generator
class GPA_CounterGeneratorDX12Base : public GPA_CounterGeneratorBase
{
public:
    /// Get the SW counter description for the given SW counter index
    /// \return The SW counter description
    /// \param[in] swCounterIndex The SW counter index
    /// \param[out] swCounterDesc Software counter description
    static bool GetSwCounterDesc(const gpa_uint32 swCounterIndex, GPA_SoftwareCounterDesc& swCounterDesc);

    /// Constructor
    GPA_CounterGeneratorDX12Base() = default;

    /// Virtual Destructor
    virtual ~GPA_CounterGeneratorDX12Base();

protected:

    /// Overridden methods -- see base for documentation
    GPA_Status GeneratePublicCounters(
        GDT_HW_GENERATION desiredGeneration, GPA_PublicCounters* pPublicCounters) override;

    GPA_Status GenerateHardwareCounters(
        GDT_HW_GENERATION desiredGeneration, GPA_HardwareCounters* pHardwareCounters) override;

    GPA_Status GenerateSoftwareCounters(
        GDT_HW_GENERATION desiredGeneration, GPA_SoftwareCounters* pSoftwareCounters) override;

    void ComputeSWCounterValue(
        gpa_uint32 counterIndex, gpa_uint64 value, void* pResult, const GPA_HWInfo* pHwInfo) const override;

private:

    static const GPA_SoftwareCounterDesc s_dx12SWCounters[];    ///< DX12 SW counters list
    static const size_t s_dx12SWCountersCount;                  ///< DX12 SW counter list size

    /// Copy constructor - private override to prevent usage
    GPA_CounterGeneratorDX12Base(const GPA_CounterGeneratorDX12Base&) = delete;

    /// Move constructor - private override to prevent usage
    GPA_CounterGeneratorDX12Base(GPA_CounterGeneratorDX12Base&&) = delete;

    /// Copy operator - private override to prevent usage
    GPA_CounterGeneratorDX12Base& operator=(const GPA_CounterGeneratorDX12Base&) = delete;

    /// Move operator - private override to prevent usage
    GPA_CounterGeneratorDX12Base& operator=(GPA_CounterGeneratorDX12Base&&) = delete;

    /// Generate DX12 API software counters
    /// \return GPA_STATUS_OK if generating counters succeeded, false if it failed
    GPA_Status GenerateDX12SoftwareCounters() const;
};

#endif // _GPA_COUNTER_GENERATOR_DX12_BASE_H_
