// Indexer.cpp : Defines the entry point for the console application.
// ASF file indexer (Experimental code to index ASF file (container / Blanket for audio / video streams). Just a windows console based c++ app program)

#include "stdafx.h"

#include <Windows.h>
#include <wmcontainer.h>
#include <mfapi.h>

#include <comdef.h>
#include <atlbase.h>
#include <atlconv.h>

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

#pragma comment(lib, "Mfuuid.lib")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplat.lib")

#define WIDEN2(x) L ## x
#define WIDEN(x)  WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)

const DWORD g_dwIndexingInterval = 1000; // msec
const DWORD g_cbBufferSize       = 16 * 1024; // bytes

#ifdef _DEBUG
#	define ASSERT(x)      Assert(!(x), __WFILE__, __LINE__)
#	define CHECK(hresult) CheckHRESULT(hresult, true, __WFILE__, __LINE__)
#	define WARN(hresult)  CheckHRESULT(hresult, false, __WFILE__, __LINE__)
#else
#	define ASSERT(x)
#	define CHECK(hresult) CheckHRESULT(hresult, true)
#	define WARN(hresult)  CheckHRESULT(hresult, false)
#endif // _DEBUG

namespace ASF
{

using namespace std::tr1::placeholders;

#ifdef _DEBUG
void Assert(bool fFailed, LPCWSTR pwszFileName, INT nLineNum)
{
	if(fFailed)
	{
		std::wcerr << L"Indexer: Assertion failed at " << pwszFileName;
		std::wcerr << L"(" << nLineNum << L")" << std::endl;
		DebugBreak();
	}
}
#endif // _DEBUG

void CheckHRESULT(
	HRESULT hRes,
	bool fThrow,
	LPCWSTR pwszFileName = NULL,
	INT nLineNum = 0)
{
	if(FAILED(hRes))
	{
		_com_error Error(hRes);
		
		std::wcerr << L"Indexer: ";		
		if(pwszFileName && nLineNum)
			std::wcerr << L"(" << pwszFileName << L":" << nLineNum << L") ";
		std::wcerr << Error.ErrorMessage() << std::endl;

#ifdef _DEBUG
		DebugBreak();
#endif
		if(fThrow)
			throw Error;
	}
}

struct CMFSession
{
	CMFSession()
	{
		CHECK(::MFStartup(MF_VERSION));
	}

	~CMFSession()
	{
		WARN(::MFShutdown());
	}
};

class CByteStream
{
private:
	class CBufferLock
	{
		DWORD m_cbMaxLength;
		DWORD m_cbCurrentLength;
		ATL::CComPtr<IMFMediaBuffer> m_pBuffer;
		BYTE* m_pData;

	public:
		CBufferLock(IMFMediaBuffer* pBuffer) :
			m_cbMaxLength(0),
			m_cbCurrentLength(0),
			m_pBuffer(pBuffer),
			m_pData(NULL)
		{
			ASSERT(m_pBuffer);

			CHECK(m_pBuffer->Lock(&m_pData, &m_cbMaxLength, &m_cbCurrentLength));		
		}

		~CBufferLock()
		{
			WARN(m_pBuffer->Unlock());
		}

		BYTE* AccessData() const
		{
			return m_pData;
		}


		inline DWORD GetMaxLength() const
		{
			return m_cbMaxLength;
		}

		inline DWORD GetCurrentLength() const
		{
			return m_cbCurrentLength;
		}
	};

public:
	CByteStream(
		const std::wstring& strFileName,
		MF_FILE_ACCESSMODE AccessMode,
		MF_FILE_OPENMODE OpenMode
	) :
		m_pByteStream(OpenFile(strFileName, AccessMode, OpenMode))
	{}

	ATL::CComPtr<IMFMediaBuffer> Read(DWORD cbSize)
	{
		ATL::CComPtr<IMFMediaBuffer> pBuffer;
		CHECK(::MFCreateMemoryBuffer(cbSize, &pBuffer));

		DWORD cbRead = 0;
		CHECK(m_pByteStream->Read(CBufferLock(pBuffer).AccessData(), cbSize, &cbRead));
		CHECK(pBuffer->SetCurrentLength(cbRead));
		if(CBufferLock(pBuffer).AccessData())
		{
			pBuffer->Unlock();
		}
		return pBuffer;
	}

	DWORD Write(IMFMediaBuffer* pBuffer)
	{
		ASSERT(pBuffer);

		DWORD cbWritten = 0;

		CBufferLock lock(pBuffer);
		CHECK(m_pByteStream->Write(lock.AccessData(), lock.GetCurrentLength(), &cbWritten));

		return cbWritten;
	}

	QWORD GetCurrentPosition() const
	{
		QWORD cbPosition = 0;
		CHECK(m_pByteStream->GetCurrentPosition(&cbPosition));

		return cbPosition;
	}

	void SetCurrentPosition(QWORD cbPosition)
	{
		CHECK(m_pByteStream->SetCurrentPosition(cbPosition));
	}

	bool IsEndOfStream() const
	{
		BOOL fEndOfStream = FALSE;
		CHECK(m_pByteStream->IsEndOfStream(&fEndOfStream));
		return fEndOfStream != FALSE;
	}

	void CopyFrom(CByteStream& SrcStream, QWORD cbSize)
	{
		QWORD cbWritten = 0;
		while((cbWritten < cbSize) && !SrcStream.IsEndOfStream())
			cbWritten += Write(SrcStream.Read(g_cbBufferSize));
	}

private:
	static ATL::CComPtr<IMFByteStream> OpenFile(
		const std::wstring& strFileName,
		MF_FILE_ACCESSMODE AccessMode,
		MF_FILE_OPENMODE OpenMode)
	{
		ATL::CComPtr<IMFByteStream> pByteStream;
		CHECK(::MFCreateFile(AccessMode, OpenMode, MF_FILEFLAGS_NONE,
			strFileName.c_str(), &pByteStream));

		return pByteStream;
	}

private:
	ATL::CComPtr<IMFByteStream> m_pByteStream;
};

class CPresentationDescriptor
{
public:
	CPresentationDescriptor(IMFPresentationDescriptor* pPD) :
		m_pPD(pPD)
	{}

	QWORD GetDataStartOffset() const
	{
		return GetQWORD(MF_PD_ASF_DATA_START_OFFSET);
	}

	inline QWORD GetDataLength() const
	{
		return GetQWORD(MF_PD_ASF_DATA_LENGTH);
	}

	QWORD GetQWORD(REFGUID guidKey) const
	{
		QWORD qwVal = 0;
		CHECK(m_pPD->GetUINT64(guidKey, &qwVal));
		return qwVal;
	}

private:
	ATL::CComPtr<IMFPresentationDescriptor> m_pPD;
};

class CContentInfo
{
public:
	CContentInfo(CByteStream& Stream) :
		m_pContentInfo(Create(Stream))
	{}

	ATL::CComPtr<IMFMediaBuffer> GenerateHeader() const
	{
		DWORD cbHeaderSize = 0;
		CHECK(m_pContentInfo->GenerateHeader(NULL, &cbHeaderSize));

		ATL::CComPtr<IMFMediaBuffer> pHeaderBuffer;
		CHECK(::MFCreateMemoryBuffer(cbHeaderSize, &pHeaderBuffer));
		CHECK(m_pContentInfo->GenerateHeader(pHeaderBuffer, &cbHeaderSize));

		return pHeaderBuffer;
	}

	template<typename TOutIter>
	void EnumStreamInfos(TOutIter out) const
	{
		ATL::CComPtr<IMFASFProfile> pProfile;
		CHECK(m_pContentInfo->GetProfile(&pProfile));

		DWORD cStreams = 0;
		CHECK(pProfile->GetStreamCount(&cStreams));
		for(WORD i = 0; i < cStreams; ++i, ++out)
			*out = CStreamInfo(pProfile, i);
	}

	CPresentationDescriptor GeneratePresentationDescriptor() const
	{
		ATL::CComPtr<IMFPresentationDescriptor> pPD;
		CHECK(m_pContentInfo->GeneratePresentationDescriptor(&pPD));

		return CPresentationDescriptor(pPD);
	}

	operator IMFASFContentInfo*() const
	{
		return m_pContentInfo;
	}

private:
	static ATL::CComPtr<IMFASFContentInfo> Create(CByteStream& Stream)
	{
		ATL::CComPtr<IMFASFContentInfo> pContentInfo;
		CHECK(::MFCreateASFContentInfo(&pContentInfo));

		static const DWORD MIN_ASF_HEADER_SIZE = 30;
		ATL::CComPtr<IMFMediaBuffer> pBuffer(Stream.Read(MIN_ASF_HEADER_SIZE));

		QWORD cbHeader = 0;
		CHECK(pContentInfo->GetHeaderSize(pBuffer, &cbHeader));
		CHECK(pContentInfo->ParseHeader(pBuffer, 0));

		pBuffer = Stream.Read(static_cast<DWORD>(cbHeader - MIN_ASF_HEADER_SIZE));
		CHECK(pContentInfo->ParseHeader(pBuffer, MIN_ASF_HEADER_SIZE));

		return pContentInfo;
	}

private:
	ATL::CComPtr<IMFASFContentInfo> m_pContentInfo;
};

struct CStreamInfo
{
public:
	CStreamInfo(IMFASFProfile* pProfile, WORD wStreamNum) :
		m_wStreamID(0),
		m_pStreamConfig(NULL)
	{
		ASSERT(pProfile);
		CHECK(pProfile->GetStream(wStreamNum, &m_wStreamID, &m_pStreamConfig));
	}

	WORD GetStreamID() const
	{
		return m_wStreamID;
	}

	GUID GetStreamType() const
	{
		GUID guidType = GUID_NULL;
		CHECK(m_pStreamConfig->GetStreamType(&guidType));

		return guidType;
	}

	LPCWSTR GetStreamTypeName() const
	{
		GUID guidType = GetStreamType();

		if(guidType == MFMediaType_Default)
			return L"Default";
		else if(guidType == MFMediaType_Audio)
			return L"Audio";
		else if(guidType == MFMediaType_Video)
			return L"Video";
		else if(guidType == MFMediaType_Protected)
			return L"Protected";
		else if(guidType == MFMediaType_SAMI)
			return L"SAMI";
		else if(guidType == MFMediaType_Script)
			return L"Script";
		else if(guidType == MFMediaType_Image)
			return L"Image";
		else if(guidType == MFMediaType_HTML)
			return L"HTML";
		else if(guidType == MFMediaType_Binary)
			return L"Binary";
		else if(guidType == MFMediaType_FileTransfer)
			return L"FileTransfer";
		else 
			return L"Unknown";
	}

private:
	WORD m_wStreamID;
	ATL::CComPtr<IMFASFStreamConfig> m_pStreamConfig;
};

class CSplitter
{
public:	
	template<typename TIter>
	CSplitter(
		const CContentInfo& ContentInfo,
		TIter firstStreamInfo,
		TIter lastStreamInfo
	) :
		m_pSplitter(Create(ContentInfo, firstStreamInfo, lastStreamInfo)),
		m_fIncomplete(false)
	{}

	void ParseData(
		IMFMediaBuffer* pBuffer,
		DWORD cbOffset = 0,
		DWORD dwLength = 0)
	{
		ASSERT(pBuffer);
		CHECK(m_pSplitter->ParseData(pBuffer, cbOffset, dwLength));
		m_fIncomplete = true;
	}

	bool GetNextSample(WORD* pwStreamID, IMFSample** ppSample)
	{
		ASSERT(pwStreamID);
		ASSERT(ppSample);
		ASSERT(!(*ppSample));

		ATL::CComPtr<IMFSample> pSample;
		WORD wStreamID = 0;
		DWORD dwStatus = ASF_STATUSFLAGS_INCOMPLETE;

		CHECK(m_pSplitter->GetNextSample(&dwStatus, &wStreamID, &pSample));
		m_fIncomplete = dwStatus & ASF_STATUSFLAGS_INCOMPLETE;

		if(pSample)
		{
			*ppSample = pSample.Detach();
			*pwStreamID = wStreamID;

			return true;
		}

		return false;
	}

	bool HasSamples() const
	{
		return m_fIncomplete;
	}

private:
	template<typename TIter>
	static ATL::CComPtr<IMFASFSplitter> Create(
		const CContentInfo& ContentInfo,
		TIter firstStreamInfo,
		TIter lastStreamInfo)
	{
		ATL::CComPtr<IMFASFSplitter> pSplitter;
		CHECK(::MFCreateASFSplitter(&pSplitter));
		CHECK(pSplitter->Initialize(ContentInfo));

		// Selecting all streams
		std::vector<WORD> seqStreamIDs;
		GetStreamIDs(firstStreamInfo, lastStreamInfo, std::back_inserter(seqStreamIDs));
		
		if(!seqStreamIDs.empty())
			CHECK(pSplitter->SelectStreams(&seqStreamIDs.front(), seqStreamIDs.size()));

		return pSplitter;
	}

	template<typename TInIter, typename TOutIter>
	static void GetStreamIDs(TInIter firstStreamInfo, TInIter lastStreamInfo, TOutIter out)
	{
		for(TInIter it = firstStreamInfo; it != lastStreamInfo; ++it, ++out)
			*out = it->GetStreamID();
	}

private:
	ATL::CComPtr<IMFASFSplitter> m_pSplitter;
	bool m_fIncomplete;
};

class CMultiplexer
{
public:
	CMultiplexer(const CContentInfo& ContentInfo) :
		m_pMultiplexer(Create(ContentInfo)),
		m_fIncomplete(false)
	{}

	void ProcessSample(WORD wStreamID, IMFSample* pSample)
	{
		CHECK(m_pMultiplexer->ProcessSample(wStreamID, pSample, 0));
		m_fIncomplete = true;
	}

	bool GetNextPacket(IMFSample** ppPacket)
	{
		ASSERT(ppPacket);
		ASSERT(!(*ppPacket));

		DWORD dwStatus = ASF_STATUSFLAGS_INCOMPLETE;
		ATL::CComPtr<IMFSample> pPacket;

		CHECK(m_pMultiplexer->GetNextPacket(&dwStatus, &pPacket));
		m_fIncomplete = dwStatus & ASF_STATUSFLAGS_INCOMPLETE;

		if(pPacket)
		{
			*ppPacket = pPacket.Detach();
			return true;
		}

		return false;
	}

	bool HasPackets() const
	{
		return m_fIncomplete;
	}

private:
	static ATL::CComPtr<IMFASFMultiplexer> Create(const CContentInfo& ContentInfo)
	{
		ATL::CComPtr<IMFASFMultiplexer> pMultiplexer;
		CHECK(::MFCreateASFMultiplexer(&pMultiplexer));
		CHECK(pMultiplexer->Initialize(ContentInfo));

		return pMultiplexer;
	}

private:
	ATL::CComPtr<IMFASFMultiplexer> m_pMultiplexer;
	bool m_fIncomplete;
};

class CStreamIndexer
{
public:
	CStreamIndexer(
		const CContentInfo& ContentInfo,
		WORD wStreamID,
		DWORD dwInterval
	) :
		m_pIndexer(Create(ContentInfo, wStreamID, dwInterval))
	{}

	void GenerateIndexEntries(IMFSample* pPacket)
	{
		CHECK(m_pIndexer->GenerateIndexEntries(pPacket));
	}

	void WriteIndex(CByteStream& Stream, CContentInfo& ContentInfo) const
	{
		CHECK(m_pIndexer->CommitIndex(ContentInfo));

		QWORD cbIndexSize = 0; 
		CHECK(m_pIndexer->GetIndexWriteSpace(&cbIndexSize));

		QWORD cbIndexWritten = 0;
		while(cbIndexWritten < cbIndexSize)
		{
			ATL::CComPtr<IMFMediaBuffer> pBuffer;
			CHECK(::MFCreateMemoryBuffer(g_cbBufferSize, &pBuffer));
			CHECK(m_pIndexer->GetCompletedIndex(pBuffer, cbIndexWritten));

			cbIndexWritten += Stream.Write(pBuffer);
		}
	}

private:
	static ATL::CComPtr<IMFASFIndexer> Create(
		const CContentInfo& ContentInfo,
		WORD wStreamID,
		DWORD dwInterval)
	{
		ATL::CComPtr<IMFASFIndexer> pIndexer;
		CHECK(::MFCreateASFIndexer(&pIndexer));
		CHECK(pIndexer->SetFlags(MFASF_INDEXER_WRITE_NEW_INDEX));
		CHECK(pIndexer->Initialize(ContentInfo));

		ASF_INDEX_IDENTIFIER IndexIdentifier = {};
		IndexIdentifier.guidIndexType = GUID_NULL;
		IndexIdentifier.wStreamNumber = wStreamID;

		ASF_INDEX_DESCRIPTOR IndexerType = {};
		IndexerType.Identifier     = IndexIdentifier;
		IndexerType.cPerEntryBytes = MFASFINDEXER_PER_ENTRY_BYTES_DYNAMIC;
		IndexerType.dwInterval     = dwInterval;

		CHECK(pIndexer->SetIndexStatus(reinterpret_cast<BYTE*>(&IndexerType),
			sizeof(ASF_INDEX_DESCRIPTOR), TRUE));

		return pIndexer;
	}

private:
	ATL::CComPtr<IMFASFIndexer> m_pIndexer;
};

class CIndexerMap
{
private:
	typedef std::map<WORD, CStreamIndexer> IndexerMapType;

public:
	template<typename TIter>
	CIndexerMap(
		const CContentInfo& ContentInfo,
		DWORD dwIndexingInterval,
		TIter firstStreamInfo,
		TIter lastStreamInfo)
	{
		// Creating an indexer for each stream
		for(TIter it = firstStreamInfo; it == lastStreamInfo; ++it)
		{
			/*if(it->GetStreamID() <= 2)*/
			{
				m_mapIndexers.insert(std::make_pair(
					it->GetStreamID(),
					CStreamIndexer(ContentInfo, it->GetStreamID(), dwIndexingInterval)));
			}
		}
	}

	void GenerateIndexEntries(WORD wStreamID, IMFSample* pPacket)
	{
		// Each stream is being indexed using appropriate indexer
		IndexerMapType::iterator it(m_mapIndexers.find(wStreamID));
		CHECK(it != m_mapIndexers.end() ? S_OK : HRESULT_FROM_WIN32(ERROR_NOT_FOUND));
		it->second.GenerateIndexEntries(pPacket);
	}

	void WriteIndex(CByteStream& Stream, CContentInfo& ContentInfo) const
	{
		// Writing all indices sequentially
		for(IndexerMapType::const_iterator first = m_mapIndexers.begin(),
			last = m_mapIndexers.end(); first != last; ++first)
		{
			first->second.WriteIndex(Stream, ContentInfo);
		}
	}

private:
	IndexerMapType m_mapIndexers;
};

class CIndexer
{
public:
	template<typename TIter>
	CIndexer(
		CContentInfo& ContentInfo,
		DWORD dwIndexingInterval,
		TIter firstStreamInfo,
		TIter lastStreamInfo
	) :
		m_Splitter(ContentInfo, firstStreamInfo, lastStreamInfo),
		m_Muxer(ContentInfo),
		m_IndexerMap(ContentInfo, dwIndexingInterval, firstStreamInfo, lastStreamInfo),
		m_PresentationDescriptor(ContentInfo.GeneratePresentationDescriptor())
		{
		}

	void IndexStream(CByteStream& Stream)
	{
		const QWORD cbStartOffset = m_PresentationDescriptor.GetDataStartOffset();
		const QWORD cbLength = m_PresentationDescriptor.GetDataLength();

//		Stream.SetCurrentPosition(cbStartOffset);
		while(!Stream.IsEndOfStream() && (Stream.GetCurrentPosition() < cbStartOffset + cbLength))
		{
			m_Splitter.ParseData(Stream.Read(g_cbBufferSize));

			while(m_Splitter.HasSamples())
				IndexSamples();
		}
	}

	void WriteIndex(CByteStream& Stream, CContentInfo& ContentInfo)
	{
		m_IndexerMap.WriteIndex(Stream, ContentInfo);
	}

private:
	void IndexSamples()
	{
		WORD wStreamID = 0;
		ATL::CComPtr<IMFSample> pSample;

		if(m_Splitter.GetNextSample(&wStreamID, &pSample))
		{
			m_Muxer.ProcessSample(wStreamID, pSample);

			while(m_Muxer.HasPackets())
				IndexPackets(wStreamID);
		}
	}

	void IndexPackets(WORD wStreamID)
	{
		ATL::CComPtr<IMFSample> pPacket;
		if(m_Muxer.GetNextPacket(&pPacket))
			m_IndexerMap.GenerateIndexEntries(wStreamID, pPacket);
	}

private:
	CSplitter m_Splitter;
	CMultiplexer m_Muxer;
	CIndexerMap m_IndexerMap;
	CPresentationDescriptor m_PresentationDescriptor;
};

void LogStreamInfo(const CStreamInfo& Info)
{
	std::wclog << L"Stream " << Info.GetStreamID();
	std::wclog << L": " << Info.GetStreamTypeName() << std::endl;
}

} // namespace ASF

int wmain(int argc, WCHAR* argv[])
{
	using namespace ASF;

	if(argc < 2)
	{
		std::wcout << L"Usage: indexer.exe <ASF file name>" << std::endl;
		return EXIT_FAILURE;
	}

	try
	{
		CMFSession Session; // ::MFStartup()

		// Opening input file
		const std::wstring strSourceFileName(argv[1]);
		CByteStream SrcStream(strSourceFileName,
			MF_ACCESSMODE_READ, MF_OPENMODE_FAIL_IF_NOT_EXIST);

		CContentInfo ContentInfo(SrcStream);

		// Remembering position after the end of the header object
		const QWORD cbDataStartOffset = SrcStream.GetCurrentPosition();
		
		// Retrieving list of streams withing the input file
		std::vector<CStreamInfo> seqStreamInfos;
		ContentInfo.EnumStreamInfos(std::back_inserter(seqStreamInfos));

		std::for_each(seqStreamInfos.begin(), seqStreamInfos.end(),
			std::tr1::bind(&LogStreamInfo, _1));

		// Setting up indexer to index all streams
		CIndexer Indexer(ContentInfo, g_dwIndexingInterval,
			seqStreamInfos.begin(), seqStreamInfos.begin());

		// Indexing
		Indexer.IndexStream(SrcStream);

		// Remembering position after the end of the data object
		const QWORD cbDataEndOffset = SrcStream.GetCurrentPosition();
		ASSERT(cbDataEndOffset >= cbDataStartOffset);

		// Opening output file
		const std::wstring strDstFileName(strSourceFileName + L"_indexed.asf");
		CByteStream DstStream(strDstFileName, MF_ACCESSMODE_WRITE,
			MF_OPENMODE_DELETE_IF_EXIST);

		// Writing updated header object
		DstStream.Write(ContentInfo.GenerateHeader());

		// Copying data object
		SrcStream.SetCurrentPosition(cbDataStartOffset);
		DstStream.CopyFrom(SrcStream, cbDataEndOffset - cbDataStartOffset);

		// Writing index objects
		Indexer.WriteIndex(DstStream, ContentInfo);

		std::wclog << std::endl << L"Indexed: " << strDstFileName << std::endl;

		return EXIT_SUCCESS;
	}
	catch(std::exception& e)
	{
		std::wcerr << L"Indexer: " << ATL::CA2W(e.what()) << std::endl;
		return EXIT_FAILURE;
	}
	catch(_com_error)
	{
		return EXIT_FAILURE;
	}
}

