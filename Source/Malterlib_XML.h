// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#ifdef __cplusplus

#include <Mib/Core/Core>
#include "../../../External/tinyxml2/tinyxml2.h"
#include <Mib/Time/Time>

namespace NMib::NXML
{
	using CXMLNode = tinyxml2::XMLNode;
	using CXMLElement = tinyxml2::XMLElement;
	using CXMLAttribute = tinyxml2::XMLAttribute;

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
		EXMLOutputDialect_TinyXML        = tinyxml2::PRINT_DIALECT_TINYXML
		, EXMLOutputDialect_Compact      = tinyxml2::PRINT_DIALECT_COMPACT
		, EXMLOutputDialect_VisualStudio = tinyxml2::PRINT_DIALECT_VISUALSTUDIO
		, EXMLOutputDialect_Xcode        = tinyxml2::PRINT_DIALECT_XCODE
	};

	struct CXMLDocument
	{
		template <typename t_CType>
		struct TCNodeIterator
		{
			t_CType *m_pToIterate;
			t_CType *m_pCurrent;
			TCNodeIterator(t_CType *_pNode);
			TCNodeIterator &operator ++ ();
			operator t_CType * () const;
			t_CType *operator -> () const;
		};

		using CNodeIterator = TCNodeIterator<CXMLNode>;
		using CConstNodeIterator = TCNodeIterator<CXMLNode const>;

		CXMLDocument(bool _bUseBOM = true);
		CXMLDocument(CXMLDocument &&_ToMove);
		~CXMLDocument();

		bool f_ParseFile(NStr::CStr const &_FileName);
		bool f_ParseString(NStr::CStr const &_String);
		void f_SaveFile(NStr::CStr const &_FileName, EXMLOutputDialect _Dialect = EXMLOutputDialect_TinyXML);

		NStr::CStr f_GetAsString(EXMLOutputDialect _Dialect = EXMLOutputDialect_TinyXML) const;
		static NStr::CStr fs_GetAsString(CXMLNode const *_pNode, EXMLOutputDialect _Dialect = EXMLOutputDialect_TinyXML);

		CXMLNode *f_GetRootNode();
		CXMLNode const *f_GetRootNode() const;
		static CXMLNode *fs_DeepClone(CXMLNode const *_pSource, tinyxml2::XMLDocument *_pDocument);
		void f_TraceNodeTree(CXMLNode *_pNode = nullptr);

		static EXMLNodeType fs_GetNodeType(CXMLNode const *_pNode);

		static CXMLNode *fs_Iterate(CXMLNode *_pNode, CXMLNode *_pLastIter = nullptr);
		static CXMLNode const *fs_Iterate(CXMLNode const *_pNode, CXMLNode const *_pLastIter = nullptr);

		static CXMLElement *fs_CreateElement(CXMLNode *_Parent, NStr::CStr const &_Name);

		static CXMLElement *fs_GetOrCreateElement(CXMLNode *_Parent, NStr::CStr const &_Name, bool &o_bWasCreated);

		static void fs_SetAttribute(CXMLElement *_Parent, NStr::CStr const &_Name, NStr::CStr const &_Value);
		static NStr::CStr fs_GetAttribute(CXMLElement const *_Parent, NStr::CStr const &_Name);

		static CXMLNode *fs_GetChildNode(CXMLNode *_pNode, NStr::CStr const &_Child);
		static CXMLNode const *fs_GetChildNode(CXMLNode const *_pNode, NStr::CStr const &_Child);

		CXMLElement *f_CreateDefaultDocument(NStr::CStr const &_RootElementName, ch8 const *_pDeclaration = nullptr);

		static void fs_AddText(CXMLNode *_pNode, NStr::CStr const &_Data);
		static void fs_SetText(CXMLNode *_pNode, NStr::CStr const &_Data);

		static CXMLNode *fs_AddElement(CXMLNode *_pNode, NStr::CStr const &_ElementName);
		static CXMLElement *fs_AddElementAndText(CXMLNode *_pNode, NStr::CStr const &_ElementName, NStr::CStr const &_Data);
		static CXMLElement *fs_AddElementAndInt(CXMLNode *_pNode, NStr::CStr const &_ElementName, int64 _Value);
		static CXMLElement *fs_AddElementAndFloat(CXMLNode *_pNode, NStr::CStr const &_ElementName, fp64 _Value);
		static CXMLElement *fs_AddElementAndBool(CXMLNode *_pNode, NStr::CStr const &_ElementName, bool _bValue);

		static NStr::CStr fs_GetNodeText(CXMLNode const *_pNode, NStr::CStr const &_Default = {});
		static NStr::CStr fs_GetValue(CXMLNode const *_pNode);
		static int64 fs_GetNodeInt(CXMLNode const *_pNode, int64 _Default = 0);
		static fp64 fs_GetNodeFloat(CXMLNode const *_pNode, fp64 _Default = fp64::fs_Inf());
		static bool fs_GetNodeBool(CXMLNode const *_pNode, bool _bDefault = false);

		static NStr::CStr fs_GetChildValue(CXMLNode const *_pNode, NStr::CStr const &_Name, NStr::CStr const &_Default);
		static int64 fs_GetChildValue(CXMLNode const *_pNode, NStr::CStr const &_Name, int64 _Default);
		static fp64 fs_GetChildValue(CXMLNode const *_pNode, NStr::CStr const &_Name, fp64 _Default);
		static bool fs_GetChildValue(CXMLNode const *_pNode, NStr::CStr const &_Name, bool _bDefault);

	private:
		CXMLDocument(CXMLDocument const &_Other);

		void fp_TraceNodeTree(CXMLNode const *_pNode, int _Depth);

		NStorage::TCUniquePointer<tinyxml2::XMLDocument> mp_pDocument;
	};
}

#ifndef DMibPNoShortCuts
using namespace NMib::NXML;
#endif

#include "Malterlib_XML.hpp"

#endif
