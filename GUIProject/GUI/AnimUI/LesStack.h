#pragma once
#include<stdlib.h>
namespace AnimUI {
	template<typename T>
	class LesStack {
	public:
		LesStack()
		{
			m_flow = 10;
			m_cur = 0;
			m_src = (T*)malloc(sizeof(T)*m_flow);
		}
		~LesStack()
		{
			if (m_src)free(m_src);
		}
		inline void Pop() {
			m_cur--;
			if (m_cur < 0)m_cur = 0;
		}
		inline void Clear() {
			m_cur = 0;
		}
		inline int Size() {
			return m_cur;
		}
		inline bool Empty() {
			return m_cur == 0;
		}
		void operator += (const T c) {
			m_src[m_cur++] = c;
			if (m_cur >= m_flow + 2) {
				m_flow *= 2;
				m_src = (T*)realloc(m_src, sizeof(T)*m_flow);
			}
		}
		T *m_src;
		int m_cur, m_flow;
	};
}