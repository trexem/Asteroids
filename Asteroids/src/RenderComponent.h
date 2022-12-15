#pragma once
#include "Component.h"

class RenderComponent : public Component {
public:
	virtual ~RenderComponent() = default;

	virtual void Render() = 0;
};

