#pragma once

# include <vector>
# include <typeinfo>
# include <memory>
# include <map>
# include <list>
# include <stdexcept>
# include <iostream>
# include <algorithm>
# include "PoolObject.h"

class Factory
{

public:
	Factory(void){};
	~Factory(void){
		for (auto item : _collection)
			delete item.second;
	};

public:
	template <typename T>
	void addObjectType(void)
	{
		_collection.push_back(std::make_pair(typeid(T).name(), new T()));
	}

	template <typename T>
	T *create(void) const
	{
		for (auto it : _collection)
		{
			if (it.first == typeid(T).name())
				return (static_cast<T*>(it.second->clone()));
		}
		std::cerr << "Factory : can't create this object (" << typeid(T).name() << " doesn't exist)" << std::endl;
		return nullptr;
	}

protected:
	std::vector<std::pair<const char *, PoolObject*>> _collection;
};

class Pool
{

public:
	enum PoolType
	{
		AUTO_EXTEND = 0,
		FIXED
	};

public:
	Pool(const PoolType &p = FIXED) : _type(p) {
	
		cout << "Type " <<_type << endl;
	}
	virtual ~Pool(void){
		for (auto item : _collection)
			for (auto object : item.second)
				delete object;
	}

public:
	template <typename T>
	void configure(unsigned int size)
	{
		_factory.addObjectType<T>();
		for (unsigned int i = 0; i != size; ++i)
		{
			T *ptr = _factory.create<T>();
			if (ptr)
				_collection[typeid(T).name()].push_back(ptr);
		}
	}

	template <typename T>
	unsigned int size(void)
	{
		const char *type = typeid(T).name();
		if (_collection.find(type) != _collection.end())
			return _collection[type].size();
		return 0;
	}

	template <typename T>
	T *get(void)
	{
		const char *type = typeid(T).name();
		auto it = _collection.find(type);
		if (it != _collection.end())
		{
			T *ptr = nullptr;
			if (!_collection[type].empty())
			{

				ptr = static_cast<T *>(_collection[type].front());
				_collection[type].pop_front();
			}
			else if (_type == AUTO_EXTEND) {
				ptr = _factory.create<T>();
			}
			return ptr;
		}
		else
		{
			std::string typeStr(type);
			throw std::logic_error("Pool : can't return an object of type " + typeStr);
		}
	}

	template <typename T>
	bool release(T* ptr)
	{
		const char *type = typeid(T).name();
		auto found = find_if(_collection[type].begin(), _collection[type].end(),
							[ptr](PoolObject *t) -> bool { return (ptr == t); });
		auto it = _collection.find(type);
		if (found == _collection[type].end() && it != _collection.end() && ptr)
		{
			ptr->reset();
			_collection[type].push_back(ptr);

			return true;
		}
		std::cerr << "Pool : can't release an object of type " + std::string(type) + " or a non-valid pointer" << std::endl;
		return false;
	}

protected:
	std::map<const char *, std::list<PoolObject*>> _collection;
	Factory _factory;
	const PoolType _type;
};

