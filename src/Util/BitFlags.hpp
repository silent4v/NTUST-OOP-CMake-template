#ifndef BitFlags_H
#define BitFlags_H

namespace Util
{
	template<typename StateType>
	class BitFlags
	{
	protected: StateType flags;
	protected: BitFlags(StateType flags) : flags(flags) {};
	protected: inline void setFlag(const StateType& flag) { this->flags = flag; }
	protected: inline void addFlag(const StateType& flag)
	{
		this->flags = static_cast<StateType>(static_cast<int>(this->flags) | static_cast<int>(flag));
	}
	protected: inline void removeFlag(const StateType& flag)
	{
		this->flags = static_cast<StateType>(static_cast<int>(this->flags) & ~static_cast<int>(flag));
	}
	protected: inline bool hasFlag(const StateType& flag) const
	{
		return (static_cast<int>(this->flags) & static_cast<int>(flag)) == static_cast<int>(flag);
	}
	};
}

#endif // !BitFlags_H