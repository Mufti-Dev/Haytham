#include <Dmo.h>
#include <DShow.h>
#include <dmodshow.h>

CLSID	g_clsidDMO;
BOOL	g_bFound;

// Enumerate audio DMO and pick Echo, DMO
void EnumAudioDMO()
{
	IEnumDMO* pEnum = NULL;
	HRESULT hr = DMOEnum(
		DMOCATEGORY_AUDIO_EFFECT, // Category
		DMO_ENUMF_INCLUDE_KEYED,  // Included keyed DMOs
		0, NULL,                  // Input types (don't care)
		0, NULL,                  // Output types (don't care)
		&pEnum);

	if (SUCCEEDED(hr)) 
	{
		CLSID clsidDMO;
		WCHAR* wszName;
		do
		{
			hr = pEnum->Next(1, &clsidDMO, &wszName, NULL);
			if (hr == S_OK) 
			{  
				// Now wszName holds the friendly name of the DMO, 
				// and clsidDMO holds the CLSID. 

				wprintf(L"DMO Name: %s\n", wszName);
				if(wcscmp(wszName, L"Echo") == 0)
				{
					g_clsidDMO = clsidDMO;
					g_bFound = TRUE;
				}
				
				// Remember to release wszName!
				CoTaskMemFree(wszName);
			}
		} while (hr == S_OK);
		pEnum->Release();
	}
}

void BuildFilterGraph()
{
	CoInitialize(NULL);

	IGraphBuilder	*pGraphBuilder = NULL;
	IBaseFilter		*pFilter = NULL;
	IMediaControl	*pMediaControl = NULL;
	IMediaEventEx	*pEvt = NULL;
	
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
		IID_IGraphBuilder, (void **)&pGraphBuilder);

	pGraphBuilder->QueryInterface(IID_IMediaControl, reinterpret_cast<void **>(&pMediaControl));
	pGraphBuilder->QueryInterface(IID_IMediaEventEx, reinterpret_cast<void **>(&pEvt));

	if(g_bFound)	// If found then add the Echo DMO to the filter graph
	{
		HRESULT hr = CoCreateInstance(CLSID_DMOWrapperFilter, NULL, 
			CLSCTX_INPROC_SERVER, IID_IBaseFilter, reinterpret_cast<void**>(&pFilter));

		if (SUCCEEDED(hr)) 
		{
			// Query for IDMOWrapperFilter.
			IDMOWrapperFilter *pDmoWrapper;
			hr = pFilter->QueryInterface(IID_IDMOWrapperFilter, reinterpret_cast<void**>(&pDmoWrapper));

			if (SUCCEEDED(hr)) 
			{     
				// Initialize the filter.
				hr = pDmoWrapper->Init(g_clsidDMO, DMOCATEGORY_VIDEO_EFFECT); 
				pDmoWrapper->Release();

				if (SUCCEEDED(hr)) 
				{
					// Add the filter to the graph.
					hr = pGraphBuilder->AddFilter(pFilter, L"Echo");
				}
			}
		}
	}

	pGraphBuilder->RenderFile(L"somevideo.wmv", NULL); // Path of windows media file
	
	pMediaControl->Run();

	long evCode; 
	pEvt->WaitForCompletion(INFINITE, &evCode);

	pFilter->Release();
	pGraphBuilder->Release();
}

int _tmain(int argc, _TCHAR* argv[])
{
	g_bFound = FALSE;

	EnumAudioDMO();
	BuildFilterGraph();

	return 0;
}