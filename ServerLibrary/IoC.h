#pragma once

#include <Hypodermic\Container.h>

class IoC
{
	static std::shared_ptr<Hypodermic::Container> container;
public:
	static void SetContainer(std::shared_ptr<Hypodermic::Container>& container)
	{
		IoC::container = container;
	}

	static std::shared_ptr<Hypodermic::Container> GetContainer()
	{
		return container;
	}
};