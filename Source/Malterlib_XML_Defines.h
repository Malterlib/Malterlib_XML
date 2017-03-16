#pragma once

#ifdef __cplusplus

namespace tinyxml2
{
	class XMLNode;
	class XMLElement;
	class XMLAttribute;
}

namespace NMib::NXML
{
	typedef tinyxml2::XMLNode CXMLNode;
	typedef tinyxml2::XMLElement CXMLElement;
	typedef tinyxml2::XMLAttribute CXMLAttribute;
	
	class CXMLDocument;
}

#ifndef DMibPNoShortCuts
using namespace NMib::NXML;
#endif

#endif
