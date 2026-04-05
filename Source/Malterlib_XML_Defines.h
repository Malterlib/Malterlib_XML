// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

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
	using CXMLNode = tinyxml2::XMLNode;
	using CXMLElement = tinyxml2::XMLElement;
	using CXMLAttribute = tinyxml2::XMLAttribute;

	class CXMLDocument;
}

#ifndef DMibPNoShortCuts
	using namespace NMib::NXML;
#endif

#endif
