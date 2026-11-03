// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#ifdef __cplusplus

// NOLINTBEGIN(malterlib-type-naming): the names in tinyxml2
namespace tinyxml2
{
	class XMLNode;
	class XMLElement;
	class XMLAttribute;
}
// NOLINTEND(malterlib-type-naming)

namespace NMib::NXML
{
	using CXMLNode = tinyxml2::XMLNode;
	using CXMLElement = tinyxml2::XMLElement;
	using CXMLAttribute = tinyxml2::XMLAttribute;

	struct CXMLDocument;
}

#ifndef DMibPNoShortCuts
using namespace NMib::NXML;
#endif

#endif
