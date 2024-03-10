#pragma once

#include <string>
#include <map>

namespace nate
{
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

	bool contains(const T& aData)
	{
		return std::find_if(mContainer.cbegin(), mContainer.cend(),
											  [&aData](const auto& aPair)
												{ return aPair.second == aData; }) != mContainer.cend();
	}

private:
	std::map<std::string, T> mContainer;
};

}
