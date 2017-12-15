//==============================================================================
// Copyright (c) 2015-2017 Advanced Micro Devices, Inc. All rights reserved.
/// \author AMD Developer Tools Team
/// \file
/// \brief  VkGPASoftwareSample implementation
//==============================================================================

#include "VkGPASoftwareSample.h"
#include "VkGPAContext.h"
#include "VkGPACommandList.h"
#include "GPASoftwareCounters.h"
#include "GPAHardwareCounters.h"

VkGPASoftwareSample::VkGPASoftwareSample(GPAPass* pPass,
    IGPACommandList* pCmdList,
    unsigned int sampleId)
    :
    VkGPASample(pPass, pCmdList, GpaSampleType::Software, sampleId),
    m_pContextState(nullptr),
    m_activeCountersList(),
    m_activeQueries(0),
    m_commandList(m_pVkGpaCmdList->GetVkCommandBuffer()),
    m_swSampleId(ms_unitializedSampleId),
    m_pSwQueries(nullptr)
{
}

VkGPASoftwareSample::~VkGPASoftwareSample()
{
    m_pContextState = nullptr;
    m_activeCountersList.clear();
    m_activeQueries = 0;
    m_swSampleId = ms_unitializedSampleId;
}

void VkGPASoftwareSample::AssignQueries(VkCommandListSwQueries* pSwQueries)
{
    m_pSwQueries = pSwQueries;
}

bool VkGPASoftwareSample::BeginRequest(
    IGPAContext* pContextState,
    const std::vector<gpa_uint32>* pCounters)
{
    bool result = true;

    if ((nullptr == pContextState) || (nullptr == pCounters) || (nullptr != m_pContextState) || (nullptr == m_pVkGpaCmdList))
    {
        result = false;
    }
    else
    {
        m_pContextState = static_cast<VkGPAContext*>(pContextState);
        const size_t counterCount = pCounters->size();

        const IGPACounterAccessor* pCounterAccessor = m_pContextState->GetCounterAccessor();
        const GPA_SoftwareCounters* pSwCounters = pCounterAccessor->GetSoftwareCounters();
        const GPA_HardwareCounters* pHwCounters = pCounterAccessor->GetHardwareCounters();
        m_activeCountersList.resize(counterCount);

        gpa_uint32 hardwareCountersCount = pHwCounters->GetNumCounters();

        for (size_t ci = 0; result && (ci < counterCount) ; ++ci)
        {
            gpa_uint32 swCounterIndex = (*pCounters)[ci];
            m_activeCountersList[ci].m_index =
                SwCounterManager::Instance()->GetSwCounterPubIndex(swCounterIndex);

            // software counter indices are after the hardware counter
            gpa_uint32 counterIdDriver = pSwCounters->m_counters[swCounterIndex - hardwareCountersCount].m_counterIdDriver;
            m_activeCountersList[ci].m_queryType = static_cast<GPA_VK_SW_QUERY_TYPE>(counterIdDriver);
            result = (counterIdDriver < sizeof(m_activeQueries));

            if (result)
            {
                m_activeQueries |= (0x1 << counterIdDriver);
            }
        }

        if (result)
        {
            unsigned int activeQueries = m_activeQueries;
            bool beginQuery = (0 != activeQueries);
            result = m_pSwQueries->BeginSwSample(m_swSampleId);

            if (result)
            {
                while (beginQuery)
                {
                    GPA_VK_SW_QUERY_TYPE queryType;
#ifdef _WIN32
                    _BitScanForward(reinterpret_cast<unsigned long*>(&queryType), activeQueries);
#else
                    queryType = static_cast<GPA_VK_SW_QUERY_TYPE>(__builtin_clz(activeQueries)); // TODO: verify that this works
#endif
                    m_pSwQueries->BeginSwQuery(m_swSampleId, queryType);
                    activeQueries &= ~(0x1 << static_cast<unsigned int>(queryType));
                    beginQuery = (0 != activeQueries);
                }
            }
        }
    }

    return result;
}

bool VkGPASoftwareSample::EndRequest()
{
    bool result = true;

    if (nullptr == m_pContextState)
    {
        result = false;
    }
    else
    {
        unsigned int activeQueries = m_activeQueries;
        bool endQuery = (0 != activeQueries);

        while (endQuery)
        {
            GPA_VK_SW_QUERY_TYPE queryType;
#ifdef _WIN32
            _BitScanForward(reinterpret_cast<unsigned long*>(&queryType), activeQueries);
#else
            queryType = static_cast<GPA_VK_SW_QUERY_TYPE>(__builtin_clz(activeQueries)); //TODO: verify that this works
#endif
            m_pSwQueries->EndSwQuery(m_swSampleId, queryType);
            activeQueries &= ~(0x1 << static_cast<unsigned int>(queryType));
            endQuery = (0 != activeQueries);
        }

        m_pSwQueries->EndSwSample(m_swSampleId);
    }

    return result;
}

void VkGPASoftwareSample::ReleaseCounters()
{
}

bool VkGPASoftwareSample::GetTimestampQueryCounterResult(
    const GpaVkSoftwareQueryResults& queryResults,
    const gpa_uint32 counterIndex,
    gpa_uint64& counterResult)
const
{
    bool result = true;

    const char* pCounterName = m_pContextState->GetCounterAccessor()->GetCounterName(counterIndex);

    if (0 == strcmp("VKGPUTime", pCounterName))
    {
        counterResult = (queryResults.timestampEnd - queryResults.timestampBegin);
    }
    else if (0 == strcmp("PreBottomTimestamp", pCounterName))
    {
        counterResult = queryResults.timestampBegin;
    }
    else if (0 == strcmp("PostBottomTimestamp", pCounterName))
    {
        counterResult = queryResults.timestampEnd;
    }
    else
    {
        result = false;
    }

    return result;
}

bool VkGPASoftwareSample::GetPipelineQueryCounterResult(
    const GpaVkSoftwareQueryResults& queryResults,
    const gpa_uint32 counterIndex,
    gpa_uint64& counterResult)
const
{
    bool result = true;
    const IGPACounterAccessor* pCounterAccessor = m_pContextState->GetCounterAccessor();
    const char* pCounterName = pCounterAccessor->GetCounterName(counterIndex);

    if (0 == strcmp("IAVertices", pCounterName))
    {
        counterResult = queryResults.inputAssemblyVertices;
    }
    else if (0 == strcmp("IAPrimitives", pCounterName))
    {
        counterResult = queryResults.inputAssemblyPrimitives;
    }
    else if (0 == strcmp("VSInvocations", pCounterName))
    {
        counterResult = queryResults.vertexShaderInvocations;
    }
    else if (0 == strcmp("GSInvocations", pCounterName))
    {
        counterResult = queryResults.geometryShaderInvocations;
    }
    else if (0 == strcmp("GSPrimitives", pCounterName))
    {
        counterResult = queryResults.geometryShaderPrimitives;
    }
    else if (0 == strcmp("CInvocations", pCounterName))
    {
        counterResult = queryResults.clippingInvocations;
    }
    else if (0 == strcmp("CPrimitives", pCounterName))
    {
        counterResult = queryResults.clippingPrimitives;
    }
    else if (0 == strcmp("PSInvocations", pCounterName))
    {
        counterResult = queryResults.fragmentShaderInvocations;
    }
    else if (0 == strcmp("TCSInvocations", pCounterName))
    {
        counterResult = queryResults.tessellationControlShaderPatches;
    }
    else if (0 == strcmp("TESInvocations", pCounterName))
    {
        counterResult = queryResults.tessellationEvaluationShaderInvocations;
    }
    else if (0 == strcmp("CSInvocations", pCounterName))
    {
        counterResult = queryResults.computeShaderInvocations;
    }
    else
    {
        result = false;
    }

    return result;
}

bool VkGPASoftwareSample::UpdateResults()
{
    if (GPASampleState::RESULTS_COLLECTED == m_sampleState)
    {
        return true;
    }

    bool isUpdated = false;

    if (IsSecondary())
    {
        MarkAsCompleted();
        isUpdated = true;
    }

    if (GPASampleState::PENDING_RESULTS == m_sampleState && !IsSecondary())
    {
        GpaVkSoftwareQueryResults queryResults;
        memset(&queryResults, 0, sizeof(queryResults));

        if (m_pSwQueries != nullptr)
        {
            if (m_pSwQueries->GetSwSampleResults(m_swSampleId, queryResults))
            {
                m_pSampleResult->m_numCounters = m_activeCountersList.size();
                const size_t counterCount = m_activeCountersList.size();
                isUpdated = (counterCount == m_pSampleResult->m_numCounters);

                for (size_t ci = 0; isUpdated && (counterCount > ci); ++ci)
                {
                    switch (m_activeCountersList[ci].m_queryType)
                    {
                    case GPA_VK_QUERY_TYPE_OCCLUSION:
                        m_pSampleResult->m_pResultBuffer[ci] = queryResults.occlusion;
                        break;

                    case GPA_VK_QUERY_TYPE_OCCLUSION_BINARY:
                        m_pSampleResult->m_pResultBuffer[ci] = queryResults.occlusionBinary;
                        break;

                    case GPA_VK_QUERY_TYPE_TIMESTAMP:
                        isUpdated = GetTimestampQueryCounterResult(
                            queryResults,
                            m_activeCountersList[ci].m_index,
                            m_pSampleResult->m_pResultBuffer[ci]);
                        break;

                    case GPA_VK_QUERY_TYPE_PIPELINE_STATISTICS:
                        isUpdated = GetPipelineQueryCounterResult(
                            queryResults,
                            m_activeCountersList[ci].m_index,
                            m_pSampleResult->m_pResultBuffer[ci]);
                        break;

                    default:
                        isUpdated = false;
                        break;
                    }
                }

                m_pSwQueries->ReleaseSwSample(m_swSampleId);
            }
        }
    }

    if (isUpdated)
    {
        MarkAsCompleted();
    }

    return isUpdated;
}