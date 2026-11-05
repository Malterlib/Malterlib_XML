// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Mib/Core/Core>
#include <Mib/XML/XML>

namespace
{
	struct CGeneral_Tests : public NMib::NTest::CTest
	{
		void f_DoTests();
	};

	void CGeneral_Tests::f_DoTests()
	{
	}
}

DMibTestRegister(CGeneral_Tests, Malterlib::XML);
