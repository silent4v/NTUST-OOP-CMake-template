#ifndef Attribute_H
#define Attribute_H

namespace FTK
{
	struct Attribute
	{
	public: int HP = 0;
	public: int Focus = 0;

	public: int PhysicalAttack = 0;
	public: int PhysicalDefense = 0;
	public: int MagicalAttack = 0;
	public: int MagicalDefense = 0;

	public: int HitRate = 0;
	public: int Speed = 0;

	public: void operator+=(const Attribute& other)
	{
		this->HP += other.HP;
		this->Focus += other.Focus;
		this->PhysicalAttack += other.PhysicalAttack;
		this->PhysicalDefense += other.PhysicalDefense;
		this->MagicalAttack += other.MagicalAttack;
		this->MagicalDefense += other.MagicalDefense;
		this->HitRate += other.HitRate;
		this->Speed += other.Speed;
	}
	public: void operator-=(const Attribute& other)
	{
		this->HP -= other.HP;
		this->Focus -= other.Focus;
		this->PhysicalAttack -= other.PhysicalAttack;
		this->PhysicalDefense -= other.PhysicalDefense;
		this->MagicalAttack -= other.MagicalAttack;
		this->MagicalDefense -= other.MagicalDefense;
		this->HitRate -= other.HitRate;
		this->Speed -= other.Speed;
	}
	};
}
#endif // !Attribute_H