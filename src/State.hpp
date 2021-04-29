#ifndef State_hpp_HEADERGUARD
#define State_hpp_HEADERGUARD

#include "Session.hpp"

#include <type_traits>


class Session::State{
	public:
		template <typename T, typename... Args>
		static const std::shared_ptr<State> make(Session *session, Args... args){
			static_assert(std::is_base_of<State, T>::value, "Class must be derived from State");
			std::shared_ptr<T> newState(new T(args...));
			newState->m_self = newState;
			return newState;
		}
		virtual ~State(){};
		virtual std::shared_ptr<State> handleCommand(Session&, Session::Command) = 0;

	protected:
		State(){};

		std::weak_ptr<State> m_self;
};

class Session::State_Login : public State{
	public:
		std::shared_ptr<State> handleCommand(Session&, Session::Command) override;
};

class Session::State_Game : public State{
	public:
		std::shared_ptr<State> handleCommand(Session&, Session::Command) override;
};


#endif //State_hpp_HEADERGUARD
