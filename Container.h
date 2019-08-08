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

	void addData(const T& aData, const std::string& aName = "")
	{
		mContainer.emplace(aName.empty() ? aData->name() : aName, aData);
	}

private:
	std::map<std::string, T> mContainer;
};

