#pragma once

#ifdef __cplusplus

#include <Mib/Core/Core>
#include "../../External/tinyxml2/tinyxml2.h"
#include <Mib/Time/Time>

namespace NMib::NXML
{
	typedef tinyxml2::XMLNode CXMLNode;
	typedef tinyxml2::XMLElement CXMLElement;
	typedef tinyxml2::XMLAttribute CXMLAttribute;
	
	enum EXMLNodeType
	{
		EXMLNodeType_Element
		, EXMLNodeType_Text 
		, EXMLNodeType_Comment 
		, EXMLNodeType_Document
		, EXMLNodeType_Declaration
		, EXMLNodeType_Unknown
	};
	
	enum EXMLOutputDialect
	{
		EXMLOutputDialect_TinyXML = tinyxml2::PRINT_DIALECT_TINYXML
		, EXMLOutputDialect_Compact = tinyxml2::PRINT_DIALECT_COMPACT
		, EXMLOutputDialect_VisualStudio = tinyxml2::PRINT_DIALECT_VISUALSTUDIO
		, EXMLOutputDialect_Xcode = tinyxml2::PRINT_DIALECT_XCODE
	};
	
	class CXMLDocument
	{
	public:
		template <typename t_CType>
		struct TCNodeIterator
		{
			t_CType *m_pToIterate;
			t_CType *m_pCurrent;
			TCNodeIterator(t_CType *_pNode);
			TCNodeIterator &operator ++();
			operator t_CType * () const;
			t_CType *operator -> () const;
		};
		
		typedef TCNodeIterator<CXMLNode> CNodeIterator;
		typedef TCNodeIterator<CXMLNode const> CConstNodeIterator;
		
		CXMLDocument(bint _bUseBOM = true);
		CXMLDocument(CXMLDocument &&_ToMove);
		~CXMLDocument();

		bint f_ParseFile(NStr::CStr const &_FileName);
		bint f_ParseString(NStr::CStr const &_String);
		void f_SaveFile(NStr::CStr const &_FileName, EXMLOutputDialect _Dialect = EXMLOutputDialect_TinyXML);

		NStr::CStr f_GetAsString(EXMLOutputDialect _Dialect = EXMLOutputDialect_TinyXML) const;
		static NStr::CStr f_GetAsString(CXMLNode const *_pNode, EXMLOutputDialect _Dialect = EXMLOutputDialect_TinyXML);

		CXMLNode *f_GetRootNode();
		CXMLNode const *f_GetRootNode() const;
		static CXMLNode *f_DeepClone(CXMLNode const *_pSource, tinyxml2::XMLDocument *_pDocument);
		void f_TraceNodeTree(CXMLNode *_pNode = nullptr);
		
		static EXMLNodeType f_GetNodeType(CXMLNode const *_pNode);

		static CXMLNode *f_Iterate(CXMLNode *_pNode, CXMLNode *_pLastIter = nullptr);
		static CXMLNode const *f_Iterate(CXMLNode const *_pNode, CXMLNode const *_pLastIter = nullptr);

		static CXMLElement *f_CreateElement(CXMLNode *_Parent, NStr::CStr const &_Name);

		static CXMLElement *f_GetOrCreateElement(CXMLNode *_Parent, NStr::CStr const &_Name, bool &o_bWasCreated);

		static void f_SetAttribute(CXMLElement *_Parent, NStr::CStr const &_Name, NStr::CStr const &_Value);
		static NStr::CStr f_GetAttribute(CXMLElement const *_Parent, NStr::CStr const &_Name);

		static CXMLNode *f_GetChildNode(CXMLNode *_pNode, NStr::CStr const &_Child);
		static CXMLNode const* f_GetChildNode(CXMLNode const *_pNode, NStr::CStr const &_Child);

		CXMLElement *f_CreateDefaultDocument(NStr::CStr const &_RootElementName);

		static CXMLElement *f_AddElementAndText(CXMLNode *_pNode, NStr::CStr const &_ElementName, NStr::CStr const &_Data);
		static void f_AddText(CXMLNode *_pNode, NStr::CStr const &_Data);
		static void f_SetText(CXMLNode *_pNode, NStr::CStr const &_Data);

		static CXMLNode *f_AddElement(CXMLNode *_pNode, NStr::CStr const &_ElementName);
		static void f_AddElementAndText(CXMLNode *_pNode, NStr::CStr const &_ElementName, int64 _Value);
		static void f_AddElementAndText(CXMLNode *_pNode, NStr::CStr const &_ElementName, fp64 _Value);

		static NStr::CStr f_GetNodeText(CXMLNode const *_pNode);
		static NStr::CStr f_GetValue(CXMLNode const *_pNode);
		static int64 f_GetNodeInt(CXMLNode const *_pNode);
		static fp64 f_GetNodeFloat(CXMLNode const *_pNode);

	private:
		CXMLDocument(CXMLDocument const&);

		void fp_TraceNodeTree(CXMLNode const *_pNode, int _Depth);

		NPtr::TCUniquePointer<tinyxml2::XMLDocument> mp_pDocument;
	};
}

#ifndef DMibPNoShortCuts
using namespace NMib::NXML;
#endif

#include "Malterlib_XML.hpp"

#endif
