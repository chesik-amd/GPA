//==============================================================================
// Copyright (c) 2010-2018 Advanced Micro Devices, Inc. All rights reserved.
/// \author AMD Developer Tools Team
/// \file
/// \brief  VK internal counter definitions for GFX8
//==============================================================================

#ifndef _GPA_INTERNAL_COUNTERS_VK_GFX8_H_
#define _GPA_INTERNAL_COUNTERS_VK_GFX8_H_
// This file is autogenerated by the ConvertHWEnums project

#include <vulkan.h>
#include <vk_amd_gpa_interface.h>
extern GPA_HardwareCounterDesc* VKCounterGroupArrayGfx8[]; ///< Array of hardware counter groups for VK for Gfx8 family
extern GPA_CounterGroupDesc HWVKGroupsGfx8[]; ///< Array of counter groups for VK for Gfx8 family
extern GPA_SQCounterGroupDesc HWVKSQGroupsGfx8[]; ///< Array of SQ groups for VK for Gfx8 family
extern unsigned int HWVKSQIsolatedGroupsGfx8[]; ///< Array of counter groups isolated from SQ counter groups for VK for Gfx8 family
extern VkGpaPerfBlockAMD HWVKDriverEnumGfx8[]; ///< Array of driver enum values per group for VK for Gfx8 family
extern const unsigned int HWVKGPUTimeIndexGfx8; ///< Index of GPUTime Group for VK for Gfx8 family
extern const unsigned int HWVKGroupCountGfx8; ///< Hardware Group Count for VK for Gfx8 family
extern const unsigned int HWVKSQGroupCountGfx8; ///< Hardware SQ Group Count for VK for Gfx8 family
extern const unsigned int HWVKSQIsolatedGroupCountGfx8; ///< Hardware Isolated Group Count for VK for Gfx8 family

#endif // _GPA_INTERNAL_COUNTERS_VK_GFX8_H_
