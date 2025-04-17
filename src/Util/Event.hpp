#ifndef Event_H
#define Event_H

#include <vector>
#include <functional>

namespace Util
{
	template<typename RETURN, typename... ARGS>
	class Event
	{
		// TODO: check parameter type
	private: std::vector<std::function<RETURN(ARGS...)>> listeners;

	private:
		//template<typename T, typename... U>
		//size_t getAddress(std::function<T(U...)> f) {
		//	typedef T(fnType)(U...);
		//	fnType** fnPointer = f.template target<fnType*>();
		//	return (size_t)*fnPointer;
		//}

	public:
		void Register(const std::function<RETURN(ARGS...)>& listener)
		{
			listeners.emplace_back(listener);
		}

		//void UnRegister(const FUNCTIONAL& listener)
		//{
		//	for (int i = 0; i < listeners.size(); i++)
		//	{
		//		if (getAddress(listeners[i]) == getAddress(listener))
		//		{
		//			listeners.erase(listeners.begin() + i);
		//			break;
		//		}
		//	}
		//}

		void Invoke(ARGS... args) const
		{
			for (int i = 0; i < listeners.size(); i++)
			{
				listeners[i](args...);
			}
		}
	};
}

#endif // !Event_H