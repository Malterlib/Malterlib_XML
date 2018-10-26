
#include "Malterlib_XML.h"

using namespace tinyxml2;
using namespace NMib::NStr;

namespace NMib::NXML
{
	CXMLDocument::CXMLDocument(bint _bUseBOM)
		: mp_pDocument(fg_Construct())
	{
		mp_pDocument->SetBOM(_bUseBOM);
	}

	CXMLDocument::CXMLDocument(CXMLDocument &&_ToMove)
		: mp_pDocument(fg_Move(_ToMove.mp_pDocument))
	{
	}
		
	CXMLDocument::~CXMLDocument()
	{
	}

	bint CXMLDocument::f_ParseString(CStr const &_String)
	{
		mp_pDocument->Clear();
		
		XMLError Error = mp_pDocument->Parse(_String.f_GetStr(), _String.f_GetLen());

		return Error == XML_SUCCESS;
	}

	bint CXMLDocument::f_ParseFile(CStr const &_FileName)
	{
		CStr ToParse = NFile::CFile::fs_ReadStringFromFile(_FileName, true);
		return f_ParseString(ToParse);
	}

	EXMLNodeType CXMLDocument::f_GetNodeType(CXMLNode const *_pNode)
	{
		if (_pNode->ToElement())
			return EXMLNodeType_Element; 
		else if (_pNode->ToText())
			return EXMLNodeType_Text; 
		else if (_pNode->ToComment())
			return EXMLNodeType_Comment; 
		else if (_pNode->ToDocument())
			return EXMLNodeType_Document;
		else if (_pNode->ToDeclaration())
			return EXMLNodeType_Declaration;
		else if (_pNode->ToUnknown())
			return EXMLNodeType_Unknown;
		return EXMLNodeType_Element;
	}
	
	CStr CXMLDocument::f_GetAsString(CXMLNode const *_pNode, EXMLOutputDialect _Dialect)
	{
		XMLPrinter Printer{nullptr, (PrintDialect)_Dialect};
		_pNode->Accept(&Printer);
		return CStr(Printer.CStr(), Printer.CStrSize() - 1);
	}

	CStr CXMLDocument::f_GetAsString(EXMLOutputDialect _Dialect) const
	{
		XMLPrinter Printer{nullptr, (PrintDialect)_Dialect};
		mp_pDocument->Print(&Printer);
		if (mp_pDocument->HasBOM())
			return CStr(Printer.CStr() + 3, Printer.CStrSize() - 4);
		else
			return CStr(Printer.CStr(), Printer.CStrSize() - 1);
	}

	void CXMLDocument::f_SaveFile(CStr const &_FileName, EXMLOutputDialect _Dialect)
	{
		FILE *pFile = nullptr;

		auto Cleanup = fg_OnScopeExit
			(
				[&]()
				{
					if (pFile)
						fclose(pFile);
				}
			)
		;
	#ifdef DPlatformFamily_Windows
		 errno_t Errno = _wfopen_s(&pFile, CWStr(_FileName), L"w");
		 if (pFile && !Errno)
	#else
		pFile = fopen(_FileName.f_GetStr(), "w");
		if (pFile)
	#endif
		{
			XMLPrinter Printer{pFile, (PrintDialect)_Dialect};
			mp_pDocument->Print(&Printer);
		}
	}

	CXMLNode *CXMLDocument::f_DeepClone(CXMLNode const *_pSource, XMLDocument *_pDocument)
	{
		XMLNode *pClone = _pSource->ShallowClone(_pDocument);
		for (XMLNode const *pChild = _pSource->FirstChild(); pChild; pChild = pChild->NextSibling())
			pClone->InsertEndChild(f_DeepClone(pChild, _pDocument));

		return pClone;
	}
	
	CXMLNode *CXMLDocument::f_GetRootNode()
	{
		return mp_pDocument.f_Get();
	}

	CXMLNode const* CXMLDocument::f_GetRootNode() const
	{
		return mp_pDocument.f_Get();
	}

	void CXMLDocument::f_TraceNodeTree(CXMLNode *_pNode)
	{
		if (!_pNode)
			_pNode = f_GetRootNode();
		fp_TraceNodeTree(_pNode, 0);
	}

	CXMLNode *CXMLDocument::f_GetChildNode(CXMLNode *_pNode, CStr const &_Child)
	{
		return _pNode->FirstChildElement(_Child.f_GetStr());
	}

	CXMLNode const* CXMLDocument::f_GetChildNode(CXMLNode const *_pNode, CStr const &_Child)
	{
		return _pNode->FirstChildElement(_Child.f_GetStr());
	}

	CStr CXMLDocument::f_GetValue(CXMLNode const *_pNode)
	{
		if (_pNode->Value()) // Handle edge case of 'document node' that will return null 
			return _pNode->Value();
		else 
			return CStr();
	}

	CStr CXMLDocument::f_GetNodeText(CXMLNode const *_pNode, CStr const &_Default)
	{
		for (CXMLNode const* pChild = _pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
		{
			XMLText const* pText = pChild->ToText();
			if (pText)
				return pText->Value();
		}

		return _Default;
	}

	int64 CXMLDocument::f_GetNodeInt(CXMLNode const *_pNode, int64 _Default)
	{
		return f_GetNodeText(_pNode).f_ToInt(_Default);
	}

	fp64 CXMLDocument::f_GetNodeFloat(CXMLNode const *_pNode, fp64 _Default)
	{
		return f_GetNodeText(_pNode).f_ToFloat(_Default);
	}

	bool CXMLDocument::f_GetNodeBool(CXMLNode const *_pNode, bool _bDefault)
	{
		CStr Text = f_GetNodeText(_pNode);
		if (Text == "true")
			return true;
		else if (Text == "false")
			return false;
		return f_GetNodeInt(_pNode, _bDefault) != 0;
	}

	NStr::CStr CXMLDocument::f_GetChildValue(CXMLNode const *_pNode, NStr::CStr const &_Name, NStr::CStr const &_Default)
	{
		auto pChild = f_GetChildNode(_pNode, _Name);
		if (!pChild)
			return _Default;
		return f_GetNodeText(pChild, _Default);
	}

	int64 CXMLDocument::f_GetChildValue(CXMLNode const *_pNode, NStr::CStr const &_Name, int64 _Default)
	{
		auto pChild = f_GetChildNode(_pNode, _Name);
		if (!pChild)
			return _Default;
		return f_GetNodeInt(pChild, _Default);
	}

	fp64 CXMLDocument::f_GetChildValue(CXMLNode const *_pNode, NStr::CStr const &_Name, fp64 _Default)
	{
		auto pChild = f_GetChildNode(_pNode, _Name);
		if (!pChild)
			return _Default;
		return f_GetNodeFloat(pChild, _Default);
	}

	bool CXMLDocument::f_GetChildValue(CXMLNode const *_pNode, NStr::CStr const &_Name, bool _bDefault)
	{
		auto pChild = f_GetChildNode(_pNode, _Name);
		if (!pChild)
			return _bDefault;
		return f_GetNodeBool(pChild, _bDefault);
	}

	void CXMLDocument::fp_TraceNodeTree(CXMLNode const *_pNode, int _Depth)
	{
		if (_pNode) 
		{
			DDTrace("{sj*}{}" DNewLine, "" << _Depth*2 << _pNode->Value());

			for (CXMLNode const* pChild = _pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
				fp_TraceNodeTree(pChild, _Depth + 1);
		}
	}

	CXMLElement *CXMLDocument::f_CreateDefaultDocument(CStr const &_RootElementName)
	{
		XMLDeclaration * pDeclaration = mp_pDocument->NewDeclaration();
		mp_pDocument->InsertEndChild(pDeclaration);
		CXMLElement *pElement = mp_pDocument->NewElement(_RootElementName.f_GetStr());
		return mp_pDocument->InsertEndChild(pElement)->ToElement();
	}

	CXMLNode *CXMLDocument::f_Iterate(CXMLNode *_pNode, CXMLNode *_pLastIter)
	{
		if (!_pLastIter)
			return _pNode->FirstChild();
		else
			return _pLastIter->NextSibling();
	}

	CXMLNode const*CXMLDocument::f_Iterate(CXMLNode const *_pNode, CXMLNode const *_pLastIter)
	{
		if (!_pLastIter)
			return _pNode->FirstChild();
		else
			return _pLastIter->NextSibling();
	}

	CXMLElement *CXMLDocument::f_GetOrCreateElement(CXMLNode *_Parent, CStr const &_Name, bool &_bWasCreated)
	{
		for (auto pElement = _Parent->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
		{
			if (pElement->Value() == _Name)
			{
				_bWasCreated = false;
				return pElement;
			}
		}

		_bWasCreated = true;
		return f_CreateElement(_Parent, _Name);
	}

	CXMLElement *CXMLDocument::f_CreateElement(CXMLNode *_pParent, CStr const &_Name)
	{
		CXMLElement *pTempElement = _pParent->GetDocument()->NewElement(_Name.f_GetStr());
		return _pParent->InsertEndChild(pTempElement)->ToElement();
	}

	CStr CXMLDocument::f_GetAttribute(CXMLElement const *_Parent, CStr const &_Name)
	{
		return _Parent->Attribute(_Name.f_GetStr());
	}

	void CXMLDocument::f_SetAttribute(CXMLElement *_Parent, CStr const &_Name, CStr const &_Value)
	{
		_Parent->SetAttribute(_Name.f_GetStr(), _Value.f_GetStr());
	}

	void CXMLDocument::f_SetText(CXMLNode *_pNode, CStr const &_Data)
	{
		for (CXMLNode *pChild = _pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
		{
			XMLText *pText = pChild->ToText();
			if (pText)
			{
				pText->SetValue(_Data.f_GetStr());
				return;
			}
		}
		_pNode->ToElement()->SetText(_Data.f_GetStr());
	}

	void CXMLDocument::f_AddText(CXMLNode *_pNode, CStr const &_Data)
	{
		auto *pTempNode = _pNode->GetDocument()->NewText(_Data.f_GetStr());
		_pNode->InsertEndChild(pTempNode);
	}

	CXMLNode* CXMLDocument::f_AddElement(CXMLNode *_pNode, CStr const &_ElementName)
	{
		CXMLElement *pTempElement = _pNode->GetDocument()->NewElement(_ElementName.f_GetStr());
		return _pNode->InsertEndChild(pTempElement);
	}


	CXMLElement *CXMLDocument::f_AddElementAndText(CXMLNode *_pNode, CStr const &strElementName, CStr const &strDataText)
	{
		CXMLElement *pTempElement = _pNode->GetDocument()->NewElement(strElementName.f_GetStr());
		pTempElement->SetText(strDataText.f_GetStr());
		return _pNode->InsertEndChild(pTempElement)->ToElement();
	}

	void CXMLDocument::f_AddElementAndText(CXMLNode *_pNode, CStr const &_ElementName, int64 _Value)
	{
		f_AddElementAndText(_pNode, _ElementName, CStr::fs_ToStr(_Value));
	}

	void CXMLDocument::f_AddElementAndText(CXMLNode *_pNode, CStr const &_ElementName, fp64 _Value)
	{
		f_AddElementAndText(_pNode, _ElementName, CStr::fs_ToStr(_Value));
	}
}
