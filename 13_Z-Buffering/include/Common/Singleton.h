#pragma once
#include <memory>

template <class T>
class Singleton
{
	public:
		static T* get()
		{
			if (m_pInstance == nullptr)
			{
				m_pInstance = std::make_unique<T>();
			}

			return m_pInstance.get();
		}

		static void release()
		{
			m_pInstance.release();
			m_pInstance = nullptr;
		}
	protected:
		Singleton() = default;									// Use Compiler default.
		~Singleton() = default;									// Use Compiler default.
		Singleton(const Singleton&) = delete;					// Restrict Copy Constructor.
		Singleton&	operator = (const Singleton&) = delete;		// Restrict Assignment operator.
	private:
		static std::unique_ptr<T>	m_pInstance;
};

template <class T>
std::unique_ptr<T> Singleton<T>::m_pInstance = nullptr;