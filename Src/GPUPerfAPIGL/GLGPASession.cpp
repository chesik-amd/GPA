//==============================================================================
// Copyright (c) 2018 Advanced Micro Devices, Inc. All rights reserved.
/// \author AMD Developer Tools Team
/// \file
/// \brief GL GPA Session Implementation
//==============================================================================

#include "GLGPASession.h"
#include "GLGPAPass.h"

GLGPASession::GLGPASession(
    IGPAContext* pParentContext,
    GPA_Session_Sample_Type sampleType) :
    GPASession(pParentContext, pParentContext->GetCounterScheduler(), sampleType)
{
}

GPA_API_Type GLGPASession::GetAPIType() const
{
    return GPA_API_OPENGL;
}

GPAPass* GLGPASession::CreateAPIPass(PassIndex passIndex)
{
    GPAPass* pRetPass = nullptr;

    CounterList* passCounters = GetCounterScheduler()->GetCountersForPass(passIndex);
    GPACounterSource counterSource = GetParentContext()->GetCounterSource((*passCounters)[0]);

    GLGPAPass* pGlPass = new(std::nothrow) GLGPAPass(this, passIndex, counterSource, GetCounterScheduler(), GetParentContext()->GetCounterAccessor());

    if (nullptr == pGlPass)
    {
        GPA_LogError("Unable to allocate memory for the pass.");
    }
    else
    {
        pRetPass = pGlPass;
    }

    return pRetPass;
}
