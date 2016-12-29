#pragma once

namespace NMib::NXML
{
	template <typename t_CType>
	CXMLDocument::TCNodeIterator<t_CType>::TCNodeIterator(t_CType *_pNode)
	{
		m_pToIterate = _pNode;
		m_pCurrent = CXMLDocument::f_Iterate(_pNode, nullptr);
	}

	template <typename t_CType>
	CXMLDocument::TCNodeIterator<t_CType> &CXMLDocument::TCNodeIterator<t_CType>::operator ++()
	{
		if (m_pCurrent)
			m_pCurrent = CXMLDocument::f_Iterate(m_pToIterate, m_pCurrent);
		return *this;
	}

	template <typename t_CType>
	CXMLDocument::TCNodeIterator<t_CType>::operator t_CType * () const
	{
		return m_pCurrent;
	}

	template <typename t_CType>
	t_CType * CXMLDocument::TCNodeIterator<t_CType>::operator -> () const
	{
		return m_pCurrent;
	}
}
