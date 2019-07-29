#pragma once

#include <string>
#include <map>

template <typename T>
class Container
{
public:
	Container() = default;
	virtual ~Container() = default;

	T getData(const std::string& aName)
	{
		auto iter = mContainer.find(aName);
		return iter == mContainer.end() ? T{} : iter->second;
	}

	void addData(const T& aData)
	{
		mContainer.emplace(aData->name(), aData);
	}

private:
	std::map<std::string, T> mContainer;
};

