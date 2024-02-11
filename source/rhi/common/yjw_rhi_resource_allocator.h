#pragma once
#include <vector>

struct RHIHandle
{
	int ID{};
	long CreateTimeStamp{};
	void* Pool{};
	int TypeId{};
	bool isNull()
	{
		return Pool == 0;
	}
};

template<typename Type>
class ResourcePool;

template<typename Type>
struct ResourceHandle
{
	ResourceHandle<Type>(){}
	ResourceHandle<Type>(RHIHandle handle)
		:ID(handle.ID), CreateTimeStamp(handle.CreateTimeStamp), Pool(handle.Pool),TypeId(handle.TypeId) 
	{
		if (handle.TypeId != Type::TypeId)
		{
			ID = 0;
			CreateTimeStamp = 0;
			Pool = nullptr;
			TypeId = -1;
		}
	}
	ResourceHandle<Type>(int ID, int CreateTimeStamp, ResourcePool<Type>* Pool)
		:ID(ID), CreateTimeStamp(CreateTimeStamp), Pool(Pool), TypeId((int)Type::TypeId) {}
	operator RHIHandle() { return RHIHandle{ ID,CreateTimeStamp,Pool,TypeId }; }
	int ID;
	long CreateTimeStamp;
	ResourcePool<Type>* Pool;
	int TypeId;
	Type* Get();
	bool IsValid();
};

template<typename Type>
class ResourcePool
{
public:
	ResourceHandle<Type> allocate(Type* resource);
	void deallocate(const ResourceHandle<Type>& handle);
	Type* get(const ResourceHandle<Type>& handle);
	bool isValid(const ResourceHandle<Type>& handle);
private:
	std::vector<Type*> resourcePool;
	std::vector<long> TimeStamp;
	std::vector<int> UnusedIds;
	long CurrentTimeStamp = 0;
};

template<typename Type, typename AllocateStrategy>
class ResourceAllocator
{
public:
	ResourceHandle<Type> create(const typename Type::Creation& creation);
	void destory(ResourceHandle<Type>& handle);
	AllocateStrategy allocateStrategy;
private:
	ResourcePool<Type> pool;
};

template<typename Type>
Type* ResourceHandle<Type>::Get()
{
	return Pool->get(*this);
}

template<typename Type>
bool ResourceHandle<Type>::IsValid()
{
	if (TypeId == Type::TypeId)
	{
		return Pool->isValid(*this);
	}
	else
	{
		return false;
	}
}

template<typename Type>
Type* ResourcePool<Type>::get(const ResourceHandle<Type>& handle)
{
	return isValid(handle) ? resourcePool[handle.ID] : nullptr;
}

template<typename Type>
bool ResourcePool<Type>::isValid(const ResourceHandle<Type>& handle)
{
	return TimeStamp[handle.ID] == handle.CreateTimeStamp;
}

template<typename Type>
ResourceHandle<Type> ResourcePool<Type>::allocate(Type* resource)
{
	++CurrentTimeStamp;
	if (!UnusedIds.empty())
	{
		int newId = UnusedIds.back();
		UnusedIds.pop_back();
		TimeStamp[newId] = CurrentTimeStamp;
		resourcePool[newId] = resource;
		return ResourceHandle<Type>(newId, CurrentTimeStamp, this);
	}
	else
	{
		resourcePool.push_back(resource);
		TimeStamp.push_back(CurrentTimeStamp);
		int newId = (int)resourcePool.size() - 1;
		return ResourceHandle<Type>(newId, CurrentTimeStamp, this);
	}
}

template<typename Type>
void ResourcePool<Type>::deallocate(const ResourceHandle<Type>& handle)
{
	if (isValid(handle))
	{
		CurrentTimeStamp++;
		TimeStamp[handle.ID] = CurrentTimeStamp;
		UnusedIds.push_back(handle.ID);
	}
}

template<typename Type, typename AllocateStrategy>
ResourceHandle<Type> ResourceAllocator<Type, AllocateStrategy>::create(const typename Type::Creation& creation)
{
	Type* resource = allocateStrategy.CreateFunc(creation);
	return pool.allocate(resource);
}

template<typename Type, typename AllocateStrategy>
void ResourceAllocator<Type, AllocateStrategy>::destory(ResourceHandle<Type>& handle)
{
	Type* resource = handle.Get();
	allocateStrategy.DestoryFunc(resource);
	pool.deallocate(handle);
}